#pragma once

class linear
{
public:
    static float ease(float t)
    {
        return t;
    };
};

class in_sine
{
public:
    static float ease(float t)
    {
        return sinf(1.5707963f * t);
    };
}; 

class out_sine
{
public:
    static float ease(float t)
    {
        return 1.f + sinf(1.5707963f * (--t));
    };
};

class in_out_sine
{
public:
    static float ease(float t)
    {
        return 0.5f * (1.f + sinf(3.1415926f * (t - 0.5f)));
    };
};

class inquad
{
public:
    static float ease(float t)
    {
        return t * t;
    };
};

class out_quad
{
public:
    static float ease(float t)
    {
        return t * (2.f - t);
    };
};

class in_out_quad
{
public:
    static float ease(float t)
    {
        return t < 0.5f ? 2.f * t * t : t * (4.f - 2.f * t) - 1.f;
    };
};

class in_cubic
{
public:
    static float ease(float t)
    {
        return t * t * t;
    };
};

class out_cubic
{
public:
    static float ease(float t)
    {
        return 1.f + (--t) * t * t;
    };
};


class in_out_cubic
{
public:
    static float ease(float t)
    {
        return t < 0.5f ? 4.f * t * t * t : 1.f + (--t) * (2.f * (--t)) * (2.f * t);
    };
};


class in_quart
{
public:
    static float ease(float t)
    {
        t *= t;
        return t * t;
    };
};


class out_quart
{
public:
    static float ease(float t)
    {
        t = (--t) * t;
        return 1.f - t * t;
    };
};


class in_out_quart
{
public:
    static float ease(float t)
    {
        if (t < 0.5f)
        {
            t *= t;
            return 8.f * t * t;
        }
        else
        {
            t = (--t) * t;
            return 1.f - 8.f * t * t;
        }
    };
};

class in_quint
{
public:
    static float ease(float t)
    {
        const float t2 = t * t;
        return t * t2 * t2;
    };
};

class out_quint
{
public:
    static float ease(float t)
    {
        const float t2 = (--t) * t;
        return 1.f + t * t2 * t2;
    };
};

class in_out_quint
{
public:
    static float ease(float t)
    {
        float t2;
        if (t < 0.5f)
        {
            t2 = t * t;
            return 16.f * t * t2 * t2;
        }
        else
        {
            t2 = (--t) * t;
            return 1.f + 16.f * t * t2 * t2;
        }
    };
};

class in_expo
{
public:
    static float ease(float t)
    {
        return (powf(2.f, 8.f * t) - 1.f) / 255.f;
    };
};

class out_expo
{
public:
    static float ease(float t)
    {
        return 1.f - powf(2.f, -8.f * t);;
    };
};

class in_out_expo
{
public:
    static float ease(float t)
    {
        if (t < 0.5)
        {
            return (powf(2, 16 * t) - 1) / 510;
        }
        else
        {
            return 1 - 0.5 * powf(2, -16 * (t - 0.5));
        }
    };
};

class in_circ
{
public:
    static float ease(float t)
    {
        return 1.f - sqrtf(1.f - t);
    };
};

class out_circ
{
public:
    static float ease(float t)
    {
        return sqrtf(t);
    };
};

class in_out_circ
{
public:
    static float ease(float t)
    {
        if (t < 0.5)
        {
            return (1 - sqrtf(1 - 2 * t)) * 0.5;
        }
        else
        {
            return (1 + sqrtf(2 * t - 1)) * 0.5;
        }
    };
};

class in_back
{
public:
    static float ease(float t)
    {
        return t * t * (2.70158f * t - 1.70158f);
    };
};

class out_back
{
public:
    static float ease(float t)
    {
        return 1.f + (--t) * t * (2.70158f * t + 1.70158f);
    };
};

class in_out_back
{
public:
    static float ease(float t)
    {
        if (t < 0.5f)
        {
            return t * t * (7.f * t - 2.5f) * 2;
        }
        else
        {
            return 1.f + (--t) * t * 2.f * (7.f * t + 2.5f);
        }
    };
};

class in_elastic
{
public:
    static float ease(float t)
    {
        const float t2 = t * t;
        return t2 * t2 * sinf(t * M_PI * 4.5f);
    };
};

class out_elastic
{
public:
    static float ease(float t)
    {
        const float t2 = (t - 1.f) * (t - 1.f);
        return 1 - t2 * t2 * cosf(t * M_PI * 4.5f);
    };
};

class in_out_elastic
{
public:
    static float ease(float t)
    {
        float t2;
        if (t < 0.45f)
        {
            t2 = t * t;
            return 8.f * t2 * t2 * sinf(t * M_PI * 9.f);
        }
        else if (t < 0.55f)
        {
            return 0.5f + 0.75f * sinf(t * M_PI * 4.f);
        }
        else
        {
            t2 = (t - 1) * (t - 1);
            return 1.f - 8.f * t2 * t2 * sinf(t * M_PI * 9.f);
        }
    };
};

class in_bounce
{
public:
    static float ease(float t)
    {
        return powf(2.f, 6.f * (t - 1.f)) * fabsf(sinf(t * M_PI * 3.5f));
    };
};

class out_bounce
{
public:
    static float ease(float t)
    {
        return 1.f - powf(2.f, -6.f * t) * fabsf(cosf(t * M_PI * 3.5));
    };
};

