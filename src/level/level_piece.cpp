#include "level.h"

bool Level::placePieceAt(const H2DE_Translate& topLeftCornerPos, const Piece& piece) {
    static const Data* data = game->getData();

    if (!pieceCanBePlacedAt(topLeftCornerPos, piece)) {
        return false;
    }

    removePreview();

    for (int y = 0; y < piece.structure.size(); y++) {
        const std::vector<bool>& line = piece.structure[y];

        for (int x = 0; x < line.size(); x++) {
            const bool& pieceCell = line[x];

            if (!pieceCell) {
                continue;
            }

            score++;

            const H2DE_Translate absTranslate = { x + topLeftCornerPos.x, y + topLeftCornerPos.y };

            H2DE_SurfaceData sd = H2DE_SurfaceData();
            sd.transform.translate = { static_cast<float>(absTranslate.x), static_cast<float>(absTranslate.y) };

            H2DE_TextureData td = H2DE_TextureData();
            td.textureName = "full-cell.png";
            td.color = piece.color;

            mapObject->addSurface<H2DE_Texture>(Level::getStrPos(absTranslate), sd, td);

            PieceCell cell = PieceCell();
            cell.translate = absTranslate;
            cell.color = piece.color;

            map.push_back(cell);
        }
    }

    checkFullLines();
    updateScore();
    return true;
}

void Level::previewPieceAt(const H2DE_Translate& topLeftCornerPos, const Piece& piece) {
    if (topLeftCornerPos == currentPreview) {
        return;
    }

    currentPreview = topLeftCornerPos;

    for (const auto& [name, surface] : mapObject->getSurfaces()) {
        if (name.find("preview-") == std::string::npos) {
            continue;
        }

        mapObject->removeSurface(name);
    }

    H2DE_ColorRGB darkPieceColor = piece.color;
    darkPieceColor.a = 127;

    for (int y = 0; y < piece.structure.size(); y++) {
        const std::vector<bool>& line = piece.structure[y];

        for (int x = 0; x < line.size(); x++) {
            const bool& pieceCell = line[x];

            if (!pieceCell) {
                continue;
            }

            const H2DE_Translate absTranslate = { x + topLeftCornerPos.x, y + topLeftCornerPos.y };

            H2DE_SurfaceData sd = H2DE_SurfaceData();
            sd.transform.translate = { static_cast<float>(absTranslate.x), static_cast<float>(absTranslate.y) };
            sd.index = 0;

            H2DE_TextureData td = H2DE_TextureData();
            td.textureName = "full-cell.png";
            td.color = darkPieceColor;

            mapObject->addSurface<H2DE_Texture>("preview-" + Level::getStrPos(absTranslate), sd, td);
        }
    }
}

void Level::removePreview() {
    constexpr H2DE_Translate defaultChoiceTranslate = { -1.0f, -1.0f };

    if (currentPreview != defaultChoiceTranslate) {
        previewPieceAt(defaultChoiceTranslate, Piece());
        currentPreview = defaultChoiceTranslate;
    }
}

void Level::createPieceSurfaces(H2DE_Object* object, const Piece& piece) {
    H2DE_BasicObject* basicObject = dynamic_cast<H2DE_BasicObject*>(object);
    H2DE_ButtonObject* buttonObject = dynamic_cast<H2DE_ButtonObject*>(object);

    if (basicObject == nullptr && buttonObject == nullptr) {
        return;
    }

    const float& pieceHeight = piece.structure.size();
    
    for (int y = 0; y < pieceHeight; y++) {
        const std::vector<bool>& row = piece.structure.at(y);
        const float& pieceWidth = row.size();

        for (int x = 0; x < pieceWidth; x++) {
            const bool& cell = row.at(x);

            if (!cell) {
                continue;
            }

            const H2DE_Translate cellTranslate = { x - pieceWidth * 0.5f + 0.5f, y - pieceHeight * 0.5f + 0.5f };

            H2DE_SurfaceData sd = H2DE_SurfaceData();
            sd.transform.translate = cellTranslate;

            H2DE_TextureData td = H2DE_TextureData();
            td.textureName = "full-cell.png";
            td.color = piece.color;

            if (basicObject != nullptr) {
                basicObject->addSurface<H2DE_Texture>(Level::getStrPos(cellTranslate), sd, td);

            } else if (buttonObject != nullptr) {

                buttonObject->addSurface<H2DE_Texture>(Level::getStrPos(cellTranslate), sd, td);
            } else {
                return;
            }
        }
    }
}

// GETTER
bool Level::pieceCanBePlacedAt(const H2DE_Translate& topLeftCornerPos, const Piece& piece) const {
    static const int& gameSize = game->getData()->getGameSize(); 

    float pieceMinX = topLeftCornerPos.x;
    float pieceMaxX = topLeftCornerPos.x + piece.structure.at(0).size();
    float pieceMinY = topLeftCornerPos.y;
    float pieceMaxY = topLeftCornerPos.y + piece.structure.size();

    bool overflowX = (pieceMinX < 0.0f || pieceMaxX > gameSize);
    bool overflowY = (pieceMinY < 0.0f || pieceMaxY > gameSize);

    if (overflowX || overflowY) {
        return false;
    }

    for (float y = 0; y < piece.structure.size(); y++) {
        const std::vector<bool>& line = piece.structure.at(y);

        for (float x = 0; x < line.size(); x++) {
            const bool& pieceCell = line.at(x);

            if (!pieceCell) {
                continue;
            }

            const H2DE_Translate absTranslate = topLeftCornerPos + H2DE_Translate{ x, y };

            bool mapCell = getPieceCell(absTranslate).has_value();

            if (mapCell) {
                return false;
            }
        }
    }

    return true;
}

std::optional<PieceCell> Level::getPieceCell(const H2DE_Translate& translate) const {
    for (const PieceCell& pieceCell : map) {
        if (pieceCell.translate == translate) {
            return pieceCell;
        }
    }

    return std::nullopt;
}
