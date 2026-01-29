#pragma once

#include <memory>
#include <stdexcept>
#include <typeinfo>
#include <utility>

struct Any {
  public:
	Any() = default;

	template <typename T>
	constexpr explicit Any(T&& data)
		: m_getter(std::make_unique<GetterImpl<T>>(std::forward<T>(data))),
		  m_empty{false} {}

	bool has_value() const { return m_empty; }
	std::type_info const& type() const noexcept { return m_getter->get(); }

	template <typename T>
	T get_value() const {
		if (m_empty || !m_getter) {
			throw std::runtime_error("any is empty\n");
		}
		if (typeid(T) != m_getter->get()) {
			throw std::runtime_error("bad cast\n");
		}
		T* val = static_cast<T*>(m_getter->value());
		return *val;
	}

	template <typename T>
	T const& get_ref() const {
		if (m_empty || !m_getter) {
			throw std::runtime_error("any is empty\n");
		}
		if (typeid(T) != m_getter->get()) {
			throw std::runtime_error("bad cast\n");
		}
		T const* val = static_cast<T const*>(m_getter->value());
		T const& valref = *val;
		return valref;
	}

  private:
	struct IGetter {
		virtual ~IGetter() = default;
		virtual std::type_info const& get() = 0;
		virtual void* value() = 0;
	};

	template <typename T>
	struct GetterImpl final : public IGetter {
		GetterImpl(T t) : m_data{t} {}
		std::type_info const& get() override { return typeid(m_data); };

		void* value() override { return &m_data; };
		T m_data;
	};

	std::unique_ptr<IGetter> m_getter{};
	bool m_empty{true};
};
