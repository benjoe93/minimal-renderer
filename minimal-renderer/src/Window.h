#pragma once

struct GLFWwindow;
class Renderer;

struct AppState 
{
	Renderer* renderer;
	bool first_mouse = true;
	float last_x = 0.0f;
	float last_y = 0.0f;
};


class MainWindow
{
	private:
		GLFWwindow* window = nullptr;
		Renderer* renderer = nullptr;

		unsigned int scr_widht = 800;
		unsigned int scr_height = 600;

		bool cursor_disabled = false;

	public:
		MainWindow();
		~MainWindow();

		void processInput();

		inline GLFWwindow* GetWindow() const { return window; };
};
