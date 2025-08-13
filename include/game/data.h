#pragma once

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

    const H2DE_ColorRGB getColor(const std::string& color) const;

    constexpr const H2DE_Translate& getHighscoreTranslate() const { return highscoreTranslate; }
    constexpr const H2DE_Translate& getScoreTranslate() const { return scoreTranslate; }

    constexpr const H2DE_Scale& getChoicePieceScale() const { return choicePieceScale; }
    constexpr const H2DE_Scale& getUpdateScoreScale() const { return updateScoreScale; }
    constexpr const H2DE_Scale& getScoreFontScale() const { return scoreFontScale; }
    constexpr const H2DE_Scale& getScoreFontSpacing() const { return scoreFontSpacing; }

    constexpr const H2DE_ColorRGB& getEmptyCellColor() const { return emptyCellColor; }
    constexpr const H2DE_ColorRGB& getMapBorderColor() const { return mapBorderColor; }

    constexpr const float& getChoicePieceTranslateX() const { return choicePieceTranslateX; }
    constexpr const float& getChoicePieceScaleY() const { return choicePieceScaleY; }
    constexpr const float& getChoicePieceSpacingY() const { return choicePieceSpacingY; }

    constexpr const int& getGameSize() const { return gameSize; } 
    constexpr const int& getPieceMoveDuration() const { return pieceMoveDuration; } 
    constexpr const int& getChoiceScaleInDuration() const { return choiceScaleInDuration; }
    constexpr const int& getUpdateScoreDuration() const { return updateScoreDuration; }
    constexpr const int& getMapBorderThickness() const { return mapBorderThickness; }

private:
    Game* game;

    std::unordered_map<PieceID, Piece> pieces = {};
    std::unordered_map<std::string, H2DE_ColorRGB> colors = {};

    const H2DE_Translate highscoreTranslate = { 7.5f, -3.5f };
    const H2DE_Translate scoreTranslate = { 7.5f, -2.0f };

    const H2DE_Scale choicePieceScale = { 0.5f, 0.5f };
    const H2DE_Scale updateScoreScale = { 0.7f, 0.7f };
    const H2DE_Scale scoreFontScale = { 0.28125f, 0.39375f };
    const H2DE_Scale scoreFontSpacing = { 0.05f, 0.2f };

    const H2DE_ColorRGB emptyCellColor = { 33, 50, 200, 255 };
    const H2DE_ColorRGB mapBorderColor = { 6, 142, 249, 255 };

    const float choicePieceTranslateX = -3.0f;
    const float choicePieceScaleY = 3.0f;
    const float choicePieceSpacingY = 1.5f;

    const int gameSize = 10;
    const int pieceMoveDuration = 350;
    const int choiceScaleInDuration = 250;
    const int updateScoreDuration = 300;
    const int mapBorderThickness = 8;

    void initPieces();
    void initBlockPieces();
    void initLinePieces();
    void initLsPieces();
    void initSsPieces();
    void initCornerPieces();
    void initPyramidPieces();

    void initColors();
};
