#pragma once

#include <netinet/in.h>
#include <stdarg.h>
#include <string>
#include <sys/prx.h>
#ifdef vector
#undef vector
#endif
#include <vector>

extern "C"
{
	int	_sys_printf(const char* format, ...);
	int	_sys_sprintf(char* s, const char* format, ...);
	int	_sys_vsprintf(char* s, const char* format, va_list arg);

	void* _sys_malloc(size_t n);
	void _sys_free(void* ptr);
}

#define printf			_sys_printf
#define sprintf			_sys_sprintf
#define vsprintf		_sys_vsprintf

namespace libpsutil
{
	uint32_t sys_dbg_read_process_memory(uint64_t address, void* data, size_t size);
	uint32_t sys_dbg_write_process_memory(uint64_t address, void* data, size_t size);
	void sleep(uint64_t milliseconds);
	
	namespace string
	{
		const char* va(const char* fmt, ...);

		std::string to_lower(const std::string& text);
		std::string to_upper(const std::string& text);
		bool begins_with(const std::string& text, const std::string& search);
		bool ends_with(const std::string& text, const std::string& search);
		std::vector<std::string> split(const std::string& text, char delimiter);
	};
	
	namespace cryptography
	{
		int rc4(char* key, char* plaintext, unsigned char* ciphertext);
		void md5(void* data, size_t length, unsigned char* digest);
		void sha1(void* data, size_t length, unsigned char* out);
		void sha256(void* data, size_t length, unsigned char* out);
		void sha512(void* data, size_t length, unsigned char* out);
		void tiger64(char* str, size_t length, uint64_t* res);
		void tiger128(char* str, size_t length, uint64_t res[2]);
		void tiger192(char* str, size_t length, uint64_t res[3]);
	}
	
	namespace filesystem
    {
        bool read_file(const std::string& file_name, void* data, size_t size);
        bool write_file(const std::string& file_name, void* data, size_t size);
        int64_t get_file_size(const std::string& file_name);

        bool directory_exists(const std::string& directory_name);
        bool file_exists(const std::string& file_name);

        void create_directory(const std::string& directory_name);
    }
	
	namespace memory
	{
		bool get(uint32_t address, void* data, size_t length);
		bool get(uint32_t address, std::vector<uint8_t>& bytes, size_t length);
		bool set(uint32_t address, void* data, size_t length);

		bool nop(uint32_t address);
		bool blr(uint32_t address);

		static void hook(uint32_t address, uint32_t function, uint32_t stub)
		{
			if(address == NULL || function == NULL || stub == NULL) return;

			uint32_t normal_stub[8], hook_stub[4];
			memory::get(address, normal_stub, 0x10);
			normal_stub[4] = 0x3D600000 + ((address + 0x10 >> 16) & 0xFFFF);
			normal_stub[5] = 0x616B0000 + (address + 0x10 & 0xFFFF);
			normal_stub[6] = 0x7D6903A6;
			normal_stub[7] = 0x4E800420;
			memory::set(stub, normal_stub, 0x20);
			hook_stub[0] = 0x3D600000 + ((function >> 16) & 0xFFFF);
			hook_stub[1] = 0x616B0000 + (function & 0xFFFF);
			hook_stub[2] = 0x7D6903A6;
			hook_stub[3] = 0x4E800420;
			memory::set(address, hook_stub, 0x10);
		}


		void jump(uint32_t address, uint32_t destination, bool linked = false);
		void jump_safe(uint32_t address, uint32_t destination, bool linked = false);

		uint32_t get_game_toc();

		template<class T> T get(uint32_t address)
		{
			T data;
			get(address, &data, sizeof(T));
			return data;
		}

		template<class T> std::vector<T> get(uint32_t address, size_t length)
		{
			std::vector<T> data;
			data.resize(length * sizeof(T));
			get(address, &data[0], length * sizeof(T));
			return data;
		}

		template<class T> bool set(uint32_t address, T data)
		{
			return set(address, &data, sizeof(T));
		}

		template<class T> bool set(uint32_t address, std::initializer_list<T> list)
		{
			return set(address, (void*)list.data(), list.size() * sizeof(T));
		}

