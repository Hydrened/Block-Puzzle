#include "level.h"

// INIT
Level::Level(Game* g) : game(g) {
    load();
    initObjects();

    updateHighscore();
    updateScore();
}

// CLEANUP
Level::~Level() {
    save();
    destroyObjects();
}

void Level::save() {
    static const Data* data = game->getData();

    json save = {};

    if (H2DE::Json::exists(saveFilePath)) {
        save = H2DE::Json::read(saveFilePath);
    } else {
        H2DE::Json::create(saveFilePath, false, true);
    }

    if (save.contains("highscore")) {
        if (H2DE::Json::getInteger(save.at("highscore")) < highscore) {
            save["highscore"] = H2DE::Json::toJson(highscore);
        }

    } else {
        save["highscore"] = H2DE::Json::toJson(highscore);
    }

    save["current"] = {};
    save["current"]["score"] = H2DE::Json::toJson(score);

    save["current"]["choices"] = json::array();
    for (int i = 0; i < choices.size(); i++) {
        const Choice& choice = choices.at(i);

        json jsonChoice = {};

        jsonChoice["index"] = H2DE::Json::toJson(i);
        jsonChoice["id"] = H2DE::Json::toJson(data->getPieceID(choice.piece));

        save["current"]["choices"].push_back(jsonChoice);
    }

    save["current"]["map"] = json::array();
    for (const PieceCell& pieceCell : map) {
        json jsonPieceCell = {};

        jsonPieceCell["translate"] = H2DE::Json::toJson(pieceCell.translate);
        jsonPieceCell["color"] = H2DE::Json::toJson(pieceCell.color, false);

        save["current"]["map"].push_back(jsonPieceCell);
    }

    H2DE::Json::write(saveFilePath, save, true);
}

// UPDATE
void Level::updateObjectText(H2DE_TextObject* object, const std::string& text) {
    static const Data* data = game->getData();
    static const int& updateScoreDuration = data->getUpdateScoreDuration();
    static const H2DE_Scale& updateScoreScale = data->getUpdateScoreScale();

    if (object == nullptr) {
        return;
    }

    object->setText(text);

    object->setScale(updateScoreScale);
    object->setScale({ 1.0f, 1.0f }, updateScoreDuration, H2DE_EASING_BACK_OUT);
}

void Level::updateHighscore() {
    updateObjectText(highscoreObject, "Highscore\n" + std::to_string(highscore));
}

void Level::updateScore() {
    updateObjectText(scoreObject, "Score\n" + std::to_string(score));
}

// ACTIONS
void Level::restart() {
    destroyObjects();

    map.clear();
    choices = {};
    
    initObjects();
    initChoicesObjects();

    score = 0;

    updateHighscore();
    updateScore();
}

void Level::gameOver() {
    if (score > highscore) {
        highscore = score;
        updateHighscore();
    }
}

void Level::checkPossibilities() {
    static const int& gameSize = game->getData()->getGameSize(); 

    int disabledChoices = 0;
    bool choiceCanBePlaced = false;

    for (const Choice& choice : choices) {

        for (float y = 0.0f; y < gameSize; y++) {
            for (float x = 0.0f; x < gameSize; x++) {
                if (pieceCanBePlacedAt({ x, y }, choice.piece)) {
                    choiceCanBePlaced = true;
                    break;
                }
            }

            if (choiceCanBePlaced) {
                break;
            }
        }

        if (!choiceCanBePlaced) {
            disableChoice(choice);
            disabledChoices++;

        } else {
            enableChoice(choice);
        }

        choiceCanBePlaced = false;
    }

    if (disabledChoices == choices.size()) {
        gameOver();
    }
}

// GETTER
H2DE_Translate Level::getSnapedMouseTranslate() const {
    static H2DE_Engine* engine = game->getEngine();

    const Piece& piece = choices.at(selectedChoicePieceIndex).piece;
    const H2DE_Scale pieceScale = { static_cast<float>(piece.structure.at(0).size()), static_cast<float>(piece.structure.size()) };

    const H2DE_Translate mouseTranslate = engine->getMouseGamePos() - pieceScale * 0.5f + H2DE_Translate{ 0.5f, 0.5f };
    const H2DE_Translate snapedMouseTranslate = { static_cast<float>(H2DE::round(mouseTranslate.x)), static_cast<float>(H2DE::round(mouseTranslate.y)) };

    return snapedMouseTranslate;
}
