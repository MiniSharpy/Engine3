#include <SDL.h>
#include "Core/Engine.h"
#include "Core/Events.h"
#include "Core/Renderer.h"
#include "Core/Window.h"
#include "Input/InputManager.h"
#include "Input/Conditions/PressedCondition.h"
#include "Input/Modifiers/DeadZoneModifier.h"
#include "Input/Modifiers/SwizzleModifier.h"
#include "Utility/BitFlags.h"

using namespace Engine3;

enum class RotationMode
{
	XYZ,
	XZY,
	YXZ,
	YZX,
	ZXY,
	ZYX
};

template <std::size_t RowSize, std::size_t ColumnSize, std::floating_point T>
	requires (RowSize == 3 && ColumnSize >= 3)
constexpr Matrix<RowSize, ColumnSize, T> EulerToMatrix(Vector<RowSize, T> euler, RotationMode mode)
{
	// TODO: How to handle 2D case? Overload would be best as it doesn't need RotationMode.

	// For 3D case, these methods just need to exist.
	Matrix x = Matrix<RowSize, ColumnSize, T>::RotationAboutX(euler.X());
	Matrix y = Matrix<RowSize, ColumnSize, T>::RotationAboutY(euler.Y());
	Matrix z = Matrix<RowSize, ColumnSize, T>::RotationAboutZ(euler.Z());

	// Euler angles rotate around the body axes (intrinsic), but as these change during rotation this is accounted
	// for by performing fixed-axis (extrinsic) rotation.
	switch (mode)
	{
	case RotationMode::XYZ:
		return z * y * x;
	case RotationMode::XZY:
		return y * z * x;
	case RotationMode::YXZ:
		return z * x * y;
	case RotationMode::YZX:
		return x * z * y;
	case RotationMode::ZXY:
		return y * x * z;
	case RotationMode::ZYX:
		return x * y * z;
	}
	std::unreachable();
}

template <std::size_t RowSize, std::size_t ColumnSize, std::floating_point T>
constexpr Matrix<RowSize, ColumnSize, T> EulerToMatrix(T euler)
{
	return Matrix<2, ColumnSize, T>::RotationAboutZ(euler);
}

template <std::size_t RowSize, std::size_t ColumnSize, std::floating_point T>
void PrintByRow(Matrix<RowSize, ColumnSize, T> matrix)
{
	for (int row = 0; row < RowSize; ++row)
	{
		for (int column = 0; column < ColumnSize; ++column)
		{
			//std::print("M{}{}: {} ", row + 1, column + 1, matrix(row, column));
			std::print("{} ", matrix(row, column));
		}
		std::print("\n");
	}
}

int main(int argc, char* argv[])
{
	Matrix a = Matrix<4, 3>::Unit();
	Matrix b = Matrix<3, 4>::Unit();
	Matrix c = Matrix<4, 4>::Unit();
	constexpr Matrix d = Matrix<4, 3>::Diagonal({1, 2, 3});
	constexpr Matrix e = Matrix<4, 3>::Diagonal(1.f, 2.f, 3.f);
	constexpr Matrix f = Matrix<2, 3>::ScalingAlongCardinalAxes(1, 2);


	constexpr int va = sizeof(Vector<3, float>);
	constexpr int vb = sizeof(Vector<3, int>);
	constexpr int vc = sizeof(int&);


	PrintByRow(a);
	std::print("\n");
	PrintByRow(b);
	std::print("\n");
	PrintByRow(c);
	std::print("\n");
	PrintByRow(d);
	std::print("\n");
	PrintByRow(e);
	std::print("\n");
	PrintByRow(f);


	return 0;

	Engine engine;
	if (!engine) { return EXIT_FAILURE; }

	Window window{u8"Engine3", 1280, 720, Window::OpenGL | Window::Resizable};
	if (!window) { return EXIT_FAILURE; }

	Renderer renderer(window);
	if (!renderer) { return EXIT_FAILURE; }

	InputManager inputManager;

	std::function printVector2 = [](Vector<2> value) { std::print("X:{} Y:{}\n", value.X(), value.Y()); };
	Action& mousePos = inputManager.AddAction(printVector2);
	mousePos.AddInput(Input::Mouse::Left).AddCondition<PressedCondition>();

	Action& leftAxis = inputManager.AddAction(printVector2);
	leftAxis.AddInput(Input::GamepadAxis::LeftX).AddModifier<DeadZoneModifier>();
	leftAxis.AddInput(Input::GamepadAxis::LeftY).AddModifier<DeadZoneModifier>().AddModifier<SwizzleModifier>();

	Events events;
	while (events.Process(window, renderer, inputManager))
	{
		engine.Update();
		renderer.Render();
	}

	return EXIT_SUCCESS;
}
