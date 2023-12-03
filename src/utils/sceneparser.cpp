#include "sceneparser.h"
#include "scenefilereader.h"
#include <glm/gtx/transform.hpp>

#include <chrono>
#include <iostream>

/**
 * @brief calcCTM - function to calculate the CTM from a list of transformations
 * @param transformations - list of transformations
 * @param ctm
 */
void calcCTM(std::vector<SceneTransformation*> &transformations, glm::mat4 &ctm) {
    for (SceneTransformation* transformation : transformations) {
        glm::mat4 t;
        switch(transformation->type) {
        case(TransformationType::TRANSFORMATION_MATRIX):
            ctm = ctm * transformation->matrix;
            break;
        case(TransformationType::TRANSFORMATION_ROTATE):
            ctm = ctm * glm::rotate(transformation->angle, transformation->rotate);
            break;
        case(TransformationType::TRANSFORMATION_SCALE):
            ctm = ctm * glm::scale(transformation->scale);
            break;
        case(TransformationType::TRANSFORMATION_TRANSLATE):
            ctm = ctm * glm::translate(transformation->translate);
            break;
        }

    }
}

/**
 * @brief dfsParseTree - recursive function to parse data read from file
 * @param n - scene node
 * @param renderData - render data
 * @param ctm - cumulated transformation matrix
 */
void dfsParseTree(SceneNode* n, RenderData &renderData, glm::mat4 ctm) {
    calcCTM(n->transformations, ctm);

    for (ScenePrimitive* primitive : n->primitives) {
        glm::mat3 top3 = glm::transpose(glm::inverse(glm::mat3(ctm)));

        renderData.shapes.push_back(RenderShapeData{*primitive, ctm, glm::inverse(ctm), top3});

    }
    for (SceneLight* light : n->lights) {
        renderData.lights.push_back(SceneLightData{light->id,
            light->type,
            light->color,
            light->function,
            ctm * glm::vec4(0.f, 0.f, 0.f, 1.f),
            ctm * light->dir,
            light->penumbra,
            light->angle,
            light->width,
            light->height
        });
    }
    if (n->children.empty()) {
        return;
    }
    else {
        for (SceneNode* nextNode : n->children) {
            dfsParseTree(nextNode, renderData, ctm);
        }
    }
}

/**
 * @brief SceneParser::parse - parses the scene
 * @param filepath - file path to JSON with scene data
 * @param renderData - stores the render data
 * @return
 */
bool SceneParser::parse(std::string filepath, RenderData &renderData) {

    ScenefileReader fileReader = ScenefileReader(filepath);
    bool success = fileReader.readJSON();
    if (!success) {
        return false;
    }

    renderData.globalData = fileReader.getGlobalData();
    renderData.cameraData = fileReader.getCameraData();


    SceneNode* root = fileReader.getRootNode();
    renderData.shapes.clear();
    renderData.lights.clear();

    dfsParseTree(root, renderData, glm::mat4(1.f));

    return true;
}
