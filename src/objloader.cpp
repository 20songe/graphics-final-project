#include "objloader.h"
#include <iostream>
#include <string>

objloader::objloader()
{

}
void insertVec3(std::vector<float> &data, glm::vec3 &v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}

void insertVec2(std::vector<float> &data, glm::vec2 &v) {
    data.push_back(v.x);
    data.push_back(v.y);
}

bool objloader::loadOBJ(
    const char * path,
    std::vector<float> &data,
    std::vector<std::string> &line) {

    std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
    std::vector< int > temp_obj;
    std::vector< float > temp_vertices;
    std::vector< float > temp_uvs;
    std::vector< float > temp_normals;

    int obj_index = 0;

    int num_ones = 0;
    FILE * file = fopen(path, "r");
    if( file == NULL ){
        printf("Impossible to open the file !\n");
        return false;
    }

    while( 1 ){

        char lineHeader[128];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);

        if (res == -1 || res == EOF) {
            break; // EOF = End Of File. Quit the loop.
        }


        // else : parse lineHeader
        if ( strcmp( lineHeader, "v" ) == 0 ){
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
            insertVec3(temp_vertices, vertex);
        } else if ( strcmp( lineHeader, "vt" ) == 0 ){
            glm::vec2 uv2;
            fscanf(file, "%f %f\n", &uv2.x, &uv2.y );
            glm::vec3 uv (uv2, 0.f);
            insertVec3(temp_uvs, uv);
        } else if ( strcmp( lineHeader, "vn" ) == 0 ){
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
            insertVec3(temp_normals, normal);
        } else if ( strcmp( lineHeader, "f" ) == 0 ){
//            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );

            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices    .push_back(uvIndex[0]);
            uvIndices    .push_back(uvIndex[1]);
            uvIndices    .push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
            temp_obj.push_back(obj_index);

        } else if ( strcmp( lineHeader, "o" ) == 0 ) {
            char obj[128];
            fscanf(file, "%s\n", obj);
            obj_index = obj_index + 1;
        }

    }


    for( unsigned int i=0; i<vertexIndices.size(); i ++ ) {
        int ind = 3 * (vertexIndices[i] - 1);
        int uvInd = 3 * (uvIndices[i] - 1);
        int normInd = 3 * (normalIndices[i] - 1);

        int objInd = floor(i / 3);
        int currObj = temp_obj[objInd];

        glm::vec3 vertex (temp_vertices[ ind ],
                         temp_vertices[ ind + 1 ],
                         temp_vertices[ ind + 2 ]);
        glm::vec2 uv = glm::vec2(
            temp_uvs [ uvInd ],
            temp_uvs [ uvInd + 1]);
        glm::vec3 norm = glm::vec3(
            temp_normals[normInd],
            temp_normals[normInd + 1],
            temp_normals[normInd + 2]);
        insertVec3(data, vertex);
        insertVec2(data, uv);
        insertVec3(data, norm);
        data.push_back(currObj);
    }

    std::cout << "end of loop" << std::endl;
    return true;

}
