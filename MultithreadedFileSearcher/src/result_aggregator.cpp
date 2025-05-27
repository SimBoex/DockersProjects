#include "result_aggregator.hpp"
#include <unordered_set>

// Add a single result (e.g. a file path)
void ResultAggregator::addResult(const std::string& result) {
    results_.push_back(result);
}

// Add multiple results at once
void ResultAggregator::addResults(const std::vector<std::string>& results) {
    for (const auto& r : results) {
        results_.push_back(r);
    }
}

// Get all results collected so far
std::vector<std::string> ResultAggregator::getResults() const {
    return results_;
}

// Get unique results (no duplicates)
std::vector<std::string> ResultAggregator::getUniqueResults() const {
    std::unordered_set<std::string> uniq(results_.begin(), results_.end());
    return { uniq.begin(), uniq.end() };
}

// Clear all stored results
void ResultAggregator::clear() {
    results_.clear();
}

// Get count of stored results
size_t ResultAggregator::size() const {
    return results_.size();
}
