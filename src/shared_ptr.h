#pragma once

// #include <initializer_list>
#include <memory>

template <typename T>
struct SharedPtr {
  public:
	constexpr SharedPtr() noexcept = default;

	explicit SharedPtr(T* raw)
		: m_destroy{std::default_delete<T>()}, m_raw{raw} {};

	template <typename Dtor>
	SharedPtr(T* raw, Dtor d)
		: m_destroy(new DestroyImpl<Dtor>(d)), m_raw{raw} {}

	template <typename Dtor>
	SharedPtr(Dtor d) : m_destroy(new DestroyImpl<Dtor>(d)) {}

	// ~SharedPtr() {
	// 	if (m_raw) { m_destroy(m_raw); }
	// }

	// TODO: move semantics
	// SharedPtr(std::initializer_list<T> args);
	// SharedPtr(T* raw, void (*dtor)(T* self));
	// SharedPtr(std::initializer_list<T> args, void (*dtor)(T* self));

	// SharedPtr(SharedPtr& other);
	// SharedPtr(SharedPtr&& other) noexcept;
	// SharedPtr& operator=(SharedPtr const& other);
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
};
