#include "GraphicalFX/OpacityLerper.h"
#include "UtilityHeaders/ComponentHeaders.h"
#include "Rendering/Renderer.h"
#include "Time/TimeUtils.h"


namespace Celeste
{
  using namespace Rendering;

  REGISTER_COMPONENT(OpacityLerper, 10)

  //------------------------------------------------------------------------------------------------
  OpacityLerper::OpacityLerper(GameObject& gameObject) :
    Inherited(gameObject),
    m_minOpacity(0),
    m_maxOpacity(1),
    m_lerpUpTime(1),
    m_lerpDownTime(1),
    m_maxOpacityWaitTime(0),
    m_minOpacityWaitTime(0),
    m_waitTime(0),
    m_lerpingUp(false)
  {
  }

  //------------------------------------------------------------------------------------------------
  void OpacityLerper::update()
  {
    Inherited::update();

#if _DEBUG
    if (!getGameObject().hasComponent<Renderer>())
    {
      ASSERT_FAIL();
      return;
    }
#endif

    observer_ptr<Renderer> renderer = getGameObject().findComponent<Renderer>();
    float currentOpacity = renderer->getOpacity();
    float elapsedGameTime = Time::getElapsedDeltaTime();

    if (m_lerpingUp)
    {
      currentOpacity = m_lerpUpTime == 0 ? m_maxOpacity : currentOpacity + (elapsedGameTime / m_lerpUpTime);

      if (currentOpacity >= m_maxOpacity)
      {
        currentOpacity = m_maxOpacity;

        if (m_waitTime >= m_maxOpacityWaitTime)
        {
          m_waitTime = 0;
          m_lerpingUp = false;
        }
        else
        {
          m_waitTime += elapsedGameTime;
        }
      }
    }
    else if (!m_lerpingUp)
    {
      currentOpacity = m_lerpDownTime == 0 ? m_minOpacity : currentOpacity - (elapsedGameTime / m_lerpDownTime);

      if (currentOpacity <= m_minOpacity)
      {
        currentOpacity = m_minOpacity;

        if (m_waitTime >= m_minOpacityWaitTime)
        {
          m_waitTime = 0;
          m_lerpingUp = true;
        }
        else
        {
          m_waitTime += elapsedGameTime;
        }
      }
    }
    
    renderer->setOpacity(currentOpacity);
  }
}