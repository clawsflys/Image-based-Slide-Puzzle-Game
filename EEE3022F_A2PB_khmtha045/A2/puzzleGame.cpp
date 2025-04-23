#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <cstring> // For strcmp
#include "PGMimage.h" // Ensure PGMimage.h is included

class Tile {
private:
    unsigned char* pixelData;
    int width, height;

public:
    Tile(int w, int h) : width(w), height(h) {
        pixelData = new unsigned char[w * h];
    }

    ~Tile() {
        delete[] pixelData;
    }

    void setPixelData(const unsigned char* src) {
        for (int i = 0; i < width * height; ++i) {
            pixelData[i] = src[i];
        }
    }

    unsigned char* getPixelData() const {
        return pixelData;
    }
};

class TileManager {
private:
    std::vector<std::vector<Tile*>> board;
    int size;
    int emptyX, emptyY;
    int tileWidth, tileHeight;

    // Track the last move direction to prevent repetitive moves
    int lastMoveX = 0, lastMoveY = 0;

public:
    TileManager(int s, int tWidth, int tHeight)
        : size(s), emptyX(s - 1), emptyY(s - 1), tileWidth(tWidth), tileHeight(tHeight) {
        board.resize(size, std::vector<Tile*>(size, nullptr));
    }

    ~TileManager() {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                delete board[i][j];
            }
        }
    }

    void initializeTiles(const PGMimage& inputImage);
    void makeMove();
    void saveBoardState(const std::string& baseName, int moveNum);
    void createSummaryImage(const std::string& summaryName, int totalMoves);
};

void TileManager::initializeTiles(const PGMimage& inputImage) {
    const unsigned char* inputBuffer = inputImage.getBuffer();

    int inputWidth, inputHeight;
    inputImage.getDims(inputWidth, inputHeight);

    // Ensure tiles fit within the input image dimensions
    if (tileWidth * size > inputWidth || tileHeight * size > inputHeight) {
        std::cerr << "Error: Grid size exceeds input image dimensions.\n";
        exit(1);
    }

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (i == size - 1 && j == size - 1) continue; // Leave bottom-right empty

            board[i][j] = new Tile(tileWidth, tileHeight);
            unsigned char* tileData = board[i][j]->getPixelData();

            // Extract pixel data for each tile from the input image
            for (int y = 0; y < tileHeight; ++y) {
                for (int x = 0; x < tileWidth; ++x) {
                    int srcX = j * tileWidth + x;
                    int srcY = i * tileHeight + y;
                    tileData[y * tileWidth + x] = inputBuffer[srcY * inputWidth + srcX];
                }
            }
        }
    }
}

void TileManager::makeMove() {
    int dx[] = {0, 0, -1, 1}; // Left, Right, Up, Down
    int dy[] = {-1, 1, 0, 0};
    int direction;

    do {
        direction = rand() % 4; // Random direction
    } while (emptyX + dx[direction] < 0 || emptyX + dx[direction] >= size ||
             emptyY + dy[direction] < 0 || emptyY + dy[direction] >= size ||
             (dx[direction] == -lastMoveX && dy[direction] == -lastMoveY)); // Prevent reverse moves

    // Perform move
    int newX = emptyX + dx[direction];
    int newY = emptyY + dy[direction];

    std::swap(board[emptyX][emptyY], board[newX][newY]);
    emptyX = newX;
    emptyY = newY;

    // Update last move
    lastMoveX = dx[direction];
    lastMoveY = dy[direction];
}

void TileManager::saveBoardState(const std::string& baseName, int moveNum) {
    int imageWidth = size * tileWidth;
    int imageHeight = size * tileHeight;
    unsigned char* imageData = new unsigned char[imageWidth * imageHeight]();

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (!board[i][j]) continue;

            unsigned char* tileData = board[i][j]->getPixelData();
            for (int y = 0; y < tileHeight; ++y) {
                for (int x = 0; x < tileWidth; ++x) {
                    imageData[(i * tileHeight + y) * imageWidth + (j * tileWidth + x)] =
                        tileData[y * tileWidth + x];
                }
            }
        }
    }

    PGMimage image;
    image.setImageData(imageData, imageWidth, imageHeight);
    image.write(baseName + std::to_string(moveNum) + ".pgm");

    delete[] imageData;
}

void TileManager::createSummaryImage(const std::string& summaryName, int totalMoves) {
    int summaryCols = static_cast<int>(std::ceil(std::sqrt(totalMoves)));
    int summaryRows = (totalMoves + summaryCols - 1) / summaryCols;
    int summaryWidth = summaryCols * size * tileWidth;
    int summaryHeight = summaryRows * size * tileHeight;
    unsigned char* summaryData = new unsigned char[summaryWidth * summaryHeight]();

    std::fill(summaryData, summaryData + (summaryWidth * summaryHeight), 255); // White background

    for (int move = 1; move <= totalMoves; ++move) {
        PGMimage tempImage;
        tempImage.read("output" + std::to_string(move) + ".pgm");

        int xOffset = ((move - 1) % summaryCols) * size * tileWidth;
        int yOffset = ((move - 1) / summaryCols) * size * tileHeight;

        const unsigned char* tempData = tempImage.getBuffer();
        for (int y = 0; y < size * tileHeight; ++y) {
            for (int x = 0; x < size * tileWidth; ++x) {
                summaryData[(yOffset + y) * summaryWidth + (xOffset + x)] =
                    tempData[y * (size * tileWidth) + x];
            }
        }
    }

    PGMimage summaryImage;
    summaryImage.setImageData(summaryData, summaryWidth, summaryHeight);
    summaryImage.write(summaryName + ".pgm");

    delete[] summaryData;
}

int main(int argc, char* argv[]) {
    if (argc < 8) {
        std::cerr << "Usage: programName -s <size> -o <baseName> -n <moves> [-x <summaryName>] inputImageName\n";
        return 1;
    }

    int gridSize = 0, numMoves = 0;
    std::string outputBaseName, summaryBaseName, inputFile;
    bool createSummary = false;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-s") == 0) {
            gridSize = std::atoi(argv[++i]);
        } else if (strcmp(argv[i], "-o") == 0) {
            outputBaseName = argv[++i];
        } else if (strcmp(argv[i], "-n") == 0) {
            numMoves = std::atoi(argv[++i]);
        } else if (strcmp(argv[i], "-x") == 0) {
            summaryBaseName = argv[++i];
            createSummary = true;
        } else {
            inputFile = argv[i];
        }
    }

    if (gridSize <= 0 || numMoves <= 0 || outputBaseName.empty() || inputFile.empty()) {
        std::cerr << "Invalid arguments. Please check and try again.\n";
        return 1;
    }

    PGMimage inputImage;
    inputImage.read(inputFile);

    int inputWidth, inputHeight;
    inputImage.getDims(inputWidth, inputHeight);

    TileManager manager(gridSize, inputWidth / gridSize, inputHeight / gridSize);
    manager.initializeTiles(inputImage);

    for (int move = 1; move <= numMoves; ++move) {
        manager.makeMove();
        manager.saveBoardState(outputBaseName, move);
    }

    if (createSummary) {
        manager.createSummaryImage(summaryBaseName, numMoves);
    }

    return 0;
}
