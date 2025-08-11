#include "level.h"

// INIT
Level::Level(Game* g) : game(g) {
    initMapObject();
    load();
}

void Level::initMapObject() {
    static H2DE_Engine* engine = game->getEngine();

    H2DE_ObjectData od = H2DE_ObjectData();
    mapObject = engine->createObject<H2DE_BasicObject>(od);

    H2DE_SurfaceData sd = H2DE_SurfaceData();
    sd.transform.translate = { 4.5f, 4.5f };
    sd.transform.scale = { 10.0f, 10.0f };
    sd.index = H2DE_INDEX_MIN;

    H2DE_BorderData bd = H2DE_BorderData(); // temp
    bd.thickness = 10;
    bd.color = { 255, 127, 127, 255 };
    bd.type = H2DE_BORDER_TYPE_RECTANGLE;
    bd.placement = H2DE_BORDER_PLACEMENT_OUTER;

    mapObject->addSurface<H2DE_Border>("border", sd, bd);
}

void Level::initChoicesObjects() {
    static const Data* data = game->getData();

    for (int i = 0; i < choices.size(); i++) {
        choices[i] = createChoice(data->getRandomPiece(), i);
    }
}

// -- load
void Level::load() {
    if (!H2DE_Json::exists(saveFilePath)) {
        initChoicesObjects();
        return;
    }

    const json jsonSave = H2DE_Json::read(saveFilePath);

    if (!jsonSave.contains("current")) {
        initChoicesObjects();
        return;
    }

    const json& jsonCurrent = jsonSave.at("current");

    loadChoices(jsonCurrent);
    loadMap(jsonCurrent);
}

void Level::loadChoices(const json& jsonCurrent) {
    static const Data* data = game->getData();

    if (!jsonCurrent.contains("choices")) {
        initChoicesObjects();
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

        const int index = H2DE_Json::getInteger(jsonChoice.at("index"));
        const int id = H2DE_Json::getInteger(jsonChoice.at("id"));

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
        pieceCell.translate = H2DE_Json::getVector2D<float>(cell.at("translate"));
        pieceCell.color = H2DE_Json::getColorRGB(cell.at("color"), false);

        H2DE_SurfaceData sd = H2DE_SurfaceData();
        sd.transform.translate = pieceCell.translate;

        H2DE_TextureData td = H2DE_TextureData();
        td.textureName = "cell.png";
        td.color = pieceCell.color;

        mapObject->addSurface<H2DE_Texture>(Level::getStrPos(pieceCell.translate), sd, td);
        map.push_back(pieceCell);
    }
}

// CLEANUP
Level::~Level() {
    save();
    destroyMapObject();
    destroySelectedPiece();
    destroyChoices();
}

void Level::destroyMapObject() {
    static H2DE_Engine* engine = game->getEngine();
    engine->destroyObject(mapObject);

    map = {};
    mapObject = nullptr;
}

void Level::destroySelectedPiece() {
    static H2DE_Engine* engine = game->getEngine();

    if (selectedPiece != nullptr) {
        engine->destroyObject(selectedPiece);
    }

    selectedChoicePieceIndex = NO_SELECTED_PIECES;
    selectedPiece = nullptr;
}

void Level::destroyChoices() {
    static H2DE_Engine* engine = game->getEngine();

    for (const Choice& choice : choices) {
        engine->destroyObject(choice.object);
    }
}

void Level::save() {
    static const Data* data = game->getData();

    json save = {};

    if (H2DE_Json::exists(saveFilePath)) {
        save = H2DE_Json::read(saveFilePath);
    } else {
        H2DE_Json::create(saveFilePath, false, true);
    }

    if (save.contains("highscore")) {
        if (H2DE_Json::getInteger(save.at("highscore")) < score) {
            save["highscore"] = H2DE_Json::toJson(score);
        }

    } else {
        save["highscore"] = H2DE_Json::toJson(score);
    }

    save["current"] = {};
    save["current"]["score"] = H2DE_Json::toJson(score);

    save["current"]["choices"] = json::array();
    for (int i = 0; i < choices.size(); i++) {
        const Choice& choice = choices.at(i);

        json jsonChoice = {};

        jsonChoice["index"] = H2DE_Json::toJson(i);
        jsonChoice["id"] = H2DE_Json::toJson(data->getPieceID(choice.piece));

        save["current"]["choices"].push_back(jsonChoice);
    }

    save["current"]["map"] = json::array();
    for (const PieceCell& pieceCell : map) {
        json jsonPieceCell = {};

        jsonPieceCell["translate"] = H2DE_Json::toJson(pieceCell.translate);
        jsonPieceCell["color"] = H2DE_Json::toJson(pieceCell.color, false);

        save["current"]["map"].push_back(jsonPieceCell);
    }

    H2DE_Json::write(saveFilePath, save, true);
}

