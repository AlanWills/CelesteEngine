#include "Physics/RigidBody2D.h"
#include "UtilityHeaders/ComponentHeaders.h"


namespace Celeste::Physics
{
  REGISTER_COMPONENT(RigidBody2D, 50)

    //------------------------------------------------------------------------------------------------
    RigidBody2D::RigidBody2D() :
    m_linearVelocity(0),
    m_minLinearVelocity(-FLT_MAX, -FLT_MAX),
    m_maxLinearVelocity(FLT_MAX, FLT_MAX),
    m_angularVelocity(0),
    m_minAngularVelocity(-FLT_MAX),
    m_maxAngularVelocity(FLT_MAX)
  {
  }

  //------------------------------------------------------------------------------------------------
  void RigidBody2D::onUpdate(float secondsPerUpdate)
  {
    Inherited::onUpdate(secondsPerUpdate);

#if _DEBUG
    if (getTransform() == nullptr)
    {
      ASSERT_FAIL();
      return;
    }
#endif

    if (m_linearVelocity != glm::zero<glm::vec2>())
    {
      getTransform()->translate(m_linearVelocity * secondsPerUpdate);
    }

    if (m_angularVelocity != 0)
    {
      getTransform()->rotate(m_angularVelocity * secondsPerUpdate);
    }
  }

  //------------------------------------------------------------------------------------------------
  void RigidBody2D::onDeath()
  {
    Inherited::onDeath();

    m_linearVelocity = glm::vec2();
    m_minLinearVelocity = glm::vec2(-FLT_MAX, -FLT_MAX);
    m_maxLinearVelocity = glm::vec2(FLT_MAX, FLT_MAX);
    m_angularVelocity = 0;
    m_minAngularVelocity = -FLT_MAX;
    m_maxAngularVelocity = FLT_MAX;
  }
}