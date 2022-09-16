#pragma once

extern bool g_unloadModule;

unsigned int get_time_now();

class scheduler
{
public:
	enum thread
	{
		main,
		render,
		dobj
	};


	static void start();
	static void stop();

	static void schedule(menu_callback_t callback, uint64_t interval = 0, thread thread = main);
	static void once(menu_callback_t callback, thread thread = main);
	static void delay(menu_callback_t callback, uint64_t delay, thread thread = main);

	struct task
	{
		scheduler::thread thread;
		menu_callback_t callback;
		uint64_t interval;
		uint64_t last_call_time;
		bool is_temp;
	};

	static std::vector<task> main_tasks;
	static std::vector<task> render_tasks;

	static std::vector<task>* get_tasks(thread thread);

	static void R_EndFrame();

	static void execute(thread thread);
};