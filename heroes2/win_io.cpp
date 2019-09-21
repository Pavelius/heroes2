#include "crt.h"
#include "io.h"
#include "win.h"

io::file::find::find(const char* url) {
	path[0] = 0;
	zcpy(path, url);
	char temp[261];
	zcpy(temp, path);
	zcat(temp, "/*.*");
	handle = FindFirstFileA(temp, (WIN32_FIND_DATA*)&reserved);
	if(handle == ((void*)-1))
		handle = 0;
}

const char* io::file::find::fullname(char* result) {
	zcpy(result, path);
	if(result[0])
		zcat(result, "/");
	zcat(result, name());
	return result;
}

void io::file::find::next() {
	if(!handle)
		return;
	if(!FindNextFileA(handle, (WIN32_FIND_DATA*)&reserved)) {
		FindClose(handle);
		handle = 0;
	}
}

io::file::find::~find() {
	if(handle)
		FindClose(handle);
	handle = 0;
}

const char* io::file::find::name() {
	return ((WIN32_FIND_DATA*)&reserved)->cFileName;
}

bool io::file::create(const char* url, unsigned flags) {
	if(handle)
		return true;
	unsigned io_right = (flags&(StreamWrite | StreamAppend)) ? GENERIC_WRITE : GENERIC_READ;
	unsigned io_flags = (flags&StreamWrite) ? CREATE_ALWAYS : OPEN_EXISTING;
	handle = (int)CreateFileA(url, io_right, 0, 0, io_flags, FILE_ATTRIBUTE_NORMAL, 0);
	if(handle == -1)
		handle = 0;
	else {
		if((flags&(StreamText | StreamWrite)) == (StreamText | StreamWrite)) {
			static unsigned char header_utf8[] = {0xEF, 0xBB, 0xBF};
			write(header_utf8, sizeof(header_utf8));
		}
	}
	return (*this);
}

void io::file::close() {
	CloseHandle((void*)handle);
	handle = 0;
}

char* io::file::getdir(char* url, int size) {
	if(!GetCurrentDirectoryA(size, url))
		return 0;
	return url;
}

bool io::file::setdir(const char* url) {
	return SetCurrentDirectoryA(url) != 0;
}

bool io::file::remove(const char* url) {
	return DeleteFileA(url) != 0;
}

int io::file::read(void* p, int size) {
	unsigned opsize;
	if(!ReadFile((void*)handle, p, size, &opsize, 0))
		return 0;
	return opsize;
}

int io::file::write(const void* p, int size) {
	unsigned opsize;
	if(!WriteFile((void*)handle, p, size, &opsize, 0))
		return 0;
	return opsize;
}

int io::file::seek(int pos, int rel) {
	int mm = FILE_BEGIN;
	switch(rel) {
	case SeekCur: mm = FILE_CURRENT; break;
	case SeekEnd: mm = FILE_END; break;
	}
	return SetFilePointer(handle, pos, 0, mm);
}

bool io::file::exist(const char* url) {
	return GetFileAttributesA(url) != 0xFFFFFFFF;
}

bool io::file::makedir(const char* url) {
	return CreateDirectoryA(url, 0) != 0;
}

unsigned io::file::getsize() {
	unsigned s = seek(0, SeekCur);
	unsigned r = seek(0, SeekEnd);
	seek(s, SeekSet);
	return r;
}

io::file::file(const char* url, unsigned flags) : handle(0) {
	create(url, flags);
}

io::file::~file() {
	close();
}

const char* szext(const char* path) {
	for(const char* r = zend((char*)path); r > path; r--) {
		if(*r == '.')
			return r + 1;
		else if(*r == '\\' || *r == '/')
			return 0;
	}
	return 0;
}

const char* szfname(const char* path) {
	for(const char* r = zend((char*)path); r > path; r--) {
		if(*r == '\\' || *r == '/')
			return r + 1;
	}
	return path;
}

const char* szfnamewe(char* result, const char* name) {
	zcpy(result, szfname(name));
	char* p = (char*)szext(result);
	if(p && p != result)
		p[-1] = 0;
	return result;
}

bool saveb(const char* url, const void* p, unsigned size) {
	io::file file(url, StreamWrite);
	if(!file)
		return false;
	file.write(p, size);
	return true;
}

bool loadb(const char* url, void* p, unsigned size) {
	io::file file(url, StreamRead);
	if(!file)
		return false;
	file.read(p, size);
	return true;
}