#ifndef NON_OWN_PTR_H
#define NON_OWN_PTR_H

#include <memory>

template<typename T>
class non_own_ptr final {
	public:

		constexpr inline non_own_ptr() noexcept :
			ptr_(nullptr)
		{

		}

		constexpr inline explicit non_own_ptr(std::nullptr_t) noexcept :
			ptr_(nullptr)
		{

		}

		constexpr inline explicit non_own_ptr(T* ptr) noexcept :
			ptr_(ptr)
		{

		}

		inline ~non_own_ptr() noexcept {
			release();
		}

		constexpr non_own_ptr(const non_own_ptr& other) noexcept :
			ptr_(other.get())
		{

		}

		constexpr inline non_own_ptr& operator =(const non_own_ptr& other) noexcept {
			ptr_ = other.get();
			return *this;
		}

		constexpr inline non_own_ptr& operator =(std::nullptr_t) noexcept {
			ptr_ = nullptr;
			return *this;
		}

		constexpr non_own_ptr(non_own_ptr&& other) noexcept :
			ptr_(other)
		{

		}

		constexpr inline non_own_ptr& operator =(non_own_ptr&& other) noexcept {
			ptr_ = other.get();
			return *this;
		}

		constexpr inline T& operator *() const noexcept {
			return *ptr_;
		}

		constexpr inline T* operator ->() const noexcept {
			return ptr_;
		}

		constexpr inline T* get() const noexcept {
			return ptr_;
		}

		constexpr inline explicit operator bool() const noexcept {
			return ptr_ != nullptr;
		}

		constexpr inline explicit operator T* () const noexcept {
			return ptr_;
		}

		constexpr inline explicit operator const T* () const noexcept {
			return ptr_;
		}

		constexpr inline void release() const noexcept {
			delete ptr_;
		}

	private:
		T* ptr_;

};


#endif