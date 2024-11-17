// src/git_scanner.cpp
#include "git_scanner.hpp"
#include <array>
#include <cstdio>
#include <filesystem>
#include <memory>
#include <stdexcept>

GitScanner::GitScanner() = default;
GitScanner::~GitScanner() = default;

bool GitScanner::isGitRepository(const std::string& path) {
    auto original_path = std::filesystem::current_path();
    try {
        std::filesystem::current_path(path);
        auto result = executeGitCommand("git rev-parse --git-dir").find(".git") != std::string::npos;
        std::filesystem::current_path(original_path);
        return result;
    } catch (const GitScannerError&) {
        std::filesystem::current_path(original_path);
        return false;
    } catch (const std::filesystem::filesystem_error&) {
        return false;
    }
}

std::vector<std::string> GitScanner::getTrackedFiles(const std::string& repo_path) {
    if (!isGitRepository(repo_path)) {
        throw GitScannerError("Not a git repository: " + repo_path);
    }

    auto original_path = std::filesystem::current_path();
    std::vector<std::string> files;
    
    try {
        std::filesystem::current_path(repo_path);
        std::string output = executeGitCommand("git ls-files");
        
        std::string current_file;
        for (char c : output) {
            if (c == '\n') {
                if (!current_file.empty()) {
                    files.push_back(current_file);
                    current_file.clear();
                }
            } else {
                current_file += c;
            }
        }
        
        if (!current_file.empty()) {
            files.push_back(current_file);
        }
        
        std::filesystem::current_path(original_path);
        return files;
    } catch (...) {
        std::filesystem::current_path(original_path);
        throw;
    }
}

std::string GitScanner::executeGitCommand(const std::string& command) {
    std::array<char, 128> buffer;
    std::string result;
    
    std::unique_ptr<FILE, decltype(&pclose)> pipe(
        popen(command.c_str(), "r"),
        pclose
    );
    
    if (!pipe) {
        throw GitScannerError("Failed to execute git command: " + command);
    }
    
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    
    return result;
}
