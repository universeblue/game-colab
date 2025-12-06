// Plant.cpp
#include "Plant.hpp"
#include <stdexcept>
#include <algorithm>

namespace
{
    struct PlantTextures
    {
        bool loaded = false;

        sf::Texture pumpkinAlive;
        sf::Texture melonAlive;
        sf::Texture pineappleAlive;
        sf::Texture wheatAlive;

        sf::Texture deadDefault;   // تنها تکسچر حالت مرده

        void load()
        {
            if (loaded) return;
            loaded = true;

            // alive
            if (!pumpkinAlive.loadFromFile("assets/sprites/pumpkin.png"))
                throw std::runtime_error("Failed to load pumpkin.png");
            if (!melonAlive.loadFromFile("assets/sprites/melon.png"))
                throw std::runtime_error("Failed to load melon.png");
            if (!pineappleAlive.loadFromFile("assets/sprites/pineapple.png"))
                throw std::runtime_error("Failed to load pineapple.png");
            if (!wheatAlive.loadFromFile("assets/sprites/wheat.png"))
                throw std::runtime_error("Failed to load wheat.png");

            // DEAD مشترک برای همه
            if (!deadDefault.loadFromFile("assets/sprites/default_withered.png"))
                throw std::runtime_error("Failed to load default_withered.png");

            pumpkinAlive.setSmooth(false);
            melonAlive.setSmooth(false);
            pineappleAlive.setSmooth(false);
            wheatAlive.setSmooth(false);
            deadDefault.setSmooth(false);
        }

        const sf::Texture& aliveFor(const std::string& typeName) const
        {
            if (typeName == Setting::PLANT_PUMPKIN)    return pumpkinAlive;
            if (typeName == Setting::PLANT_BLUE_MELON) return melonAlive;
            if (typeName == Setting::PLANT_PINEAPPLE)  return pineappleAlive;
            if (typeName == Setting::PLANT_WHEAT)      return wheatAlive;
            return pumpkinAlive;
        }

        const sf::Texture& deadFor(const std::string& /*typeName*/) const
        {
            // یک تکسچر مشترک برای همه‌ی گیاه‌های مرده
            return deadDefault;
        }
    };

    PlantTextures g_plantTextures;



        // const sf::Texture& deadFor(const std::string& typeName) const
        // {
        //     if (typeName == Setting::PLANT_PUMPKIN)    return pumpkinDead;
        //     if (typeName == Setting::PLANT_BLUE_MELON) return melonDead;
        //     if (typeName == Setting::PLANT_PINEAPPLE)  return pineappleDead;
        //     if (typeName == Setting::PLANT_WHEAT)      return wheatDead;
        //     return pumpkinDead;
        // }
}



Plant::Plant(const std::string& typeName, const sf::Vector2f& centerPosition)
    : m_typeName(typeName)
    , m_cfg(Setting::PLANT_TYPES.at(typeName))
    , m_sprite()
    , m_currentStage(0)
    , m_waitingForWater(false)
    , m_dead(false)
    , m_readyFinal(false)
    , m_stageTimer(0.0f)
    , m_dryoutTimer(0.0f)
    , m_frameCount(1)
    , m_frameWidth(0)
    , m_frameHeight(0)
    , m_center(centerPosition)
{
    g_plantTextures.load();

    const sf::Texture& tex = g_plantTextures.aliveFor(m_typeName);
    m_sprite.setTexture(tex);

    // فرض: اسپریت‌شیت افقی است و فریم‌ها مربعی‌اند.
    auto texSize = tex.getSize();
    m_frameHeight = static_cast<int>(texSize.y);
    m_frameWidth  = m_frameHeight;
    if (m_frameWidth <= 0) m_frameWidth = static_cast<int>(texSize.x);

    m_frameCount = std::max(1, static_cast<int>(texSize.x / m_frameWidth));
    if (m_frameCount <= 0) m_frameCount = 1;

    // scale بر اساس یک فریم
    float sx = GameConfig::TILE_SIZE * 0.9f / static_cast<float>(m_frameWidth);
    float sy = GameConfig::TILE_SIZE * 0.9f / static_cast<float>(m_frameHeight);
    float s  = std::min(sx, sy);

    m_sprite.setScale(s, s);
    m_sprite.setOrigin(m_frameWidth / 2.0f, m_frameHeight / 2.0f);
    m_sprite.setPosition(m_center);

    updateAppearance();
}

