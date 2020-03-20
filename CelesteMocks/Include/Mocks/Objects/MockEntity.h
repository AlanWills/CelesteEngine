#pragma once

#include "Objects/Entity.h"


namespace CelesteMocks
{
  class MockEntity : public Celeste::Entity
  {
    private:
      using Inherited = Celeste::Entity;

    public:
      MockEntity() :
        m_setActiveCalled(false),
        m_handleInputCalled(false),
        m_updateCalled(false)
        { }

      ~MockEntity() = default;

      bool isSetActiveCalled() const { return m_setActiveCalled; }
      bool isHandleInputCalled() const { return m_handleInputCalled; }
      bool isUpdateCalled() const { return m_updateCalled; }

      void reset()
      {
        m_setActiveCalled = false;
        m_handleInputCalled = false;
        m_updateCalled = false;
      }

      void setActive(bool isActive) { Inherited::setActive(isActive); m_setActiveCalled = isActive; }
      void handleInput() override { Inherited::handleInput(); m_handleInputCalled = true; }
      void update(float elapsedGameTime) override { Inherited::update(elapsedGameTime); m_updateCalled = true; }

    private:
      bool m_setActiveCalled;
      bool m_handleInputCalled;
      bool m_updateCalled;
  };
}