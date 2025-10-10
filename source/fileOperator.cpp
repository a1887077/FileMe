#include <fileme/fileOperator.h>

#include <fstream>

int FileOperator::remove(fs::path target_path) {
  if (!fs::exists(target_path)) {
    return -E_ITEM_DOES_NOT_EXIST;
  }

  int ret = 0;

  if (fs::is_directory(target_path)) {
    ret = fs::remove_all(target_path);  // delete the directory and it's contents
  } else {
    ret = fs::remove(target_path);  // delete the file
  }

  if (ret == 0) {
    return -E_INVALID_PATH;  // no files/directories were deleted
  } else {
    return SUCCESS;  // the target file / directory was deleted
  }
}

// rename a file or empty directory
int FileOperator::rename(fs::path old_path, fs::path new_path) {
  if (!fs::exists(old_path)) {
    return -E_ITEM_DOES_NOT_EXIST;
  } else if (fs::exists(new_path)) {
    return -E_ITEM_EXISTS;
  }

  if (old_path.has_filename() != new_path.has_filename()) {
    return -E_DIFFERENT_TYPES;
  }

  fs::rename(old_path, new_path);

  return SUCCESS;
}

int FileOperator::create(fs::path new_path) {
  if (fs::exists(new_path)) {
    return -E_ITEM_EXISTS;
  }

  if (new_path.has_filename()) {
    std::ofstream{new_path}.flush();
  } else {
    fs::create_directories(new_path);
  }

  return SUCCESS;
}

int FileOperator::copy(fs::path target_path) {
  if (!fs::exists(target_path)) {
    return -E_ITEM_DOES_NOT_EXIST;
  }

  this->copy_path = target_path;

  return SUCCESS;
}

int FileOperator::paste(fs::path destination_path) {
  if (fs::exists(destination_path)) {
    return -E_ITEM_EXISTS;
  } else if (!fs::exists(this->copy_path)) {
    return -E_ITEM_DOES_NOT_EXIST;
  }

  fs::copy(this->copy_path, destination_path);

  return SUCCESS;
}
