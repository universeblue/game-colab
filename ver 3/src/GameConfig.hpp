// GameConfig.hpp
#ifndef GAME_CONFIG_HPP
#define GAME_CONFIG_HPP

#include "settings.hpp"   // همین فایل بالا

namespace GameConfig
{
    // اندازه هر خانه روی صفحه (پیکسل)
    inline constexpr int TILE_SIZE      = 32;
    inline constexpr int TOOLBAR_HEIGHT = 96;

    inline int windowWidth()
    {
        return Setting::MAP_COLS * TILE_SIZE;
    }

    inline int windowHeight()
    {
        return TOOLBAR_HEIGHT + Setting::MAP_ROWS * TILE_SIZE;
    }
}

#endif // GAME_CONFIG_HPP
