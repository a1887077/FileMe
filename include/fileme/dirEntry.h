#ifndef FILE_H
#define FILE_H

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

class DirEntry {
private:
  fs::path entrypath;
  std::string entryname;
  bool entrytype;

public:
  DirEntry(fs::path _entrypath, std::string _entryname, bool _entrytype)
      : entrypath(_entrypath), entryname(_entryname), entrytype(_entrytype) {};
  DirEntry() : DirEntry(fs::path(), "", false) {};
};

#endif  // FILE_H