class in_out_bounce
{
public:
    static float ease(float t)
    {
        if (t < 0.5f)
        {
            return 8.f * powf(2.f, 8.f * (t - 1.f)) * fabsf(sinf(t * M_PI * 7.f));
        }
        else
        {
            return 1.f - 8.f * powf(2.f, -8.f * t) * fabsf(sinf(t * M_PI * 7.f));
        }
    };
};

struct anim_context_t
{
    float value;
    unsigned int id;

    void adjust(float adjust_to, bool clamp = true);
};

struct animation_controller_t
{

    anim_context_t get(std::string str);

    void clear_stack();

    template<class T>
    float ease(float t)
    {
        return T::ease(t);
    };
};


namespace easing
{
    constexpr float m_pi = 3.14159265358979323846;
    inline float in_sine(const float t)
    {
        return sinf(1.5707963 * t);
    }
    inline float out_sine(float t)
    {
        return 1 + sinf(1.5707963 * (--t));
    }
    inline float in_out_sine(const float t)
    {
        return 0.5 * (1 + sinf(3.1415926 * (t - 0.5)));
    }
    inline float inquad(const float t)
    {
        return t * t;
    }
    inline float out_quad(const float t)
    {
        return t * (2 - t);
    }
    inline float in_out_quad(const float t)
    {
        return t < 0.5 ? 2 * t * t : t * (4 - 2 * t) - 1;
    }
    inline float in_cubic(const float t)
    {
        return t * t * t;
    }
    inline float out_cubic(float t)
    {
        return 1 + (--t) * t * t;
    }
    inline float in_out_cubic(float t)
    {
        return t < 0.5 ? 4 * t * t * t : 1 + (--t) * (2 * (--t)) * (2 * t);
    }
    inline float in_quart(float t)
    {
        t *= t;
        return t * t;
    }
    inline float out_quart(float t)
    {
        t = (--t) * t;
        return 1 - t * t;
    }
    inline float in_out_quart(float t)
    {
        if (t < 0.5)
        {
            t *= t;
            return 8 * t * t;
        }
        else
        {
            t = (--t) * t;
            return 1 - 8 * t * t;
        }
    }
    inline float in_quint(const float t)
    {
        const float t2 = t * t;
        return t * t2 * t2;
    }
    inline float out_quint(float t)
    {
        const float t2 = (--t) * t;
        return 1 + t * t2 * t2;
    }
    inline float in_out_quint(float t)
    {
        float t2;
        if (t < 0.5)
        {
            t2 = t * t;
            return 16 * t * t2 * t2;
        }
        else
        {
            t2 = (--t) * t;
            return 1 + 16 * t * t2 * t2;
        }
    }
    inline float in_expo(const float t)
    {
        return (powf(2, 8 * t) - 1) / 255;
    }
    inline float out_expo(const float t)
    {
        return 1 - powf(2, -8 * t);
    }
    inline float in_out_expo(const float t)
    {
        if (t < 0.5)
        {
            return (powf(2, 16 * t) - 1) / 510;
        }
        else
        {
            return 1 - 0.5 * powf(2, -16 * (t - 0.5));
        }
    }
    inline float in_circ(const float t)
    {
        return 1 - sqrtf(1 - t);
    }
    inline float out_circ(const float t)
    {
        return sqrtf(t);
    }
    inline float in_out_circ(const float t)
    {
        if (t < 0.5)
        {
            return (1 - sqrtf(1 - 2 * t)) * 0.5;
        }
        else
        {
            return (1 + sqrtf(2 * t - 1)) * 0.5;
        }
    }
    inline float in_back(const float t)
    {
        return t * t * (2.70158 * t - 1.70158);
    }
    inline float out_back(float t)
    {
        return 1 + (--t) * t * (2.70158 * t + 1.70158);
    }
    inline  float in_out_back(float t)
    {
        if (t < 0.5)
        {
            return t * t * (7 * t - 2.5) * 2;
        }
        else
        {
            return 1 + (--t) * t * 2 * (7 * t + 2.5);
        }
    }
    inline float in_elastic(const float t)
    {
        const float t2 = t * t;
        return t2 * t2 * sinf(t * m_pi * 4.5);
    }
    inline float out_elastic(const float t)
    {
        const float t2 = (t - 1) * (t - 1);
        return 1 - t2 * t2 * cosf(t * m_pi * 4.5);
    }
    inline float in_out_elastic(const float t)
    {
        float t2;
        if (t < 0.45)
        {
            t2 = t * t;
            return 8 * t2 * t2 * sinf(t * m_pi * 9);
        }
        else if (t < 0.55)
        {
            return 0.5 + 0.75 * sinf(t * m_pi * 4);
        }
        else
        {
            t2 = (t - 1) * (t - 1);
            return 1 - 8 * t2 * t2 * sinf(t * m_pi * 9);
        }
    }
    inline float in_bounce(const float t)
    {
        return powf(2, 6 * (t - 1)) * fabsf(sinf(t * m_pi * 3.5));
    }
    inline float out_bounce(const float t)
    {
        return 1 - powf(2, -6 * t) * fabsf(cosf(t * m_pi * 3.5));
    }
    inline float in_out_bounce(const float t)
    {
        if (t < 0.5)
        {
            return 8 * powf(2, 8 * (t - 1)) * fabsf(sinf(t * m_pi * 7));
        }
        else
        {
            return 1 - 8 * powf(2, -8 * t) * fabsf(sinf(t * m_pi * 7));
        }
    }
}