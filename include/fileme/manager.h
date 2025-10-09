#ifndef MANAGER_H
#define MANAGER_H

#include <fileme/dirEntry.h>
#include <fileme/fileOperator.h>

#include <vector>

class Manager {
private:
  std::vector<DirEntry> entrylist;

public:
  Manager() {};
};

#endif  // MANAGER_H