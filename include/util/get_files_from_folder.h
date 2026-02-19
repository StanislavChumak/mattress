#ifndef GET_FILES_FROM_FOLDER_H
#define GET_FILES_FROM_FOLDER_H

#include <unordered_set>
#include <string>

std::unordered_set<std::string> get_files_from_folder(const std::string &dirPath, const std::string &extension);

#endif
