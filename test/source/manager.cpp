#include <doctest/doctest.h>
#include <fileme/dirEntry.h>
#include <fileme/manager.h>

#include <filesystem>
#include <fstream>
#include <vector>
namespace fs = std::filesystem;

TEST_CASE("FileMe File Manager Creation") {
  fs::path temp_path = fs::temp_directory_path();
  fs::create_directory(temp_path / "temptesting");
  temp_path /= "temptesting";

  fs::path file_path = temp_path / "foo.bar";
  std::ofstream(file_path).write("baz", 3);

  fs::path dir_path = temp_path / "empty";
  fs::create_directories(dir_path);

  Manager manager(temp_path);

  SUBCASE("New file with existing name") {
    size_t before_size = manager.getEntries().size();
    int ret = manager.create("foo.bar", DirEntryType::FILE_ENTRY);

    CHECK_EQ(ret, -FileOperator::OperatorStatus::E_ITEM_EXISTS);
    CHECK(fs::exists(temp_path / "foo.bar"));
    CHECK_EQ(manager.getEntries().size(), before_size);
  }

  SUBCASE("New directory with existing name") {
    size_t before_size = manager.getEntries().size();
    int ret = manager.create("empty", DirEntryType::DIRECTORY_ENTRY);

    CHECK_EQ(ret, -FileOperator::OperatorStatus::E_ITEM_EXISTS);
    CHECK(fs::exists(temp_path / "empty"));
    CHECK_EQ(manager.getEntries().size(), before_size);
  }

  SUBCASE("New file with unique name") {
    size_t before_size = manager.getEntries().size();
    int ret = manager.create("new.file", DirEntryType::FILE_ENTRY);

    CHECK_EQ(ret, FileOperator::OperatorStatus::SUCCESS);
    CHECK(fs::exists(temp_path / "foo.bar"));
    CHECK(fs::exists(temp_path / "new.file"));
    CHECK_EQ(manager.getEntries().size(), before_size + 1);
  }

  SUBCASE("New directory with unique name") {
    size_t before_size = manager.getEntries().size();
    int ret = manager.create("newdir", DirEntryType::DIRECTORY_ENTRY);

    CHECK_EQ(ret, FileOperator::OperatorStatus::SUCCESS);
    CHECK(fs::exists(temp_path / "empty"));
    CHECK(fs::exists(temp_path / "newdir"));
    CHECK_EQ(manager.getEntries().size(), before_size + 1);
  }

  fs::remove_all(temp_path);
}

TEST_CASE("FileMe File Manager Deletion") {
  fs::path temp_path = fs::temp_directory_path();
  fs::create_directory(temp_path / "temptesting");
  temp_path /= "temptesting";

  fs::path file_path = temp_path / "foo.bar";
  std::ofstream(file_path).write("baz", 3);

  fs::path dir_path = temp_path / "empty";
  fs::create_directories(dir_path);

  Manager manager(temp_path);

  SUBCASE("File does not exist") {
    size_t before_size = manager.getEntries().size();
    DirEntry temp_file(temp_path / "something.txt", "something.txt", FILE_ENTRY);
    int ret = manager.remove(temp_file);

    CHECK_EQ(ret, -FileOperator::OperatorStatus::E_ITEM_DOES_NOT_EXIST);
    CHECK_EQ(manager.getEntries().size(), before_size);
  }

  SUBCASE("Directory does not exist") {
    size_t before_size = manager.getEntries().size();
    DirEntry temp_dir(temp_path / "fakedir", "fakedir", DIRECTORY_ENTRY);
    int ret = manager.remove(temp_dir);

    CHECK_EQ(ret, -FileOperator::OperatorStatus::E_ITEM_DOES_NOT_EXIST);
    CHECK_EQ(manager.getEntries().size(), before_size);
  }

  SUBCASE("Single File") {
    size_t before_size = manager.getEntries().size();
    DirEntry real_file(temp_path / "foo.bar", "foo.bar", FILE_ENTRY);
    int ret = manager.remove(real_file);

    CHECK_EQ(ret, FileOperator::OperatorStatus::SUCCESS);
    CHECK_FALSE(fs::exists(temp_path / "foo.bar"));
    CHECK_EQ(manager.getEntries().size(), before_size - 1);
  }

  SUBCASE("Empty Directory") {
    size_t before_size = manager.getEntries().size();
    DirEntry empty_dir(temp_path / "empty", "empty", DIRECTORY_ENTRY);
    int ret = manager.remove(empty_dir);

    CHECK_EQ(ret, FileOperator::OperatorStatus::SUCCESS);
    CHECK_FALSE(fs::exists(temp_path / "empty"));
    CHECK_EQ(manager.getEntries().size(), before_size - 1);
  }

  fs::remove_all(temp_path);
}

