#include "stdafx.h"

#define clamp(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))

animations g_anim;

void animation_data::process_animation()
{
	float elapsed_time = static_cast<float>(move - start_data);
	float delta_time = static_cast<float>(get_time_now() - start_time);
	float scale = clamp(delta_time / duration, 0.0f, 1.0f);
	float progression = static_cast<float>(elapsed_time * interpolate(scale));

	*data = start_data + progression;

	if (get_time_now() >= end_time)
	{
		for (int i = 0; i < g_anim.animation_stack.size(); i++)
		{
			if (this->id == g_anim.animation_stack[i].id)
			{
				auto callback = g_anim.animation_stack[i].callback;
				if (callback)
					callback();

				g_anim.animation_stack.erase(g_anim.animation_stack.begin() + i);
			}
		}
	}
}

void animation_data::add_animation(const char* name, void(*callback)(), float(*interpolate)(float), float* data, int* move, int duration)
{
	if (animations::already_added(name))
		return;

	animation_data newAnim = animation_data();
	newAnim.id = hash(name);
	newAnim.interpolate = interpolate;
	newAnim.start_time = get_time_now();
	newAnim.duration = duration;
	newAnim.end_time = newAnim.start_time + newAnim.duration;

	newAnim.data = data;
	newAnim.move = *(float*)move;
	newAnim.start_data = *(float*)newAnim.data;

	newAnim.callback = callback;

	g_anim.animation_stack.push_back(newAnim);
}

void animations::add_animation(const char* name, void(*callback)(), float(*interpolate)(float), float* data, float move, clock_t duration)
{
	if (*data == move || interpolate == nullptr) return;

	animation_data::add_animation(name, callback, interpolate, (float*)data, (int*)&move, duration);
}

void animations::add_animation(const char* name, void(*callback)(), float(*interpolate)(float), int* data, int move, clock_t duration)
{
	if (*data == move || interpolate == nullptr) return;

	animation_data::add_animation(name, callback, interpolate, (float*)data, &move, duration);
}

void animations::add_animation(const char* name, void(*callback)(), float(*interpolate)(float), vec2_t* data, vec2_t move, clock_t duration)
{
	if (*data == move || interpolate == nullptr) return;

	animation_data::add_animation(name, callback, interpolate, (float*)&data->x, (int*)&move.x, duration);
	animation_data::add_animation(va("%s_2", name), callback, interpolate, (float*)&data->y, (int*)&move.y, duration);
}

void animations::add_animation(const char* name, void(*callback)(), float(*interpolate)(float), vec3_t* data, vec3_t move, clock_t duration)
{
	if (*data == move || interpolate == nullptr) return;

	animation_data::add_animation(name, callback, interpolate, (float*)&data->x, (int*)&move.x, duration);
	animation_data::add_animation(va("%s_1", name), callback, interpolate, (float*)&data->y, (int*)&move.y, duration);
	animation_data::add_animation(va("%s_2", name), callback, interpolate, (float*)&data->z, (int*)&move.z, duration);
}

void animations::run_frame()
{
	for (animation_data anim : animation_stack)
		anim.process_animation();
}

animation_data* animations::find_anim(anim_id id)
{
	for (auto anim : g_anim.animation_stack)
		if (anim.id == id)
			return &anim;

	return nullptr;
}

animation_data* animations::find_anim(const char* id_name)
{
	const auto& hashed_id = hash(id_name);
	for (auto anim : g_anim.animation_stack)
		if (anim.id == hashed_id)
			return &anim;

	return nullptr;
}

bool animations::already_added(anim_id id)
{
	for (int i = 0; i < g_anim.animation_stack.size(); i++)
	{
		if (g_anim.animation_stack[i].id == id)
			return true;
	}

	return false;
}

bool animations::already_added(const char* id_name)
{
	for (int i = 0; i < g_anim.animation_stack.size(); i++)
	{
		if (g_anim.animation_stack[i].id == hash(id_name))
			return true;
	}

	return false;
}
