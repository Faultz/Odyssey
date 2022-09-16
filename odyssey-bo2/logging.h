#pragma once

enum log_level
{
	LOG_NORMAL = 0,
	LOG_ERROR = 1,
	LOG_CRIT = 2,
};

struct log_data
{
	std::string message;
	log_level level;
	float y;
	float alpha;
	unsigned int start_time;
	unsigned int fade_out_interval;
	bool expired;
};

extern std::deque<log_data> log_queue;

class logging_t
{
public:
	void print(char* text, ...);
	void run();

	bool enabled;
	int grace_period;
};

extern logging_t g_log;