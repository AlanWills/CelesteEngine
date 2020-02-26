#include "UtilityHeaders/UnitTestHeaders.h"

#include "Lua/ScriptCommands/Input/InputScriptCommands.h"
#include "Lua/LuaState.h"

#include "Objects/Component.h"

using LuaState = Celeste::Lua::LuaState;


namespace TestCeleste
{
  namespace Lua
  {
    namespace Input
    {
      CELESTE_TEST_CLASS(TestInputScriptCommands)
      
      //------------------------------------------------------------------------------------------------
      void TestInputScriptCommands::testInitialize()
      {
        LuaState::instance().new_usertype<Celeste::Component>("Component");
      }

      //------------------------------------------------------------------------------------------------
      void TestInputScriptCommands::testCleanup()
      {
        LuaState::instance().new_usertype<Celeste::Component>("Component");
      }

#pragma region Initialize Tests

      //------------------------------------------------------------------------------------------------
      TEST_METHOD(InputScriptCommands_Initialize_InitializesKeyboardActivatorScriptCommands)
      {
        Assert::IsFalse(LuaState::instance().globals()["KeyboardActivator"].valid());

        Celeste::Lua::Input::ScriptCommands::initialize();

        Assert::IsTrue(LuaState::instance().globals()["KeyboardActivator"].valid());
      }

      //------------------------------------------------------------------------------------------------
      TEST_METHOD(InputScriptCommands_Initialize_InitializesKeyboardVisibilityScriptCommands)
      {
        Assert::IsFalse(LuaState::instance().globals()["KeyboardVisibility"].valid());

        Celeste::Lua::Input::ScriptCommands::initialize();

        Assert::IsTrue(LuaState::instance().globals()["KeyboardVisibility"].valid());
      }

      //------------------------------------------------------------------------------------------------
      TEST_METHOD(InputScriptCommands_Initialize_InitializesMouseInteractionHandlerScriptCommands)
      {
        Assert::IsFalse(LuaState::instance().globals()["MouseInteractionHandler"].valid());

        Celeste::Lua::Input::ScriptCommands::initialize();

        Assert::IsTrue(LuaState::instance().globals()["MouseInteractionHandler"].valid());
      }

#pragma endregion

      };
    }
  }
}