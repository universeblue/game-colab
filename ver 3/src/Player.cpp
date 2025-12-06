// Player.cpp
#include "Player.hpp"
#include <stdexcept>
#include <algorithm>

Player::Player(Map& map)
    : m_map(map)
    , m_row(1)
    , m_col(1)
    , m_coins(Setting::INITIAL_COINS)
    , m_waterCapacity(Setting::WATERING_CAN_CAPACITY)
    , m_currentWater(0)
    , m_dir(Direction::Down)
    , m_frameWidth(0)
    , m_frameHeight(0)
{
    if (!m_textureMain.loadFromFile("assets/sprites/main_character.png"))
        throw std::runtime_error("Failed to load main_character.png");

    m_textureMain.setSmooth(false);

    // اسپریت‌شیت 4 ستون × 4 ردیف (عکسی که فرستادی)
    const int FRAME_COLS = 4;
    const int FRAME_ROWS = 4;

    auto texSize   = m_textureMain.getSize();
    m_frameWidth   = texSize.x / FRAME_COLS;
    m_frameHeight  = texSize.y / FRAME_ROWS;

    m_sprite.setTexture(m_textureMain);

    // جهت اولیه: پایین
    m_dir = Direction::Down;
    updateSpriteRect();

    // اسکیل به اندازه‌ی TILE
    float sx = GameConfig::TILE_SIZE * 0.9f / static_cast<float>(m_frameWidth);
    float sy = GameConfig::TILE_SIZE * 0.9f / static_cast<float>(m_frameHeight);
    float s  = std::min(sx, sy);

    m_sprite.setScale(s, s);
    m_sprite.setOrigin(m_frameWidth / 2.0f, m_frameHeight / 2.0f);
    m_sprite.setPosition(m_map.tileCenter(m_row, m_col));
}

void Player::updateSpriteRect()
{
    // فرض: هر ردیف برای یک جهت است.
    // این مپ را اگر دیدی برعکس است راحت می‌شه جابه‌جا کرد.
    int row = 0;
    switch (m_dir)
    {
        case Direction::Down:  row = 0; break;
        case Direction::Left:  row = 3; break;
        case Direction::Right: row = 1; break;
        case Direction::Up:    row = 2; break;
    }

    // یک ستون ثابت (مثلاً ستون 1 = حالت ایستاده)
    int col = 1;

    sf::IntRect rect(
        col * m_frameWidth,
        row * m_frameHeight,
        m_frameWidth,
        m_frameHeight
    );
    m_sprite.setTextureRect(rect);
}

void Player::draw(sf::RenderWindow& window) const
{
    window.draw(m_sprite);
}

void Player::move(int dr, int dc)
{
    if (dr == 0 && dc == 0)
        return;

    // تعیین جهت بر اساس حرکت
    if (dr == 1 && dc == 0)       m_dir = Direction::Down;
    else if (dr == -1 && dc == 0) m_dir = Direction::Up;
    else if (dr == 0 && dc == -1) m_dir = Direction::Left;
    else if (dr == 0 && dc == 1)  m_dir = Direction::Right;

    int nr = m_row + dr;
    int nc = m_col + dc;

    if (m_map.isWalkable(nr, nc))
    {
        m_row = nr;
        m_col = nc;
        m_sprite.setPosition(m_map.tileCenter(m_row, m_col));
    }

    updateSpriteRect();
}

void Player::plantSeed(const std::string& typeName)
{
    const Setting::PlantConfig& cfg = Setting::PLANT_TYPES.at(typeName);

    if (m_coins < cfg.price)
        return;

    if (!m_map.isPlantable(m_row, m_col) || m_map.hasPlant(m_row, m_col))
        return;

    m_coins -= cfg.price;
    m_map.plantSeed(m_row, m_col, typeName);
}

void Player::useSelectedItem(SelectedItem item)
{
    switch (item)
    {
        case SelectedItem::Sickle:
        {
            Plant* p = m_map.getPlant(m_row, m_col);
            if (p)
            {
                int gain = p->harvest();
                if (gain > 0)
                    m_coins += gain;

                if (p->isDead())
                    m_map.removePlant(m_row, m_col);
            }
        } break;

        case SelectedItem::WateringCan:
        {
            if (m_map.isAdjacentToFillable(m_row, m_col))
            {
                m_currentWater = m_waterCapacity;
            }
            else if (m_map.hasPlant(m_row, m_col) && m_currentWater > 0)
            {
                Plant* p = m_map.getPlant(m_row, m_col);
                if (p)
                {
                    p->water();
                    --m_currentWater;
                }
            }
        } break;

        case SelectedItem::PumpkinSeed:
            plantSeed(Setting::PLANT_PUMPKIN);
            break;

        case SelectedItem::BlueMelonSeed:
            plantSeed(Setting::PLANT_BLUE_MELON);
            break;

        case SelectedItem::PineappleSeed:
            plantSeed(Setting::PLANT_PINEAPPLE);
            break;

        case SelectedItem::WheatSeed:
            plantSeed(Setting::PLANT_WHEAT);
            break;
    }
}
