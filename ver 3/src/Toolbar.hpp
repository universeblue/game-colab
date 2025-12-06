#ifndef TOOLBAR_HPP
#define TOOLBAR_HPP

#include <SFML/Graphics.hpp>
#include "GameConfig.hpp"

enum class SelectedItem
{
    Sickle = 0,
    WateringCan = 1,
    PumpkinSeed = 2,
    BlueMelonSeed = 3,
    PineappleSeed = 4,
    WheatSeed = 5
};

class Toolbar
{
public:
    Toolbar();

    void handleNumberKey(int num);
    SelectedItem selected() const { return m_selected; }

    void draw(sf::RenderWindow& window) const;

    void updateWaterIcon(int currentWater, int capacity);

private:
    SelectedItem m_selected;

    sf::RectangleShape m_background;
    sf::RectangleShape m_slots[6];

    sf::Sprite  m_icons[6];
    sf::Texture m_texSickle;
    sf::Texture m_texWaterEmpty;
    sf::Texture m_texWaterFull;
    sf::Texture m_texPumpkin;
    sf::Texture m_texMelon;
    sf::Texture m_texPineapple;
    sf::Texture m_texWheat;

    void loadTextures();
    void updateAppearance();
};

#endif
