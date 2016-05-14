#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include "GLtexture.h"
#include <string>

class ImageLoader
{
public:
    static GLTexture loadPNG(std::string filePath);
};
#endif
