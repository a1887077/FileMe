#ifndef MANAGER_H
#define MANAGER_H

#include <fileme/dirEntry.h>
#include <fileme/fileOperator.h>

#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

/**
 * @brief A manager class for files and directories, to be used as the high-level interface
 */
class Manager : protected FileOperator {
private:
  // the path of the workspace in which the manager is working
  // a list of DirEntries in the current workspace
  std::vector<DirEntry> entry_list;
  
protected:
  void buildList(void);
  
  public:
  fs::path workspace_path;
  Manager(fs::path _workspace_path);
  Manager() : Manager(fs::current_path()) {};

  /**
   * @brief List the DirEntries in the current workspace
   * @retval A vector of DirEntry types describing the currently visible files and directories
   */
  std::vector<DirEntry> getEntries(void);

  /**
   * @brief Get the number of file entries recorded in the current directory
   * @retval A size_t describing the number of elements in the entry_list member variable
   */
  size_t numEntries(void);

  /**
   * @brief Remove a DirEntry item and it's corresponding filesystem entry
   * @param entry The DirEntry object describing the file or directory to remove
   * @retval 0 on success, or negative FileOperator::OperatorError code on failure
   */
  int remove(DirEntry entry);

  /**
   * @brief Create a new DirEntry item and corresponding filesystem entry
   * @param name The name of the new file or directory to create
   * @param type A dirEntry::dirEntryType describing the type dirEntry to create
   * @retval 0 on success, or negative FileOperator::OperatorError code on failure
   */
  int create(std::string name, DirEntryType type);

  /**
   * @brief Rename a DirEntry and it's corresponding filesystem entry
   * @param entry A reference to the DirEntry object to rename
   * @param new_name The new name of the DirEntry
   * @retval 0 on success, or negative FileOperator::OperatorError code on failure
   */
  int rename(DirEntry &entry, std::string new_name);

  /**
   * @brief Select a DirEntry to be copied
   * @param entry The DirEntry item to be selected
   * @retval 0 on success, or negative FileOperator::OperatorError code on failure
   */
  int copy(DirEntry entry);

  /**
   * @brief Paste the selected DirEntry object into the current working directory with a new name
   * @param new_name The new name of the pasted DirEntry item
   * @retval 0 on success, or negative FileOperator::OperatorError code on failure
   */
  int paste(std::string new_name);

  /**
   * @brief Paste the selected DirEntry object into the current working directory
   * @retval 0 on success, or negative FileOperator::OperatorError code on failure
   */
  int paste(void);
};

#endif  // MANAGER_H