void Plant::update(float dt)
{
    if (m_dead || m_readyFinal)
        return;

    if (!m_waitingForWater)
    {
        // در حال سپری کردن مرحله رشد
        m_stageTimer += dt;
        if (m_stageTimer >= m_cfg.stageTime)
        {
            m_stageTimer      = 0.0f;
            m_waitingForWater = true;
            m_dryoutTimer     = 0.0f;
            updateAppearance();
        }
    }
    else
    {
        // منتظر آبیاری
        m_dryoutTimer += dt;
        if (m_dryoutTimer >= m_cfg.dryoutTime)
        {
            m_dead            = true;
            m_waitingForWater = false;
            updateAppearance();
        }
    }
}

void Plant::water()
{
    if (m_dead || !m_waitingForWater)
        return;

    m_waitingForWater = false;
    m_dryoutTimer     = 0.0f;

    if (m_currentStage + 1 < m_cfg.growthStages)
    {
        ++m_currentStage;
        m_stageTimer = 0.0f;

        if (m_currentStage == m_cfg.growthStages - 1)
            m_readyFinal = true;

        updateAppearance();
    }
}

bool Plant::canBeHarvested() const
{
    return (m_readyFinal && !m_dead);
}

bool Plant::isDead() const
{
    return m_dead;
}

int Plant::harvest()
{
    int gain = 0;
    if (canBeHarvested())
        gain = m_cfg.profit;

    if (m_cfg.persistent && !m_dead)
    {
        // گیاه پایدار: بعد از برداشت در آخرین مرحله می‌ماند
        m_readyFinal      = false;
        m_waitingForWater = false;
        m_stageTimer      = 0.0f;
        m_dryoutTimer     = 0.0f;
        updateAppearance();
    }
    else
    {
        // گیاه ناپایدار یا خشک‌شده
        m_dead            = true;
        m_readyFinal      = false;
        m_waitingForWater = false;
        updateAppearance();
    }

    return gain;
}

void Plant::updateFrameByStage()
{
    if (m_frameCount <= 0)
        return;

    int maxStage = std::max(1, m_cfg.growthStages - 1);
    int maxFrame = std::max(1, m_frameCount    - 1);

    int frameIndex = (m_currentStage * maxFrame) / maxStage;
    frameIndex = std::max(0, std::min(frameIndex, m_frameCount - 1));

    sf::IntRect rect(
        frameIndex * m_frameWidth,
        0,
        m_frameWidth,
        m_frameHeight
    );
    m_sprite.setTextureRect(rect);
}

void Plant::updateAppearance()
{
    if (m_dead)
    {
        // ---------- حالت مرده: استفاده از اسپریت‌شیت مشترک default_whitered ----------
        const sf::Texture& texDead = g_plantTextures.deadFor(m_typeName);
        m_sprite.setTexture(texDead, true);

        auto sz = texDead.getSize();

        // فرض: فریم‌ها مربعی هستند (height = frameHeight)
        int deadFrameHeight = static_cast<int>(sz.y);
        int deadFrameWidth  = deadFrameHeight;
        int deadFrameCount  = std::max(1, static_cast<int>(sz.x / deadFrameWidth));

        // فعلاً از آخرین فریم اسپریت‌شیت خشک‌شده استفاده می‌کنیم
        int frameIndex = deadFrameCount - 1;
        if (frameIndex < 0) frameIndex = 0;

        sf::IntRect rect(
            frameIndex * deadFrameWidth,
            0,
            deadFrameWidth,
            deadFrameHeight
        );
        m_sprite.setTextureRect(rect);

        // اسکیل به اندازه‌ی یک خانه (TILE)
        float sx = GameConfig::TILE_SIZE * 0.9f / static_cast<float>(deadFrameWidth);
        float sy = GameConfig::TILE_SIZE * 0.9f / static_cast<float>(deadFrameHeight);
        float s  = std::min(sx, sy);

        m_sprite.setScale(s, s);
        m_sprite.setOrigin(deadFrameWidth / 2.0f, deadFrameHeight / 2.0f);
        m_sprite.setPosition(m_center);

        m_sprite.setColor(sf::Color::White);  // رنگ طبیعی تصویر خشک‌شده
    }
    else
    {
        // ---------- حالت زنده: اسپریت‌شیت اصلی گیاه ----------
        const sf::Texture& texAlive = g_plantTextures.aliveFor(m_typeName);
        m_sprite.setTexture(texAlive, true);

        // استفاده از فریم‌های alive بر اساس stage
        updateFrameByStage();

        sf::Color color = sf::Color::White;
        if (m_waitingForWater)
        {
            // تشنه: کمی شفاف / مایل به آبی
            color = sf::Color(200, 200, 255, 200);
        }

        m_sprite.setColor(color);
        m_sprite.setPosition(m_center);
    }
}


void Plant::draw(sf::RenderWindow& window) const
{
    window.draw(m_sprite);
}
