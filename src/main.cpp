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
	// TODO: How to handle 2D case?

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
	requires (RowSize == 2 && ColumnSize >= 2)
constexpr Matrix<RowSize, ColumnSize, T> EulerToMatrix(T euler)
{
	return Matrix<2, ColumnSize, T>::RotationAboutZ(euler);
}

int main(int argc, char* argv[])
{
	Vector<2> point{5, 10};
	Matrix rotationMatrix = EulerToMatrix<2, 2, float>(DegreesToRadians(90.f));
	Vector<2> result = point * rotationMatrix;


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
