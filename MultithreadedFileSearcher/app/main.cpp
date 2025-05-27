#include <iostream>
#include <string>
#include <filesystem>
#include "file_searcher.hpp"
#include "result_aggregator.hpp"

int main() {
    std::string inputPath;
    std::cout << "Enter the absolute path of the directory to search: ";
    std::getline(std::cin, inputPath);

    std::filesystem::path dirPath(inputPath);

    // Check if the path is valid and exists
    if (!std::filesystem::exists(dirPath) || !std::filesystem::is_directory(dirPath)) {
        std::cerr << "Error: The provided path does not exist or is not a directory.\n";
        return 1;
    }

    // Create a FileSearcher for the given directory
    FileSearcher searcher(dirPath.string());

    // Search for all ".txt" files
    auto txtFiles = searcher.searchByExtension(".txt");

    // Search for files containing "log" in their names
    auto logFiles = searcher.searchByNameContains("log");

    // Aggregate the results
    ResultAggregator aggregator;
    aggregator.addResults(txtFiles);
    aggregator.addResults(logFiles);

    // Get unique results (no duplicates)
    auto uniqueResults = aggregator.getUniqueResults();

    // Print the results
    std::cout << "Found " << uniqueResults.size() << " unique files:\n";
    for (const auto& file : uniqueResults) {
        std::cout << file << '\n';
    }

    return 0;
}
