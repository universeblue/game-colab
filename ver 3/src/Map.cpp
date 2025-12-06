#include "Map.hpp"
#include <stdexcept>

Map::Map()
{
    loadTileTextures();
    m_cells.resize(Setting::MAP_ROWS * Setting::MAP_COLS);
    buildFromDefaultMap();
}

void Map::loadTileTextures()
{
    for (const auto& pair : Setting::TILE_TEXTURE_PATHS)
    {
        char tileChar = pair.first;
        const std::string& path = pair.second;

        sf::Texture tex;
        if (!tex.loadFromFile(path))
            throw std::runtime_error("Failed to load tile texture: " + path);

        tex.setRepeated(false);
        m_tileTextures[tileChar] = tex;

        // NEW: تکسچر water جداگانه
        if (!m_waterTexture.loadFromFile("assets/sprites/water.png"))
            throw std::runtime_error("Failed to load water.png");
        m_waterTexture.setRepeated(false);

    }
}

bool Map::hasAdjacentPond(int r, int c) const
{
    // فقط 4 جهت اصلی را چک می‌کنیم: بالا، پایین، چپ، راست
    static const int dr[4] = { -1, 1,  0, 0 };
    static const int dc[4] = {  0, 0, -1, 1 };

    for (int i = 0; i < 4; ++i)
    {
        int nr = r + dr[i];
        int nc = c + dc[i];

        if (nr < 0 || nr >= Setting::MAP_ROWS ||
            nc < 0 || nc >= Setting::MAP_COLS)
            continue;

        // اگر همسایه هم P باشد → کنار این خونه آب دیگر هم هست
        if (Setting::DEFAULT_MAP[nr][nc] == Setting::TILE_POND)
            return true;
    }
    return false;
}


void Map::buildFromDefaultMap()
{
    for (int r = 0; r < Setting::MAP_ROWS; ++r)
    {
        const std::string& rowStr = Setting::DEFAULT_MAP.at(r);
        for (int c = 0; c < Setting::MAP_COLS; ++c)
        {
            char ch = rowStr.at(c);
            Cell& cell = at(r, c);
            cell.tileChar = ch;
            cell.tileCfg  = Setting::TILE_TYPES.at(ch);

            // --- انتخاب تکسچر ---
            if (ch == Setting::TILE_POND)
            {
                // اگر این P تکی است → از pond.png (همان m_tileTextures['P'])
                // اگر کنار این، P دیگری هست → از water.png
                if (hasAdjacentPond(r, c))
                    cell.sprite.setTexture(m_waterTexture);
                else
                    cell.sprite.setTexture(m_tileTextures.at(ch));
            }
            else
            {
                // سایر tileها طبق تنظیمات قبلی
                cell.sprite.setTexture(m_tileTextures.at(ch));
            }

            // موقعیت و اسکیل مثل قبل
            cell.sprite.setPosition(
                static_cast<float>(c * GameConfig::TILE_SIZE),
                static_cast<float>(GameConfig::TOOLBAR_HEIGHT + r * GameConfig::TILE_SIZE)
            );

            float sx = GameConfig::TILE_SIZE / static_cast<float>(cell.sprite.getTexture()->getSize().x);
            float sy = GameConfig::TILE_SIZE / static_cast<float>(cell.sprite.getTexture()->getSize().y);
            cell.sprite.setScale(sx, sy);
        }
    }
}


Map::Cell& Map::at(int r, int c)
{
    return m_cells[r * Setting::MAP_COLS + c];
}

const Map::Cell& Map::at(int r, int c) const
{
    return m_cells[r * Setting::MAP_COLS + c];
}

bool Map::inBounds(int r, int c) const
{
    return (r >= 0 && r < Setting::MAP_ROWS &&
            c >= 0 && c < Setting::MAP_COLS);
}

bool Map::isWalkable(int r, int c) const
{
    if (!inBounds(r, c)) return false;
    return at(r, c).tileCfg.walkable;
}

bool Map::isPlantable(int r, int c) const
{
    if (!inBounds(r, c)) return false;
    return at(r, c).tileCfg.plantable;
}

bool Map::canFillWater(int r, int c) const
{
    if (!inBounds(r, c)) return false;
    return at(r, c).tileCfg.fillable;
}

bool Map::isAdjacentToFillable(int r, int c) const
{
    static const int dr[4] = {-1, 1, 0, 0};
    static const int dc[4] = {0, 0, -1, 1};

    for (int i = 0; i < 4; ++i)
    {
        int nr = r + dr[i];
        int nc = c + dc[i];
        if (inBounds(nr, nc) && canFillWater(nr, nc))
            return true;
    }
    return false;
}

sf::Vector2f Map::tileCenter(int r, int c) const
{
    return sf::Vector2f(
        c * GameConfig::TILE_SIZE + GameConfig::TILE_SIZE / 2.0f,
        GameConfig::TOOLBAR_HEIGHT + r * GameConfig::TILE_SIZE + GameConfig::TILE_SIZE / 2.0f
    );
}

Plant* Map::getPlant(int r, int c)
{
    if (!inBounds(r, c)) return nullptr;
    return at(r, c).plant.get();
}

const Plant* Map::getPlant(int r, int c) const
{
    if (!inBounds(r, c)) return nullptr;
    return at(r, c).plant.get();
}

bool Map::hasPlant(int r, int c) const
{
    return getPlant(r, c) != nullptr;
}

void Map::plantSeed(int r, int c, const std::string& plantTypeName)
{
    if (!inBounds(r, c)) return;
    Cell& cell = at(r, c);

    if (!cell.tileCfg.plantable || cell.plant)
        return;

    cell.plant = std::make_unique<Plant>(plantTypeName, tileCenter(r, c));
}

void Map::removePlant(int r, int c)
{
    if (!inBounds(r, c)) return;
    at(r, c).plant.reset();
}

void Map::updatePlants(float dt)
{
    for (int r = 0; r < Setting::MAP_ROWS; ++r)
        for (int c = 0; c < Setting::MAP_COLS; ++c)
        {
            Plant* p = getPlant(r, c);
            if (p)
                p->update(dt);
        }
}

void Map::draw(sf::RenderWindow& window) const
{
    for (int r = 0; r < Setting::MAP_ROWS; ++r)
        for (int c = 0; c < Setting::MAP_COLS; ++c)
        {
            const Cell& cell = at(r, c);
            window.draw(cell.sprite);
            if (cell.plant)
                cell.plant->draw(window);
        }
}
