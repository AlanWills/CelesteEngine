#include "UtilityHeaders/UnitTestHeaders.h"

#include "Lua/ScriptCommands/UI/UIScriptCommands.h"
#include "Lua/LuaState.h"

#include "UI/StackPanel.h"
#include "UI/ProgressBar.h"
#include "UI/Slider.h"

using LuaState = Celeste::Lua::LuaState;


namespace TestCeleste::Lua::UI
{
  CELESTE_TEST_CLASS(TestUIScriptCommands)

  //------------------------------------------------------------------------------------------------
  void TestUIScriptCommands::testInitialize()
  {
    LuaState::instance().new_usertype<Celeste::Component>("Component");
  }

  //------------------------------------------------------------------------------------------------
  void TestUIScriptCommands::testCleanup()
  {
    LuaState::instance().new_usertype<Celeste::Component>("Component");
  }

#pragma region Initialize Tests

  //------------------------------------------------------------------------------------------------
  TEST_METHOD(UIScriptCommands_Initialize_InitializesStackPanelScriptCommands)
  {
    Assert::IsFalse(LuaState::instance().globals()[Celeste::UI::StackPanel::type_name()].valid());

    Celeste::Lua::UI::ScriptCommands::initialize();

    Assert::IsTrue(LuaState::instance().globals()[Celeste::UI::StackPanel::type_name()].valid());
  }

  //------------------------------------------------------------------------------------------------
  TEST_METHOD(UIScriptCommands_Initialize_InitializesProgressBarScriptCommands)
  {
    Assert::IsFalse(LuaState::instance().globals()[Celeste::UI::ProgressBar::type_name()].valid());

    Celeste::Lua::UI::ScriptCommands::initialize();

    Assert::IsTrue(LuaState::instance().globals()[Celeste::UI::ProgressBar::type_name()].valid());
  }

  //------------------------------------------------------------------------------------------------
  TEST_METHOD(UIScriptCommands_Initialize_InitializesSliderScriptCommands)
  {
    Assert::IsFalse(LuaState::instance().globals()[Celeste::UI::Slider::type_name()].valid());

    Celeste::Lua::UI::ScriptCommands::initialize();

    Assert::IsTrue(LuaState::instance().globals()[Celeste::UI::Slider::type_name()].valid());
  }

#pragma endregion

  };
}