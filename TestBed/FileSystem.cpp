#include "TestBed/FileSystem.hpp"

#include <cstdio>
#include <dirent.h>
#include <sys/types.h>

namespace TestBed {

std::vector<std::string> readFiles(const std::string& path, const std::string& ext) {
  DIR* dir;
  struct dirent* entry;
  std::vector<std::string> files;

  if ((dir = opendir(path.c_str())) == nullptr) {
    std::perror("");
    return files;
  }

  while ((entry = readdir(dir)) != nullptr) {
    std::string filename(entry->d_name);

    if (filename.size() > ext.size() && //
        filename.compare(filename.size() - ext.size(), filename.size(), ext) == 0) {
      files.push_back(path + entry->d_name);
    }
  }
  closedir(dir);

  return files;
}

} /* namespace TestBed */
