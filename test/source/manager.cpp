#include <doctest/doctest.h>
#include <fileme/manager.h>

#include <string>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

TEST_CASE("FileMe Manager") {
  // the working directory of the file manager
  fs::path temp_path = fs::temp_directory_path();
  
  // initialise the file manager
  Manager manager(temp_path);

  // ensure that the manager has been initialised with the correct working directory
  REQUIRE_EQ(temp_path, manager.get_workspace_path());

  SUBCASE("Removal of Files") {
    // creating files to test with
    fs::path file = "foo.hpp";
    fs::path file_path = temp_path / file;

    std::ofstream(file_path).write("bar", 3);

    fs::path dir_path = temp_path / "empty";
    fs::create_directories(dir_path);

    dir_path = temp_path / "temp1";
    fs::create_directories(dir_path);

    file_path = dir_path / "baz.foo";
    std::ofstream(file_path).write("far", 3);

    dir_path = temp_path / "temp2/bar";
    fs::create_directories(dir_path);

    file_path = temp_path / "temp2/hello.world";
    std::ofstream(file_path).write("far", 3);

    SUBCASE("File does not exist") {
      fs::path target = temp_path / "target.txt";
      int ret = manager.remove(target);

      CHECK_EQ(ret, -1);
    }

    SUBCASE("Directory does not exist") {
      fs::path target = temp_path / "helloworld";
      int ret = manager.remove(target);

      CHECK_EQ(ret, -1);
    }

    SUBCASE("Single File") {
      fs::path target = temp_path / "foo.hpp";
      int ret = manager.remove(target);

      CHECK_EQ(ret, 0);
    }

    SUBCASE("Empty Directory") {
      fs::path target = temp_path / "empty";
      int ret = manager.remove(target);

      CHECK_EQ(ret, 0);
    }

    SUBCASE("Directory with file contents") {
      fs::path target = temp_path / "temp1";
      int ret = manager.remove(target);

      CHECK_EQ(ret, 0);
    }

    SUBCASE("Directory with file and directory contents") {
      fs::path target = temp_path / "temp2";
      int ret = manager.remove(target);

      CHECK_EQ(ret, 0);
    }
  }
}
