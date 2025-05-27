// Include the Google Test framework header
#include "gtest/gtest.h"

// Include necessary standard library headers for testing
#include <fstream>      // For creating temporary test files
#include <filesystem>   // For managing test directories (C++17 standard library)
#include <atomic>       // For thread-safe counters (useful for ThreadPool tests)
#include <vector>
#include <string>
#include <future>
#include <set>

// Include your application's headers for the classes you want to test
// These paths are relative to the 'include/' directory due to the -I flag in compilation
#include "file_searcher.hpp"
#include "thread_pool.hpp"
#include "result_aggregator.hpp"


// --- 1. Placeholder Unit Tests for ThreadPool Class  ---
TEST(ThreadPoolTest, BasicTaskExecution) {
    ThreadPool pool(4);  // 4 threads in pool

    std::atomic<int> counter{0};
    const int numTasks = 100;

    std::vector<std::future<void>> futures;

    for (int i = 0; i < numTasks; ++i) {
        futures.emplace_back(pool.enqueue([&counter] {
            counter.fetch_add(1, std::memory_order_relaxed);
        }));
    }

    // Wait for all tasks to finish
    for (auto &fut : futures) {
        fut.get();
    }

    // Check if all tasks executed and incremented the counter
    EXPECT_EQ(counter.load(), numTasks);
}

// --- 2. Placeholder Unit Tests for ResultAggregator Class (Update when code provided) ---
// You will replace these with actual tests once result_aggregator.hpp/cpp are available.
TEST(ResultAggregatorTest, PlaceholderTest) {
    // This is a placeholder. You'll add real tests here once result_aggregator.hpp/cpp is available.
    // Ensure result_aggregator.hpp and result_aggregator.cpp exist (even if empty) to compile.
    // ResultAggregator aggregator; // Example, uncomment only if you have ResultAggregator defined.
    EXPECT_TRUE(true) << "ResultAggregator tests will go here.";
}




// Define a test fixture class that inherits from ::testing::Test
class FileSearcherTest : public ::testing::Test {
protected:
    // This path will be the root for our temporary test files
    std::filesystem::path temp_test_dir_path;

    // SetUp() is called before each test in this fixture
    void SetUp() override {
        // Create a unique temporary directory for each test case
        temp_test_dir_path = std::filesystem::temp_directory_path() / "fs_test_data";
        std::filesystem::create_directories(temp_test_dir_path);

        // Create some known test files with specific content and names
        // File for .txt extension search
        std::ofstream(temp_test_dir_path / "document1.txt") << "content";
        std::ofstream(temp_test_dir_path / "another_file.txt") << "more content";


        // File for .log extension search
        std::ofstream(temp_test_dir_path / "app.log") << "log data";

        // File for name contains search
        std::ofstream(temp_test_dir_path / "report_final.doc") << "final report";
        std::ofstream(temp_test_dir_path / "summary_report.xls") << "summary data";
        std::ofstream(temp_test_dir_path / "image.png") << "binary data"; // Should not match text search

        


        // Create subdirectories with files
        std::filesystem::create_directory(temp_test_dir_path / "subdir");
        std::ofstream((temp_test_dir_path / "subdir" /"nested_file.txt"));
        std::ofstream((temp_test_dir_path / "subdir" / "sub_report.pdf").string()); // For name contains "report"

        // Empty folder
        std::filesystem::create_directory(temp_test_dir_path / "empty_folder");
        std::cout << "setting the files" << std::endl;
        std::cout << "Created files and directories:" << std::endl;
        
        

    }

    // TearDown() is called after each test in this fixture
    void TearDown() override {
        // Clean up the temporary directory and all its contents
        if (std::filesystem::exists(temp_test_dir_path)) {
            std::filesystem::remove_all(temp_test_dir_path);
        }
    }
};

