#include <fileme/manager.h>

#include <fstream>

int Manager::remove(fs::path target_path) {
  if (target_path.empty()) {
    return -1;
  }

  int ret = 0;

  if (fs::is_directory(target_path)) {
    ret = fs::remove_all(target_path); // delete the directory and it's contents
  } else {
    ret = fs::remove(target_path); // delete the file
  }

  if (ret == 0) {
    return -1; // no files/directories were deleted
  } else {
    return 0; // the target file / directory was deleted
  }
}

// rename a file or empty directory
int Manager::rename(fs::path old_path, fs::path new_path) {
  if (old_path.empty() || new_path.empty()) {
    return -1;
  }

  if (fs::is_directory(old_path)) {
    if (fs::is_directory(new_path) && fs::is_empty(new_path)) {
      fs::rename(old_path, new_path);
      return 0;
    }

    return -1;
  } else {
    if (!fs::is_directory(new_path)) {
      fs::rename(old_path, new_path);
      return 0;
    }

    return -1;
  }
}

int Manager::create(fs::path new_path) {
  std::ofstream{ new_path }.put('a');
}

int Manager::copy(fs::path target_path) {

}

int Manager::paste(fs::path destination_path) {
  
  
  return 0;
}