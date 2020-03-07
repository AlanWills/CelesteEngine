#include "UtilityHeaders/UnitTestHeaders.h"

#include "Events/EventTriggerer.h"
#include "Registries/ComponentRegistry.h"
#include "Registries/ComponentRegistry.h"
#include "Objects/GameObject.h"
#include "AssertCel.h"

using namespace Celeste;


namespace TestCeleste
{
  CELESTE_TEST_CLASS(TestEventTriggerer)

#pragma region Registration Tests

  //------------------------------------------------------------------------------------------------
  TEST_METHOD(EventTriggerer_IsRegisteredWithComponentRegistry)
  {
    Assert::IsTrue(ComponentRegistry::hasComponent<EventTriggerer>());
  }

#pragma endregion

#pragma region Constructor Tests

    //------------------------------------------------------------------------------------------------
    TEST_METHOD(EventTriggerer_Constructor_SetsValuesToDefault)
    {
      EventTriggerer eventTriggerer;

      Assert::IsTrue(EventTriggerer::TriggerMode::kOnce == eventTriggerer.getTriggerMode());
    }

#pragma endregion

#pragma region Update Tests

    //------------------------------------------------------------------------------------------------
    TEST_METHOD(EventTriggerer_Update_WithNoConditionSet_DoesNothing)
    {
      EventTriggerer eventTriggerer;

      bool called = false;
      auto f = [&called](const GameObject&) -> void { called = true; };
      eventTriggerer.getEvent().subscribe(f);
      eventTriggerer.update(0);

      Assert::IsFalse(called);
    }

    //------------------------------------------------------------------------------------------------
    TEST_METHOD(EventTriggerer_Update_ConditionFalse_DoesNotTriggerEvent)
    {
      EventTriggerer eventTriggerer;

      bool called = false;
      auto f = [&called](const GameObject&) -> void { called = true; };
      eventTriggerer.setCondition([](const GameObject&) -> bool { return false; });
      eventTriggerer.getEvent().subscribe(f);
      eventTriggerer.update(0);

      Assert::IsFalse(called);
    }

    //------------------------------------------------------------------------------------------------
    TEST_METHOD(EventTriggerer_Update_ConditionTrue_TriggersEvent)
    {
      EventTriggerer eventTriggerer;

      bool called = false;
      auto f = [&called](const GameObject&) -> void { called = true; };
      eventTriggerer.setCondition([](const GameObject&) -> bool { return true; });
      eventTriggerer.getEvent().subscribe(f);
      eventTriggerer.update(0);

      Assert::IsTrue(called);
    }

    //------------------------------------------------------------------------------------------------
    TEST_METHOD(EventTriggerer_Update_ConditionTrue_TriggerModeOnce_KillsComponent)
    {
      GameObject gameObject;
      AutoDeallocator<EventTriggerer> eventTriggerer = gameObject.addComponent<EventTriggerer>();

      Assert::IsTrue(eventTriggerer->getTriggerMode() == EventTriggerer::TriggerMode::kOnce);
      AssertCel::HasComponent<EventTriggerer>(gameObject);

      eventTriggerer->setCondition([](const GameObject&) -> bool { return true; });
      eventTriggerer->update(0);

      AssertCel::DoesNotHaveComponent<EventTriggerer>(gameObject);
    }

    //------------------------------------------------------------------------------------------------
    TEST_METHOD(EventTriggerer_Update_ConditionTrue_TriggerModeContinuous_DoesNotKillComponent)
    {
      GameObject gameObject;
      AutoDeallocator<EventTriggerer> eventTriggerer = gameObject.addComponent<EventTriggerer>();
      eventTriggerer->setTriggerMode(EventTriggerer::TriggerMode::kUnlimited);

      Assert::IsTrue(eventTriggerer->getTriggerMode() == EventTriggerer::TriggerMode::kUnlimited);

      bool called = false;
      auto f = [&called](const GameObject&) -> void { called = true; };
      eventTriggerer->setCondition([](const GameObject&) -> bool { return true; });
      eventTriggerer->getEvent().subscribe(f);
      eventTriggerer->update(0);

      AssertCel::HasComponent<EventTriggerer>(gameObject);
      Assert::IsTrue(called);

      called = false;
      eventTriggerer->update(0);

      AssertCel::HasComponent<EventTriggerer>(gameObject);
      Assert::IsTrue(called);
    }

#pragma endregion

  };
}