		template <typename T> T byte_swap(T u)
		{
			union
			{
				T u;
				unsigned char u8[sizeof(T)];
			} source, dest;

			source.u = u;

			for (size_t k = 0; k < sizeof(T); k++)
				dest.u8[k] = source.u8[sizeof(T) - k - 1];

			return dest.u;
		}

		static int make_branch(int F, int T)
		{
			if (F > T)
				return 0x4C000000 - (F - T) + 1;
			else if (F < T)
				return T - F + 0x48000000 + 1;
			else
				return 0x48000001;
		}

		class detour
		{
		private:
			static uint8_t hook_stub_section[0x10000] __attribute__((section(".text")));
			static uint32_t hook_count;
			static uint32_t force_stub_addr;

			uint32_t* address;
			uint32_t stub_opd[2];
			uint32_t original_instructions[4];
			
			uint32_t allocate_stub()
			{
				uint8_t* stub_section = this->hook_stub_section;
				if(detour::force_stub_addr != 0)
				{
					stub_section = reinterpret_cast<uint8_t*>(detour::force_stub_addr);
				}

				auto stub_address = reinterpret_cast<uint32_t>(&stub_section[this->hook_count * 0x90]);
				this->hook_count++;

				return stub_address;
			}

			uint32_t resolve_branch(uint32_t instruction, uint32_t branch_address);

			void detour::setup_detour(uint32_t address, void* destination, uint32_t toc_override)
			{
				if (address == NULL)
				{
					return;
				}

				memcpy(this->original_instructions, reinterpret_cast<void*>(address), 0x10);

				this->address = reinterpret_cast<uint32_t*>(address);
				auto* stub_address = reinterpret_cast<uint32_t*>(this->allocate_stub());

				uint32_t instruction_count = 0;
				for (int i = 0; i < 4; i++)
				{
					auto current_address = reinterpret_cast<uint32_t>(&stub_address[instruction_count]);
					if ((this->address[i] & 0xF8000000) == 0x48000000)
					{
						int branch = memory::make_branch(current_address, this->resolve_branch(this->address[i], (int)&this->address[i]));
						memory::set(current_address, &branch, 4);
						instruction_count++;
					}
					else
					{
						memory::set(current_address, &this->address[i], 4);
						instruction_count++;
					}
				}

				memory::jump_safe(reinterpret_cast<uint32_t>(&stub_address[instruction_count]), address + 0x10, false);
				memory::jump(address, *reinterpret_cast<uint32_t*>(destination), false);

				this->stub_opd[0] = reinterpret_cast<uint32_t>(stub_address);
				this->stub_opd[1] = toc_override != 0 ? toc_override : memory::get_game_toc();
			}


		public:
			template<class T> detour(uint32_t address, T(*destination), uint32_t toc_override = 0)
			{
				this->setup_detour(address, reinterpret_cast<void*>(destination), toc_override);
			}

			~detour();

			static void force_stub_address(uint32_t address);

			template<typename T = void, typename... params> T invoke(params... parameters)
			{
				T(*original)(params...) = (T(*)(params...))this->stub_opd;
				return original(parameters...);
			}
		};
	}
	
	namespace network
	{
		std::string http_get(const std::string& url, std::string query, uint16_t port = 80);
		
		enum socket_type
		{
			SOCKET_TYPE_TCP,
			SOCKET_TYPE_UDP
		};

		class socket
		{
		private:
			int socket_;
			bool connected_;
			uint32_t ip_;
			uint16_t port_;
			socket_type type_;
			sockaddr_in server_addr;

		public:
			socket(const std::string& ip, uint16_t port, socket_type type = SOCKET_TYPE_TCP);
			socket(uint32_t ip, uint16_t port, socket_type type = SOCKET_TYPE_TCP);

			void close();
			bool connect();
			bool receive(void* data, size_t length);
			bool send(const void* data, size_t length);
		};
	}
	
	namespace math
	{
		int atoi(char* str);
		float atof(char* str);

		void srand(uint32_t seed);
		uint32_t rand();

		class vec2_t
		{
		public:
			float x, y;

			vec2_t();
			vec2_t(float x, float y);
			vec2_t(float vec[2]);

