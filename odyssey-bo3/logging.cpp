#include "stdafx.h"

logging_t g_log;
std::deque<log_data> log_queue;

void logging_t::print(char* text, ...)
{
	if (g_log.enabled == false)
		return;

	static char buffer[0x2000];
	va_list va;
	va_start(va, text);
	vsprintf(buffer, text, va);

	log_data _log = { buffer, LOG_NORMAL, 15.f, 255.f, get_time_now(), 1000, 9000 };
	
	if (strlen(buffer) >= 256)
		return;

	log_queue.push_back(_log);
}

void logging_t::run()
{
	if (log_queue.empty() || g_log.enabled == false)
		return;

	std::vector<int> text_sizes;
	for (int i = 0; i < log_queue.size(); i++)
	{
		text_sizes.push(g_graphics.get_text_width(log_queue[i].message.data(), .6f));
	}

	std::sort(text_sizes.begin(), text_sizes.end(), [](int r3, int r4) {
		return r3 > r4;
	});

	static float log_x = 0.f, log_y = 0.f;
	calculate_interpolation((log_queue.size() * (g_fontNormal->pixelHeight * .6f)) + 5.f, 9, log_y);
	calculate_interpolation(text_sizes.front() + 25.f, 5, log_x);

	GRect log_rect { 5.f, 15.f, log_x, log_y };
	GRect log_highlight_rect { 5.f - 1.5f, 15.f - 1.5f, log_x + 3.f, log_y + 3.f };

	GColor log_background_color { 15, 15, 15, 255 };

	g_graphics.add_filled_rect(log_highlight_rect, g_vars->menu.accent_color);
	g_graphics.add_filled_rect(log_rect, log_background_color);

	for (int i = 0; i < log_queue.size(); i++)
	{
		log_data& _log = log_queue[i];

		calculate_interpolation(15.f + (i * g_graphics.get_text_height(_log.message.data(), .6f)), 10, _log.y);

		GRect log_data_rect { 15.f, _log.y, 300.f, 20.f };

		if (get_time_now() - _log.start_time > grace_period)
		{
			_log.expired = true;
			calculate_interpolation(0.f, 5, _log.alpha);
		}

		if (_log.alpha <= 0.4)
		{
			log_queue.erase(log_queue.begin() + i);
			continue;
		}

		g_graphics.add_text(_log.message.data(), log_data_rect, 0, horz_left | vert_center, .6f, .6f, color(255, 255, 255, _log.alpha));
	}
}