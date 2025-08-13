#ifndef UTILS_H
#define UTILS_H

#include <H2DE/utils/H2DE_utils.h>

#define GAME_SIZE 10
#define NO_SELECTED_PIECES -1

using PieceID = uint8_t;
using PieceStructure = std::vector<std::vector<bool>>;

struct Piece {
    PieceStructure structure = {{}};
    H2DE_ColorRGB color = H2DE_ColorRGB();
};

struct PieceCell {
    H2DE_Translate translate = H2DE_Translate();
    H2DE_ColorRGB color = H2DE_ColorRGB();

    bool operator==(const PieceCell& other) const {
        return (translate == other.translate && color == other.color);
    }
};

struct Choice {
    Piece piece = Piece();
    H2DE_ButtonObject* object = nullptr;
};

#endif
