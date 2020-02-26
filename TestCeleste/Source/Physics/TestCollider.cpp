#include "UtilityHeaders/UnitTestHeaders.h"

#include "Mocks/Physics/MockCollider.h"
#include "Objects/GameObject.h"
#include "AssertCel.h"
#include "Utils/ObjectUtils.h"

using namespace Celeste;


namespace TestCeleste
{

  CELESTE_TEST_CLASS(TestCollider)

#pragma region Constructor Tests

    //------------------------------------------------------------------------------------------------
    TEST_METHOD(Collider_DefaultConstructor_SetsTransformToNull)
    {
      MockCollider collider;

      Assert::IsNull(collider.getTransform());
    }

    //------------------------------------------------------------------------------------------------
    TEST_METHOD(Collider_DefaultConstructor_SetsIsColliderTypeTo_kCollider)
    {
      MockCollider collider;

      Assert::IsTrue(collider.getColliderType() == Physics::ColliderType::kCollider);
    }

    //------------------------------------------------------------------------------------------------
    TEST_METHOD(Collider_DefaultConstructor_SetsIsHitByRayToFalse)
    {
      MockCollider collider;

      Assert::IsFalse(collider.isHitByRay());
    }

    //------------------------------------------------------------------------------------------------
    TEST_METHOD(Collider_DefaultConstructor_SetsOffsetToZero)
    {
      MockCollider collider;

      Assert::AreEqual(glm::vec2(), collider.getOffset());
    }

#pragma endregion

#pragma region Set Game Object Tests

    //------------------------------------------------------------------------------------------------
    TEST_METHOD(Collider_SetGameObject_WithParent_SetsTransformToParentsTransform)
    {
      GAMEOBJECT(gameObject);
      AutoDeallocator<MockCollider> collider = gameObject.addComponent<MockCollider>();

      Assert::IsNotNull(collider->getTransform());
      Assert::IsTrue(collider->getTransform() == gameObject.getTransform());
    }

#pragma endregion

#pragma region Set Offset Tests

    //------------------------------------------------------------------------------------------------
    TEST_METHOD(Collider_SetOffset_CallsOnSetOffset)
    {
      MockCollider collider;
      collider.setOffset(100, 200);

      Assert::IsTrue(collider.isOnSetOffsetCalled());

      collider.reset();

      Assert::IsFalse(collider.isOnSetOffsetCalled());

      collider.setOffset(glm::vec2(200, 100));

      Assert::IsTrue(collider.isOnSetOffsetCalled());
    }

#pragma endregion

#pragma region Die Tests

    //------------------------------------------------------------------------------------------------
    TEST_METHOD(Collider_Die_SetsColliderTypeTo_kCollider)
    {
      MockCollider collider;
      collider.setColliderType(Physics::ColliderType::kTrigger);

      Assert::IsTrue(collider.getColliderType() == Physics::ColliderType::kTrigger);

      collider.die();

      Assert::IsTrue(collider.getColliderType() == Physics::ColliderType::kCollider);
    }

    //------------------------------------------------------------------------------------------------
    TEST_METHOD(Collider_Die_SetsIsHitByRayToFalse)
    {
      MockCollider collider;
      collider.setHitByRay(true);

      Assert::IsTrue(collider.isHitByRay());

      collider.die();

      Assert::IsFalse(collider.isHitByRay());
    }

    //------------------------------------------------------------------------------------------------
    TEST_METHOD(Collider_Die_SetsOffsetToZero)
    {
      MockCollider collider;
      collider.setOffset(100, 200);

      Assert::AreEqual(glm::vec2(100, 200), collider.getOffset());

      collider.die();

      Assert::AreEqual(glm::vec2(), collider.getOffset());
    }

#pragma endregion
  };
}