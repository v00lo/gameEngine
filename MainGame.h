#ifndef MAINGAME_H
#define MAINGAME_H

#include <SDL2/SDL.h>
#include "Sprites.h"
#include "GLSLProgram.h"
#include "GLtexture.h"

enum class GameState{PLAY, EXIT};

class MainGame
{
public:
    MainGame();
    ~MainGame();

    void run();


private:
    void initSystems();
    void initShaders();
    void processInput();
    void gameLoop();
    SDL_Window* _window;
    int _screenWidth;
    int _screenHeight;
    void drawGame();

    GameState _gameState;
    Sprites _sprites;
    GLSLProgram _colorProgram;
    GLTexture _playerTexture;

    float _time;
};
#endif
