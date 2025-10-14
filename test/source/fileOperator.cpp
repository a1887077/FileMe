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
  fs::path file_path = temp_path / "foo.hpp";
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

  REQUIRE(fs::exists(temp_path / "foo.hpp"));
  REQUIRE(fs::exists(temp_path / "empty"));
  REQUIRE(fs::exists(temp_path / "temp1/baz.foo"));
  REQUIRE(fs::exists(temp_path / "temp2/bar"));
  REQUIRE(fs::exists(temp_path / "temp2/hello.world"));

  SUBCASE("File does not exist") {
    fs::path target = temp_path / "target.txt";
    int ret = fileoperator.remove(target);

    CHECK_EQ(ret, -FileOperator::OperatorStatus::E_ITEM_DOES_NOT_EXIST);
    CHECK_FALSE(fs::exists(target));
  }

  SUBCASE("Directory does not exist") {
    fs::path target = temp_path / "helloworld";
    int ret = fileoperator.remove(target);

    CHECK_EQ(ret, -FileOperator::OperatorStatus::E_ITEM_DOES_NOT_EXIST);
    CHECK_FALSE(fs::exists(target));
  }

  SUBCASE("Single File") {
    fs::path target = temp_path / "foo.hpp";
    int ret = fileoperator.remove(target);

    CHECK_EQ(ret, FileOperator::OperatorStatus::SUCCESS);
    CHECK_FALSE(fs::exists(target));
  }

  SUBCASE("Empty Directory") {
    fs::path target = temp_path / "empty";
    int ret = fileoperator.remove(target);

    CHECK_EQ(ret, FileOperator::OperatorStatus::SUCCESS);
    CHECK_FALSE(fs::exists(target));
  }

  SUBCASE("Directory with file contents") {
    fs::path target = temp_path / "temp1";
    int ret = fileoperator.remove(target);

    CHECK_EQ(ret, FileOperator::OperatorStatus::SUCCESS);
    CHECK_FALSE(fs::exists(target));
  }

  SUBCASE("Directory with file and directory contents") {
    fs::path target = temp_path / "temp2";
    int ret = fileoperator.remove(target);

    CHECK_EQ(ret, FileOperator::OperatorStatus::SUCCESS);
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
  fs::path file_path = temp_path / "foo.bar";
  std::ofstream(file_path).write("baz", 3);

  file_path = temp_path / "filetwo.h";
  std::ofstream(file_path).write("testing", 7);

  fs::path dir_path = temp_path / "testdir";
  fs::create_directory(dir_path);

  dir_path = temp_path / "thedir";
  fs::create_directory(dir_path);

  SUBCASE("Nonexistent File") {
    fs::path old_path = temp_path / "none.txt";
    fs::path new_path = temp_path / "new.txt";
    int ret = fileoperator.rename(old_path, new_path);

    CHECK_EQ(ret, -FileOperator::OperatorStatus::E_ITEM_DOES_NOT_EXIST);
    CHECK_FALSE(fs::exists(old_path));
    CHECK_FALSE(fs::exists(new_path));
  }

  SUBCASE("Nonexistent Directory") {
    fs::path old_path = temp_path / "none";
    fs::path new_path = temp_path / "new";
    int ret = fileoperator.rename(old_path, new_path);

    CHECK_EQ(ret, -FileOperator::OperatorStatus::E_ITEM_DOES_NOT_EXIST);
    CHECK_FALSE(fs::exists(old_path));
    CHECK_FALSE(fs::exists(new_path));
  }

  SUBCASE("File to existing name") {
    fs::path old_path = temp_path / "filetwo.h";
    fs::path new_path = temp_path / "foo.bar";
    int ret = fileoperator.rename(old_path, new_path);

    CHECK_EQ(ret, -FileOperator::OperatorStatus::E_ITEM_EXISTS);
    CHECK(fs::exists(old_path));
    CHECK(fs::exists(new_path));
  }

  SUBCASE("Directory to existing name") {
    fs::path old_path = temp_path / "thedir";
    fs::path new_path = temp_path / "testdir";
    int ret = fileoperator.rename(old_path, new_path);

    CHECK_EQ(ret, -FileOperator::OperatorStatus::E_ITEM_EXISTS);
    CHECK(fs::exists(old_path));
    CHECK(fs::exists(new_path));
  }

  SUBCASE("File to unique name") {
    fs::path old_path = temp_path / "foo.bar";
    fs::path new_path = temp_path / "bar.foo";
    int ret = fileoperator.rename(old_path, new_path);

    CHECK_EQ(ret, FileOperator::OperatorStatus::SUCCESS);
    CHECK_FALSE(fs::exists(old_path));
    CHECK(fs::exists(new_path));
  }

  SUBCASE("Directory to unique name") {
    fs::path old_path = temp_path / "testdir";
    fs::path new_path = temp_path / "newtestdir";
    int ret = fileoperator.rename(old_path, new_path);

    CHECK_EQ(ret, FileOperator::OperatorStatus::SUCCESS);
    CHECK_FALSE(fs::exists(old_path));
    CHECK(fs::exists(new_path));
  }

  fs::remove_all(temp_path);
}

