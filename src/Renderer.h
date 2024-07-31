#pragma once
#include "Window.h"
#include "gl/glew.h"
#include "Maths/Matrix.h"
#include <array>
#include <vector>

namespace Engine3
{
	class Renderer
	{
	private:
		std::unique_ptr<void, void(*)(void*)> OpenGLContext_ = {nullptr, nullptr};

		Window& Window_;

		bool IsInitialised_ = true;

		GLuint VertexBufferHandle_;

		GLuint VertexArrayHandle_;

		GLuint IndexBufferHandle_;

		GLuint ShaderProgram_;

		GLint OffsetUniform_;

		GLint PerspectiveMatrixUniform_;

		Matrix<4> PerspectiveMatrix_;

		float FrustumScale_ = 1.0f;

		const int numberOfVertices = 36;

#define RIGHT_EXTENT 0.8f
#define LEFT_EXTENT -RIGHT_EXTENT
#define TOP_EXTENT 0.20f
#define MIDDLE_EXTENT 0.0f
#define BOTTOM_EXTENT -TOP_EXTENT
#define FRONT_EXTENT -1.25f
#define REAR_EXTENT -1.75f

#define GREEN_COLOR 0.75f, 0.75f, 1.0f, 1.0f
#define BLUE_COLOR 	0.0f, 0.5f, 0.0f, 1.0f
#define RED_COLOR 1.0f, 0.0f, 0.0f, 1.0f
#define GREY_COLOR 0.8f, 0.8f, 0.8f, 1.0f
#define BROWN_COLOR 0.5f, 0.5f, 0.0f, 1.0f

		const std::array<float, 252> Vertices_{
			//Object 1 positions
			LEFT_EXTENT, TOP_EXTENT, REAR_EXTENT,
			LEFT_EXTENT, MIDDLE_EXTENT, FRONT_EXTENT,
			RIGHT_EXTENT, MIDDLE_EXTENT, FRONT_EXTENT,
			RIGHT_EXTENT, TOP_EXTENT, REAR_EXTENT,

			LEFT_EXTENT, BOTTOM_EXTENT, REAR_EXTENT,
			LEFT_EXTENT, MIDDLE_EXTENT, FRONT_EXTENT,
			RIGHT_EXTENT, MIDDLE_EXTENT, FRONT_EXTENT,
			RIGHT_EXTENT, BOTTOM_EXTENT, REAR_EXTENT,

			LEFT_EXTENT, TOP_EXTENT, REAR_EXTENT,
			LEFT_EXTENT, MIDDLE_EXTENT, FRONT_EXTENT,
			LEFT_EXTENT, BOTTOM_EXTENT, REAR_EXTENT,

			RIGHT_EXTENT, TOP_EXTENT, REAR_EXTENT,
			RIGHT_EXTENT, MIDDLE_EXTENT, FRONT_EXTENT,
			RIGHT_EXTENT, BOTTOM_EXTENT, REAR_EXTENT,

			LEFT_EXTENT, BOTTOM_EXTENT, REAR_EXTENT,
			LEFT_EXTENT, TOP_EXTENT, REAR_EXTENT,
			RIGHT_EXTENT, TOP_EXTENT, REAR_EXTENT,
			RIGHT_EXTENT, BOTTOM_EXTENT, REAR_EXTENT,

			//Object 2 positions
			TOP_EXTENT, RIGHT_EXTENT, REAR_EXTENT,
			MIDDLE_EXTENT, RIGHT_EXTENT, FRONT_EXTENT,
			MIDDLE_EXTENT, LEFT_EXTENT, FRONT_EXTENT,
			TOP_EXTENT, LEFT_EXTENT, REAR_EXTENT,

			BOTTOM_EXTENT, RIGHT_EXTENT, REAR_EXTENT,
			MIDDLE_EXTENT, RIGHT_EXTENT, FRONT_EXTENT,
			MIDDLE_EXTENT, LEFT_EXTENT, FRONT_EXTENT,
			BOTTOM_EXTENT, LEFT_EXTENT, REAR_EXTENT,

			TOP_EXTENT, RIGHT_EXTENT, REAR_EXTENT,
			MIDDLE_EXTENT, RIGHT_EXTENT, FRONT_EXTENT,
			BOTTOM_EXTENT, RIGHT_EXTENT, REAR_EXTENT,

			TOP_EXTENT, LEFT_EXTENT, REAR_EXTENT,
			MIDDLE_EXTENT, LEFT_EXTENT, FRONT_EXTENT,
			BOTTOM_EXTENT, LEFT_EXTENT, REAR_EXTENT,

			BOTTOM_EXTENT, RIGHT_EXTENT, REAR_EXTENT,
			TOP_EXTENT, RIGHT_EXTENT, REAR_EXTENT,
			TOP_EXTENT, LEFT_EXTENT, REAR_EXTENT,
			BOTTOM_EXTENT, LEFT_EXTENT, REAR_EXTENT,

			//Object 1 colors
			GREEN_COLOR,
			GREEN_COLOR,
			GREEN_COLOR,
			GREEN_COLOR,

			BLUE_COLOR,
			BLUE_COLOR,
			BLUE_COLOR,
			BLUE_COLOR,

			RED_COLOR,
			RED_COLOR,
			RED_COLOR,

			GREY_COLOR,
			GREY_COLOR,
			GREY_COLOR,

			BROWN_COLOR,
			BROWN_COLOR,
			BROWN_COLOR,
			BROWN_COLOR,

			//Object 2 colors
			RED_COLOR,
			RED_COLOR,
			RED_COLOR,
			RED_COLOR,

			BROWN_COLOR,
			BROWN_COLOR,
			BROWN_COLOR,
			BROWN_COLOR,

			BLUE_COLOR,
			BLUE_COLOR,
			BLUE_COLOR,

			GREEN_COLOR,
			GREEN_COLOR,
			GREEN_COLOR,

			GREY_COLOR,
			GREY_COLOR,
			GREY_COLOR,
			GREY_COLOR,
		};

		const std::array<GLshort, 24> IndexData_ =
		{
			//Object 1
			0, 2, 1, 3, 2, 0,
			4, 5, 6, 6, 7, 4,
			8, 9, 10, 11, 13, 12,
			14, 16, 15, 17, 16, 14,
		};

		static GLenum LoadShader(GLenum shaderType, std::string_view shaderFileName);

		static GLuint CreateProgram(const std::vector<GLuint>& shaderList);

		void InitialiseProgram(int width, int height);

		void InitialiseVertexBufferObjects();

		void InitialiseVertexArrayObjects();

	public:
		/* CONSTRUCTORS */
		Renderer(Window& window);

		~Renderer() = default;

		/* COPY AND MOVE OPERATIONS*/
		Renderer(const Renderer& other) = delete;

		Renderer(Renderer&& other) noexcept = delete;

		Renderer& operator=(const Renderer& other) = delete;

		Renderer& operator=(Renderer&& other) noexcept = delete;

		/* METHODS */
		void Render();

		void SetSize(const int width, const int height);

		/* CONVERSION OPERATORS */
		explicit operator bool() const { return IsInitialised_; }
	};
}