// Use TEST_F for tests that use a fixture (F for Fixture)
TEST_F(FileSearcherTest, SearchByExtensionTxt) {
    // std::cout << temp_test_dir_path << std::endl;
    FileSearcher searcher(temp_test_dir_path.string());
    std::vector<std::string> results = searcher.searchByExtension(".txt");

    // We expect document1.txt, another_file.txt, and subdir/nested_file.txt
    ASSERT_EQ(results.size(), 3) << "Should find 3 .txt files.";

    // Convert results to a set for easier checking of existence
    std::set<std::string> resultSet(results.begin(), results.end());

    EXPECT_TRUE(resultSet.count((temp_test_dir_path / "document1.txt").string()));
    EXPECT_TRUE(resultSet.count((temp_test_dir_path / "another_file.txt").string()));
    EXPECT_TRUE(resultSet.count((temp_test_dir_path / "subdir" / "nested_file.txt").string()));
    EXPECT_FALSE(resultSet.count((temp_test_dir_path / "app.log").string())); // Ensure other extensions are not included
}



TEST_F(FileSearcherTest, SearchByExtensionLog) {
    FileSearcher searcher(temp_test_dir_path.string());
    std::vector<std::string> results = searcher.searchByExtension(".log");

    ASSERT_EQ(results.size(), 1) << "Should find 1 .log file.";
    EXPECT_EQ(results[0], (temp_test_dir_path / "app.log").string());
}



TEST_F(FileSearcherTest, SearchByExtensionNoMatch) {
    FileSearcher searcher(temp_test_dir_path.string());
    std::vector<std::string> results = searcher.searchByExtension(".json"); // Non-existent extension

    EXPECT_TRUE(results.empty()) << "Should find no .json files.";
}


TEST_F(FileSearcherTest, SearchByNameContainsReport) {
    FileSearcher searcher(temp_test_dir_path.string());
    std::vector<std::string> results = searcher.searchByNameContains("report");

    // We expect report_final.doc and sub_report.pdf
    ASSERT_EQ(results.size(), 3) << "Should find 3 files containing 'report' in their name.";

    std::set<std::string> resultSet(results.begin(), results.end());
    EXPECT_TRUE(resultSet.count((temp_test_dir_path / "report_final.doc").string()));
    EXPECT_TRUE(resultSet.count((temp_test_dir_path / "subdir" / "sub_report.pdf").string()));
    EXPECT_FALSE(resultSet.count((temp_test_dir_path / "document1.txt").string())); // Should not match
}

TEST_F(FileSearcherTest, SearchByNameContainsDoc) {
    FileSearcher searcher(temp_test_dir_path.string());
    std::vector<std::string> results = searcher.searchByNameContains(".doc");

    ASSERT_EQ(results.size(), 1) << "Should find 1 file containing 'doc' in its name.";
    EXPECT_EQ(results[0], (temp_test_dir_path / "report_final.doc").string());
}




TEST_F(FileSearcherTest, SearchByNameContainsNoMatch) {
    FileSearcher searcher(temp_test_dir_path.string());
    std::vector<std::string> results = searcher.searchByNameContains("nonexistentname");

    EXPECT_TRUE(results.empty()) << "Should find no files with 'nonexistentname'.";
}


TEST_F(FileSearcherTest, SearchInInvalidOrNonExistentDirectory) {
    std::filesystem::path invalid_path = temp_test_dir_path / "nonexistent_subdir";
    FileSearcher searcher(invalid_path.string());
 // A path that definitely doesn't exist
    std::vector<std::string> results_ext = searcher.searchByExtension(".txt");
    EXPECT_TRUE(results_ext.empty()) << "Should return empty for invalid directory (extension search).";

    std::vector<std::string> results_name = searcher.searchByNameContains("file");
    EXPECT_TRUE(results_name.empty()) << "Should return empty for invalid directory (name search).";
}


// --- Main Entry Point for Google Tests ---
// This main function is automatically provided by Google Test if you link against `gtest_main`.
// You typically don't write this yourself when using GTest::gtest_main.
// Your CMakeLists.txt handles linking with GTest::gtest_main, so this section is commented out.
/*
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv); // Initializes the Google Test framework
    return RUN_ALL_TESTS();                 // Runs all tests defined with TEST() and TEST_F()
}
*/