#include "level.h"

void Level::load() {
    if (!H2DE::Json::exists(saveFilePath)) {
        return;
    }

    const json jsonSave = H2DE::Json::read(saveFilePath);
    highscore = H2DE::Json::getInteger(jsonSave.at("highscore"));

    if (!jsonSave.contains("current")) {
        return;
    }

    const json& jsonCurrent = jsonSave.at("current");

    loadChoices(jsonCurrent);
    loadMap(jsonCurrent);
    loadScore(jsonCurrent);

    checkPossibilities();
}

void Level::loadChoices(const json& jsonCurrent) {
    static const Data* data = game->getData();

    if (!jsonCurrent.contains("choices")) {
        return;
    }

    const json& jsonChoices = jsonCurrent.at("choices");

    for (const json& jsonChoice : jsonChoices) {
        if (!jsonChoice.contains("id")) {
            continue;
        }

        if (!jsonChoice.contains("index")) {
            continue;
        }

        const int index = H2DE::Json::getInteger(jsonChoice.at("index"));
        const int id = H2DE::Json::getInteger(jsonChoice.at("id"));

        choices[index] = createChoice(data->getPiece(id), index);
    }
}

void Level::loadMap(const json& jsonCurrent) {
    if (!jsonCurrent.contains("map")) {
        return;
    }

    for (const json& cell : jsonCurrent.at("map")) {
        if (!cell.contains("translate")) {
            continue;
        }

        if (!cell.contains("color")) {
            continue;
        }

        PieceCell pieceCell = PieceCell();
        pieceCell.translate = H2DE::Json::getVector2D<float>(cell.at("translate"));
        pieceCell.color = H2DE::Json::getColorRGB(cell.at("color"), false);

        map.push_back(pieceCell);
    }
}

void Level::loadScore(const json& jsonCurrent) {
    if (!jsonCurrent.contains("score")) {
        return;
    }

    score = H2DE::Json::getInteger(jsonCurrent.at("score"));
}
