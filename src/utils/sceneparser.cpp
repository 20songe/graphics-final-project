#include "sceneparser.h"
#include "scenefilereader.h"
#include <glm/gtx/transform.hpp>

#include <chrono>
#include <iostream>

// --- student code ---

void appendTransformation(SceneTransformation &ctm, SceneTransformation *transformation) {

    if (ctm.type != TransformationType::TRANSFORMATION_MATRIX) {return;}
    switch (transformation->type) {
    case TransformationType::TRANSFORMATION_TRANSLATE:
        ctm.matrix *= glm::translate(transformation->translate);
        break;

    case TransformationType::TRANSFORMATION_SCALE:
        ctm.matrix *= glm::scale(transformation->scale);
        break;

    case TransformationType::TRANSFORMATION_ROTATE:
        ctm.matrix *= glm::rotate(transformation->angle, transformation->rotate);
        break;

    case TransformationType::TRANSFORMATION_MATRIX:
        ctm.matrix *= transformation->matrix;
        break;
    }

}

void recursiveHelper(SceneNode* node, SceneTransformation ctm, std::vector<RenderShapeData> &shapes, std::vector<SceneLightData> &lights) {

    // create the ctm
    for (auto transformation : node->transformations) {
        // todo: should this be sored by type?
        appendTransformation(ctm, transformation);
    }

    // go through primitives
    for (auto primitive : node->primitives) {
        RenderShapeData data = RenderShapeData();
        data.primitive = *primitive;
        data.ctm = ctm.matrix;
        data.ctmInverse = glm::inverse(ctm.matrix);
        shapes.push_back(data);
    }

    // go through lights
    for (auto light : node->lights) {
        SceneLightData data = SceneLightData();
        data.angle = light->angle;
        data.color = light->color;
        data.dir = ctm.matrix * light->dir;
        data.function = light->function;
        data.height = light->height;
        data.id = light->id;
        data.penumbra = light->penumbra;
        data.pos = ctm.matrix * glm::vec4{0.f, 0.f, 0.f, 1.f};
        data.type = light->type;
        data.width = light->width;

        lights.push_back(data);
    }

    // go though the children (depth first)
    for (auto child : node->children) {
        recursiveHelper(child, ctm, shapes, lights);
    }

}

// --- end student code

bool SceneParser::parse(std::string filepath, RenderData &renderData) {
    ScenefileReader fileReader = ScenefileReader(filepath);
    bool success = fileReader.readJSON();
    if (!success) {
        return false;
    }

    // TODO: Use your Lab 5 code here

    // --- student code ---

    renderData.globalData = fileReader.getGlobalData();
    renderData.cameraData = fileReader.getCameraData();

    SceneNode* root = fileReader.getRootNode();

    renderData.shapes.clear();
    renderData.lights.clear();

    SceneTransformation ctm = SceneTransformation();
    ctm.type = TransformationType::TRANSFORMATION_MATRIX;
    ctm.matrix = glm::mat4(1.f);

    recursiveHelper(root, ctm, renderData.shapes, renderData.lights);

    // --- student code end ---

    return true;
}
