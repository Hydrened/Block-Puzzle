#include "level.h"

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
    hitbox.transform.scale = { 5.0f, 5.0f }; // temp
    hitbox.color = { 127, 127, 255, 255 };
    object->addHitbox("main", hitbox);

    return { piece, object };
}

void Level::selectChoice(int pieceIndex) {
    static H2DE_Engine* engine = game->getEngine();
    static H2DE_Window* window = engine->getWindow();
    static const int& choiceScaleInDuration = game->getData()->getChoiceScaleInDuration();

    if (selectedChoicePieceIndex != NO_SELECTED_PIECES || selectedPiece != nullptr) {
        return;
    }

    selectedChoicePieceIndex = pieceIndex;

    Choice& selectedChoice = choices.at(pieceIndex);
    selectedChoice.object->setScale({ 0.0f, 0.0f });

    H2DE_ObjectData od = H2DE_ObjectData();
    od.transform.translate = engine->getMouseGamePos();
    od.transform.scale = { 0.0f, 0.0f };
    od.index = 1;

    selectedPiece = engine->createObject<H2DE_BasicObject>(od);
    selectedPiece->setScale({ 1.0f, 1.0f }, choiceScaleInDuration, H2DE_EASING_BACK_OUT);
    createPieceSurfaces(selectedPiece, selectedChoice.piece);
}

void Level::deselectChoice() {
    static H2DE_Engine* engine = game->getEngine();
    static const Data* data = game->getData();
    static const int& pieceMoveDuration = data->getPieceMoveDuration();
    const H2DE_Scale& choicePieceScale = data->getChoicePieceScale();

    if (selectedChoicePieceIndex == NO_SELECTED_PIECES) {
        return;
    }

    H2DE_ButtonObject* button = choices.at(selectedChoicePieceIndex).object;
    selectedChoicePieceIndex = NO_SELECTED_PIECES;
    removePreview();

    selectedPiece->setScale(choicePieceScale, pieceMoveDuration, H2DE_EASING_BACK_OUT);

    selectedPiece->setTranslate(button->getTranslate(), pieceMoveDuration, H2DE_EASING_BACK_OUT, [this, button, choicePieceScale]() { 
        engine->destroyObject(selectedPiece);
        selectedPiece = nullptr;

        button->setScale(choicePieceScale);
    }, true);
}

void Level::enableChoice(const Choice& choice) {
    choice.object->enable();
    choice.object->setOpacity(255);
}

void Level::disableChoice(const Choice& choice) {
    choice.object->disable();
    choice.object->setOpacity(100);
}
