#pragma once
namespace Engine3
{
	class Engine
	{
	private:
		float deltaTime = 0;

		bool IsInitialised = true;

	public:
		Engine();

		~Engine();

		Engine(const Engine& other) = delete;

		Engine(Engine&& other) noexcept = delete;

		Engine& operator=(const Engine& other) = delete;

		Engine& operator=(Engine&& other) noexcept = delete;

		void Update();

		explicit operator bool() const;
	};
}
