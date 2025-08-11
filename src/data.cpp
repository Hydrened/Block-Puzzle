#include "data.h"

// INIT
Data::Data(Game* g) : game(g) {
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

void Data::initBlockPieces() {
    pieces[0] = { { { 1 } }, { 255, 85, 85, 255 } };
    pieces[1] = { { { 1, 1 }, { 1, 1 } }, { 255, 170, 0, 255 } };
    pieces[2] = { { { 1, 1, 1 }, { 1, 1, 1 }, { 1, 1, 1 } }, { 255, 255, 85, 255 } };
}

void Data::initLinePieces() {
    pieces[3] = { { { 1, 1 } }, { 85, 255, 85, 255 } };
    pieces[4] = { { { 1 }, { 1 } }, { 85, 255, 85, 255 } };

    pieces[5] = { { { 1, 1, 1 } }, { 85, 255, 255, 255 } };
    pieces[6] = { { { 1 }, { 1 }, { 1 } }, { 85, 255, 255, 255 } };

    pieces[7] = { { { 1, 1, 1, 1 } }, { 85, 170, 255, 255 } };
    pieces[8] = { { { 1 }, { 1 }, { 1 }, { 1 } }, { 85, 170, 255, 255 } };

    pieces[9] = { { { 1, 1, 1, 1, 1 } }, { 170, 85, 255, 255 } };
    pieces[10] = { { { 1 }, { 1 }, { 1 }, { 1 }, { 1 } }, { 170, 85, 255, 255 } };
}

void Data::initLsPieces() {
    pieces[11] = { { { 1, 0, 0 }, { 1, 1, 1 } }, { 255, 85, 170, 255 } };
    pieces[12] = { { { 0, 0, 1 }, { 1, 1, 1 } }, { 255, 85, 170, 255 } };

    pieces[13] = { { { 1, 1, 1 }, { 1, 0, 0 } }, { 255, 85, 170, 255 } };
    pieces[14] = { { { 1, 1, 1 }, { 0, 0, 1 } }, { 255, 85, 170, 255 } };

    pieces[15] = { { { 1, 1 }, { 0, 1 }, { 0, 1 } }, { 255, 85, 170, 255 } };
    pieces[16] = { { { 1, 1 }, { 1, 0 }, { 1, 0 } }, { 255, 85, 170, 255 } };

    pieces[17] = { { { 0, 1 }, { 0, 1 }, { 1, 1 } }, { 255, 85, 170, 255 } };
    pieces[18] = { { { 1, 0 }, { 1, 0 }, { 1, 1 } }, { 255, 85, 170, 255 } };
}

void Data::initSsPieces() {
    pieces[19] = { { { 1, 1, 0 }, { 0, 1, 1 } }, { 170, 255, 85, 255 } };
    pieces[20] = { { { 0, 1, 1 }, { 1, 1, 0 } }, { 170, 255, 85, 255 } };

    pieces[21] = { { { 1, 0 }, { 1, 1 }, { 0, 1 } }, { 170, 255, 85, 255 } };
    pieces[22] = { { { 0, 1 }, { 1, 1 }, { 1, 0 } }, { 170, 255, 85, 255 } };
}

void Data::initCornerPieces() {
    pieces[23] = { { { 1, 1 }, { 1, 0 } }, { 255, 200, 124, 255 } };
    pieces[24] = { { { 1, 1 }, { 0, 1 } }, { 255, 200, 124, 255 } };
    pieces[25] = { { { 1, 0 }, { 1, 1 } }, { 255, 200, 124, 255 } };
    pieces[26] = { { { 0, 1 }, { 1, 1 } }, { 255, 200, 124, 255 } };

    pieces[27] = { { { 1, 1, 1 }, { 1, 0, 0 }, { 1, 0, 0 } }, { 144, 238, 144, 255 } };
    pieces[28] = { { { 1, 1, 1 }, { 0, 0, 1 }, { 0, 0, 1 } }, { 144, 238, 144, 255 } };
    pieces[29] = { { { 1, 0, 0 }, { 1, 0, 0 }, { 1, 1, 1 } }, { 144, 238, 144, 255 } };
    pieces[30] = { { { 0, 0, 1 }, { 0, 0, 1 }, { 1, 1, 1 } }, { 144, 238, 144, 255 } };
}

void Data::initPyramidPieces() {
    pieces[31] = { { { 0, 1, 0 }, { 1, 1, 1 } }, { 240, 128, 128, 255 } };
    pieces[32] = { { { 1, 1, 1 }, { 0, 1, 0 } }, { 240, 128, 128, 255 } };

    pieces[33] = { { { 1, 0 }, { 1, 1 }, { 1, 0 } }, { 240, 128, 128, 255 } };
    pieces[34] = { { { 0, 1 }, { 1, 1 }, { 0, 1 } }, { 240, 128, 128, 255 } };
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
