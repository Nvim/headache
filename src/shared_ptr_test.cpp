#include "shared_ptr.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <functional>
#include <iostream>

struct dtor_struct {
	void operator()(int const* a) const {
		std::cout << "destroying..\n";
		delete a;
	}
};

void dtor_func(int const* a) {
	std::cout << "destroying..\n";
	delete a;
}

auto const dtor_lambda = [](int const* a) {
	std::cout << "destroying..\n";
	delete a;
};

TEST_CASE("Create SharedPtr with struct dtor") {
	int a = 28;
	auto p = SharedPtr{&a, dtor_struct{}};
	REQUIRE(p.get() == &a);
}

TEST_CASE("Create SharedPtr with function pointer dtor") {
	int a = 28;
	auto p = SharedPtr{&a, dtor_func};
	REQUIRE(p.get() == &a);
}

TEST_CASE("SharedPtr with lambda dtor") {
	int a = 28;
	auto p = SharedPtr{&a, dtor_lambda};
	REQUIRE(p.get() == &a);
}

TEST_CASE("SharedPtr with std::function dtor") {
	std::function<void(int const* a)> f_lambda = dtor_lambda;
	std::function<void(int const* a)> f_ptr = dtor_func;

	int a = 28;
	auto p = SharedPtr{&a, f_lambda};
	REQUIRE(p.get() == &a);

	auto p2 = SharedPtr{&a, f_ptr};
	REQUIRE(p2.get() == &a);
}

TEST_CASE("SharedPtr member access operators") {
	std::function<void(int const* a)> f_lambda = dtor_lambda;
	std::function<void(int const* a)> f_ptr = dtor_func;

	int a = 28;
	auto p = SharedPtr{&a, f_lambda};
	REQUIRE(p.get() == &a);
	REQUIRE(*p == &a);
	REQUIRE(!p == false);
}

TEST_CASE("Empty SharedPtr") {
	SharedPtr<int> p{};
	REQUIRE(p.get() == nullptr);
	REQUIRE(!p);
	REQUIRE_FALSE(p);
}

TEST_CASE("Empty SharedPtr with dtor") {
	SharedPtr<int> p{dtor_struct{}};
	REQUIRE(p.get() == nullptr);
	REQUIRE(!p);
	REQUIRE_FALSE(p);
}

// TEST_CASE("SharedPtr dtor is called") {
// 	bool destroyed{false};
// 	auto const destroy = [&destroyed](int const* a) {
// 		destroyed = false;
// 		delete a;
// 	};
//
// 	int a = 28;
// 	{
// 		auto p = SharedPtr{&a, destroy};
// 		REQUIRE(p.get() == &a);
// 	}
// 	REQUIRE(destroyed);
// }
