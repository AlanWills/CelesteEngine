#pragma once

#include "CelesteTestUtilsDllExport.h"
#include "Objects/Component.h"


namespace CelesteTestUtils
{
  class MockComponent : public Celeste::Component
  {
    DECLARE_UNMANAGED_COMPONENT(MockComponent, CelesteTestUtilsDllExport)

    public:
      bool handleInputCalled() const { return m_handleInputCalled; }
      bool updateCalled() const { return m_updateCalled; }

      void reset()
      {
        m_handleInputCalled = false;
        m_updateCalled = false;
      }

      void handleInput() override { Component::handleInput(); m_handleInputCalled = true; }
      void update(float elapsedGameTime) override { Component::update(elapsedGameTime); m_updateCalled = true; }

      static Celeste::EntityAllocator<MockComponent>& getAllocator() { return m_allocator; }

    private:
      using Inherited = Celeste::Component;

      bool m_handleInputCalled;
      bool m_updateCalled;
  };
}