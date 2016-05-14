#include "Errors.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <cstdlib>

extern void fatalError(std::string errorString)
{
    std::cout << errorString << std::endl;
    std::cout << "Enter any key to quit";
    int tmp;
    std::cin >> tmp;
    SDL_Quit();
    exit(42);
}
