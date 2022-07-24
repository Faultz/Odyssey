#include "stdafx.h"

#pragma diag_suppress 77
SYS_MODULE_INFO(odyssey_mw2, 0, 1, 1);
SYS_MODULE_START(start);
SYS_MODULE_STOP(stop);

extern "C"
{
	int start()
	{
		sys_ppu_thread_t thread;
		sys_ppu_thread_create(&thread, [](uint64_t /*unused*/) {
			scheduler::start();

			menu::start();

			renderables::start();

			sys_ppu_thread_exit(0);
		}, NULL, 0x7FC, 0x100, 0, "boot");

		return SYS_PRX_RESIDENT;
	}

	int stop()
	{
		g_unloadModule = true;

		libpsutil::sleep(300);

		scheduler::stop();

		menu::stop();

		renderables::stop();

		return SYS_PRX_RESIDENT;
	}
}
