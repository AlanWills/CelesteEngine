#include "Animation/Animators/ChangeScaleAnimator.h"
#include "UtilityHeaders/ComponentHeaders.h"
#include "Time/TimeUtils.h"


namespace Celeste::Animators
{
  REGISTER_COMPONENT(ChangeScaleAnimator, 5)

  //------------------------------------------------------------------------------------------------
  ChangeScaleAnimator::ChangeScaleAnimator(GameObject& gameObject) :
    Inherited(gameObject),
    m_time(0),
    m_elapsedTime(0),
    m_startingScale(1),
    m_targetScale(1)
  {
  }

  //------------------------------------------------------------------------------------------------
  void ChangeScaleAnimator::update()
  {
    Inherited::update();

    // Has to be multiplicative to do lerping scale?
    // What does speed correspond to?
    // Maybe something more intuitive, like time?
    // Add time to move to position animator too
    // Should move animator die too?

    m_elapsedTime += Time::getElapsedDeltaTime();
    glm::vec3 newScale = (m_time == 0 || m_elapsedTime >= m_time) ? m_targetScale : (m_startingScale + ((m_targetScale - m_startingScale) * m_elapsedTime / m_time));
    getGameObject().getTransform()->setScale(newScale);

    if (m_elapsedTime >= m_time)
    {
      // Animation complete
      setActive(false);
    }
  }

  //------------------------------------------------------------------------------------------------
  void ChangeScaleAnimator::setTargetScale(const glm::vec3& targetScale)
  {
    m_targetScale = targetScale;
    reset();
  }

  //------------------------------------------------------------------------------------------------
  void ChangeScaleAnimator::reset()
  {
    m_startingScale = getGameObject().getTransform()->getScale();
    m_elapsedTime = 0;
  }
}