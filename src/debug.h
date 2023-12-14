#pragma once
#include <GL/glew.h>
#include <iostream>

namespace Debug
{
//Add file name and line number parameters
inline void glErrorCheck(const char* fileName, int lineNum) { //this function will be very useful
    GLenum errorNumber = glGetError();
    while (errorNumber != GL_NO_ERROR) {
        // Task 2: Edit this print statement to be more descriptive
        std::cout << "File Name "<< fileName << " Line Number "<< lineNum<< " Error: " << gluErrorString(errorNumber) << std::endl;
        errorNumber = glGetError();

    }
}
}

//Add a preprocessor directive to automate the writing of this function
#define glErrorCheck() glErrorCheck(__FILE__, __LINE__)
