#ifndef FILE_H
#define FILE_H

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

/**
 * @brief The type of DirEntry
 */
enum DirEntryType {
  NULL_ENTRY,
  FILE_ENTRY,
  DIRECTORY_ENTRY
};

/**
 * @brief A class to describe entries in a directory
 */
class DirEntry {
  friend class Manager;
private:
  // the absolute path of the entry
  fs::path entry_path;
  // the name of the entry
  std::string entry_name;
  // the type of the entry
  DirEntryType entry_type;

public:
  DirEntry(fs::path _entrypath, std::string _entryname, DirEntryType _entrytype)
      : entry_path(_entrypath), entry_name(_entryname), entry_type(_entrytype) {};
  DirEntry() : DirEntry(fs::path(), "", NULL_ENTRY) {};
};

#endif  // FILE_H