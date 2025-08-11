#include "../include/bmp_image.h"
#include <iostream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

std::string resolveInputPath(const std::string& filename) {
    fs::path exeDir = fs::current_path();
    fs::path inputPath = exeDir / "input" / filename;

    if (fs::exists(inputPath)) {
        std::cout << "Found file in: " << inputPath << std::endl;
        return inputPath.string();
    }

    fs::path projectDir = exeDir.parent_path();
    inputPath = projectDir / "input" / filename;

    if (fs::exists(inputPath)) {
        std::cout << "Found file in project directory: " << inputPath << std::endl;
        return inputPath.string();
    }

    if (fs::exists(filename)) {
        std::cout << "Found file via direct path: " << fs::absolute(filename) << std::endl;
        return filename;
    }

    throw std::runtime_error("File not found in any of the checked paths");
}

int main() {

    try {
        BMPImage image;
        std::string inputFile;

        std::cout << "Enter BMP filename (e.g., 'test.bmp'): ";
        std::cin >> inputFile;
        std::string resolvedPath = resolveInputPath(inputFile);
        std::cout << "Trying to load: " << resolvedPath << std::endl;

        if (!image.load(resolvedPath)) {
            std::cerr << "Failed to load image!" << std::endl;
            return 1;
        }
        std::cout << "File loaded successfully!" << std::endl;

        std::cout << "\nOriginal image:" << std::endl;

        image.display();
        image.drawCross();

        std::cout << "\nImage with cross:" << std::endl;
        image.display();

        fs::create_directory("output");

        fs::path inputPath(resolvedPath);
        std::string outputFilename = "output/modified_" + inputPath.filename().string();

        if (image.save(outputFilename)) {
            std::cout << "\nImage saved to: " << fs::absolute(outputFilename) << std::endl;
        } else {
            std::cerr << "Failed to save image!" << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}