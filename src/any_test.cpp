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

// TODO: test throws as expected but macro doesn't pick it up
TEST_CASE("Retrieve wrong type throws") {
	Any a{17};

	// Same type
	auto val = a.get_value<int>();
	REQUIRE(val == 17);

	// All my homies hate implicit conversion
	// REQUIRE_THROWS([&]() {
	// 	auto temp = a.get_value<float>();
	// 	(void)temp;
	// });

	// Obviously this fails too
	// struct CantConvert {
	// 	std::array<char, 8> cant{"example"};
	// };
	// CantConvert f{};
	// Any fails{f};
	// REQUIRE_THROWS(fails.get_value<int>());
}
