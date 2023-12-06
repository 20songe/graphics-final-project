#include "WaterTile.h"

WaterTile::WaterTile(float centerX, float centerZ, float height)
    : x(centerX), z(centerZ), height(height) {}

float WaterTile::getHeight() const {
    return height;
}

float WaterTile::getX() const {
    return x;
}

float WaterTile::getZ() const {
    return z;
}
