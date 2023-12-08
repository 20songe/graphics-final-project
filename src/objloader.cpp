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
    std::vector< float > temp_vertices;
    std::vector< float > temp_uvs;
    std::vector< float > temp_normals;

    int num_lines = 0;
    FILE * file = fopen(path, "r");
    if( file == NULL ){
        printf("Impossible to open the file !\n");
        return false;
    }

    std::cout << "before while" << std::endl;
    while( 1 ){

        //        if (num_lines == 500) {
        //            break;
        //        }

        char lineHeader[1000];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);

        if (res == -1 || res == EOF) {
            std::cout << num_lines << std::endl;
            break; // EOF = End Of File. Quit the loop.
        }

        num_lines += 1;


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
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
            //            if (matches != 6){
            //                printf("File can't be read by our simple parser : ( Try exporting with other options\n");
            //                return false;
            //            }
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices    .push_back(uvIndex[0]);
            uvIndices    .push_back(uvIndex[1]);
            uvIndices    .push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        }

    }

    for( unsigned int i=0; i<vertexIndices.size(); i ++ ) {
        int ind = 3 * (vertexIndices[i] - 1);
        int uvInd = 3 * (uvIndices[i] - 1);
        int normInd = 3 * (normalIndices[i] - 1);
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
    }

    //    for ( unsigned int i = 0; i < vertexIndices.size(); i++) {
    //        unsigned int vertexIndex = 3 * (vertexIndices[i] - 1);
    //        glm::vec3 vertex(temp_vertices[ vertexIndex ],
    //                         temp_vertices[ vertexIndex + 1],
    //                         temp_vertices[ vertexIndex + 2]);
    //        insertVec3(out_vertices, vertex);
    //    }

    //    for( unsigned int i=0; i<uvIndices.size(); i++ ) {
    //        unsigned int uvIndex = 2 * (uvIndices[i] - 1);
    //        glm::vec2 uv (temp_uvs[ uvIndex],
    //                     temp_uvs[ uvIndex + 1 ]);
    //        insertVec2(out_uvs, uv);
    //    }

    //    for( unsigned int i=0; i<normalIndices.size(); i++ ) {
    //        unsigned int normalIndex = 3 * (normalIndices[i] - 1);
    //        glm::vec3 normal(temp_normals[ normalIndex ],
    //                         temp_normals[ normalIndex + 1],
    //                         temp_normals[normalIndex + 2]);
    //        insertVec3(out_normals, normal);
    //    }

    std::cout << "end of loop" << std::endl;
    return true;

}
