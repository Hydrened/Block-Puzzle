#ifndef LEVEL_H
#define LEVEL_H

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

    std::array<Choice, 3> choices = {};
    H2DE_BasicObject* selectedPiece = nullptr;
    int selectedChoicePieceIndex = NO_SELECTED_PIECES;

    int score = 0;

    void initMapObject();
    void initChoicesObjects();
    void load();
    void loadChoices(const json& jsonCurrent);
    void loadMap(const json& jsonCurrent);

    void destroyMapObject();
    void destroySelectedPiece();
    void destroyChoices();
    void save();

    void handleEvents_mousedown(SDL_Event event);
    void handleEvents_mouseup(SDL_Event event);
    void handleEvents_mousemove(SDL_Event event);
    void handleEvents_keydown(SDL_Event event);

    void checkFullLines();
    void destroyRow(float y);
    void destroyColumn(float x);
    void destroyPieceCell(const H2DE_Translate& translate);

    Choice createChoice(const Piece& piece, int index);    
    void selectChoice(int pieceIndex);
    void deselectChoice();

    void checkLose();








    bool placePieceAt(const H2DE_Translate& topLeftCornerPos, const Piece& piece);


    static void createPieceSurfaces(H2DE_Object* object, const Piece& piece);

    bool pieceCanBePlacedAt(const H2DE_Translate& topLeftCornerPos, const Piece& piece) const;
    std::optional<PieceCell> getPieceCell(const H2DE_Translate& translate) const;
    static inline std::string getStrPos(const H2DE_Translate& pos) {
        return std::to_string(static_cast<float>(pos.x)) + '-' + std::to_string(static_cast<float>(pos.y));
    }
};

#endif
