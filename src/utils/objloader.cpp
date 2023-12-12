#include "objloader.h"

#include <cstring>

objloader::objloader() {}

void push_vec2(std::vector<float> &data, glm::vec2 vec) {
    data.push_back(vec[0]);
    data.push_back(vec[1]);
}

void push_vec3(std::vector<float> &data, glm::vec3 vec) {
    data.push_back(vec[0]);
    data.push_back(vec[1]);
    data.push_back(vec[2]);
}

bool objloader::parse(const char *path, std::vector<float> &m_vertexData) {

    // debugging information
    bool verbose = true;
    verbose = false;
    std::cout << "parsing!!" << std::endl;

    // variables
    std::vector<int> indices;

    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;

    // check valid file path
    FILE * file = fopen(path, "r");
    if( file == NULL ){
        printf("Impossible to open the file !\n");
        return false;
    }

    // parse obj
    while(1) {

        char lineHeader[256]; // line header
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break; // end of file, stop

        if (strcmp(lineHeader, "v") == 0){ // vertex
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            temp_vertices.push_back(vertex);

            // debugging
            if (verbose) {
                std::cout << "vertex: (" << vertex[0] << ", " << vertex[1] << ", " << vertex[2] << ")" << std::endl;
            }
        } else

        if (strcmp(lineHeader, "vt") == 0){ // vertex uv coordinate
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            temp_uvs.push_back(uv);

            // debugging
            if (verbose) {
                std::cout << "uv: (" << uv[0] << ", " << uv[1] << ")" << std::endl;
            }
        } else

        if (strcmp(lineHeader, "vn") == 0){ // vertex normal
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            temp_normals.push_back(normal);

            // debugging
            if (verbose) {
                std::cout << "normal: (" << normal[0] << ", " << normal[1] << ", " << normal[2] << ")" << std::endl;
            }

        }

    }

    file = fopen(path, "r");
    while(1) {

        char lineHeader[256]; // line header
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break; // end of file, stop

        if (strcmp(lineHeader, "f") == 0){ // triangular face

            // face with uvs
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d",
                &vertexIndex[0],
                &uvIndex[0],
                &normalIndex[0],
                &vertexIndex[1],
                &uvIndex[1],
                &normalIndex[1],
                &vertexIndex[2],
                &uvIndex[2],
                &normalIndex[2]
            );

            if (matches != 9) { continue; }

            indices.push_back(vertexIndex[0]); // vertex indices
            indices.push_back(vertexIndex[1]);
            indices.push_back(vertexIndex[2]);

            indices.push_back(normalIndex[0]); // normal indices
            indices.push_back(normalIndex[1]);
            indices.push_back(normalIndex[2]);

            indices.push_back(uvIndex[0]); // uv indices
            indices.push_back(uvIndex[1]);
            indices.push_back(uvIndex[2]);

        }

    }

    file = fopen(path, "r");
    while(1) {

        char lineHeader[256]; // line header
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break; // end of file, stop

        if (strcmp(lineHeader, "f") == 0){ // triangular face

            // face without uvs
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], normalIndex[3];

            int matches = fscanf(file, "%d//%d %d//%d %d//%d\n",
                &vertexIndex[0],
                &normalIndex[0],
                &vertexIndex[1],
                &normalIndex[1],
                &vertexIndex[2],
                &normalIndex[2]
            );

            if (matches != 6) { continue; }

            indices.push_back(vertexIndex[0]); // vertex indices
            indices.push_back(vertexIndex[1]);
            indices.push_back(vertexIndex[2]);

            indices.push_back(normalIndex[0]); // normal indices
            indices.push_back(normalIndex[1]);
            indices.push_back(normalIndex[2]);

            indices.push_back(-1);
            indices.push_back(-1);
            indices.push_back(-1);

        }

    }

    std::cout << "faces found: " << indices.size() / 9 << std::endl;

    // indexing faces
    for (unsigned int i = 0; i < indices.size(); i += 9) {

        unsigned int vertexIndex0 = indices[i];
        unsigned int vertexIndex1 = indices[i + 1];
        unsigned int vertexIndex2 = indices[i + 2];

        glm::vec3 vertex0 = temp_vertices[vertexIndex0 - 1];
        glm::vec3 vertex1 = temp_vertices[vertexIndex1 - 1];
        glm::vec3 vertex2 = temp_vertices[vertexIndex2 - 1];

        unsigned int normalIndex0 = indices[i + 3];
        unsigned int normalIndex1 = indices[i + 4];
        unsigned int normalIndex2 = indices[i + 5];

        glm::vec3 normal0 = temp_normals[normalIndex0 - 1];
        glm::vec3 normal1 = temp_normals[normalIndex1 - 1];
        glm::vec3 normal2 = temp_normals[normalIndex2 - 1];

        unsigned int uvIndex0 = indices[i + 6];
        unsigned int uvIndex1 = indices[i + 7];
        unsigned int uvIndex2 = indices[i + 8];

        glm::vec2 uv0 = uvIndex0 == -1 ? glm::vec2(0.f) : temp_uvs[uvIndex0 - 1];
        glm::vec2 uv1 = uvIndex1 == -1 ? glm::vec2(0.f) : temp_uvs[uvIndex1 - 1];
        glm::vec2 uv2 = uvIndex2 == -1 ? glm::vec2(0.f) : temp_uvs[uvIndex2 - 1];

        push_vec3(m_vertexData, vertex0);
        push_vec3(m_vertexData, normal0);
        push_vec2(m_vertexData, uv0);
        m_vertexData.push_back(float(fileCount));

        push_vec3(m_vertexData, vertex1);
        push_vec3(m_vertexData, normal1);
        push_vec2(m_vertexData, uv1);
        m_vertexData.push_back(float(fileCount));

        push_vec3(m_vertexData, vertex2);
        push_vec3(m_vertexData, normal2);
        push_vec2(m_vertexData, uv2);
        m_vertexData.push_back(float(fileCount));

    }

    // update file count
    fileCount++;

    return true;

}
