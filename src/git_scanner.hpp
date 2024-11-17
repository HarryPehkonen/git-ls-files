// src/git_scanner.hpp
#pragma once
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

class GitScannerError : public std::runtime_error {
public:
    explicit GitScannerError(const std::string& msg) : std::runtime_error(msg) {}
};

class GitScanner {
public:
    GitScanner();
    ~GitScanner();

    // Get list of tracked files using git command
    std::vector<std::string> getTrackedFiles(const std::string& repo_path);
    
    // Check if path is in a git repository
    static bool isGitRepository(const std::string& path);

private:
    // Execute git command and return output
    static std::string executeGitCommand(const std::string& command);
};
