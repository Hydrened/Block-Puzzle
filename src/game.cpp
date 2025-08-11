#include "game.h"

// INIT
Game::Game() {
    initEngine();
    initData();
    initLevel();
}

void Game::initEngine() {
    H2DE_WindowData window = H2DE_WindowData();
    window.fps = 60;
    window.size = { 1200, 800 };
    window.ratio = H2DE_WINDOW_RATIO_CUSTOM;
    window.resizable = true;
    window.saveState = true;
    window.title = "Block Puzzle";

    H2DE_CameraData camera = H2DE_CameraData();
    camera.translate = { GAME_SIZE * 0.5f - 0.5f, GAME_SIZE * 0.5f - 0.5f };
    camera.gameWidth = 18.0f;
    camera.interfaceWidth = 18.0f;
    camera.grid = false;

    engine = H2DE_CreateEngine({ window, camera });

    engine->setHandleEventCall([this](SDL_Event event) {
        handleEvents(event);
    });

    engine->loadAssets("assets");
}

void Game::initData() {
    data = new Data(this);
}

void Game::initLevel() {
    level = new Level(this);
}

// CLEANUP
Game::~Game() {
    delete level;
    delete data;
    H2DE_DestroyEngine(engine);
}

// RUN
void Game::run() {
    engine->run();
}

// EVENTS
void Game::handleEvents(SDL_Event event) {
    level->handleEvents(event);
}
