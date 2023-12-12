#ifndef SHAPES_H
#define SHAPES_H

// --- student code ---

// TODO: get a shape to render

#include <vector>
#include <glm/glm.hpp>

class shapes {

public:
    std::vector<float> makeCube();
    std::vector<float> makeSphere();
    std::vector<float> makeCylinder();
    std::vector<float> makeCone();

private:
    void makeTile(glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 bottomLeft, glm::vec3 bottomRight);
    void makeSphereTile(glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 bottomLeft, glm::vec3 bottomRight);
    void makeSquareFace(glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 bottomLeft, glm::vec3 bottomRight);
    void makeWedge(float currentTheta, float nextTheta);
    void makeTriangleTile(glm::vec3 origin, glm::vec3 first, glm::vec3 second);
    void makeVerticalTile(glm::vec3 normal0, glm::vec3 normal1, glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 bottomLeft, glm::vec3 bottomRight);
    void makeConeEdge(std::vector<glm::vec3> normals, std::vector<glm::vec3> vertices);

    std::vector<float> m_vertexData;

};

// --- student code end ---

#endif // SHAPES_H
