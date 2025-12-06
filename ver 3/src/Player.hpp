// Player.hpp
#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include "settings.hpp"  
#include "GameConfig.hpp"
#include "Map.hpp"
#include "Plant.hpp"
#include "Toolbar.hpp"

enum class SelectedItem;

class Player
{
public:
    explicit Player(Map& map);

    void draw(sf::RenderWindow& window) const;

    void move(int dr, int dc);
    void useSelectedItem(SelectedItem item);
    void plantSeed(const std::string& typeName);

    int  getCoins() const         { return m_coins; }
    int  getWater() const         { return m_currentWater; }
    int  getWaterCapacity() const { return m_waterCapacity; }

private:
    enum class Direction
    {
        Down  = 0,
        Left  = 1,
        Right = 2,
        Up    = 3
    };

    Map&        m_map;
    int         m_row;
    int         m_col;

    int         m_coins;
    int         m_waterCapacity;
    int         m_currentWater;

    sf::Texture m_textureMain;
    sf::Sprite  m_sprite;

    Direction   m_dir;

    int         m_frameWidth;
    int         m_frameHeight;

    void updateSpriteRect();   // فقط انتخاب فریم مناسب بر اساس جهت
};

#endif // PLAYER_HPP
