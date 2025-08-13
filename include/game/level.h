#pragma once

#include "game.h"
class Game;

class Level {
public: 
    Level(Game* game);
    ~Level();

    void handleEvents(SDL_Event event);

private:
    Game* game;

    const std::string saveFilePath = "save.dat";

    std::vector<PieceCell> map = {};
    H2DE_BasicObject* mapObject = nullptr;

    std::array<Choice, NB_CHOICES> choices = {};
    H2DE_BasicObject* selectedPiece = nullptr;
    int selectedChoicePieceIndex = NO_SELECTED_PIECES;
    H2DE_Translate currentPreview = { -1.0f, -1.0f };

    H2DE_TextObject* highscoreObject = nullptr;
    H2DE_TextObject* scoreObject = nullptr;

    int score = 0;
    int highscore = 0;

    void load();
    void loadChoices(const json& jsonCurrent);
    void loadMap(const json& jsonCurrent);
    void loadScore(const json& jsonCurrent);

    void initObjects();
    void initMapObject();
    void initMapSurfaces();
    void initChoicesObjects();
    void initHighscoreObject();
    void initScoreObject();

    void save();
    void destroyObjects();
    void destroyMapObject();
    void destroyChoices();
    void destroySelectedPiece();
    void destroyUI();

    void handleEvents_mousedown(SDL_Event event);
    void handleEvents_mouseup(SDL_Event event);
    void handleEvents_mousemove(SDL_Event event);
    void handleEvents_keydown(SDL_Event event);

    void updateObjectText(H2DE_TextObject* object, const std::string& text);
    void updateHighscore();
    void updateScore();

    void restart();
    void gameOver();
    void checkPossibilities();

    void checkFullLines();
    void destroyRow(float y);
    void destroyColumn(float x);
    void destroyPieceCell(const H2DE_Translate& translate);

    bool placePieceAt(const H2DE_Translate& topLeftCornerPos, const Piece& piece);
    void previewPieceAt(const H2DE_Translate& topLeftCornerPos, const Piece& piece);
    void removePreview();
    static void createPieceSurfaces(H2DE_Object* object, const Piece& piece);

    Choice createChoice(const Piece& piece, int index);    
    void selectChoice(int pieceIndex);
    void deselectChoice();
    void enableChoice(const Choice& choice);
    void disableChoice(const Choice& choice);

    bool pieceCanBePlacedAt(const H2DE_Translate& topLeftCornerPos, const Piece& piece) const;
    std::optional<PieceCell> getPieceCell(const H2DE_Translate& translate) const;
    H2DE_Translate getSnapedMouseTranslate() const;
    static inline std::string getStrPos(const H2DE_Translate& pos) {
        return std::to_string(static_cast<float>(pos.x)) + '-' + std::to_string(static_cast<float>(pos.y));
    }
};
