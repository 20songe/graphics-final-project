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
        std::vector < glm::vec3 > & out_vertices,
        std::vector < glm::vec2 > & out_uvs,
        std::vector < glm::vec3 > & out_normals
        );
};

#endif // OBJLOADER_H
