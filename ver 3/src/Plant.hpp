// Plant.hpp
#ifndef PLANT_HPP
#define PLANT_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include "settings.hpp"
#include "GameConfig.hpp"

class Plant
{
public:
    // typeName یکی از رشته‌های Setting::PLANT_PUMPKIN, PLANT_BLUE_MELON, ...
    Plant(const std::string& typeName, const sf::Vector2f& centerPosition);

    void update(float dt);                 // dt بر حسب ثانیه
    void draw(sf::RenderWindow& window) const;

    void water();                          // آبیاری در بازه dryoutTime
    bool canBeHarvested() const;           // آماده برداشت؟
    bool isDead() const;                   // خشک شده؟
    int  harvest();                        // مقدار سود؛ بعدش یا حذف می‌شود یا دوباره رشد می‌کند

    const std::string& getTypeName() const { return m_typeName; }

private:
    std::string                   m_typeName;
    const Setting::PlantConfig&   m_cfg;

    sf::Sprite   m_sprite;

    int   m_currentStage;        // از 0 تا growthStages-1
    bool  m_waitingForWater;     // در بازه dryoutTime
    bool  m_dead;
    bool  m_readyFinal;          // محصول نهایی آماده برداشت

    float m_stageTimer;          // زمان طی شده از شروع مرحله فعلی
    float m_dryoutTimer;         // زمان طی شده از شروع dryout

    // اطلاعات اسپریت‌شیت (فقط برای تکسچر زنده)
    int   m_frameCount;
    int   m_frameWidth;
    int   m_frameHeight;

    sf::Vector2f m_center;       // مرکز کاشی

    void updateAppearance();     // رنگ + فریم را بر اساس وضعیت تنظیم می‌کند
    void updateFrameByStage();   // stage -> frameIndex روی اسپریت‌شیت alive
};

#endif // PLANT_HPP
