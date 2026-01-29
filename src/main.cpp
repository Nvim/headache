#include <iostream>
#include "any.h"

namespace {
struct Debug {
	int m_value;

	explicit Debug(int v) : m_value{v} {}
	Debug(Debug const& other) : m_value{other.m_value} {
		std::cout << "copied\n";
	}

	Debug(Debug&& other) noexcept : m_value{other.m_value} {
		std::cout << "moved\n";
	}

	Debug& operator=(Debug const& other) {
		std::cout << "copied\n";
		m_value = other.m_value;
		return *this;
	}

	Debug& operator=(Debug&& other) noexcept {
		std::cout << "moved\n";
		if (this != &other) { m_value = other.m_value; }
		return *this;
	}

	~Debug() { std::cout << "destroying\n"; }
};

void print_any(Any& a) {
	std::cout << "casting.." << "\n";
	auto casted = a.get_value<Debug>();
	std::cout << casted.m_value << "\n";
}

void print_any_nocopy(Any& a) {
	std::cout << "casting.." << "\n";
	auto const& casted = a.get_ref<Debug>();
	std::cout << casted.m_value << "\n";
}
} // namespace

int main() {
	Debug j{123};
	Any a{j};

	std::cout << "printing..\n";
	print_any(a);

	std::cout << "printing, no copy:\n";
	print_any_nocopy(a);

	return 0;
}
