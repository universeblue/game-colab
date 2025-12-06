// Game.cpp
#include "Game.hpp"
#include <algorithm>
#include <sstream>
#include <iostream>

Game::Game()
    : m_window(
        sf::VideoMode(GameConfig::windowWidth(), GameConfig::windowHeight()),
        "Pixel Harvest"
      )
    , m_map()
    , m_player(m_map)
    , m_toolbar()
    , m_finished(false)
    , m_won(false)
{
    if (!m_font.loadFromFile(Setting::FONT_PATH))
    std::cerr << "Failed to load font: " << Setting::FONT_PATH << "\n";

    // همه‌ی متن‌ها از همین فونت استفاده کنند
    m_coinsText.setFont(m_font);
    m_timeText.setFont(m_font);
    m_waterText.setFont(m_font);

    // کمی بزرگ‌تر و خواناتر
    m_coinsText.setCharacterSize(22);
    m_timeText.setCharacterSize(22);
    m_waterText.setCharacterSize(22);

    m_coinsText.setFillColor(sf::Color::White);
    m_timeText.setFillColor(sf::Color::White);
    m_waterText.setFillColor(sf::Color::White);

    // جای HUD: گوشه‌ی بالا-راست، بیرون از اسلات‌های تولبار
    float hudX = GameConfig::windowWidth() - 220.0f;  // فاصله از راست

    m_coinsText.setPosition(hudX, 8.0f);
    m_timeText.setPosition(hudX, 8.0f + 26.0f);   // کمی زیر Coins
    m_waterText.setPosition(hudX, 8.0f + 52.0f); // زیر Time


    m_resultText.setFont(m_font);
    m_resultText.setCharacterSize(36);
    m_resultText.setFillColor(sf::Color::White);
    m_resultText.setPosition(200.0f, 200.0f);

    m_dtClock.restart();
    m_gameClock.restart();
}

void Game::run()
{
    while (m_window.isOpen())
    {
        processEvents();

        float dt = m_dtClock.restart().asSeconds();
        if (!m_finished)
            update(dt);

        render();
    }
}

void Game::processEvents()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            m_window.close();

        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
                m_window.close();

            if (!m_finished)
            {
                // حرکت
                if (event.key.code == sf::Keyboard::W) m_player.move(-1,  0);
                if (event.key.code == sf::Keyboard::S) m_player.move( 1,  0);
                if (event.key.code == sf::Keyboard::A) m_player.move( 0, -1);
                if (event.key.code == sf::Keyboard::D) m_player.move( 0,  1);

                // یک helper برای انتخاب ابزار + استفاده‌ی فوری
                auto selectAndUse = [&](int idx)
                {
                    m_toolbar.handleNumberKey(idx);
                    m_player.useSelectedItem(m_toolbar.selected());
                };

                if (event.key.code == sf::Keyboard::Num0) selectAndUse(0); // داس
                if (event.key.code == sf::Keyboard::Num1) selectAndUse(1); // آب‌پاش
                if (event.key.code == sf::Keyboard::Num2) selectAndUse(2); // Pumpkin
                if (event.key.code == sf::Keyboard::Num3) selectAndUse(3); // Melon
                if (event.key.code == sf::Keyboard::Num4) selectAndUse(4); // Pineapple
                if (event.key.code == sf::Keyboard::Num5) selectAndUse(5); // Wheat

                // دیگه به Space احتیاج نداریم. اگر خواستی می‌تونی برای راحتی نگهش داری:
                // if (event.key.code == sf::Keyboard::Space)
                //     m_player.useSelectedItem(m_toolbar.selected());
            }
        }
    }
}


void Game::update(float dt)
{
    float elapsed   = m_gameClock.getElapsedTime().asSeconds();
    float remaining = std::max(0.0f, static_cast<float>(Setting::TOTAL_TIME) - elapsed);

    m_map.updatePlants(dt);
    updateHUD();

    if (!m_finished)
    {
        if (m_player.getCoins() >= Setting::TARGET_COINS)
        {
            m_finished = true;
            m_won      = true;
            m_resultText.setString("You Win!");
        }
        else if (remaining <= 0.0f)
        {
            m_finished = true;
            m_won      = false;
            m_resultText.setString("You Lose!");
        }
    }
}

void Game::updateHUD()
{
    std::ostringstream ss;

    ss << "Coins: " << m_player.getCoins()
       << " / " << Setting::TARGET_COINS;
    m_coinsText.setString(ss.str());

    float elapsed   = m_gameClock.getElapsedTime().asSeconds();
    float remaining = std::max(0.0f, static_cast<float>(Setting::TOTAL_TIME) - elapsed);
    ss.str("");
    ss << "Time: " << static_cast<int>(remaining) << " s";
    m_timeText.setString(ss.str());

    ss.str("");
    ss << "Water: " << m_player.getWater()
       << " / " << m_player.getWaterCapacity();
    m_waterText.setString(ss.str());
    m_toolbar.updateWaterIcon(m_player.getWater(),
                          m_player.getWaterCapacity());

}

void Game::render()
{
    m_window.clear(sf::Color(20, 20, 20));

    m_toolbar.draw(m_window);
    m_map.draw(m_window);
    m_player.draw(m_window);

    m_window.draw(m_coinsText);
    m_window.draw(m_timeText);
    m_window.draw(m_waterText);

    if (m_finished)
        m_window.draw(m_resultText);

    m_window.display();
}
