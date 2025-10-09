#ifndef FILEOPERATOR_H
#define FILEOPERATOR_H

#include <filesystem>

namespace fs = std::filesystem;

class FileOperator {
private:
  fs::path copy_path;

public:
  // constructor
  FileOperator() : copy_path(fs::path()) {};

  // file or directory manipulation functions
  int remove(fs::path target_path);
  int rename(fs::path old_path, fs::path new_path);
  int create(fs::path new_path);
  int copy(fs::path target_path);
  int paste(fs::path destination_path);

  // search the current workspace
  fs::path search(fs::path search_directory);

  // navigation to different directories
  int nav_back(void);
  int nav_to(fs::path target_path);
};

#endif  // FILEOPERATOR_H
