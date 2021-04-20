#ifndef ARENA_ALLOCATOR_H
#define ARENA_ALLOCATOR_H

#include <type_traits>
#include <iostream>

#ifndef ARENA_ALLOCATOR_SIZE
#define ARENA_ALLOCATOR_SIZE 1024
#endif

class internal_allocator_arena final {
public:

    internal_allocator_arena() {
        start_ = static_cast<char*>(::operator new(ARENA_ALLOCATOR_SIZE));
        current_ = start_;
        end_ = start_ + ARENA_ALLOCATOR_SIZE;

#ifdef ARENA_ALLOC_DEBUG
        std::cout << "new arena allocated\n";
#endif
    }

    // does not free arena on purpose
    // the only instance of this class is global
    ~internal_allocator_arena() = default;

    template<typename T>
    constexpr T* alloc(const size_t bytes) {
        auto* old_current = current_;
        current_ += bytes;
        [[unlikely]] if (current_ >= end_)      // fall back to the standard allocator if the buffer gets full
            return static_cast<T*>(::operator new(bytes));  // leaks all memory at the moment
        return reinterpret_cast<T*>(old_current);
    }

    constexpr internal_allocator_arena(const internal_allocator_arena&) = delete;
    constexpr internal_allocator_arena& operator = (const internal_allocator_arena&) = delete;
    constexpr internal_allocator_arena(const internal_allocator_arena&&) = delete;
    constexpr internal_allocator_arena& operator = (const internal_allocator_arena&&) = delete;

    constexpr int debug() const {
        if (static_cast<int>(current_ - start_) >= ARENA_ALLOCATOR_SIZE)
            return ARENA_ALLOCATOR_SIZE;
        return static_cast<int>(current_ - start_);
    }

private:

    char* start_;
    char* current_;
    char* end_;

};

static inline internal_allocator_arena internal_state{};

template <typename T>
class arena_allocator final {
public:
    using value_type = T;

    using pointer = value_type*;
    using const_pointer = typename std::pointer_traits<pointer>::template rebind<value_type const>;
    using void_pointer = typename std::pointer_traits<pointer>::template rebind<void>;
    using const_void_pointer = typename std::pointer_traits<pointer>::template rebind<const void>;

    using difference_type = typename std::pointer_traits<pointer>::difference_type;
    using size_type = std::make_unsigned_t<difference_type>;

    template <class U> struct rebind { typedef arena_allocator<U> other; };

    using propagate_on_container_copy_assignment = std::false_type;
    using propagate_on_container_move_assignment = std::false_type;
    using propagate_on_container_swap = std::false_type;
    using is_always_equal = std::is_empty<arena_allocator>;

    constexpr arena_allocator() = default;
    constexpr arena_allocator(const arena_allocator&) = default;
    constexpr arena_allocator& operator = (const arena_allocator&) = default;
    constexpr arena_allocator(arena_allocator&&) = default;
    constexpr arena_allocator& operator = (arena_allocator&&) = default;
    ~arena_allocator() = default;

    template <class U>
    explicit constexpr arena_allocator(const arena_allocator<U>&) {}

    constexpr T* allocate(const std::size_t n) noexcept {
        T* result = internal_state.alloc<T>(n * sizeof(T));
#ifdef ARENA_ALLOC_DEBUG
        std::cout << '\n' << (n * sizeof(T)) << " bytes allocated\n";
        std::cout << internal_state.debug() << "/" << ARENA_ALLOCATOR_SIZE << " of arena being used\n";
#endif
        return result;
    }

    constexpr void deallocate(T* p, std::size_t n) const noexcept {
        static_cast<void>(p);
        static_cast<void>(n);
        //::operator delete(p, n * sizeof(T));
    }

    template <class U, class ...Args>
    constexpr void construct(U* p, Args&& ...args) const noexcept {
        std::construct_at(p, std::forward<Args>(args)...);
        //::new(p) U(std::forward<Args>(args)...); if c++ 20 isn't supported
    }
};

template <class T, class U>
constexpr bool operator==(const arena_allocator<T>&, const arena_allocator<U>&) noexcept {
    return true;
}

template <class T, class U>
constexpr bool operator!=(const arena_allocator<T>&, const arena_allocator<U>&) noexcept {
    return false;
}

#endif
