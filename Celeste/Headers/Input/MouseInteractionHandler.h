#pragma once

#include "Objects/Script.h"
#include "Events/Event.h"

#include <array>


namespace Celeste
{
  class GameObject;

  namespace Physics
  {
    class Collider;
  }

  namespace Input
  {
    using GameObjectEvent = Event<GameObject&>;
    using GameObjectClickCallback = std::function<void(GameObject&)>;

    class MouseInteractionHandler : public Script
    {
      DECLARE_SCRIPT(MouseInteractionHandler, CelesteDllExport)

      public:
        /// An event that will be called when the mouse first enters the parent's collider
        inline const GameObjectEvent& getOnEnterEvent() const { return m_onEnter; }

        /// An event that will be called when the mouse first leaves the parent's collider
        inline const GameObjectEvent& getOnLeaveEvent() const { return m_onLeave; }

        /// An event that will be called the first time the parent's collider is left clicked using the mouse
        inline const GameObjectEvent& getOnLeftButtonDownEvent() const { return m_onLeftButtonDown; }

        /// An event that will be called the first time the mouse is released having been left pressed whilst the mouse was over this collider.
        /// The mouse does not have to be over the collider when it is released for this event to fire.
        inline const GameObjectEvent& getOnLeftButtonUpEvent() const { return m_onLeftButtonUp; }

        /// An event that will be called the first time the parent's collider is middle clicked using the mouse
        inline const GameObjectEvent& getOnMiddleButtonDownEvent() const { return m_onMiddleButtonDown; }

        /// An event that will be called the first time the mouse is released having been middle pressed whilst the mouse was over this collider.
        inline const GameObjectEvent& getOnMiddleButtonUpEvent() const { return m_onMiddleButtonUp; }

        /// An event that will be called the first time the parent's collider is right clicked using the mouse
        inline const GameObjectEvent& getOnRightButtonDownEvent() const { return m_onRightButtonDown; }

        /// An event that will be called the first time the mouse is released having been right pressed whilst the mouse was over this collider.
        inline const GameObjectEvent& getOnRightButtonUpEvent() const { return m_onRightButtonUp; }

        CelesteDllExport void handleInput() override;

      protected:
        CelesteDllExport void onSetGameObject(GameObject& gameObject) override;

        inline observer_ptr<Physics::Collider> getCollider() const { return m_collider; }
        inline bool isMouseOver() const { return m_isMouseOver; }

      private:
        using Inherited = Component;

        observer_ptr<Physics::Collider> m_collider;
        bool m_isMouseOver;
        std::array<bool, 3> m_mouseButtonPressed;

        GameObjectEvent m_onEnter;
        GameObjectEvent m_onLeave;
        GameObjectEvent m_onLeftButtonDown;
        GameObjectEvent m_onLeftButtonUp;
        GameObjectEvent m_onMiddleButtonDown;
        GameObjectEvent m_onMiddleButtonUp;
        GameObjectEvent m_onRightButtonDown;
        GameObjectEvent m_onRightButtonUp;
    };
  }
}