#ifndef MAP_HPP
#define MAP_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <map>

#include "settings.hpp"
#include "GameConfig.hpp"
#include "Plant.hpp"

class Map
{
public:
    Map();

    void draw(sf::RenderWindow& window) const;
    void updatePlants(float dt);

    bool inBounds(int r, int c) const;
    bool isWalkable(int r, int c) const;
    bool isPlantable(int r, int c) const;
    bool canFillWater(int r, int c) const;
    bool isAdjacentToFillable(int r, int c) const;

    Plant*       getPlant(int r, int c);
    const Plant* getPlant(int r, int c) const;

    bool hasPlant(int r, int c) const;

    void plantSeed(int r, int c, const std::string& plantTypeName);
    void removePlant(int r, int c);

    sf::Vector2f tileCenter(int r, int c) const;

private:
    struct Cell
    {
        char                  tileChar;
        Setting::TileConfig   tileCfg;
        sf::Sprite            sprite;
        std::unique_ptr<Plant> plant;
    };

    std::vector<Cell>           m_cells;
    std::map<char, sf::Texture> m_tileTextures;

    // NEW: تکسچر مجزا برای water
    sf::Texture m_waterTexture;

    Cell& at(int r, int c);
    const Cell& at(int r, int c) const;

    void loadTileTextures();
    void buildFromDefaultMap();

    // NEW: چک می‌کند کنار این خانه، خانه‌ی P دیگری هست یا نه
    bool hasAdjacentPond(int r, int c) const;

};

#endif
