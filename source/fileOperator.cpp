#include <fileme/fileOperator.h>

#include <fstream>

int FileOperator::remove(fs::path target_path) {
  if (target_path.empty()) {
    return -1;
  }

  int ret = 0;

  if (fs::is_directory(target_path)) {
    ret = fs::remove_all(target_path);  // delete the directory and it's contents
  } else {
    ret = fs::remove(target_path);  // delete the file
  }

  if (ret == 0) {
    return -1;  // no files/directories were deleted
  } else {
    return 0;  // the target file / directory was deleted
  }
}

// rename a file or empty directory
int FileOperator::rename(fs::path old_path, fs::path new_path) {
  if (!fs::exists(old_path) || fs::exists(new_path)) {
    return -1;
  }

  if (old_path.has_filename() == new_path.has_filename()) {
    fs::rename(old_path, new_path);
    return 0;
  } else {
    return -1;
  }
}

int FileOperator::create(fs::path new_path) {
  std::ofstream{new_path}.put('a');

  return 0;
}

/* int FileOperator::copy(fs::path target_path) {

  return 0;
}

int FileOperator::paste(fs::path destination_path) {


  return 0;
} */