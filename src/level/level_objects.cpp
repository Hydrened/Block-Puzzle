#include "level.h"

// INIT
void Level::initObjects() {
    initMapObject();
    initHighscoreObject();
    initChoicesObjects();
    initScoreObject();
}

void Level::initMapObject() {
    static H2DE_Engine* engine = game->getEngine();
    static const Data* data = game->getData();

    static const int& gameSize = data->getGameSize(); 
    static const H2DE_ColorRGB& mapBorderColor = data->getMapBorderColor(); 
    static const int& mapBorderThickness = data->getMapBorderThickness(); 

    mapObject = engine->createObject<H2DE_BasicObject>(H2DE_ObjectData());

    H2DE_SurfaceData sd = H2DE_SurfaceData();
    sd.transform.translate = { gameSize * 0.5f - 0.5f, gameSize * 0.5f - 0.5f };
    sd.transform.scale = { static_cast<float>(gameSize), static_cast<float>(gameSize) };
    sd.index = -2;

    H2DE_BorderData bd = H2DE_BorderData();
    bd.thickness = mapBorderThickness;
    bd.color = mapBorderColor;
    bd.type = H2DE_BORDER_TYPE_RECTANGLE;
    bd.placement = H2DE_BORDER_PLACEMENT_OUTER;

    mapObject->addSurface<H2DE_Border>("border", sd, bd);

    initMapSurfaces();
}

void Level::initMapSurfaces() {
    static const Data* data = game->getData();

    static const int& gameSize = data->getGameSize(); 
    static const H2DE_ColorRGB& emptyCellColor = data->getEmptyCellColor(); 

    for (int y = 0; y < gameSize; y++) {
        for (int x = 0; x < gameSize; x++) {
            const H2DE_Translate cellTranslate = { static_cast<float>(x), static_cast<float>(y) };

            H2DE_SurfaceData sd = H2DE_SurfaceData();
            sd.transform.translate = cellTranslate;
            sd.index = -1;

            H2DE_TextureData td = H2DE_TextureData();
            td.textureName = "empty-cell.png";
            td.color = emptyCellColor;

            mapObject->addSurface<H2DE_Texture>("empty-" + Level::getStrPos(cellTranslate), sd, td);
        }
    }

    for (const PieceCell& pieceCell : map) {
        H2DE_SurfaceData sd = H2DE_SurfaceData();
        sd.transform.translate = pieceCell.translate;

        H2DE_TextureData td = H2DE_TextureData();
        td.textureName = "full-cell.png";
        td.color = pieceCell.color;

        mapObject->addSurface<H2DE_Texture>(Level::getStrPos(pieceCell.translate), sd, td);
    }
}

void Level::initChoicesObjects() {
    static const Data* data = game->getData();

    for (int i = 0; i < choices.size(); i++) {
        if (choices.at(i).piece.color != H2DE_ColorRGB()) {
            continue;
        }

        choices[i] = createChoice(data->getRandomPiece(), i);
    }
}

void Level::initHighscoreObject() {
    static H2DE_Engine* engine = game->getEngine();
    static const Data* data = game->getData();

    static const H2DE_Translate& highscoreTranslate = data->getHighscoreTranslate();
    static const H2DE_Scale& scoreFontScale = data->getScoreFontScale();
    static const H2DE_Scale& scoreFontSpacing = data->getScoreFontSpacing();

    H2DE_ObjectData od = H2DE_ObjectData();
    od.transform.translate = highscoreTranslate;
    od.absolute = true;

    H2DE_TextObjectData tod = H2DE_TextObjectData();
    tod.text.font = "font";
    tod.text.fontSize = scoreFontScale;
    tod.text.spacing = scoreFontSpacing;

    highscoreObject = engine->createObject<H2DE_TextObject>(od, tod);
}

void Level::initScoreObject() {
    static H2DE_Engine* engine = game->getEngine();
    static const Data* data = game->getData();

    static const H2DE_Translate& scoreTranslate = data->getScoreTranslate();

    H2DE_ObjectData od = H2DE_ObjectData();
    od.transform.translate = scoreTranslate;
    od.absolute = true;

    H2DE_TextObjectData tod = H2DE_TextObjectData();
    tod.text.font = "font";
    tod.text.fontSize = highscoreObject->getFontSize();
    tod.text.spacing = highscoreObject->getSpacing();

    scoreObject = engine->createObject<H2DE_TextObject>(od, tod);
}

// CLEANUP
void Level::destroyObjects() {
    destroyMapObject();
    destroyChoices();
    destroyUI();
    destroySelectedPiece();
}

void Level::destroyMapObject() {
    static H2DE_Engine* engine = game->getEngine();
    engine->destroyObject(mapObject);

    map = {};
    mapObject = nullptr;
}

void Level::destroyChoices() {
    static H2DE_Engine* engine = game->getEngine();

    for (const Choice& choice : choices) {
        engine->destroyObject(choice.object);
    }
}

void Level::destroyUI() {
    static H2DE_Engine* engine = game->getEngine();

    engine->destroyObject(highscoreObject);
    engine->destroyObject(scoreObject);
}

void Level::destroySelectedPiece() {
    static H2DE_Engine* engine = game->getEngine();

    if (selectedPiece != nullptr) {
        engine->destroyObject(selectedPiece);
    }

    selectedChoicePieceIndex = NO_SELECTED_PIECES;
    selectedPiece = nullptr;
}
