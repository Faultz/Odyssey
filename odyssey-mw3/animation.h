#pragma once

typedef uint32_t anim_id;
class animation_data
{
public:
	anim_id id;

	int start_time;
	int move_time;
	int end_time;

	void(*callback)();
	float(*interpolate)(float val);

	float* data;
	float start_data;
	float move;

	int duration;

	void process_animation();
	static void add_animation(const char* name, void(*callback)(), float(*interpolate)(float), float* data, int* move, int duration);
};

class animations
{
public:
	~animations()
	{
		animation_stack.clear();
	}

	void add_animation(const char* name, void(*callback)(), float(*interpolate)(float), float* data, float move, clock_t duration);
	void add_animation(const char* name, void(*callback)(), float(*interpolate)(float), int* data, int move, clock_t duration);
	void add_animation(const char* name, void(*callback)(), float(*interpolate)(float), vec2_t* data, vec2_t move, clock_t duration);
	void add_animation(const char* name, void(*callback)(), float(*interpolate)(float), vec3_t* data, vec3_t move, clock_t duration);
	void run_frame();

	animation_data* find_anim(anim_id id);
	animation_data* find_anim(const char* id_name);

	static bool already_added(anim_id id);
	static bool already_added(const char* id_name);

	std::vector<animation_data> animation_stack;
};

extern animations g_anim;