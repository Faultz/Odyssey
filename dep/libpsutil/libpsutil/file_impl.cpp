#include "file_impl.h"

#include <cell/cell_fs.h>

#include "system/imports.hpp"

#include <errno.h>

int __sflags(const char* mode, int* optr)
{
	int ret, m, o;
	switch (*mode++)
	{
		case 'r':	/* open for reading */
			ret = 2;
			m = O_RDONLY;
			o = 0;
			break;
		case 'w':	/* open for writing */
			ret = 1;
			m = O_WRONLY;
			o = O_CREAT | O_TRUNC;
			break;
		case 'a':	/* open for appending */
			ret = 1;
			m = O_WRONLY;
			o = O_CREAT | O_APPEND;
			break;
		default:	/* illegal mode */
			return (0);
	}
	/* [rwa]\+ or [rwa]b\+ means read and write */
	if (*mode == '+' || (*mode == 'b' && mode[1] == '+'))
	{
		ret = 1;
		m = O_RDWR;
	}
	*optr = m | o;
	return (ret);
}

cell_fs* fsopen(const char* name, const char* mode)
{
	cell_fs fp;
	int f;
	int flags, oflags;
	if ((flags = __sflags(mode, &oflags)) == 0)
		return (NULL);
	if ((cellFsOpen(name, oflags, &fp.file_descriptor, 0, 0)) != CELL_FS_SUCCEEDED)
		return (NULL);

	if (oflags & O_APPEND)
	{
		uint64_t offset;
		cellFsLseek(fp.file_descriptor, 0, SEEK_END, &offset);
	}

	fp.name = name;
	return &fp;
}

size_t fswrite(const void* buf, size_t size, size_t count, cell_fs* fp)
{
	int err = 0;

	if (!fp->file_descriptor) return;

	uint64_t file_bytes_written;
	if ((err = cellFsWrite(fp->file_descriptor, buf, size * count, &file_bytes_written)) == CELL_FS_SUCCEEDED)
		return file_bytes_written;
	else
		return NULL;
}

size_t fsread(void* ptr, size_t size, size_t count, cell_fs* stream)
{
	int err = 0;

	if (!stream->file_descriptor) return;

	uint64_t file_bytes_read;
	if ((err = cellFsRead(stream->file_descriptor, ptr, size * count, &file_bytes_read)) == CELL_FS_SUCCEEDED)
	{
		stream->offset = fstell(stream);
		return file_bytes_read;
	}
	else
		return NULL;

}

size_t fsread_off(int offset, void* ptr, size_t size, size_t count, cell_fs* stream)
{
	int err = 0;

	if (!stream->file_descriptor) return;

	uint64_t file_bytes_read;
	if ((err = cellFsReadWithOffset(stream->file_descriptor, offset, ptr, size * count, &file_bytes_read)) == CELL_FS_SUCCEEDED)
	{
		stream->offset = fstell(stream);
		return file_bytes_read;
	}
	else
		return NULL;

}

int fsseek(cell_fs* stream, long int offset, int origin)
{
	int err = 0;

	if (!stream->file_descriptor) return;

	uint64_t _offset;
	if ((err = cellFsLseek(stream->file_descriptor, offset, origin, (uint64_t*) &_offset)) == CELL_FS_SUCCEEDED)
	{
		stream->offset = _offset;
		return 0;
	}
	else
		return -1;
}

int fsprintf(cell_fs* fp, const char* fmt, ...)
{
	static char buffer[2048];
	int ret;
	va_list ap;
	va_start(ap, fmt);
	ret = vsprintf(buffer, fmt, ap);
	va_end(ap);

	fswrite(buffer, strlen(buffer), 1, fp);
	return (ret);
}

int fstell(cell_fs* stream)
{
	int err = 0;

	if (!stream->file_descriptor) return;

	uint64_t seek_position;
	if ((err = cellFsLseek(stream->file_descriptor, 0, SEEK_CUR, &seek_position)) == CELL_FS_SUCCEEDED)
		return seek_position;
	else
		return -1;
}

int fsclose(cell_fs* stream)
{
	int err = 0;

	if (!stream->file_descriptor) return;

	if ((err = cellFsClose(stream->file_descriptor)) == CELL_FS_SUCCEEDED)
		return err;
	else
		return 0;
}

cell_fs::cell_fs(const char* name, const char* mode)
{
	open(name, mode);
}

bool cell_fs::is_open()
{
	return file_descriptor != 0;
}

int cell_fs::open(const char* name, const char* mode)
{
	cell_fs* file = fsopen(name, mode);
	if (file)
	{
		memcpy(this, file, sizeof(cell_fs));
		return 1;
	}
	else
		return 0;
}

int cell_fs::write(const void* buf, size_t size, size_t count)
{
	return fswrite(buf, size, count, this);
}

int cell_fs::read(void* ptr, size_t size, size_t count)
{
	return fsread(ptr, size, count, this);
}

int cell_fs::read_off(int offset, void* ptr, size_t size, size_t count)
{
	return fsread_off(offset, ptr, size, count, this);
}

int cell_fs::seek(int offset, int whence)
{
	return fsseek(this, offset, whence);
}

int cell_fs::tell()
{
	return fstell(this);
}

int cell_fs::print(const char* fmt, ...)
{
	static char buffer[2048];
	int ret;
	va_list ap;
	va_start(ap, fmt);
	ret = vsprintf(buffer, fmt, ap);
	va_end(ap);

	fswrite(buffer, strlen(buffer), 1, this);
	return (ret);
}

int cell_fs::close()
{
	return fsclose(this);
}

size_t cell_fs::size()
{
	CellFsStat stat;
	if ((cellFsStat(name.data(), &stat)) == CELL_FS_SUCCEEDED)
	{
		return static_cast<int64_t>(stat.st_size);
	}

	return 0;
}
