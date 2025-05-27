#ifndef FILE_SEARCHER_HPP
#define FILE_SEARCHER_HPP

#include <string>
#include <vector>
#include <filesystem>   // C++17 filesystem API

class FileSearcher {
public:
    // Constructor: initialize with directory path to search in
    explicit FileSearcher(const std::string& directory);

    // Search for files with specific extension (e.g. ".txt")
    // Returns vector of file paths as strings
    std::vector<std::string> searchByExtension(const std::string& extension) const;

    // Search for files containing substring in their filename
    std::vector<std::string> searchByNameContains(const std::string& substring) const;

private:
    std::string directoryPath;  // Directory to search in
};

#endif // FILE_SEARCHER_HPP
