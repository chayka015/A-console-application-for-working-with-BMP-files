#include "../include/bmp_image.h"
#include <fstream>
#include <iostream>
#include <stdexcept>

bool BMPImage::load(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    file.read(reinterpret_cast<char*>(&header), sizeof(header));
    file.read(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));

    if (header.signature != 0x4D42) {
        throw std::runtime_error("Not a BMP file");
    }

    is32bit = (infoHeader.bitsPerPixel == 32);
    if (!(infoHeader.bitsPerPixel == 24 || infoHeader.bitsPerPixel == 32)) {
        throw std::runtime_error("Only 24/32-bit BMP supported");
    }

    file.seekg(header.dataOffset, std::ios::beg);
    pixelData.resize(infoHeader.width * infoHeader.height * (is32bit ? 4 : 3));
    file.read(reinterpret_cast<char*>(pixelData.data()), pixelData.size());

    validateImage();
    return true;
}

bool BMPImage::save(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot create file: " + filename);
    }

    file.write(reinterpret_cast<const char*>(&header), sizeof(header));
    file.write(reinterpret_cast<const char*>(&infoHeader), sizeof(infoHeader));
    file.seekp(header.dataOffset, std::ios::beg);
    file.write(reinterpret_cast<const char*>(pixelData.data()), pixelData.size());

    return true;
}

void BMPImage::display() const {
    for (int y = infoHeader.height - 1; y >= 0; --y) {
        for (int x = 0; x < infoHeader.width; ++x) {
            int idx = (y * infoHeader.width + x) * (is32bit ? 4 : 3);
            uint8_t r = pixelData[idx + 2];
            uint8_t g = pixelData[idx + 1];
            uint8_t b = pixelData[idx];

            std::cout << (r == 0 && g == 0 && b == 0 ? "#" : " ");
        }
        std::cout << '\n';
    }
}

void BMPImage::drawCross() {

    drawLine(0, infoHeader.height-1, infoHeader.width-1, 0);
    drawLine(0, 0, infoHeader.width-1, infoHeader.height-1);
}

void BMPImage::drawLine(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        setPixel(x1, y1, 0, 0, 0); // Черный цвет для линий

        if (x1 == x2 && y1 == y2) break;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void BMPImage::setPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    if (x < 0 || x >= infoHeader.width || y < 0 || y >= infoHeader.height) return;

    int idx = (y * infoHeader.width + x) * (is32bit ? 4 : 3);
    pixelData[idx] = b;
    pixelData[idx + 1] = g;
    pixelData[idx + 2] = r;
}

void BMPImage::validateImage() const {
    for (size_t i = 0; i < pixelData.size(); i += (is32bit ? 4 : 3)) {
        uint8_t r = pixelData[i + 2];
        uint8_t g = pixelData[i + 1];
        uint8_t b = pixelData[i];

        bool isBlack = (r == 0 && g == 0 && b == 0);
        bool isWhite = (r == 255 && g == 255 && b == 255);

        if (!isBlack && !isWhite) {
            throw std::runtime_error("Image contains colors other than black and white");
        }
    }
}