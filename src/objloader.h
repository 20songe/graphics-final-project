#ifndef OBJLOADER_H
#define OBJLOADER_H


#include "glm/fwd.hpp"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
class objloader
{
public:
    objloader();
    bool loadOBJ(
        const char * path,
        std::vector<float> &data,
        std::vector<std::string> &line
        );
};

#endif // OBJLOADER_H
