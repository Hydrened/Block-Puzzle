#pragma once

#include <H2DE/H2DE.h>
#include "utils.h"
#include "data.h"
#include "level.h"
class Level;

class Game {
public:
    Game();
    ~Game();

    void run();

    inline H2DE_Engine* getEngine() const { return engine; }
    inline const Data* getData() const { return data; }

private:
    H2DE_Engine* engine = nullptr;
    Data* data = nullptr;
    Level* level = nullptr;

    void initEngine();
    void initData();
    void initLevel();
    void loadAssets();

    void handleEvents(SDL_Event event);
};
