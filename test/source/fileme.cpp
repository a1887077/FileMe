#include <doctest/doctest.h>
#include <fileme/fileme.h>
#include <fileme/version.h>

#include <string>

TEST_CASE("FileMe") {
  using namespace fileme;

  FileMe fileme("Tests");

  CHECK(fileme.file(LanguageCode::EN) == "Hello, Tests!");
  CHECK(fileme.file(LanguageCode::DE) == "Hallo Tests!");
  CHECK(fileme.file(LanguageCode::ES) == "¡Hola Tests!");
  CHECK(fileme.file(LanguageCode::FR) == "Bonjour Tests!");
}

TEST_CASE("FileMe version") {
  static_assert(std::string_view(FILEME_VERSION) == std::string_view("1.0"));
  CHECK(std::string(FILEME_VERSION) == std::string("1.0"));
}