TEST_CASE("FileMe File Manager Renaming") {
  fs::path temp_path = fs::temp_directory_path();
  fs::create_directory(temp_path / "temptesting");
  temp_path /= "temptesting";

  fs::path file_path = temp_path / "foo.bar";
  std::ofstream(file_path).write("baz", 3);

  file_path = temp_path / "test.file";
  std::ofstream(file_path).write("testing", 3);

  fs::path dir_path = temp_path / "empty";
  fs::create_directories(dir_path);

  dir_path = temp_path / "bigdir";
  fs::create_directories(dir_path);

  Manager manager(temp_path);

  SUBCASE("File to existing name") {
    size_t before_size = manager.getEntries().size();
    DirEntry foo_file(temp_path / "foo.bar", "foo.bar", FILE_ENTRY);
    int ret = manager.rename(foo_file, "test.file");

    CHECK_EQ(ret, -FileOperator::OperatorStatus::E_ITEM_EXISTS);
    CHECK(fs::exists(temp_path / "foo.bar"));
    CHECK(fs::exists(temp_path / "test.file"));
    CHECK_EQ(manager.getEntries().size(), before_size);
  }

  SUBCASE("Directory to existing name") {
    size_t before_size = manager.getEntries().size();
    DirEntry empty_dir(temp_path / "empty", "empty", DIRECTORY_ENTRY);
    int ret = manager.rename(empty_dir, "bigdir");

    CHECK_EQ(ret, -FileOperator::OperatorStatus::E_ITEM_EXISTS);
    CHECK(fs::exists(temp_path / "empty"));
    CHECK(fs::exists(temp_path / "bigdir"));
    CHECK_EQ(manager.getEntries().size(), before_size);
  }

  SUBCASE("File to unique name") {
    size_t before_size = manager.getEntries().size();
    DirEntry foo_file(temp_path / "foo.bar", "foo.bar", FILE_ENTRY);
    int ret = manager.rename(foo_file, "bar.foo");

    CHECK_EQ(ret, FileOperator::OperatorStatus::SUCCESS);
    CHECK_FALSE(fs::exists(temp_path / "foo.bar"));
    CHECK(fs::exists(temp_path / "bar.foo"));
    CHECK(fs::exists(temp_path / "test.file"));
    CHECK_EQ(manager.getEntries().size(), before_size);
  }

  SUBCASE("Directory to unique name") {
    size_t before_size = manager.getEntries().size();
    DirEntry empty_dir(temp_path / "empty", "empty", DIRECTORY_ENTRY);
    int ret = manager.rename(empty_dir, "nothing");

    CHECK_EQ(ret, FileOperator::OperatorStatus::SUCCESS);
    CHECK_FALSE(fs::exists(temp_path / "empty"));
    CHECK(fs::exists(temp_path / "nothing"));
    CHECK(fs::exists(temp_path / "bigdir"));
    CHECK_EQ(manager.getEntries().size(), before_size);
  }

  fs::remove_all(temp_path);
}