
// --- student code ---

// TODO: get a shape to render

#include "shapes.h"
#include "settings.h"

void insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}

void shapes::makeTile(glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 bottomLeft, glm::vec3 bottomRight) {

    // point 1
    glm::vec3 v0 = (bottomLeft - topLeft);
    glm::vec3 v1 = (bottomRight - topLeft);
    glm::vec3 n = glm::normalize(glm::cross(v0, v1));

    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, n);

    // point 2
    v0 = (bottomRight - bottomLeft);
    v1 = (topLeft - bottomLeft);
    n = glm::normalize(glm::cross(v0, v1));

    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, n);

    // point 3
    v0 = (topLeft - bottomRight);
    v1 = (bottomLeft - bottomRight);
    n = glm::normalize(glm::cross(v0, v1));

    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, n);

    // point 4
    v0 = (bottomRight - topLeft);
    v1 = (topRight - topLeft);
    n = glm::normalize(glm::cross(v0, v1));

    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, n);

    // point 5
    v0 = (topRight - bottomRight);
    v1 = (topLeft - bottomRight);
    n = glm::normalize(glm::cross(v0, v1));

    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, n);

    // point 6
    v0 = (topLeft - topRight);
    v1 = (bottomRight - topRight);
    n = glm::normalize(glm::cross(v0, v1));

    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, n);

}

void shapes::makeSquareFace(glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 bottomLeft, glm::vec3 bottomRight) {

    // values
    glm::vec3 iterX = (topRight - topLeft) / (float) settings.shapeParameter1;
    glm::vec3 iterY = (bottomLeft - topLeft) / (float) settings.shapeParameter1;

    // iteration
    for (int i = 0; i < settings.shapeParameter1; i++) {
        for (int j = 0; j < settings.shapeParameter1; j++) {

            glm::vec3 topLeft0     = topLeft + (float) i * iterX + (float) j * iterY;
            glm::vec3 topRight0    = topLeft + (float) (i + 1) * iterX + (float) j * iterY;
            glm::vec3 bottomLeft0  = topLeft + (float) i * iterX + (float) (j + 1) * iterY;
            glm::vec3 bottomRight0 = topLeft + (float) (i + 1) * iterX + (float) (j + 1) * iterY;

            makeTile(topLeft0, topRight0, bottomLeft0, bottomRight0);

        }
    }

}

std::vector<float> shapes::makeCube() {

    // init value
    m_vertexData = std::vector<float>();

    // make shape
    makeSquareFace( //
        glm::vec3(-0.5f,  0.5f,  0.5f), // top left
        glm::vec3( 0.5f,  0.5f,  0.5f), // top right
        glm::vec3(-0.5f, -0.5f,  0.5f), // bottom left
        glm::vec3( 0.5f, -0.5f,  0.5f)  // bottom right
    );
    makeSquareFace(
        glm::vec3(-0.5f,  0.5f, -0.5f),
        glm::vec3(-0.5f, -0.5f, -0.5f),
        glm::vec3( 0.5f,  0.5f, -0.5f),
        glm::vec3( 0.5f, -0.5f, -0.5f)
    );
    makeSquareFace(
        glm::vec3(-0.5f,  0.5f,  0.5f),
        glm::vec3(-0.5f,  0.5f, -0.5f),
        glm::vec3( 0.5f,  0.5f,  0.5f),
        glm::vec3( 0.5f,  0.5f, -0.5f)
    );
    makeSquareFace(
        glm::vec3(-0.5f, -0.5f,  0.5f),
        glm::vec3( 0.5f, -0.5f,  0.5f),
        glm::vec3(-0.5f, -0.5f, -0.5f),
        glm::vec3( 0.5f, -0.5f, -0.5f)
    );
    makeSquareFace(
        glm::vec3( 0.5f, -0.5f,  0.5f),
        glm::vec3( 0.5f,  0.5f,  0.5f),
        glm::vec3( 0.5f, -0.5f, -0.5f),
        glm::vec3( 0.5f,  0.5f, -0.5f)
    );
    makeSquareFace(
        glm::vec3(-0.5f, -0.5f,  0.5f),
        glm::vec3(-0.5f, -0.5f, -0.5f),
        glm::vec3(-0.5f,  0.5f,  0.5f),
        glm::vec3(-0.5f,  0.5f, -0.5f)
    );

    // return
    return m_vertexData;

}

