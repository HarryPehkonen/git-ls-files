// tests/git_scanner_test.cpp
#include <gtest/gtest.h>
#include "../src/git_scanner.hpp"
#include <filesystem>

class GitScannerTest : public ::testing::Test {
protected:
    std::filesystem::path original_path;
    std::filesystem::path test_repo_path;

    void SetUp() override {
        // Save current path
        original_path = std::filesystem::current_path();
        
        // Create test repo path
        test_repo_path = original_path / "test_repo";
        
        // Remove test_repo if it exists
        if (std::filesystem::exists(test_repo_path)) {
            std::filesystem::remove_all(test_repo_path);
        }
        
        // Create a temporary git repository
        std::filesystem::create_directory(test_repo_path);
        std::filesystem::current_path(test_repo_path);
        system("git init");
        system("touch test1.txt test2.txt");
        system("git add .");
        system("git commit -m 'Initial commit'");
    }

    void TearDown() override {
        // Change back to original directory before cleanup
        std::filesystem::current_path(original_path);
        
        // Clean up test repository
        if (std::filesystem::exists(test_repo_path)) {
            std::filesystem::remove_all(test_repo_path);
        }
    }
};

TEST_F(GitScannerTest, DetectsGitRepository) {
    GitScanner scanner;
    EXPECT_TRUE(GitScanner::isGitRepository(test_repo_path.string()));
    EXPECT_FALSE(GitScanner::isGitRepository("/nonexistent"));
}

TEST_F(GitScannerTest, ListsTrackedFiles) {
    GitScanner scanner;
    auto files = scanner.getTrackedFiles(test_repo_path.string());
    EXPECT_EQ(files.size(), 2);
    EXPECT_TRUE(std::find(files.begin(), files.end(), "test1.txt") != files.end());
    EXPECT_TRUE(std::find(files.begin(), files.end(), "test2.txt") != files.end());
}
