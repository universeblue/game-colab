#ifndef CONFIG_HPP_INCLUDED
#define CONFIG_HPP_INCLUDED

#include <string>
#include <map>
#include <vector>

namespace Setting
{

    const int TARGET_COINS = 500;  // win condition
    const int INITIAL_COINS = 100; // starting money
    const int TOTAL_TIME = 120;    // total time for the game in seconds

    //
    // ============================
    //  Map & Tile Configuration
    // ============================
    //

    // Tile type characters (as used in map)
    const char TILE_GRASS = 'G';
    const char TILE_WALL = 'W';
    const char TILE_POND = 'P';
    const char TILE_ROAD = 'R';

    struct TileConfig
    {
        bool walkable;  // 1 = player can walk
        bool plantable; // 1 = player can plant
        bool fillable;  // 1 = can fill watering can (pond)
    };

    const std::map<char, TileConfig> TILE_TYPES = {
        {TILE_GRASS, {1, 1, 0}},
        {TILE_WALL, {0, 0, 0}},
        {TILE_POND, {0, 0, 1}},
        {TILE_ROAD, {1, 0, 0}}};

    // Map settings
    const int MAP_ROWS = 18;
    const int MAP_COLS = 32;

    // Default map data using characters
    const std::vector<std::string> DEFAULT_MAP = {
        "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
        "WGGGWPPPPPRRRRRRRRRRRRRRRRGGGGGW",
        "WGGGWPPPPPRRRRRRRRRRRRRRRRGGGGGW",
        "WGGGWRRRRRRRRRRRRRRRRRRRRRRRRGGW",
        "WGGGGRRRRRRRRRRRRRRRRRRRRRRRRGGW",
        "WGGGGRRRRRWWWWWWRRRRWWWWWWRRRGGW",
        "WGGGGRRRRRPPPPPPRRRRPPPPPPRRRGGW",
        "WWWWWRRRRRPPPPPPRRRRPPPPPPRRRGGW",
        "WPPPRRRRRRPPPPPPRRRRPPPPPPRRRGGW",
        "WPPPRRRRRRPPPPPPRRRRPPPPPPRRRGGW",
        "WWWWWRRRRRWWWWWWRRRRWWWWWWRRRGGW",
        "WGGGGRRRRRRRRRRRRRRRRRRRRRRRRGGW",
        "WGGGGRRRRRRRRRRRRRRRRRRRRRRRRGGW",
        "WGGGGWWWWWWRRRWWWWWWRRRWRRRWWWWW",
        "WGGGGGGGGGWRRRWGGGGGGGGWRRRGGGGW",
        "WGGGGGGGGGWRRRWGGGGGGGGWRRRGGGGW",
        "WGGGGGGGGGWRRRWGGGGGGGGWRRRGGGGW",
        "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"};

    //
    // ============================
    //  Plant Configuration
    // ============================
    //

    const std::string PLANT_PUMPKIN = "pumpkin";
    const std::string PLANT_BLUE_MELON = "melon";
    const std::string PLANT_PINEAPPLE = "pineapple";
    const std::string PLANT_WHEAT = "wheat";

    struct PlantConfig
    {
        int price;
        int growthStages; // Number of growth stages
        int stageTime;    // Time per stage (seconds)
        int dryoutTime;   // Time before plant dies without water
        int profit;
        int persistent; // 1 = keeps growing after harvest, 0 = dies and disappears
    };

    const std::map<std::string, PlantConfig> PLANT_TYPES = {
        {PLANT_PUMPKIN, {30, 4, 7, 15, 25, 0}},
        {PLANT_BLUE_MELON, {25, 3, 6, 12, 22, 0}},
        {PLANT_PINEAPPLE, {50, 5, 7, 20, 35, 1}},
        {PLANT_WHEAT, {0, 2, 3, 7, 13, 0}}};

    const int WATERING_CAN_CAPACITY = 5;

    //
    // ============================
    //  File Paths
    // ============================
    //
    const std::string FONT_PATH = "assets/fonts/sh-pinscher/SHPinscher-Regular.otf";
    const std::string MUSIC_PATH = "assets/audio/background.ogg";

    //
    // ============================
    //  Texture Paths
    // ============================
    //

    const std::map<char, std::string> TILE_TEXTURE_PATHS = {
        {TILE_GRASS, "assets/sprites/grass.png"},
        {TILE_WALL, "assets/sprites/wall_combined.png"},
        {TILE_POND, "assets/sprites/pond.png"},
        {TILE_ROAD, "assets/sprites/Road.png"}};

}

#endif