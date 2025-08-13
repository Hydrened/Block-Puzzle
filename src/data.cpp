#include "data.h"

// INIT
Data::Data(Game* g) : game(g) {
    initColors();
    initPieces();
}

void Data::initPieces() {
    initBlockPieces();
    initLinePieces();
    initLsPieces();
    initSsPieces();
    initCornerPieces();
    initPyramidPieces();
}

void Data::initColors() {
    colors["green"] = { 85, 255, 85, 255 };
    colors["pink"] = { 255, 85, 170, 255 };
    colors["light-blue"] = { 85, 255, 255, 255 };
    colors["purple"] = { 170, 85, 255, 255 };
    colors["yellow"] = { 255, 255, 85, 255 };
    colors["orange"] = { 255, 130, 56, 255 };
    colors["red"] = { 255, 56, 56, 255 };
    colors["dark-blue"] = { 63, 56, 255, 255 };
    colors["magenta"] = { 235, 56, 255, 255 };
}

void Data::initBlockPieces() {
    pieces[0] = { { { 1 } }, getColor("green") };
    pieces[1] = { { { 1, 1 }, { 1, 1 } }, getColor("orange") };
    pieces[2] = { { { 1, 1, 1 }, { 1, 1, 1 }, { 1, 1, 1 } }, getColor("purple") };
}

void Data::initLinePieces() {
    pieces[3] = { { { 1, 1 } }, getColor("dark-blue") };
    pieces[4] = { { { 1 }, { 1 } }, getColor("dark-blue") };

    pieces[5] = { { { 1, 1, 1 } }, getColor("red") };
    pieces[6] = { { { 1 }, { 1 }, { 1 } }, getColor("red") };

    pieces[7] = { { { 1, 1, 1, 1 } }, getColor("pink") };
    pieces[8] = { { { 1 }, { 1 }, { 1 }, { 1 } }, getColor("pink") };

    pieces[9] = { { { 1, 1, 1, 1, 1 } }, getColor("yellow") };
    pieces[10] = { { { 1 }, { 1 }, { 1 }, { 1 }, { 1 } }, getColor("yellow") };
}

void Data::initLsPieces() {
    pieces[11] = { { { 1, 0, 0 }, { 1, 1, 1 } }, getColor("magenta") };
    pieces[12] = { { { 0, 0, 1 }, { 1, 1, 1 } }, getColor("magenta") };

    pieces[13] = { { { 1, 1, 1 }, { 1, 0, 0 } }, getColor("magenta") };
    pieces[14] = { { { 1, 1, 1 }, { 0, 0, 1 } }, getColor("magenta") };

    pieces[15] = { { { 1, 1 }, { 0, 1 }, { 0, 1 } }, getColor("magenta") };
    pieces[16] = { { { 1, 1 }, { 1, 0 }, { 1, 0 } }, getColor("magenta") };

    pieces[17] = { { { 0, 1 }, { 0, 1 }, { 1, 1 } }, getColor("magenta") };
    pieces[18] = { { { 1, 0 }, { 1, 0 }, { 1, 1 } }, getColor("magenta") };
}

void Data::initSsPieces() {
    pieces[19] = { { { 1, 1, 0 }, { 0, 1, 1 } }, getColor("purple") };
    pieces[20] = { { { 0, 1, 1 }, { 1, 1, 0 } }, getColor("purple") };

    pieces[21] = { { { 1, 0 }, { 1, 1 }, { 0, 1 } }, getColor("purple") };
    pieces[22] = { { { 0, 1 }, { 1, 1 }, { 1, 0 } }, getColor("purple") };
}

void Data::initCornerPieces() {
    pieces[23] = { { { 1, 1 }, { 1, 0 } }, getColor("light-blue") };
    pieces[24] = { { { 1, 1 }, { 0, 1 } }, getColor("light-blue") };
    pieces[25] = { { { 1, 0 }, { 1, 1 } }, getColor("light-blue") };
    pieces[26] = { { { 0, 1 }, { 1, 1 } }, getColor("light-blue") };

    pieces[27] = { { { 1, 1, 1 }, { 1, 0, 0 }, { 1, 0, 0 } }, getColor("green") };
    pieces[28] = { { { 1, 1, 1 }, { 0, 0, 1 }, { 0, 0, 1 } }, getColor("green") };
    pieces[29] = { { { 1, 0, 0 }, { 1, 0, 0 }, { 1, 1, 1 } }, getColor("green") };
    pieces[30] = { { { 0, 0, 1 }, { 0, 0, 1 }, { 1, 1, 1 } }, getColor("green") };
}

void Data::initPyramidPieces() {
    pieces[31] = { { { 0, 1, 0 }, { 1, 1, 1 } }, getColor("orange") };
    pieces[32] = { { { 1, 1, 1 }, { 0, 1, 0 } }, getColor("orange") };

    pieces[33] = { { { 1, 0 }, { 1, 1 }, { 1, 0 } }, getColor("orange") };
    pieces[34] = { { { 0, 1 }, { 1, 1 }, { 0, 1 } }, getColor("orange") };
}

// GETTER
const PieceID Data::getPieceID(const Piece& piece) const {
    for (const auto& [id, otherPIece] : pieces) {
        if (otherPIece.structure == piece.structure) {
            return id;
        }
    }

    throw std::runtime_error("Could not find piece");
    return -1;
}

const Piece& Data::getPiece(PieceID id) const {
    auto it = pieces.find(id);
    
    if (it == pieces.end()) {
        throw std::runtime_error("Can't find piece id \"" + std::to_string(static_cast<int>(id)) + "\"");
    }

    return it->second;
}

const Piece& Data::getRandomPiece() const {
    return getPiece(static_cast<PieceID>(H2DE::randomIntegerInRange(0, pieces.size() - 1)));
}

const H2DE_ColorRGB Data::getColor(const std::string& color) const {
    auto it = colors.find(color);
    
    if (it == colors.end()) {
        throw std::runtime_error("Can't find color \"" + color + "\"");
    }

    return it->second;
}
