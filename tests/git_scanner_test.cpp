// tests/git_scanner_test.cpp
#include <gtest/gtest.h>
#include "../src/git_scanner.hpp"
#include <filesystem>

class GitScannerTest : public ::testing::Test {
protected:
    std::filesystem::path original_path;
    std::filesystem::path test_repo_path;
    std::filesystem::path empty_repo_path;
    std::filesystem::path untracked_repo_path;

    void SetUp() override {
        // Save current path
        original_path = std::filesystem::current_path();
        
        // Create test repo path
        test_repo_path = original_path / "test_repo";
        empty_repo_path = original_path / "empty_repo";
        untracked_repo_path = original_path / "untracked_repo";
        
        // Remove test directories if they exist
        if (std::filesystem::exists(test_repo_path)) {
            std::filesystem::remove_all(test_repo_path);
        }
        if (std::filesystem::exists(empty_repo_path)) {
            std::filesystem::remove_all(empty_repo_path);
        }
        if (std::filesystem::exists(untracked_repo_path)) {
            std::filesystem::remove_all(untracked_repo_path);
        }
        
        // Create a temporary git repository with files
        std::filesystem::create_directory(test_repo_path);
        std::filesystem::current_path(test_repo_path);
        system("git init");
        system("touch test1.txt test2.txt");
        system("git add .");
        system("git commit -m 'Initial commit'");
        
        // Create an empty git repository
        std::filesystem::current_path(original_path);
        std::filesystem::create_directory(empty_repo_path);
        std::filesystem::current_path(empty_repo_path);
        system("git init");
        
        // Create a git repository with no tracked files
        std::filesystem::current_path(original_path);
        std::filesystem::create_directory(untracked_repo_path);
        std::filesystem::current_path(untracked_repo_path);
        system("git init");
        system("touch test1.txt test2.txt");
        
        // Return to original directory
        std::filesystem::current_path(original_path);
    }

    void TearDown() override {
        // Clean up test repositories
        std::filesystem::current_path(original_path);
        if (std::filesystem::exists(test_repo_path)) {
            std::filesystem::remove_all(test_repo_path);
        }
        if (std::filesystem::exists(empty_repo_path)) {
            std::filesystem::remove_all(empty_repo_path);
        }
        if (std::filesystem::exists(untracked_repo_path)) {
            std::filesystem::remove_all(untracked_repo_path);
        }
    }
};

TEST_F(GitScannerTest, DetectsGitRepository) {
    EXPECT_TRUE(GitScanner::isGitRepository(test_repo_path.string()));
    EXPECT_TRUE(GitScanner::isGitRepository(empty_repo_path.string()));
    EXPECT_TRUE(GitScanner::isGitRepository(untracked_repo_path.string()));
    EXPECT_FALSE(GitScanner::isGitRepository("/nonexistent"));
}

TEST_F(GitScannerTest, ListsTrackedFiles) {
    GitScanner scanner;
    auto files = scanner.getTrackedFiles(test_repo_path.string());
    EXPECT_EQ(files.size(), 2);
    EXPECT_TRUE(std::find(files.begin(), files.end(), "test1.txt") != files.end());
    EXPECT_TRUE(std::find(files.begin(), files.end(), "test2.txt") != files.end());
}

TEST_F(GitScannerTest, EmptyRepoHasNoFiles) {
    GitScanner scanner;
    auto files = scanner.getTrackedFiles(empty_repo_path.string());
    EXPECT_TRUE(files.empty());
}

TEST_F(GitScannerTest, UntrackedRepoHasNoFiles) {
    GitScanner scanner;
    auto files = scanner.getTrackedFiles(untracked_repo_path.string());
    EXPECT_TRUE(files.empty());
}
