#ifndef WATER_TILE_H
#define WATER_TILE_H

class WaterTile {
public:
    static constexpr float TILE_SIZE = 60;

    WaterTile(float centerX, float centerZ, float height);

    float getHeight() const;
    float getX() const;
    float getZ() const;

private:
    float height;
    float x, z;
};

#endif // WATER_TILE_H
