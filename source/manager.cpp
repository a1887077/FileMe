#include <fileme/manager.h>

/**
 * @brief Construct a list of files in the current workspace directory
 */
void Manager::buildList(void) {
  this->entry_list.clear();
  auto dir_iter = fs::directory_iterator(this->workspace_path);

  for (auto dir_item : dir_iter) {
    fs::path item_path = dir_item.path();
    std::string item_name = item_path.filename();
    DirEntryType item_type = dir_item.is_directory() ? DIRECTORY_ENTRY : FILE_ENTRY;
    this->entry_list.emplace_back(item_path, item_name, item_type);
  }
}

/**
 * @brief Construct a Manager given a workspace directory
 * @param _workspace_path The desired workspace path
 */
Manager::Manager(fs::path _workspace_path) {
  this->workspace_path = _workspace_path;

  this->buildList();
}

/**
 * @brief Get the list of the contents of the workspace directory
 * @retval A vector of DirEntry objects describing the entries in the workspace directory
 */
std::vector<DirEntry> Manager::getEntries(void) { return this->entry_list; }

/**
 * @brief Get the number of entries recorded in the workspace directory
 * @retval A size_t describing the list of elements int the entry_list vector
 */
size_t Manager::numEntries(void) { return this->entry_list.size(); }

/**
 * @brief Remove a DirEntry item and it's corresponding filesystem entry
 * @param entry The DirEntry object describing the file or directory to remove
 * @retval 0 on success, or negative FileOperator::OperatorError code on failure
 */
int Manager::remove(DirEntry entry) {
  int ret = 0;
  ret = FileOperator::remove(entry.entry_path);

  this->buildList();

  return ret;
}

/**
 * @brief Create a new DirEntry item and corresponding filesystem entry
 * @param name The name of the new file or directory to create
 * @param type A dirEntry::dirEntryType describing the type dirEntry to create
 * @retval 0 on success, or negative FileOperator::OperatorError code on failure
 */
int Manager::create(std::string name, DirEntryType type) {
  fs::path new_item_path = this->workspace_path / name;

  int ret = 0;
  ret = FileOperator::create(new_item_path);

  if (ret == 0) {
    this->entry_list.emplace_back(new_item_path, name, type);
  }

  return ret;
}

/**
 * @brief Rename a DirEntry and it's corresponding filesystem entry
 * @param entry A reference to the DirEntry object to rename
 * @param new_name The new name of the DirEntry
 * @retval 0 on success, or negative FileOperator::OperatorError code on failure
 */
int Manager::rename(DirEntry& entry, std::string new_name) {
  fs::path old_path = entry.entry_path;
  fs::path new_path = entry.entry_path.replace_filename(new_name);

  int ret = 0;
  ret = FileOperator::rename(old_path, new_path);

  if (ret == 0) {
    entry.entry_name = new_name;
    entry.entry_path = new_path;
  }

  return ret;
}

/**
 * @brief Select a DirEntry to be copied
 * @param entry The DirEntry item to be selected
 * @retval 0 on success, or negative FileOperator::OperatorError code on failure
 */
int Manager::copy(DirEntry entry) { return FileOperator::copy(entry.entry_path); }
