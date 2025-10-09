#include <doctest/doctest.h>
#include <fileme/fileOperator.h>

#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

TEST_CASE("FileMe File Operator Deletion") {
  // the working directory of the file operator
  fs::path temp_path = fs::temp_directory_path();
  fs::create_directory(temp_path / "temptesting");
  temp_path /= "temptesting";

  // initialise the file operator
  FileOperator fileoperator;

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
    int ret = fileoperator.remove(target);

    CHECK_EQ(ret, -1);
    CHECK_FALSE(fs::exists(target));
  }

  SUBCASE("Directory does not exist") {
    fs::path target = temp_path / "helloworld";
    int ret = fileoperator.remove(target);

    CHECK_EQ(ret, -1);
    CHECK_FALSE(fs::exists(target));
  }

  SUBCASE("Single File") {
    fs::path target = temp_path / "foo.hpp";
    int ret = fileoperator.remove(target);

    CHECK_EQ(ret, 0);
    CHECK_FALSE(fs::exists(target));
  }

  SUBCASE("Empty Directory") {
    fs::path target = temp_path / "empty";
    int ret = fileoperator.remove(target);

    CHECK_EQ(ret, 0);
    CHECK_FALSE(fs::exists(target));
  }

  SUBCASE("Directory with file contents") {
    fs::path target = temp_path / "temp1";
    int ret = fileoperator.remove(target);

    CHECK_EQ(ret, 0);
    CHECK_FALSE(fs::exists(target));
  }

  SUBCASE("Directory with file and directory contents") {
    fs::path target = temp_path / "temp2";
    int ret = fileoperator.remove(target);

    CHECK_EQ(ret, 0);
    CHECK_FALSE(fs::exists(target));
  }

  fs::remove_all(temp_path);
}

TEST_CASE("FileMe File Operator Renaming") {
  // the working directory of the file operator
  fs::path temp_path = fs::temp_directory_path();
  fs::create_directory(temp_path / "temptesting");
  temp_path /= "temptesting";

  // initialise the file operator
  FileOperator fileoperator;

  // create a file and directory to test with
  fs::path file = "foo.bar";
  fs::path file_path = temp_path / file;
  std::ofstream(file_path).write("baz", 3);

  file_path = temp_path / "filetwo.h";
  std::ofstream(file_path).write("testing", 7);

  fs::path dir = "testdir";
  fs::path dir_path = temp_path / dir;
  fs::create_directory(dir_path);

  dir = "thedir";
  dir_path = temp_path / dir;
  fs::create_directory(dir_path);

  SUBCASE("Nonexistent File") {
    fs::path old_path = temp_path / "none.txt";
    fs::path new_path = temp_path / "new.txt";
    int ret = fileoperator.rename(old_path, new_path);

    CHECK_EQ(ret, -1);
    CHECK_FALSE(fs::exists(old_path));
    CHECK_FALSE(fs::exists(new_path));
  }

  SUBCASE("Nonexistent Directory") {
    fs::path old_path = temp_path / "none";
    fs::path new_path = temp_path / "new";
    int ret = fileoperator.rename(old_path, new_path);

    CHECK_EQ(ret, -1);
    CHECK_FALSE(fs::exists(old_path));
    CHECK_FALSE(fs::exists(new_path));
  }

  SUBCASE("File to existing name") {
    fs::path old_path = temp_path / "filetwo.h";
    fs::path new_path = temp_path / "foo.bar";
    int ret = fileoperator.rename(old_path, new_path);

    CHECK_EQ(ret, -1);
    CHECK(fs::exists(old_path));
    CHECK(fs::exists(new_path));
  }

  SUBCASE("Directory to existing name") {
    fs::path old_path = temp_path / "thedir";
    fs::path new_path = temp_path / "testdir";
    int ret = fileoperator.rename(old_path, new_path);

    CHECK_EQ(ret, -1);
    CHECK(fs::exists(old_path));
    CHECK(fs::exists(new_path));
  }

  SUBCASE("File to unique name") {
    fs::path old_path = temp_path / "foo.bar";
    fs::path new_path = temp_path / "bar.foo";
    int ret = fileoperator.rename(old_path, new_path);

    CHECK_EQ(ret, 0);
    CHECK_FALSE(fs::exists(old_path));
    CHECK(fs::exists(new_path));
  }

  SUBCASE("Directory to unique name") {
    fs::path old_path = temp_path / "testdir";
    fs::path new_path = temp_path / "newtestdir";
    int ret = fileoperator.rename(old_path, new_path);

    CHECK_EQ(ret, 0);
    CHECK_FALSE(fs::exists(old_path));
    CHECK(fs::exists(new_path));
  }

  fs::remove_all(temp_path);
}
