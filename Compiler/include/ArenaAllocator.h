// TODO 
// This currently works fine and I started to set up the rest of the codebase to use it effectively
// However, since I would have to use construct_at to contruct the objects in place I can't use this allocator
// for the AST nodes. At the moment only the latest versions of gcc and msvc support the feature I need.
// I need to be able to contruct a struct that inherits from another struct using contruct_at. 
// When this feature has better support I will convert all allocations in the AST to use this allocator.
//
// TODO
// This is also a problem for the lexer at the moment. However, there is an easy way to workaround this
// I just need to find the time to do it.
//

#include <type_traits>
#include <iostream>

#ifndef ARENA_ALLOCATOR_H
#define ARENA_ALLOCATOR_H

#ifndef ARENA_ALLOCATOR_INITIAL_SIZE
#define ARENA_ALLOCATOR_INITIAL_SIZE 1024
#endif

class internal_allocator_arena {
    public:

        char* start;
        char* current;
        char* end;

        internal_allocator_arena():
            start(reinterpret_cast<char*>(::operator new(ARENA_ALLOCATOR_INITIAL_SIZE))),
            current(start),
            end(start + ARENA_ALLOCATOR_INITIAL_SIZE)
        {

        }

        template<typename T>
        constexpr T* alloc(size_t bytes) {
            char* old_current = current;
            current += bytes;
            if (current >= end) [[unlikely]]      // fall back to the standard allocator if the buffer gets full
                return reinterpret_cast<T*>(::operator new(bytes));
            return reinterpret_cast<T*>(old_current);
        }

        constexpr int debug() {
            if (static_cast<int>(current - start) >= ARENA_ALLOCATOR_INITIAL_SIZE)
                return ARENA_ALLOCATOR_INITIAL_SIZE;
            return static_cast<int>(current - start);
        }
};

template <typename T>
class ArenaAllocator {
    public:
        internal_allocator_arena internal_alloc;

        using value_type = T;

        using pointer = value_type*;
        using const_pointer = typename std::pointer_traits<pointer>::template rebind<value_type const>;
        using void_pointer = typename std::pointer_traits<pointer>::template rebind<void>;
        using const_void_pointer = typename std::pointer_traits<pointer>::template rebind<const void>;

        using difference_type = typename std::pointer_traits<pointer>::difference_type;
        using size_type = std::make_unsigned_t<difference_type>;

        template <class U> struct rebind { typedef ArenaAllocator<U> other; };

        using propagate_on_container_copy_assignment = std::false_type;
        using propagate_on_container_move_assignment = std::false_type;
        using propagate_on_container_swap = std::false_type;
        using is_always_equal = std::is_empty<ArenaAllocator>;

        constexpr ArenaAllocator() noexcept {}
        constexpr ArenaAllocator(const ArenaAllocator&) noexcept = default;
        ~ArenaAllocator() = default;

        template <class U>
        constexpr ArenaAllocator(const ArenaAllocator<U>&) noexcept {
            //std::cout << "CREATED A NEW ALLOCATOR" << std::endl;
        }

        constexpr T* allocate(std::size_t n) noexcept {
            T* result = internal_alloc.alloc<T>(n * sizeof(T));
            std::cout << (n * sizeof(T)) << " bytes allocated" << std::endl;
            std::cout << internal_alloc.debug() << "/" << ARENA_ALLOCATOR_INITIAL_SIZE << " of arena being used" << std::endl;
            return result;
        }

        constexpr void deallocate(T* p, std::size_t n) const noexcept {
            //::operator delete(p, n * sizeof(T));
        }

        template <class U, class ...Args>
        constexpr void construct(U* p, Args&& ...args) const noexcept {
            std::construct_at(p, std::forward<Args>(args)...);
            //::new(p) U(std::forward<Args>(args)...); if c++ 20 isn't supported
        }
};

template <class T, class U>
constexpr bool operator==(const ArenaAllocator<T>&, const ArenaAllocator<U>&) noexcept {
    return true;
}

template <class T, class U>
constexpr bool operator!=(const ArenaAllocator<T>&, const ArenaAllocator<U>&) noexcept {
    return false;
}

#endif



/*#ifndef ARENAALLOC_HEADER_H
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
*/