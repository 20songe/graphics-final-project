#ifndef OBJLOADER_H
#define OBJLOADER_H

#pragma once

#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

class objloader
{
public:
    objloader();

    int fileCount = 0;

    bool parse(const char *path, std::vector<float> &m_vertexData);

};

#endif // OBJLOADER_H
