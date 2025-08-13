#include "level.h"

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

    const H2DE_Translate snapedMouseTranslate = getSnapedMouseTranslate();
    const Piece& piece = choices.at(selectedChoicePieceIndex).piece;

    if (placePieceAt(snapedMouseTranslate, piece)) {
        engine->destroyObject(choices.at(selectedChoicePieceIndex).object);
        choices[selectedChoicePieceIndex] = createChoice(data->getRandomPiece(), selectedChoicePieceIndex);

        selectedChoicePieceIndex = NO_SELECTED_PIECES;
        engine->destroyObject(selectedPiece);
        selectedPiece = nullptr;

        checkPossibilities();

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

    const H2DE_Translate snapedMouseTranslate = getSnapedMouseTranslate();
    const Piece& piece = choices.at(selectedChoicePieceIndex).piece;

    if (!pieceCanBePlacedAt(snapedMouseTranslate, piece)) {
        removePreview();
        return;
    }

    previewPieceAt(snapedMouseTranslate, piece);
}

void Level::handleEvents_keydown(SDL_Event event) {
    switch (event.key.keysym.sym) {
        case SDLK_r: restart(); break;
        case SDLK_ESCAPE: deselectChoice(); break;
        default: break;
    }
}
