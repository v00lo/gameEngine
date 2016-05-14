#ifndef SPRITES_H
#define SPRITES_H

#include <GL/glew.h>

class Sprites
{
public:
    Sprites();
    ~Sprites();

    void init(float x, float y, float width, float height);
    void draw();

private:
    float _x;
    float _y;
    float _width;
    float _height;
    GLuint _vboID;
};

#endif
