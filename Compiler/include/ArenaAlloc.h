#ifndef ARENAALLOC_HEADER_H
#define ARENAALLOC_HEADER_H

#include <cstdlib>

//TODO make a real C++ style allocator instead of this more C style one
// This is a temporary allocator and will be replaced soon with a standards compliant allocator
class ArenaAlloc {

private:
	char* start;
	char* pos;
	char* end;

	void reallocate() {
		size_t offset = pos - start;
		size_t newSize = static_cast<size_t>(static_cast<size_t>(end - start) * 1.5);
		start = reinterpret_cast<char*>(realloc(start, newSize));
		pos = start + offset;
		end = start + newSize;
	}

public:
	ArenaAlloc(size_t size) {
		start = static_cast<char*>(malloc(size));
		pos = start;
		end = (start + size);
	}

	template<typename T>
	T* alloc() {
		while (pos + sizeof(T) + 1 >= end)
			reallocate();
		T* ptr = reinterpret_cast<T*>(pos);
		pos += sizeof(T);
		return ptr;
	}

	template<typename T>
	T* alloc(size_t size) {
		while (pos + size >= end)
			reallocate();
		T* ptr = reinterpret_cast<T*>(pos);
		pos += size;
		return ptr;
	}

	inline void freeArena() {
		free(start);
	}


};
#endif