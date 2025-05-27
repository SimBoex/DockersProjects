#include "file_searcher.hpp"
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

FileSearcher::FileSearcher(const std::string& directory)
    : directoryPath(directory)
{}

// Search for files with the given extension (e.g. ".txt")
std::vector<std::string> FileSearcher::searchByExtension(const std::string& extension) const {
    std::vector<std::string> results;

    if (!fs::exists(directoryPath) || !fs::is_directory(directoryPath))
        return results;  // empty if invalid directory

    for (const auto& entry : fs::recursive_directory_iterator(directoryPath)) {
        if (entry.is_regular_file() && entry.path().extension() == extension) {
            results.push_back(entry.path().string());
        }
    }

    return results;
}

// Search for files with the substring in their filename
std::vector<std::string> FileSearcher::searchByNameContains(const std::string& substring) const {
    std::vector<std::string> results;

    if (!fs::exists(directoryPath) || !fs::is_directory(directoryPath))
        return results;  // empty if invalid directory

    for (const auto& entry : fs::recursive_directory_iterator(directoryPath)) {
        if (entry.is_regular_file()) {
            std::string filename = entry.path().filename().string();
            if (filename.find(substring) != std::string::npos) {
                results.push_back(entry.path().string());
            }
        }
    }
            
    return results;
}                   