// EVENTS
void Level::handleEvents(SDL_Event event) {
    switch (event.type) {
        case SDL_MOUSEBUTTONDOWN: handleEvents_mousedown(event); break;
        case SDL_MOUSEBUTTONUP: handleEvents_mouseup(event); break;
        case SDL_MOUSEMOTION: handleEvents_mousemove(event); break;
        case SDL_KEYDOWN: handleEvents_keydown(event); break;
        default: return;
    }
}

void Level::handleEvents_mousedown(SDL_Event event) {
    if (event.button.button == SDL_BUTTON_RIGHT) {
        deselectChoice();
    }
}

void Level::handleEvents_mouseup(SDL_Event event) {
    if (event.button.button != SDL_BUTTON_LEFT) {
        return;
    }

    static H2DE_Engine* engine = game->getEngine();
    static const Data* data = game->getData();

    if (selectedChoicePieceIndex == NO_SELECTED_PIECES) {
        return;
    }

    const Piece& piece = choices.at(selectedChoicePieceIndex).piece;
    const H2DE_Scale pieceScale = { piece.structure.at(0).size() - 0.5f, piece.structure.size() - 0.5f };

    const H2DE_Translate mouseTranslate = engine->getMouseGamePos() - pieceScale * 0.5f;
    const H2DE_Translate snapedMouseTranslate = { (float)H2DE::round(mouseTranslate.x), (float)H2DE::round(mouseTranslate.y) };

    if (placePieceAt(snapedMouseTranslate, piece)) {
        engine->destroyObject(choices.at(selectedChoicePieceIndex).object);
        choices[selectedChoicePieceIndex] = createChoice(data->getRandomPiece(), selectedChoicePieceIndex);

        selectedChoicePieceIndex = NO_SELECTED_PIECES;
        engine->destroyObject(selectedPiece);
        selectedPiece = nullptr;

        checkLose();

    } else {
        deselectChoice();
    }
}

void Level::handleEvents_mousemove(SDL_Event event) {
    static H2DE_Engine* engine = game->getEngine();

    if (selectedChoicePieceIndex == NO_SELECTED_PIECES) {
        return;
    }

    selectedPiece->setTranslate(engine->getMouseGamePos());
}

void Level::handleEvents_keydown(SDL_Event event) {
    if (event.key.keysym.sym == SDLK_r) {
        destroyMapObject();
        destroySelectedPiece();
        destroyChoices();

        initMapObject();
        initChoicesObjects();
    }
}

// ACTIONS

// -- lines
void Level::checkFullLines() {
    std::unordered_map<float, int> countByRow;
    std::unordered_map<float, int> countByCol;

    for (const PieceCell& pieceCell : map) {
        countByRow[pieceCell.translate.y]++;
        countByCol[pieceCell.translate.x]++;
    }

    for (const auto& [y, count] : countByRow) {
        if (count == GAME_SIZE) {
            destroyRow(y);
        }
    }

    for (const auto& [x, count] : countByCol) {
        if (count == GAME_SIZE) {
            destroyColumn(x);
        }
    }
}

void Level::destroyRow(float y) {
    for (float x = 0; x < GAME_SIZE; x++) {
        destroyPieceCell({ x, y });
    }
}

