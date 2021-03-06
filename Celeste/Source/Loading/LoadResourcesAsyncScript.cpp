#include "Loading/LoadResourcesAsyncScript.h"
#include "UtilityHeaders/ComponentHeaders.h"


namespace Celeste
{
  REGISTER_COMPONENT(LoadResourcesAsyncScript, 1)

  //------------------------------------------------------------------------------------------------
  LoadResourcesAsyncScript::LoadResourcesAsyncScript(GameObject& gameObject) :
    Inherited(gameObject)
  {
  }

  //------------------------------------------------------------------------------------------------
  void LoadResourcesAsyncScript::update()
  {
    Inherited::update();

    m_loadComplete.invoke();
    m_loadComplete.unsubscribeAll();

    setActive(false);
  }
}