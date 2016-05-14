#include "MainGame.h"
#include "Errors.h"
#include <iostream>
#include <string>
#include <GL/glew.h>
#include "ImageLoader.h"

MainGame::MainGame() : _window(nullptr), _screenWidth(800), _screenHeight(600), _gameState(GameState::PLAY), _time(0.f)
{

}
MainGame::~MainGame()
{
}
//Method that runs in main, starts off everything
void MainGame::run()
{
    initSystems();
    _sprites.init(-1.0f, -1.0f, 2.0f, 2.0f);
    _playerTexture = ImageLoader::loadPNG("../graphics/Textures/jimmy/PNG/CharacterRight_Standing.png");
    gameLoop();
}
//Create opengl context
void MainGame::initSystems()
{
    SDL_Init(SDL_INIT_EVERYTHING); //Initialize SDL
    _window = SDL_CreateWindow("Graphics Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWidth, _screenHeight, SDL_WINDOW_OPENGL);
    if(_window == nullptr){
        fatalError("SDL window could not be created");
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(_window);
    if (glContext == nullptr){
        fatalError("SDL_GL context could not be created");
    }

    GLenum err = glewInit();
    if(err != GLEW_OK){
        fatalError("Could not initialize GLEW");
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    initShaders();
}

void  MainGame::initShaders()
{
    _colorProgram.compileShaders("vertex.vs", "fragment.fs");
    _colorProgram.addAttribute("vertexPosition");
    _colorProgram.addAttribute("vertexColor");
    _colorProgram.linkShaders();
}

void MainGame::gameLoop()
{
    while(_gameState != GameState::EXIT){
        processInput();
        _time += 0.01;
        drawGame();
    }
}

void MainGame::processInput()
{
    SDL_Event e;
    while(SDL_PollEvent(&e)){
        switch(e.type) {
            case SDL_QUIT:
                _gameState = GameState::EXIT;
                break;
        }
        switch(e.type) {
            case SDL_MOUSEMOTION:
                std::cout << e.motion.x << " " << e.motion.y << std::endl;
                break;
        }
    }
}

void MainGame::drawGame()
{
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    _colorProgram.use();
    GLuint timeLocation = _colorProgram.getUniformLocation("time");
    glUniform1f(timeLocation, _time);

    _sprites.draw();

    _colorProgram.unuse();
    SDL_GL_SwapWindow(_window);
}