TEST_CASE("FileMe File Operator Creation") {
  fs::path temp_path = fs::temp_directory_path();
  fs::create_directory(temp_path / "temptesting");
  temp_path /= "temptesting";

  FileOperator fileoperator;

  fs::path file_path = temp_path / "foo.bar";
  std::ofstream(file_path).write("baz", 3);

  fs::path dir_path = temp_path / "bardir";
  fs::create_directory(dir_path);

  REQUIRE(fs::exists(file_path));
  REQUIRE(fs::is_directory(dir_path));

  SUBCASE("New file with existing name") {
    fs::path new_file = temp_path / "foo.bar";
    int ret = fileoperator.create(new_file);

    CHECK_EQ(ret, -FileOperator::OperatorStatus::E_ITEM_EXISTS);
    CHECK(fs::exists(new_file));
  }

  SUBCASE("New directory with existing name") {
    fs::path new_dir = temp_path / "bardir";
    int ret = fileoperator.create(new_dir);

    CHECK_EQ(ret, -FileOperator::OperatorStatus::E_ITEM_EXISTS);
    CHECK(fs::exists(new_dir));
  }

  SUBCASE("New file with unique name") {
    fs::path new_file = temp_path / "bar.foo";
    int ret = fileoperator.create(new_file);

    CHECK_EQ(ret, FileOperator::OperatorStatus::SUCCESS);
    CHECK(fs::exists(new_file));
  }

  SUBCASE("New directory with unqiue name") {
    fs::path new_dir = temp_path / "foodir";
    int ret = fileoperator.create(new_dir);

    CHECK_EQ(ret, FileOperator::OperatorStatus::SUCCESS);
    CHECK(fs::exists(new_dir));
  }

  fs::remove_all(temp_path);
}

TEST_CASE("FileMe File Operator Copying") {
  // the working directory of the file operator
  fs::path temp_path = fs::temp_directory_path();
  fs::create_directory(temp_path / "temptesting");
  temp_path /= "temptesting";

  // initialise the file operator
  FileOperator fileoperator;

  // creating files to test with
  fs::path file_path = temp_path / "foo.hpp";

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

  REQUIRE(fs::exists(temp_path / "foo.hpp"));
  REQUIRE(fs::exists(temp_path / "empty"));
  REQUIRE(fs::exists(temp_path / "temp1/baz.foo"));
  REQUIRE(fs::exists(temp_path / "temp2/bar"));
  REQUIRE(fs::exists(temp_path / "temp2/hello.world"));

  SUBCASE("Select nonexistent file") {
    fs::path selected_file = temp_path / "nonefile.t";
    int ret = fileoperator.copy(selected_file);

    CHECK_FALSE(fs::exists(selected_file));
    CHECK_EQ(ret, -FileOperator::OperatorStatus::E_ITEM_DOES_NOT_EXIST);
  }

  SUBCASE("Select nonexistent directory") {
    fs::path selected_file = temp_path / "nulldir";
    int ret = fileoperator.copy(selected_file);

    CHECK_FALSE(fs::exists(selected_file));
    CHECK_EQ(ret, -FileOperator::OperatorStatus::E_ITEM_DOES_NOT_EXIST);
  }

  SUBCASE("Paste existing file") {
    fs::path selected_file = temp_path / "foo.hpp";
    fs::path pasted_file = temp_path / "foo.hpp";

    int ret = fileoperator.copy(selected_file);

    CHECK(fs::exists(selected_file));
    CHECK_EQ(ret, FileOperator::OperatorStatus::SUCCESS);

    ret = fileoperator.paste(pasted_file);
    CHECK_EQ(ret, -FileOperator::OperatorStatus::E_ITEM_EXISTS);
  }

  SUBCASE("Paste existing directory") {
    fs::path selected_dir = temp_path / "empty";
    fs::path pasted_dir = temp_path / "empty";

    int ret = fileoperator.copy(selected_dir);

    CHECK(fs::exists(selected_dir));
    CHECK_EQ(ret, FileOperator::OperatorStatus::SUCCESS);

    ret = fileoperator.paste(pasted_dir);

    CHECK_EQ(ret, -FileOperator::OperatorStatus::E_ITEM_EXISTS);
  }

  SUBCASE("Paste new file") {
    fs::path selected_file = temp_path / "foo.hpp";
    fs::path pasted_file = temp_path / "bar.hpp";

    int ret = fileoperator.copy(selected_file);

    CHECK(fs::exists(selected_file));
    CHECK_EQ(ret, FileOperator::OperatorStatus::SUCCESS);

    ret = fileoperator.paste(pasted_file);

    CHECK(fs::exists(selected_file));
    CHECK(fs::exists(pasted_file));
    CHECK_EQ(ret, FileOperator::OperatorStatus::SUCCESS);
  }

  SUBCASE("Paste new empty directory") {
    fs::path selected_dir = temp_path / "empty";
    fs::path pasted_dir = temp_path / "alsoempty";

    int ret = fileoperator.copy(selected_dir);

    CHECK(fs::exists(selected_dir));
    CHECK_EQ(ret, FileOperator::OperatorStatus::SUCCESS);

    ret = fileoperator.paste(pasted_dir);

    CHECK(fs::exists(selected_dir));
    CHECK(fs::exists(pasted_dir));
    CHECK_EQ(ret, FileOperator::OperatorStatus::SUCCESS);
  }

  SUBCASE("Paste new directory with contents") {
    fs::path selected_dir = temp_path / "temp2";
    fs::path pasted_dir = temp_path / "notempty";

    int ret = fileoperator.copy(selected_dir);

    CHECK(fs::exists(selected_dir));
    CHECK_EQ(ret, FileOperator::OperatorStatus::SUCCESS);

    ret = fileoperator.paste(pasted_dir);

    CHECK(fs::exists(selected_dir));
    CHECK(fs::exists(pasted_dir));
    CHECK_EQ(ret, FileOperator::OperatorStatus::SUCCESS);
  }

  fs::remove_all(temp_path);
}
