#ifndef DATA_H
#define DATA_H

#include "utils.h"
#include <vector>
#include <unordered_map>
#include <string>
class Game;

class Data {
public:
    Data(Game* game);
    ~Data() = default;

    const PieceID getPieceID(const Piece& piece) const;
    const Piece& getPiece(PieceID id) const;
    const Piece& getRandomPiece() const;

    constexpr const H2DE_Scale& getChoicePieceScale() const {
        return choicePieceScale;
    }

    constexpr const float& getChoicePieceTranslateX() const {
        return choicePieceTranslateX;
    }
    constexpr const float& getChoicePieceScaleY() const {
        return choicePieceScaleY;
    }
    constexpr const float& getChoicePieceSpacingY() const {
        return choicePieceSpacingY;
    }

    constexpr const int& getPieceMoveDuration() const {
        return pieceMoveDuration;
    } 
    constexpr const int& getChoiceScaleInDuration() const {
        return choiceScaleInDuration;
    }

private:
    Game* game;

    std::unordered_map<PieceID, Piece> pieces = {};

    const H2DE_Scale choicePieceScale = { 0.5f, 0.5f };

    const float choicePieceTranslateX = -2.5f;
    const float choicePieceScaleY = 3.0f;
    const float choicePieceSpacingY = 1.5f;

    const int pieceMoveDuration = 350;
    const int choiceScaleInDuration = 250;

    void initPieces();
    void initBlockPieces();
    void initLinePieces();
    void initLsPieces();
    void initSsPieces();
    void initCornerPieces();
    void initPyramidPieces();
};

#endif