void shapes::makeSphereTile(glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 bottomLeft, glm::vec3 bottomRight) {

    // point 1
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, normalize(topLeft));

    // point 2
    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, normalize(bottomLeft));

    // point 3
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, normalize(bottomRight));

    // point 4
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, normalize(topLeft));

    // point 5
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, normalize(bottomRight));

    // point 6
    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, normalize(topRight));

}

void shapes::makeWedge(float currentTheta, float nextTheta) {

    // condition
    if (settings.shapeParameter1 <= 1) settings.shapeParameter1 = 1;

    for (int i = 0; i < settings.shapeParameter1; i++) {

        float phi0 = glm::radians(180.f * i / settings.shapeParameter1);
        float phi1 = glm::radians(180.f * (i + 1) / settings.shapeParameter1);

        float x0 = glm::sin(phi0) * glm::sin(currentTheta);
        float y0 = glm::cos(phi0);
        float z0 = glm::sin(phi0) * glm::cos(currentTheta);

        float x1 = glm::sin(phi0) * glm::sin(nextTheta);
        float y1 = glm::cos(phi0);
        float z1 = glm::sin(phi0) * glm::cos(nextTheta);

        float x2 = glm::sin(phi1) * glm::sin(currentTheta);
        float y2 = glm::cos(phi1);
        float z2 = glm::sin(phi1) * glm::cos(currentTheta);

        float x3 = glm::sin(phi1) * glm::sin(nextTheta);
        float y3 = glm::cos(phi1);
        float z3 = glm::sin(phi1) * glm::cos(nextTheta);

        glm::vec3 v0 = 0.5f * glm::vec3{x0, y0, z0};
        glm::vec3 v1 = 0.5f * glm::vec3{x1, y1, z1};
        glm::vec3 v2 = 0.5f * glm::vec3{x2, y2, z2};
        glm::vec3 v3 = 0.5f * glm::vec3{x3, y3, z3};

        makeSphereTile(v0, v1, v2, v3);

    }

}

std::vector<float> shapes::makeSphere() {

    // init value
    m_vertexData = std::vector<float>();

    // condition
    auto store = settings.shapeParameter1;
    if (settings.shapeParameter1 < 2) settings.shapeParameter1 = 2;
    if (settings.shapeParameter2 < 2) settings.shapeParameter2 = 2;

    // base theta
    float theta = glm::radians(360.f / settings.shapeParameter2);

    // wedges
    for (int i = 0; i < settings.shapeParameter2; i++) {
        makeWedge(theta * (float) i, theta * (float) (i + 1));
    }

    // restore value
    settings.shapeParameter1 = store;

    // return
    return m_vertexData;

}

void shapes::makeTriangleTile(glm::vec3 origin, glm::vec3 first, glm::vec3 second) {

    // triangle sides
    auto side1 = (first - origin) / (float) settings.shapeParameter1;
    auto side2 = (second - origin) / (float) settings.shapeParameter1;

    // normal
    auto normal = glm::normalize(glm::cross(side2, side1));

    // iterations
    auto iterFirst = side1 + origin;
    auto iterSecond = side2 + origin;

    // first face
    insertVec3(m_vertexData, origin);     // point
    insertVec3(m_vertexData, normal);     // normal

    insertVec3(m_vertexData, iterSecond); // point
    insertVec3(m_vertexData, normal);     // normal

    insertVec3(m_vertexData, iterFirst);  // point
    insertVec3(m_vertexData, normal);     // normal

    // build triangle face
    for (int i = 0; i < settings.shapeParameter1 - 1; i++) {

        // temp sides
        auto tempIterFirst = iterFirst + side1;
        auto tempIterSecond = iterSecond + side2;

        // build faces
        insertVec3(m_vertexData, iterFirst);
        insertVec3(m_vertexData, normal);
        insertVec3(m_vertexData, iterSecond);
        insertVec3(m_vertexData, normal);
        insertVec3(m_vertexData, tempIterSecond);
        insertVec3(m_vertexData, normal);

        insertVec3(m_vertexData, iterFirst);
        insertVec3(m_vertexData, normal);
        insertVec3(m_vertexData, tempIterSecond);
        insertVec3(m_vertexData, normal);
        insertVec3(m_vertexData, tempIterFirst);
        insertVec3(m_vertexData, normal);

        // update iter
        iterFirst = tempIterFirst;
        iterSecond = tempIterSecond;

    }

}

