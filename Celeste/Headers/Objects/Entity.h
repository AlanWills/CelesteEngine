#pragma once

#include "CelesteDllExport.h"
#include "Object.h"
#include "Debug/Assert.h"


namespace Celeste
{
  class Entity : public Object
  {
    public:
      CelesteDllExport Entity();
      virtual ~Entity() = default;

      virtual void handleInput() {}
      virtual void update(float /*elapsedGameTime*/) {}

      inline bool isActive() const { return m_active; }
      virtual void setActive(bool isActive) { m_active = isActive; }

    private:
      bool m_active = false;
  };
}