#include <doctest/doctest.h>
#include <fileme/manager.h>
#include <fileme/dirEntry.h>

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

    CHECK_EQ(ret, -FileOperator::OperatorStatus::SUCCESS);
    CHECK(fs::exists(temp_path / "foo.bar"));
    CHECK(fs::exists(temp_path / "new.file"));
    CHECK_EQ(manager.getEntries().size(), before_size + 1);
  }

  SUBCASE("New directory with unique name") {
    size_t before_size = manager.getEntries().size();
    int ret = manager.create("newdir", DirEntryType::DIRECTORY_ENTRY);

    CHECK_EQ(ret, -FileOperator::OperatorStatus::SUCCESS);
    CHECK(fs::exists(temp_path / "empty"));
    CHECK(fs::exists(temp_path / "newdir"));
    CHECK_EQ(manager.getEntries().size(), before_size + 1);
  }
}