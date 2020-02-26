#include "Resources/ObjectFX/LimitedLifeTimeLoadingResources.h"
#include "Resources/ResourceManager.h"


namespace CelesteTestUtils
{
  REGISTER_TEST_RESOURCE_CLASS(LimitedLifeTimeLoadingResources)

  //------------------------------------------------------------------------------------------------
  void LimitedLifeTimeLoadingResources::addAllResourcesToLua()
  {
    sol::table resources = createLuaResourcesTable();
    ADD_RESOURCE_TO_LUA(resources, ValidWithCallbacks);
    ADD_RESOURCE_TO_LUA(resources, ValidNoCallbacks);
    ADD_RESOURCE_TO_LUA(resources, Invalid);
  }

  //------------------------------------------------------------------------------------------------
  void LimitedLifeTimeLoadingResources::unloadAllResources()
  {
    UNLOAD_DATA(ValidWithCallbacks);
    UNLOAD_DATA(ValidNoCallbacks);
    UNLOAD_DATA(Invalid);
  }
}