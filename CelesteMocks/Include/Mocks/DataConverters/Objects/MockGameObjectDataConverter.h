#pragma once

#include "DataConverters/Objects/GameObjectDataConverter.h"


namespace CelesteMocks
{
  class MockGameObjectDataConverter : public Celeste::GameObjectDataConverter
  {
    public:
      MockGameObjectDataConverter(const std::string& elementName) : Celeste::GameObjectDataConverter(elementName) { }
  };
}