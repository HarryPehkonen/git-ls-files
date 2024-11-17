// src/main.cpp
#include "git_scanner.hpp"
#include <iostream>
#include <filesystem>

int main(int argc, char* argv[]) {
    try {
        std::string path = argc > 1 ? argv[1] : std::filesystem::current_path().string();
        
        GitScanner scanner;
        auto files = scanner.getTrackedFiles(path);
        
        for (const auto& file : files) {
            std::cout << file << '\n';
        }
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}
