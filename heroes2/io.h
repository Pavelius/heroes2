#pragma once

enum seek_flags { SeekSet, SeekCur, SeekEnd };
enum stream_flags {
	StreamRead = 1,
	StreamWrite = 2,
	StreamText = 4,
	StreamAppend = 8,
};

namespace io {
struct file {
	struct find {
		find(const char* url);
		~find();
		const char*		name();
		const char*		fullname(char* result);
		void			next();
		explicit operator bool() { return handle != 0; }
	private:
		char			path[261];
		char			reserved[512];
		void*			handle;
	};
	file(const char* url, unsigned flags = StreamRead);
	~file();
	operator bool() const { return handle != 0; }
	void				close();
	bool				create(const char* url, unsigned flags);
	static bool			exist(const char* url);
	static char*		getdir(char* url, int size);
	unsigned			getsize();
	static bool			makedir(const char* url);
	int					read(void* result, int count);
	static bool			remove(const char* url);
	int					seek(int count, int rel);
	static bool			setdir(const char* url);
	int					write(const void* result, int count);
private:
	int					handle;
};
}
const char*				szfname(const char* path);
const char*				szfnamewe(char* result, const char* name);
const char*				szext(const char* path);