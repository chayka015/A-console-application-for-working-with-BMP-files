#ifndef BMP_IMAGE_H
#define BMP_IMAGE_H

#include <vector>
#include <string>
#include <cstdint>

#pragma pack(push, 1)
struct BMPHeader {
    uint16_t signature;
    uint32_t fileSize;
    uint32_t reserved;
    uint32_t dataOffset;
};

struct BMPInfoHeader {
    uint32_t headerSize;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitsPerPixel;
    uint32_t compression;
    uint32_t imageSize;
    int32_t xPixelsPerMeter;
    int32_t yPixelsPerMeter;
    uint32_t colorsUsed;
    uint32_t colorsImportant;
};
#pragma pack(pop)

class BMPImage {
public:
    bool load(const std::string& filename);
    bool save(const std::string& filename) const;
    void display() const;
    void drawCross();

private:
    void drawLine(int x1, int y1, int x2, int y2);
    void setPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);
    void validateImage() const;

    BMPHeader header{};
    BMPInfoHeader infoHeader{};
    std::vector<uint8_t> pixelData;
    bool is32bit = false;
};
#endif