#include "stdafx.h"

bool g_unloadModule;

unsigned int get_time_now()
{
	return sys_time_get_system_time() / 1000;
}

std::vector<scheduler::task> scheduler::main_tasks;
std::vector<scheduler::task> scheduler::render_tasks;
std::vector<scheduler::task> scheduler::dobj_tasks;

std::vector<scheduler::task>* scheduler::get_tasks(thread thread)
{
	if (thread == thread::main) return &main_tasks;
	else if (thread == thread::render) return &render_tasks;
	else if (thread == thread::dobj) return &dobj_tasks;
	else return {};
}

void scheduler::schedule(boost::function<void()> callback, uint64_t interval, scheduler::thread thread)
{
	scheduler::task task;
	task.callback = callback;
	task.interval = interval;
	task.thread = thread;
	task.last_call_time = get_time_now();
	task.is_temp = false;

	get_tasks(thread)->push_back(task);
}

void scheduler::once(boost::function<void()> callback, thread thread)
{
	scheduler::task task;
	task.callback = callback;
	task.interval = 0;
	task.thread = thread;
	task.last_call_time = get_time_now();
	task.is_temp = true;

	get_tasks(thread)->push_back(task);
}

void scheduler::delay(boost::function<void()> callback, uint64_t delay, thread thread)
{
	scheduler::task task;
	task.callback = callback;
	task.interval = delay;
	task.thread = thread;
	task.last_call_time = get_time_now();
	task.is_temp = true;

	get_tasks(thread)->push_back(task);
}


void scheduler::execute(thread thread)
{
	if (g_unloadModule) return;

	auto tasks = get_tasks(thread);
	for (int i = 0; i < tasks->size(); i++)
	{
		auto task = tasks->at(i);

		const auto diff = get_time_now() - task.last_call_time;
		if (diff < task.interval)
			continue;

		task.last_call_time = get_time_now();
		task.callback();

		if (task.is_temp)
		{
			tasks->erase(tasks->begin() + i);
			--i;
		}
	}
}

detour* R_EndFrame_d = nullptr;
void scheduler::R_EndFrame()
{
	scheduler::execute(scheduler::thread::render);
	R_EndFrame_d->invoke();
}

void scheduler::start()
{
	R_EndFrame_d = new detour(R_EndFrame_t, R_EndFrame);
}

void scheduler::stop()
{
	delete R_EndFrame_d;
}