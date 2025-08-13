#include "game.h"

// INIT
Game::Game() {
    initEngine();
    initData();
    initLevel();
}

void Game::initEngine() {
    H2DE_WindowData window = H2DE_WindowData();
    window.fps = 144;
    window.size = { 1280, 720 }; // { 1200, 800 };
    window.ratio = H2DE_WINDOW_RATIO_16_9; // H2DE_WINDOW_RATIO_CUSTOM
    window.resizable = true;
    window.saveState = false; // true
    window.fullscreen = true; // false
    window.title = "Block Puzzle";

    H2DE_CameraData camera = H2DE_CameraData();
    camera.translate = { 10 * 0.5f - 0.5f, 10 * 0.5f - 0.5f };
    camera.gameWidth = 20.0f; // 18
    camera.interfaceWidth = 20.0f;
    camera.grid = false;

    engine = H2DE::createEngine({ window, camera });

    engine->setHandleEventCall([this](SDL_Event event) {
        handleEvents(event);
    });

    loadAssets();
}

void Game::initData() {
    data = new Data(this);
}

void Game::initLevel() {
    level = new Level(this);
}

void Game::loadAssets() {
    engine->loadAssetsSync("assets", true);

    H2DE_Font font = H2DE_Font();
    font.textureName = "font.png";
    font.charOrder = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890.:,;'\"(!?)+-*/=%";
    font.charSize = { 5, 9 };
    font.spacing = 0;
    font.scaleMode = H2DE_SCALE_MODE_NEAREST;
    engine->loadFont("font", font, true);
}

// CLEANUP
Game::~Game() {
    delete level;
    delete data;
    H2DE::destroyEngine(engine);
}

// RUN
void Game::run() {
    engine->run();
}

// EVENTS
void Game::handleEvents(SDL_Event event) {
    level->handleEvents(event);
}
