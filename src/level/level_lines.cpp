#include "level.h"

void Level::checkFullLines() {
    static const int& gameSize = game->getData()->getGameSize(); 

    std::unordered_map<float, int> countByRow;
    std::unordered_map<float, int> countByCol;
    int numberOfFullLines = 0;

    for (const PieceCell& pieceCell : map) {
        countByRow[pieceCell.translate.y]++;
        countByCol[pieceCell.translate.x]++;
    }

    for (const auto& [y, count] : countByRow) {
        if (count == gameSize) {
            destroyRow(y);
            numberOfFullLines++;
        }
    }

    for (const auto& [x, count] : countByCol) {
        if (count == gameSize) {
            destroyColumn(x);
            numberOfFullLines++;
        }
    }

    score += H2DE::pow(numberOfFullLines, 2) * 100;
}

void Level::destroyRow(float y) {
    static const int& gameSize = game->getData()->getGameSize(); 

    for (float x = 0; x < gameSize; x++) {
        destroyPieceCell({ x, y });
    }
}

void Level::destroyColumn(float x) {
    static const int& gameSize = game->getData()->getGameSize(); 

    for (float y = 0; y < gameSize; y++) {
        destroyPieceCell({ x, y });
    }
}

void Level::destroyPieceCell(const H2DE_Translate& translate) {
    mapObject->removeSurface(Level::getStrPos(translate));

    const std::optional<const PieceCell>& possiblePieceCell = getPieceCell(translate);
    if (!possiblePieceCell.has_value()) {
        return;
    }

    auto it = std::find(map.begin(), map.end(), possiblePieceCell.value());
    if (it == map.end()) {
        return;
    }

    map.erase(it);
}