			inline vec2_t operator+(const vec2_t vec);
			inline vec2_t operator+(const float* vec);
			inline vec2_t operator-(const vec2_t vec);
			inline vec2_t operator-(const float* vec);
			inline vec2_t operator-() const;
			inline vec2_t operator*(const vec2_t vec);
			inline vec2_t operator*(const float scalar); 

			inline void operator-=(const vec2_t vec);
			inline void operator+=(const vec2_t vec);
			inline void operator*=(const vec2_t vec);
			inline void operator*=(const float scalar);

			inline bool operator==(const vec2_t vec);
			inline bool operator==(const float value);
			inline bool operator!=(const vec2_t vec);
			inline bool operator!=(const float value);

			vec2_t operator-(const vec2_t v) const
			{
				return { v.x - this->x, v.y - this->y };
			}

			vec2_t operator+(const vec2_t v) const
			{
				return { v.x + this->x, v.y + this->y };
			}

			operator float* ();
		};

		class vec4_t;
		class vec3_t
		{
		public:
			float x, y, z;

			vec3_t();
			vec3_t(float x, float y, float z);
			vec3_t(float vec[3]);

			inline vec3_t operator+(const vec3_t vec);
			inline vec3_t operator+(const float* vec);
			inline vec3_t operator-(const vec3_t vec);
			inline vec3_t operator-(const float* vec);
			inline vec3_t operator-() const;
			inline vec3_t operator*(const vec3_t vec);
			inline vec3_t operator*(const float scalar);

			inline void operator-=(const vec3_t vec);
			inline void operator+=(const vec3_t vec);
			inline void operator*=(const vec3_t vec);
			inline void operator*=(const float scalar);

			inline bool operator==(const vec3_t vec);
			inline bool operator==(const float value);
			inline bool operator!=(const vec3_t vec);
			inline bool operator!=(const float value);

			vec3_t operator-(const vec3_t v) const
			{
				return { this->x - v.x, this->y - v.y, this->z - v.z };
			}			
			
			vec3_t operator+(const vec3_t v) const
			{
				return { this->x + v.x, this->y + v.y, this->z + v.z };
			}
			operator float* ();
			vec3_t vec3_t::angles_forward(vec3_t position, float x)
			{
				vec3_t forward, right, up;
				position.vectors(&forward, &right, &up);

				return (*this + (forward * x));
			}

			vec3_t forward(float x = 1.0f);
			vec3_t normalize();
			float distance(vec3_t vec);
			float length();
			void vectors(vec3_t* forward, vec3_t* right, vec3_t* up);
		};

		class vec4_t
		{
		public:
			float x, y, z, w;

			vec4_t();
			vec4_t(float x, float y, float z, float w);
			vec4_t(float vec[4]);

			vec4_t operator+(const vec4_t vec);
			vec4_t operator+(const float* vec);
			vec4_t operator-(const vec4_t vec);
			vec4_t operator-(const float* vec);
			vec4_t operator-() const;
			vec4_t operator*(const vec4_t vec);
			vec4_t operator*(const float scalar);

			void operator-=(const vec4_t vec);
			void operator+=(const vec4_t vec);
			void operator*=(const vec4_t vec);
			void operator*=(const float scalar);

			bool operator==(const vec4_t vec);
			bool operator==(const float value);
			bool operator!=(const vec4_t vec);
			bool operator!=(const float value);

			operator float* ();
		};
	}
	
    namespace symbol_helper
    {
		struct opd_s
		{
			unsigned int address;
			unsigned int toc;
		};

		static std::vector<opd_s> symbol_table;
		static void* get_symbol(unsigned int address, unsigned int toc)
		{
			for(auto i = 0; i < symbol_table.size(); i++)
			{
				opd_s* opd = &symbol_table[i];
				if(opd->address == address)
					return opd;
			}

			symbol_table.push_back(opd_s());
			opd_s* opd = &symbol_table[symbol_table.size() - 1];
			opd->address = address;
			opd->toc = toc == 0 ? memory::get_game_toc() : toc;

			return opd;
		}
    }

    template<typename T> class symbol
    {
    public:
        operator T* ()
        {
            T* type = *reinterpret_cast<T*>(this->mp_object_);
            return (decltype(type))symbol_helper::get_symbol(this->mp_object_, toc);
        }

        T* get()
        {
            return this->mp_object_;
        }

        int mp_object_;
		int toc;
    };
}