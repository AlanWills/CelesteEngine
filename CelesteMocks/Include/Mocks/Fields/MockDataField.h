#pragma once

#include "Fields/DataField.h"


namespace CelesteMocks
{
  template <typename T, bool is_reference = false>
  class MockDataField : public Celeste::DataField<T, is_reference>
  {
    public:
      MockDataField(const std::string& fieldName, typename Celeste::DataField<T, is_reference>::field_type defaultValue = T()) :
        Celeste::DataField<T, is_reference>(fieldName, defaultValue)
      {
      }
  };
}