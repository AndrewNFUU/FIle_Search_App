#include <iostream>
#include <filesystem>
#include <thread>
#include <vector>

namespace fs = std::filesystem;

void searchFiles(const std::string& directory, const std::string& targetFileName) {
    for (const auto& entry : fs::recursive_directory_iterator(directory, fs::directory_options::skip_permission_denied)) {
        try {
            if (entry.path().filename() == targetFileName) {
                std::cout << "File found: " << entry.path() << std::endl;
            }
        }
        catch (const fs::filesystem_error& ex) {}
    }
}

void searchRootDirectories(const std::string& root, const std::string& targetFileName) {
    std::vector<std::thread> threads;
    for (const auto& entry : fs::directory_iterator(root, fs::directory_options::skip_permission_denied)) {
        try {
            if (entry.path().filename() == targetFileName) {
                std::cout << "File found: " << entry.path() << std::endl;
            }

            if (fs::is_directory(entry)) {
                threads.push_back(std::thread(searchFiles, entry.path().string(), targetFileName));
            }
        }
        catch (const fs::filesystem_error& ex) {}
    }

    for (auto& thread : threads) {
        thread.join();
    }
}

int main() {
    std::string targetFileName;
    std::cout << "Enter the name of the file to search: ";
    std::cin >> targetFileName;

    std::string searchDirectory = "C:\\";
    searchRootDirectories(searchDirectory, targetFileName);

    system("pause"); return 0;
}