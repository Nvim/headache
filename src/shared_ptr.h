#pragma once

// #include <initializer_list>
#include <memory>
#include <print>

namespace {
struct RefCount {
	uint64_t count{1}; // TODO: atomic?
};
} // namespace

template <typename T>
struct SharedPtr {
  public:
	constexpr SharedPtr() noexcept = default;

	explicit SharedPtr(T* raw)
		: m_destroy{std::default_delete<T>()}, m_raw{raw},
		  m_refcount{new RefCount} {}

	template <typename Dtor>
	SharedPtr(T* raw, Dtor d)
		: m_destroy(new DestroyImpl<Dtor>(d)), m_raw{raw},
		  m_refcount{new RefCount} {}

	template <typename Dtor>
	SharedPtr(std::nullptr_t, Dtor d)
		: m_destroy(new DestroyImpl<Dtor>(d)), m_refcount{nullptr} {}

	~SharedPtr() {
		if (m_raw == nullptr) { return; }
		if (m_refcount != nullptr && --m_refcount->count == 0) {
			m_destroy->operator()(m_raw);
			delete m_destroy;
		}
	}

	// SharedPtr(std::initializer_list<T> args);
	// SharedPtr(std::initializer_list<T> args, void (*dtor)(T* self));

	// TODO: move semantics
	// SharedPtr(SharedPtr& other);
	// SharedPtr(SharedPtr&& other) noexcept;
	// SharedPtr& operator=(SharedPtr const& other) {
	// 	if (this == other) {
	// 		return *this;
	// 	}
	// 	this->m_refcount = other.m_refcount;
	// 	this->m_destroy = other.m_destroy;
	// 	this->m_raw = other.m_raw;
	// 	m_refcount->count++;
	// 	return *this;
	// }
	// SharedPtr& operator=(SharedPtr&& other) noexcept;

	T* get() { return m_raw; }
	T* operator*() { return m_raw; }
	T* operator->() { return m_raw; }
	bool operator!() const { return m_raw == nullptr; }
	explicit operator bool() const noexcept { return m_raw != nullptr; }

	bool operator==(SharedPtr<T> const& other) { return m_raw == other.m_raw; }

  private:
	struct IDestroy {
		virtual ~IDestroy() = default;
		virtual void operator()(T* t) const = 0;
	};

	template <typename Dtor>
	struct DestroyImpl : public IDestroy {
		DestroyImpl(Dtor d) : m_dtor{d} {}
		void operator()(T* t) const override { m_dtor(t); }
		Dtor m_dtor;
	};

	IDestroy* m_destroy{nullptr};
	T* m_raw{nullptr};
	RefCount* m_refcount;
};
