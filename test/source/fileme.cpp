#include <doctest/doctest.h>
#include <fileme/version.h>

#include <string>

TEST_CASE("FileMe version") {
  static_assert(std::string_view(FILEME_VERSION) == std::string_view("1.0"));
  CHECK(std::string(FILEME_VERSION) == std::string("1.0"));
}

