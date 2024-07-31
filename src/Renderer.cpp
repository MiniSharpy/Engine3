#include "Renderer.h"
#include "Maths/Matrix.h"
#include <filesystem>
#include <fstream>
#include <numbers>
#include <print>
#include <SDL.h>
#include <string>
#include <GL/glew.h>

GLenum Engine3::Renderer::LoadShader(GLenum shaderType, const std::string_view shaderFileName)
{
	std::string shaderFileContents;
	{
		// TODO: Convert to LoadFile function.
		// Unnecessary, but ensures file is closed a little bit earlier.
		const std::filesystem::path ShaderPath{"Data/Shaders"};

		// TODO: See how Linux handles this.
		std::ifstream in{ShaderPath / shaderFileName, std::ios::ate};
		if (!in)
		{
			std::print("Error! File does not exist!\n");
			assert(false);
		}

		shaderFileContents.resize(in.tellg());
		in.seekg(0);
		in.read(shaderFileContents.data(), shaderFileContents.size());
	}

	GLuint shader = glCreateShader(shaderType);
	const GLchar* temporaryString = shaderFileContents.data(); // Can't address RValues.
	const GLint temporaryLength = shaderFileContents.length();
	glShaderSource(shader, 1, &temporaryString, &temporaryLength);
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		std::string infoLog;

		// Get error length for resize.
		int stringLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &stringLength);
		infoLog.resize(stringLength);

		// Copy error into the string
		glGetShaderInfoLog(shader, stringLength, nullptr, infoLog.data());
		std::print("{}", infoLog);
		assert(false);
	}

	return shader;
}

GLuint Engine3::Renderer::CreateProgram(const std::vector<GLuint>& shaderList)
{
	GLuint shaderProgram = glCreateProgram();

	for (GLuint shaderHandle : shaderList) { glAttachShader(shaderProgram, shaderHandle); }

	glLinkProgram(shaderProgram);

	// Validate linked successfully.
	GLint status;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		std::string infoLog;

		// Get error length for resize.
		int stringLength;
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &stringLength);
		infoLog.resize(stringLength);

		// Copy error into the string
		glGetProgramInfoLog(shaderProgram, stringLength, nullptr, infoLog.data());
		std::print("{}", infoLog);
		assert(false);
	}

	for (GLuint shaderHandle : shaderList)
	{
		glDetachShader(shaderProgram, shaderHandle);
		glDeleteShader(shaderHandle);
	}

	return shaderProgram;
}

void Engine3::Renderer::InitialiseProgram(const int width, const int height)
{
	std::vector<GLuint> shaderList;

	shaderList.push_back(LoadShader(GL_VERTEX_SHADER, "vertex.vert"));
	shaderList.push_back(LoadShader(GL_FRAGMENT_SHADER, "fragment.frag"));

	ShaderProgram_ = CreateProgram(shaderList);

	OffsetUniform_ = glGetUniformLocation(ShaderProgram_, "offset");
	PerspectiveMatrixUniform_ = glGetUniformLocation(ShaderProgram_, "perspectiveMatrix");

	float near = 0.1f;
	float far = 3.0f;

	std::ranges::fill(PerspectiveMatrix_, 0.f);
	PerspectiveMatrix_(0, 0) = FrustumScale_ / (width / static_cast<float>(height));
	PerspectiveMatrix_(1, 1) = FrustumScale_;
	PerspectiveMatrix_(2, 2) = (far + near) / (near - far);
	PerspectiveMatrix_(2, 3) = (2 * far * near) / (near - far);
	PerspectiveMatrix_(3, 2) = -1.0f;

	glUseProgram(ShaderProgram_);
	// ``transpose`` determines means the matrix is in row-major order.
	glUniformMatrix4fv(PerspectiveMatrixUniform_, 1, GL_TRUE, PerspectiveMatrix_.data());
	glUseProgram(0);
}

void Engine3::Renderer::InitialiseVertexBufferObjects()
{
	glGenBuffers(1, &VertexBufferHandle_);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferHandle_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices_), Vertices_.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &IndexBufferHandle_);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferHandle_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(IndexData_), IndexData_.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Engine3::Renderer::InitialiseVertexArrayObjects()
{
	glGenVertexArrays(1, &VertexArrayHandle_);
	glBindVertexArray(VertexArrayHandle_);

	size_t colorDataOffset = sizeof(float) * 3 * numberOfVertices;
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferHandle_);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)colorDataOffset);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferHandle_);

	glBindVertexArray(0);
}

Engine3::Renderer::Renderer(Window& window) :
	OpenGLContext_{
		SDL_GL_CreateContext(window.Window_.get()),
		SDL_GL_DeleteContext
	},
	Window_{window}
{
	int value;
	SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &value);
	std::print("{}\n", value);

	// Check OpenGL context was correctly initialised.
	if (!OpenGLContext_)
	{
		std::print("{}", SDL_GetError());
		IsInitialised_ = false;
		assert(false);
	}

	// Initialize GLEW to setup OpenGL functions.
	GLenum glewStatus = glewInit();
	if (glewStatus != GLEW_OK)
	{
		// TODO: Why is glewGetErrorString an unsigned char? And why doesn't print work with it?
		std::print("{}\n", reinterpret_cast<const char*>(glewGetErrorString(glewStatus)));
		IsInitialised_ = false;
		assert(false);
	}

	// Create viewport. TODO: Why does this not seem to affect anything?
	std::pair<int, int> size = window.GetSize();
	glViewport(0, 0, size.first, size.second);

	/* Create Vertex Buffer Object */
	InitialiseProgram(window.GetSize().first, window.GetSize().second);
	InitialiseVertexBufferObjects();
	InitialiseVertexArrayObjects();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);
}

void Engine3::Renderer::Render()
{
	/* Clear the screen. */
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Draw to the screen*/
	glUseProgram(ShaderProgram_);

	glBindVertexArray(VertexArrayHandle_);
	glUniform3f(OffsetUniform_, 0.0f, 0.0f, -1.0f);
	glDrawElements(GL_TRIANGLES, IndexData_.size(), GL_UNSIGNED_SHORT, nullptr);

	glUniform3f(OffsetUniform_, 0.0f, 0.0f, -1.0f);
	glDrawElementsBaseVertex(GL_TRIANGLES, IndexData_.size(), GL_UNSIGNED_SHORT, nullptr, numberOfVertices / 2);

	glBindVertexArray(0);
	glUseProgram(0);

	/* Finally, swap the buffers. */
	SDL_GL_SwapWindow(Window_.Window_.get());
}

void Engine3::Renderer::SetSize(const int width, const int height)
{
	PerspectiveMatrix_(0, 0) = FrustumScale_ / (width / static_cast<float>(height));
	PerspectiveMatrix_(1, 1) = FrustumScale_;

	glUseProgram(ShaderProgram_);
	glUniformMatrix4fv(PerspectiveMatrixUniform_, 1, GL_TRUE, PerspectiveMatrix_.data());
	glUseProgram(0);

	glViewport(0, 0, width, height);
}
