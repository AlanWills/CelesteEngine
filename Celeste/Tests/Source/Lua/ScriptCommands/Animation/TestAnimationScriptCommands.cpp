#include "UtilityHeaders/UnitTestHeaders.h"

#include "Lua/ScriptCommands/Animation/AnimationScriptCommands.h"
#include "Lua/LuaState.h"

#include "Animation/Animator.h"

using LuaState = Celeste::Lua::LuaState;
using namespace Celeste;


namespace TestCeleste::Lua::ScriptCommands
{
  CELESTE_TEST_CLASS(TestAnimationScriptCommands)

#pragma region Initialize Tests

  //------------------------------------------------------------------------------------------------
  TEST_METHOD(AnimationScriptCommands_Initialize_Adds_AnimatorUserType_ToGlobalTable)
  {
    sol::state& state = LuaState::instance();

    Assert::IsFalse(state.globals()[Animation::Animator::type_name()].valid());

    Celeste::Lua::Animation::ScriptCommands::initialize();

    Assert::IsTrue(state.globals()[Animation::Animator::type_name()].valid());
  }

#pragma endregion

  };
}