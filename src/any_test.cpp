#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "any.h"

TEST_CASE("Retrieve Any by value") {
	int a = 24;
	Any any{a};

	int val = any.get_value<int>();
	REQUIRE(val == 24);

	int val2 = any.get_value<int>();
	REQUIRE(val2 == 24);

	// Val returns copies -> new addresses every time
	REQUIRE(&val != &val2);
}

TEST_CASE("Retrieve Any by reference") {
	int a = 24;
	Any any{a};

	int ref = any.get_ref<int>();
	REQUIRE(ref == 24);

	int ref2 = any.get_ref<int>();
	REQUIRE(ref2 == 24);

	// Refs should have same addresses:
	REQUIRE(&ref != &ref2);

	// Any stores a copy of a:
	REQUIRE(&a != &ref);
}

TEST_CASE("Create any from r-value") {
	Any a{42.2f};

	auto val = a.get_value<float>();
	REQUIRE_THAT(val, Catch::Matchers::WithinAbs(42.2f, .1f));
}

TEST_CASE("Retrieve wrong type throws (obvious)") {
	Any a{17};

	// Same type
	auto val = a.get_value<int>();
	REQUIRE(val == 17);

	struct CantConvert {
		std::array<char, 8> cant{"example"};
	};
	CantConvert f{};
	Any fails{f};
	REQUIRE_THROWS(fails.get_value<int>());
}

// All my homies hate implicit conversion
TEST_CASE("Retrieve wrong type throws (implicit conversions)") {

	Any int_any{17};
	REQUIRE_THROWS([&]() {
		auto temp = int_any.get_value<float>();
		(void)temp;
	}());
	auto val = int_any.get_value<int>();
	REQUIRE(val == 17);


	// Trickier ones:
	REQUIRE_THROWS([&]() {
		auto temp = int_any.get_value<long>();
		(void)temp;
	}());

	Any long_any{17l};
	REQUIRE_THROWS([&]() {
		auto temp = long_any.get_value<int>();
		(void)temp;
	}());
	auto temp = long_any.get_value<long>();
	(void)temp;

	Any double_any{17.0};
	REQUIRE_THROWS([&]() {
		auto temp = double_any.get_value<float>();
		(void)temp;
	}());
	auto temp2 = double_any.get_value<double>();
	(void)temp2;
}
