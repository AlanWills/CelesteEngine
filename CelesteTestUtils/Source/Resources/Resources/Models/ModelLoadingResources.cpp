#include "Resources/Resources/Models/ModelLoadingResources.h"
#include "Resources/ResourceManager.h"


namespace CelesteTestUtils
{
  REGISTER_TEST_RESOURCE_CLASS(ModelLoadingResources)

  //------------------------------------------------------------------------------------------------
  void ModelLoadingResources::addAllResourcesToLua()
  {
    sol::table resources = createLuaResourcesTable();
    ADD_RESOURCE_TO_LUA(resources, Box);
  }

  //------------------------------------------------------------------------------------------------
  void ModelLoadingResources::unloadAllResources()
  {
    UNLOAD_DATA(Box);
  }
}