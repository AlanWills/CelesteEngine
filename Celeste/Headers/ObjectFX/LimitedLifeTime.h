#pragma once

#include "Objects/Component.h"
#include "Events/Event.h"


namespace Celeste
{
  class LimitedLifeTime : public Component
  {
    DECLARE_UNMANAGED_COMPONENT(LimitedLifeTime, CelesteDllExport)

    public:
      using DeathEvent = Event<GameObject&>;

      CelesteDllExport ~LimitedLifeTime() override;

      inline float getCurrentTimeAlive() const { return m_currentTimeAlive; }

      inline float getLifeTime() const { return m_lifeTime; }
      inline void setLifeTime(float lifeTime) { m_lifeTime = lifeTime; }

      inline int getTriggerKey() const { return m_triggerKey; }
      inline void setTriggerKey(int triggerKey) { m_triggerKey = triggerKey; }

      template <typename ...DeathCallbacks>
      void subscribeOnDeathCallback(const DeathEvent::Function& callback, const DeathCallbacks&... callbacks);
      inline void subscribeOnDeathCallback(const DeathEvent::Function& callback) { m_onDeathEvent.subscribe(callback); }

      inline const DeathEvent& getOnDeathEvent() const { return m_onDeathEvent; }

      CelesteDllExport void update(float elapsedGameTime) override;

    private:
      using Inherited = Component;

      void onKeyUp(int key);

      float m_currentTimeAlive = 0.0f;
      float m_lifeTime = 0.0f;
      int m_triggerKey = -1;
      StringId m_keyUpEvent = 0;

      DeathEvent m_onDeathEvent;
  };

  //------------------------------------------------------------------------------------------------
  template <typename ...DeathCallbacks>
  void LimitedLifeTime::subscribeOnDeathCallback(
    const Event<GameObject&>::Function& callback,
    const DeathCallbacks&... callbacks)
  {
    subscribeOnDeathCallback(callback);
    subscribeOnDeathCallback(callbacks...);
  }
}