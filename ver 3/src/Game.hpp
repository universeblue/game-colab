// Game.hpp
#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "settings.hpp"
#include "GameConfig.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "Toolbar.hpp"

class Game
{
public:
    Game();
    void run();

private:
    sf::RenderWindow m_window;
    Map              m_map;
    Player           m_player;
    Toolbar          m_toolbar;

    sf::Font         m_font;
    sf::Text         m_coinsText;
    sf::Text         m_timeText;
    sf::Text         m_waterText;
    sf::Text         m_resultText;

    sf::Clock        m_dtClock;
    sf::Clock        m_gameClock;

    bool             m_finished;
    bool             m_won;

    void processEvents();
    void update(float dt);
    void render();
    void updateHUD();
};

#endif // GAME_HPP
