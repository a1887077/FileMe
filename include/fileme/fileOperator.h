#ifndef FILEOPERATOR_H
#define FILEOPERATOR_H

#include <filesystem>

namespace fs = std::filesystem;

/**
 * @brief A class to handle operations on files or directories
 */
class FileOperator {
private:
  // the path of the currently copied file object, if available
  fs::path copy_path;

public:
  /**
   * @brief The different types of errors which can be emitted from an operation
   */
  enum OperatorStatus {
    SUCCESS = 0,
    E_DIFFERENT_TYPES = 1,
    E_INVALID_PATH = 2,
    E_ITEM_EXISTS = 3,
    E_ITEM_DOES_NOT_EXIST = 4,
  };

  /**
   * @brief The default FileOperator constructor
   */
  FileOperator() : copy_path(fs::path()) {};

  // file or directory manipulation functions
  /**
   * @brief Remove an item given it's path
   * @param target_path The path of the file or directory to remove
   * @retval 0 on success, negative error on failure
   */
  int remove(fs::path target_path);

  /**
   * @brief Rename an item given it's path and the new path
   * @param old_path The old path of the object before renaming
   * @param new_path The new path of the object once being renamed
   * @retval 0 on success, negative error on failure
   */
  int rename(fs::path old_path, fs::path new_path);

  /**
   * @brief Create an item given it's desired path
   * @param new_path The path of the new object to create
   * @retval 0 on success, negative error on failure
   */
  int create(fs::path new_path);

  /**
   * @brief Select an item to copy given the item's path
   * @param target_path The path of the item to select
   * @retval 0 on success, negative error on failure
   */
  int copy(fs::path target_path);

  /**
   * @brief Paste a previously selected item into a destination path
   * @param destination_path The path in which to paste the new item
   * @retval 0 on success, negative error on failure
   */
  int paste(fs::path destination_path);

  // search the current workspace
  fs::path search(fs::path search_directory);
};

#endif  // FILEOPERATOR_H