void shapes::makeVerticalTile(glm::vec3 normal0, glm::vec3 normal1, glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 bottomLeft, glm::vec3 bottomRight) {

    // triangle sides
    auto side1 = (bottomLeft  - topLeft)  / (float) settings.shapeParameter1;
    auto side2 = (bottomRight - topRight) / (float) settings.shapeParameter1;

    // iterations
    auto iterFirst  = topLeft;
    auto iterSecond = topRight;

    // build faces
    for (int i = 0; i < settings.shapeParameter1; i++) {

        // temp sides
        auto tempIterFirst = iterFirst + side1;
        auto tempIterSecond = iterSecond + side2;

        // build faces
        insertVec3(m_vertexData, tempIterSecond);
        insertVec3(m_vertexData, normal1);
        insertVec3(m_vertexData, iterFirst);
        insertVec3(m_vertexData, normal0);
        insertVec3(m_vertexData, iterSecond);
        insertVec3(m_vertexData, normal1);

        insertVec3(m_vertexData, tempIterFirst);
        insertVec3(m_vertexData, normal0);
        insertVec3(m_vertexData, iterFirst);
        insertVec3(m_vertexData, normal0);
        insertVec3(m_vertexData, tempIterSecond);
        insertVec3(m_vertexData, normal1);

        // update iter
        iterFirst = tempIterFirst;
        iterSecond = tempIterSecond;

    }

}

std::vector<float> shapes::makeCylinder() {

    // bounds check
    if (settings.shapeParameter2 < 3) settings.shapeParameter2 = 3;

    // values
    m_vertexData = std::vector<float>();
    auto m_radius = 0.5f;
    auto angle = glm::radians(360.f / settings.shapeParameter2);

    auto offset = glm::radians(360.f / 4.f);

    // iterate over m_param1
    for (int i = 0; i < settings.shapeParameter2; i++) {

        auto angle0 = angle * i + offset;
        auto angle1 = angle * ((i + 1) % settings.shapeParameter2) + offset;

        // vertices
        auto origin = glm::vec3(0.f, 0.5f, 0.f);
        auto first  = glm::vec3(m_radius * glm::cos(angle0), 0.5f, m_radius * glm::sin(angle0));
        auto second = glm::vec3(m_radius * glm::cos(angle1), 0.5f, m_radius * glm::sin(angle1));

        auto origin1 = glm::vec3(0.f, -0.5f, 0.f);
        auto first1  = glm::vec3(m_radius * glm::cos(angle0), -0.5f, m_radius * glm::sin(angle0));
        auto second1 = glm::vec3(m_radius * glm::cos(angle1), -0.5f, m_radius * glm::sin(angle1));

        // caps
        makeTriangleTile(origin, first, second);
        makeTriangleTile(origin1, second1, first1);

        // edges
        auto normal0 = glm::normalize(first - origin);
        auto normal1 = glm::normalize(second - origin);
        makeVerticalTile(normal0, normal1, first, second, first1, second1);

    }

    // return data
    return m_vertexData;

}

