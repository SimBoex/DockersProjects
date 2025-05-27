#ifndef RESULT_AGGREGATOR_HPP
#define RESULT_AGGREGATOR_HPP

#include <string>
#include <vector>
#include <set>

class ResultAggregator {
public:
    ResultAggregator() = default;

    // Add a single result (e.g. a file path)
    void addResult(const std::string& result);

    // Add multiple results at once
    void addResults(const std::vector<std::string>& results);

    // Get all results collected so far
    std::vector<std::string> getResults() const;

    // Get unique results (no duplicates)
    std::vector<std::string> getUniqueResults() const;

    // Clear all stored results
    void clear();

    // Get count of stored results
    size_t size() const;

private:
    std::vector<std::string> results_;
};

#endif // RESULT_AGGREGATOR_HPP
