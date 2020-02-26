#pragma once

#include "CelesteTestUtilsDllExport.h"
#include "Loading/LoadResourcesAsyncScript.h"


namespace CelesteTestUtils
{
  class MockLoadResourcesAsyncScript : public Celeste::LoadResourcesAsyncScript
  {
    DECLARE_SCRIPT(MockLoadResourcesAsyncScript, CelesteTestUtilsDllExport)
  };
}