void shapes::makeConeEdge(std::vector<glm::vec3> normals, std::vector<glm::vec3> vertices) {

    // values
    auto origin = vertices[0];
    auto first  = vertices[1];
    auto second = vertices[2];

    // triangle sides
    auto side1 = (first - origin) / (float) settings.shapeParameter1;
    auto side2 = (second - origin) / (float) settings.shapeParameter1;

    // iterations
    auto iterFirst = side1 + origin;
    auto iterSecond = side2 + origin;

    // first face
    insertVec3(m_vertexData, origin);     // point
    insertVec3(m_vertexData, normals[0]); // normal

    insertVec3(m_vertexData, iterSecond); // point
    insertVec3(m_vertexData, normals[2]); // normal

    insertVec3(m_vertexData, iterFirst);  // point
    insertVec3(m_vertexData, normals[1]); // normal

    // build triangle face
    for (int i = 0; i < settings.shapeParameter1 - 1; i++) {

        // temp sides
        auto tempIterFirst = iterFirst + side1;
        auto tempIterSecond = iterSecond + side2;

//        // build faces
//        insertVec3(m_vertexData, iterFirst);
//        insertVec3(m_vertexData, normals[1]);
//        insertVec3(m_vertexData, iterSecond);
//        insertVec3(m_vertexData, normals[2]);
//        insertVec3(m_vertexData, tempIterSecond);
//        insertVec3(m_vertexData, normals[2]);

//        insertVec3(m_vertexData, iterFirst);
//        insertVec3(m_vertexData, normals[1]);
//        insertVec3(m_vertexData, tempIterSecond);
//        insertVec3(m_vertexData, normals[2]);
//        insertVec3(m_vertexData, tempIterFirst);
//        insertVec3(m_vertexData, normals[1]);

        // build faces
        insertVec3(m_vertexData, iterFirst);
        insertVec3(m_vertexData, normals[1]);
        insertVec3(m_vertexData, iterSecond);
        insertVec3(m_vertexData, normals[2]);
        insertVec3(m_vertexData, tempIterFirst);
        insertVec3(m_vertexData, normals[1]);

        insertVec3(m_vertexData, iterSecond);
        insertVec3(m_vertexData, normals[2]);
        insertVec3(m_vertexData, tempIterSecond);
        insertVec3(m_vertexData, normals[2]);
        insertVec3(m_vertexData, tempIterFirst);
        insertVec3(m_vertexData, normals[1]);

        // update iter
        iterFirst = tempIterFirst;
        iterSecond = tempIterSecond;

    }

}

std::vector<float> shapes::makeCone() {

    // values
    m_vertexData = std::vector<float>();
    auto m_radius = 0.5f;

    // bounds check
    if (settings.shapeParameter2 < 3) settings.shapeParameter2 = 3;

    // values
    auto angle = glm::radians(360.f / settings.shapeParameter2);
    auto offset = glm::radians(90.f);
    auto tip = glm::vec3(0.f, 0.5f, 0.f);

    // iterate over m_param1
    for (int i = 0; i < settings.shapeParameter2; i++) {

        // angles
        auto angle0 = angle * i + offset;
        auto angle1 = angle * ((i + 1) % settings.shapeParameter2) + offset;

        // vertices
        auto origin = glm::vec3(0.f, -0.5f, 0.f);
        auto first  = glm::vec3(m_radius * glm::cos(angle0), -0.5f, m_radius * glm::sin(angle0));
        auto second = glm::vec3(m_radius * glm::cos(angle1), -0.5f, m_radius * glm::sin(angle1));

        // caps
        makeTriangleTile(origin, second, first);

        // normals
        auto normal0 = glm::normalize((first + second) / 2.f - origin);
        normal0[1] = 0.5f;
        normal0 = glm::normalize(normal0);

        auto normal1 = glm::normalize(first - origin);
        normal1[1] = 0.5f;
        normal1 = glm::normalize(normal1);

        auto normal2 = glm::normalize(second - origin);
        normal2[1] = 0.5f;
        normal2 = glm::normalize(normal2);

        // cone edge
        makeConeEdge(std::vector<glm::vec3>{normal0, normal1, normal2}, std::vector<glm::vec3>{tip, first, second});

    }

    // return data
    return m_vertexData;
}

// --- student code end --
