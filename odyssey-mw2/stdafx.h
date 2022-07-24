#pragma once

#pragma once

#include <cellstatus.h>
#include <sys/prx.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include <sys/prx.h>
#include <sys/syscall.h>
#include <sys/ppu_thread.h>
#include <sys/sys_time.h>
#include <sys/time_util.h>
#include <sys/process.h>
#include <sys/memory.h>
#include <sys/timer.h>
#include <sys/types.h>
#include <math.h>
#include <fastmath.h>
#include <cellstatus.h>
#include <sys/timer.h>
#include <cell/sysmodule.h>
#include <sys/random_number.h>
#include <ppu_intrinsics.h>
#include <spu_printf.h>
#include <ctype.h>
#include <cell\pad.h>
#include <cell\padfilter.h>
#include <cell\pad.h>
#include <cellstatus.h>
#include <sys/integertypes.h>
#include <algorithm>
#include <stack>
#include <functional>

#include <../dep/libpsutil/include/libpsutil.h>

#include <cell/gcm.h>
#include <gcmutil.h>

typedef void(*menu_callback_t)();

#define zero_memory(a, b) memset(a, 0, b) 

using namespace libpsutil::math;
using namespace libpsutil::memory;

template <typename T> static T clamp(const T& value, const T& low, const T& high)
{
	return value < low ? low : (value > high ? high : value);
}

inline uint32_t hash(const char* str)
{
    uint64_t hash = 0xCBF29CE484222325;

    while(*str)
    {
        hash =
            (hash ^ uint32_t((*str >= 'A' && *str <= 'Z') ? *str - ('A' - 'a') : *str)) * 0x100000001B3;
        str++;
    }

    return hash;
}

using namespace libpsutil;

#include "color.h"

#include "structures.h"
#include "globals.h"
#include "functions.h"

#include "keyboard.h"
#include "easing.h"
#include "animation.h"

#include "logging.h"

// render stuff
#include "previewer.h"
#include "engine.h"

#include "aimbot.h"
#include "antiaim.h"
#include "esp.h"

#include "graphics.h"

#include "scheduler.h"

#include "menu.h"