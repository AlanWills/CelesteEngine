#pragma once

#include "CelesteDllExport.h"
#include "System/ISystem.h"
#include "Events/Event.h"


namespace Celeste::Time
{
  class TimeNotifierSystem : public System::ISystem
  {
    private:
      using TimeNotifierEvent = Event<float>;

    public:
      CelesteDllExport void update(float elapsedGameTime) override;

      CelesteDllExport StringId subscribe(TimeNotifierEvent::Function&& callback);
      CelesteDllExport void unsubscribe(StringId eventHandle);

    private:
      TimeNotifierEvent m_timeNotifierEvent;
  };
}