void Level::destroyColumn(float x) {
    for (float y = 0; y < GAME_SIZE; y++) {
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

// choices
Choice Level::createChoice(const Piece& piece, int index) {
    static H2DE_Engine* engine = game->getEngine();
    static const Data* data = game->getData();
    static const int& choiceScaleInDuration = data->getChoiceScaleInDuration();

    static const H2DE_Scale& choicePieceScale = data->getChoicePieceScale();
    static const float& choicePieceTranslateX =  data->getChoicePieceTranslateX();
    static const float& choicePieceScaleY = data->getChoicePieceScaleY();
    static const float& choicePieceSpacingY = data->getChoicePieceSpacingY();

    H2DE_ObjectData od = H2DE_ObjectData();
    od.transform.translate = { choicePieceTranslateX, index * choicePieceScaleY + choicePieceSpacingY };
    od.transform.scale = { 0.0f, 0.0f };

    H2DE_ButtonObjectData bod = H2DE_ButtonObjectData();
    bod.onMouseDown = [this, index](H2DE_ButtonEventData data) {
        selectChoice(index);
    };

    H2DE_ButtonObject* object = engine->createObject<H2DE_ButtonObject>(od, bod);
    object->setScale(choicePieceScale, choiceScaleInDuration, H2DE_EASING_BACK_OUT);
    createPieceSurfaces(object, piece);

    H2DE_Hitbox hitbox = H2DE_Hitbox();
    hitbox.transform.scale = { 5.0f, 5.0f };
    hitbox.color = { 127, 127, 255, 255 };
    object->addHitbox("main", hitbox);

    return { piece, object };
}

void Level::selectChoice(int pieceIndex) {
    static H2DE_Engine* engine = game->getEngine();
    static const int& choiceScaleInDuration = game->getData()->getChoiceScaleInDuration();

    if (selectedChoicePieceIndex != NO_SELECTED_PIECES) {
        return;
    }

    selectedChoicePieceIndex = pieceIndex;

    Choice& selectedChoice = choices.at(pieceIndex);
    selectedChoice.object->setScale({ 0.0f, 0.0f });

    H2DE_ObjectData od = H2DE_ObjectData();
    od.transform.translate = engine->getMouseGamePos();
    od.transform.scale = { 0.0f, 0.0f };
    od.index = 50;

    selectedPiece = engine->createObject<H2DE_BasicObject>(od);
    selectedPiece->setScale({ 1.0f, 1.0f }, choiceScaleInDuration, H2DE_EASING_BACK_OUT);
    createPieceSurfaces(selectedPiece, selectedChoice.piece);
}

void Level::deselectChoice() {
    static H2DE_Engine* engine = game->getEngine();
    static const Data* data = game->getData();
    static const int& pieceMoveDuration = data->getPieceMoveDuration();
    const H2DE_Scale& choicePieceScale = data->getChoicePieceScale();

    H2DE_ButtonObject* button = choices.at(selectedChoicePieceIndex).object;

    selectedPiece->setScale(choicePieceScale, pieceMoveDuration, H2DE_EASING_BACK_OUT);

    selectedPiece->setTranslate(button->getTranslate(), pieceMoveDuration, H2DE_EASING_BACK_OUT, [this, button, choicePieceScale]() { 
        selectedChoicePieceIndex = NO_SELECTED_PIECES;
        engine->destroyObject(selectedPiece);
        selectedPiece = nullptr;

        button->setScale(choicePieceScale);
    }, true);
}

// -- lose
void Level::checkLose() {
    for (const Choice& choice : choices) {
        for (float y = 0.0f; y < GAME_SIZE; y++) {
            for (float x = 0.0f; x < GAME_SIZE; x++) {
                if (pieceCanBePlacedAt({ x, y }, choice.piece)) {
                    return;
                }
            }
        }
    }

    for (const Choice& choice : choices) {
        choice.object->disable();
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

            const H2DE_Translate cellTranslate = { x - pieceWidth * 0.5f + 0.5f, y - pieceHeight * 0.5f + 0.5f }; // temp

            H2DE_SurfaceData sd = H2DE_SurfaceData();
            sd.transform.translate = cellTranslate;

            H2DE_TextureData td = H2DE_TextureData();
            td.textureName = "cell.png";
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

bool Level::placePieceAt(const H2DE_Translate& topLeftCornerPos, const Piece& piece) {
    static const Data* data = game->getData();

    if (!pieceCanBePlacedAt(topLeftCornerPos, piece)) {
        return false;
    }

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

            H2DE_TextureData td = H2DE_TextureData();
            td.textureName = "cell.png";
            td.color = piece.color;

            mapObject->addSurface<H2DE_Texture>(Level::getStrPos(absTranslate), sd, td);

            PieceCell cell = PieceCell();
            cell.translate = absTranslate;
            cell.color = piece.color;

            map.push_back(cell);
        }
    }

    checkFullLines();
    return true;
}

// GETTER
bool Level::pieceCanBePlacedAt(const H2DE_Translate& topLeftCornerPos, const Piece& piece) const {
    float pieceMinX = topLeftCornerPos.x;
    float pieceMaxX = topLeftCornerPos.x + piece.structure.at(0).size();
    float pieceMinY = topLeftCornerPos.y;
    float pieceMaxY = topLeftCornerPos.y + piece.structure.size();

    bool overflowX = (pieceMinX < 0.0f || pieceMaxX > GAME_SIZE);
    bool overflowY = (pieceMinY < 0.0f || pieceMaxY > GAME_SIZE);

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
