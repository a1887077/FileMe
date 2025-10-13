#include <fileme/dirEntry.h>

std::string DirEntry::getName(void) { return this->entry_name; }

fs::path DirEntry::getPath(void) { return this->entry_path; }

DirEntryType DirEntry::getType(void) { return this->entry_type; }
