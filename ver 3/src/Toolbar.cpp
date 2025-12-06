// Toolbar.cpp
#include "Toolbar.hpp"
#include <stdexcept>
#include <algorithm>

namespace
{
    // برمی‌گرداند آخرین فریم وقتی می‌دانیم تکسچر چند فریم افقی دارد
    sf::IntRect lastFrameRectWithCount(const sf::Texture& tex, int frameCount)
    {
        auto size = tex.getSize();
        int frameWidth  = static_cast<int>(size.x) / frameCount;
        int frameHeight = static_cast<int>(size.y);

        return sf::IntRect(
            (frameCount - 1) * frameWidth, // x شروع فریم آخر
            0,
            frameWidth,
            frameHeight
        );
    }
}

Toolbar::Toolbar()
    : m_selected(SelectedItem::WateringCan)
{
    loadTextures();

    // پس‌زمینه‌ی نوار ابزار
    m_background.setSize(sf::Vector2f(
        static_cast<float>(GameConfig::windowWidth()),
        static_cast<float>(GameConfig::TOOLBAR_HEIGHT)
    ));
    m_background.setPosition(0.f, 0.f);
    m_background.setFillColor(sf::Color(40, 40, 40));

    // تنظیم خود اسلات‌ها
    float slotWidth  = 72.0f;
    float slotHeight = GameConfig::TOOLBAR_HEIGHT - 16.0f;
    float marginLeft = 16.0f;
    float gap        = 16.0f;

    for (int i = 0; i < 6; ++i)
    {
        m_slots[i].setSize(sf::Vector2f(slotWidth, slotHeight));
        m_slots[i].setPosition(
            marginLeft + i * (slotWidth + gap),
            8.0f
        );
        m_slots[i].setFillColor(sf::Color(90, 90, 90));
        m_slots[i].setOutlineThickness(2.0f);
        m_slots[i].setOutlineColor(sf::Color::Black);
    }

    // آیکون‌ها
    m_icons[0].setTexture(m_texSickle);
    m_icons[1].setTexture(m_texWaterEmpty);
    m_icons[2].setTexture(m_texPumpkin);
    m_icons[3].setTexture(m_texMelon);
    m_icons[4].setTexture(m_texPineapple);
    m_icons[5].setTexture(m_texWheat);

    // فقط نسخه‌ی کاملاً رشد کرده‌ی هر گیاه را در تولبار نشان بده
    // این اعداد را اگر لازم بود با تعداد فریم واقعی هر تکسچر عوض کن
    m_icons[2].setTextureRect(lastFrameRectWithCount(m_texPumpkin,   6)); // pumpkin: 6 فریم
    m_icons[3].setTextureRect(lastFrameRectWithCount(m_texMelon,     6)); // blue melon: حدس 6
    m_icons[4].setTextureRect(lastFrameRectWithCount(m_texPineapple, 7)); // pineapple: 7 فریم
    m_icons[5].setTextureRect(lastFrameRectWithCount(m_texWheat,     6)); // wheat: حدس 6



    for (int i = 0; i < 6; ++i)
    {
        // اندازه‌ی واقعی فریم را از TextureRect می‌گیریم
        sf::IntRect rect = m_icons[i].getTextureRect();
        int w = rect.width;
        int h = rect.height;

        // اگر TextureRect تنظیم نشده باشد (مثل داس/آب‌پاش)،
        // از اندازه‌ی کل تکسچر استفاده می‌کنیم.
        if (w == 0 || h == 0)
        {
            auto texSize = m_icons[i].getTexture()->getSize();
            w = texSize.x;
            h = texSize.y;
        }

        float sx = (slotWidth  - 12.0f) / static_cast<float>(w);
        float sy = (slotHeight - 12.0f) / static_cast<float>(h);
        float s  = std::min(sx, sy);

        m_icons[i].setScale(s, s);

        m_icons[i].setOrigin(
            w / 2.0f,
            h / 2.0f
        );

        sf::Vector2f slotPos = m_slots[i].getPosition();
        m_icons[i].setPosition(
            slotPos.x + slotWidth  / 2.0f,
            slotPos.y + slotHeight / 2.0f
        );
    }



    updateAppearance();
}

void Toolbar::loadTextures()
{
    if (!m_texSickle.loadFromFile("assets/sprites/sickle.png"))
        throw std::runtime_error("Failed to load sickle.png");
    if (!m_texWaterEmpty.loadFromFile("assets/sprites/watering_can_empty.png"))
        throw std::runtime_error("Failed to load watering_can_empty.png");
    if (!m_texWaterFull.loadFromFile("assets/sprites/watering_can_full.png"))
        throw std::runtime_error("Failed to load watering_can_full.png");
    if (!m_texPumpkin.loadFromFile("assets/sprites/pumpkin.png"))
        throw std::runtime_error("Failed to load pumpkin.png");
    if (!m_texMelon.loadFromFile("assets/sprites/melon.png"))
        throw std::runtime_error("Failed to load melon.png");
    if (!m_texPineapple.loadFromFile("assets/sprites/pineapple.png"))
        throw std::runtime_error("Failed to load pineapple.png");
    if (!m_texWheat.loadFromFile("assets/sprites/wheat.png"))
        throw std::runtime_error("Failed to load wheat.png");

    m_texSickle.setSmooth(false);
    m_texWaterEmpty.setSmooth(false);
    m_texWaterFull.setSmooth(false);
    m_texPumpkin.setSmooth(false);
    m_texMelon.setSmooth(false);
    m_texPineapple.setSmooth(false);
    m_texWheat.setSmooth(false);
}

void Toolbar::handleNumberKey(int num)
{
    if (num < 0 || num > 5) return;
    m_selected = static_cast<SelectedItem>(num);
    updateAppearance();
}

void Toolbar::updateAppearance()
{
    for (int i = 0; i < 6; ++i)
    {
        if (static_cast<int>(m_selected) == i)
            m_slots[i].setOutlineColor(sf::Color::Yellow);
        else
            m_slots[i].setOutlineColor(sf::Color::Black);
    }
}

void Toolbar::draw(sf::RenderWindow& window) const
{
    window.draw(m_background);
    for (int i = 0; i < 6; ++i)
    {
        window.draw(m_slots[i]);
        window.draw(m_icons[i]);
    }
}

void Toolbar::updateWaterIcon(int currentWater, int capacity)
{
    if (capacity <= 0)
        return;

    // اگر خالی → آیکون خالی
    // اگر هر مقدار آب > 0 → آیکون پر
    if (currentWater <= 0)
    {
        m_icons[1].setTexture(m_texWaterEmpty);
    }
    else
    {
        m_icons[1].setTexture(m_texWaterFull);
    }
}
