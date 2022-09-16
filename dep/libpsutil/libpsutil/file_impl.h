#pragma once

#include <string>

//typedef uint32_t FILE;

#define O_CREAT         CELL_FS_O_CREAT
#define O_EXCL          CELL_FS_O_EXCL
#define O_TRUNC         CELL_FS_O_TRUNC
#define O_APPEND        CELL_FS_O_APPEND
#define O_ACCMODE       CELL_FS_O_ACCMODE
#define O_RDONLY        CELL_FS_O_RDONLY
#define O_RDWR          CELL_FS_O_RDWR
#define O_WRONLY        CELL_FS_O_WRONLY

// cell fs wrapper
class cell_fs
{
public:
	cell_fs() = default;
	cell_fs(const char* name, const char* mode);

	bool is_open();

	int open(const char* name, const char* mode);
	int write(const void* buf, size_t size, size_t count);
	int read(void* ptr, size_t size, size_t count);
	int read_off(int offset, void* ptr, size_t size, size_t count);

	int seek(int offset, int whence);
	int tell();
	int print(const char* fmt, ...);
	int close();

	size_t size();

	std::string name;
	int file_descriptor;
	int offset;
	unsigned int mode;
	unsigned int file_size;
};

void sys_setvalue(int key, void* val)
{
	if (void** ptr = (void**)__reg(13))
	{
		ptr[key - 1] = val;
	}
}

int __sflags(const char* mode, int* optr);
cell_fs* fsopen(const char* cell_fs, const char* mode);
size_t fswrite(const void* buf, size_t size, size_t count, cell_fs* fp);
size_t fsread(void* ptr, size_t size, size_t count, cell_fs* stream);
size_t fsread_off(int offset, void* ptr, size_t size, size_t count, cell_fs* stream);
int fsseek(cell_fs* stream, long int offset, int origin);
int fsprintf(cell_fs* fp, const char* fmt, ...);
int fstell(cell_fs* stream);
int fsclose(cell_fs* stream);
