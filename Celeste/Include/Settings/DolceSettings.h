#pragma once

#ifdef _DEBUG

#include "CelesteDllExport.h"
#include "Objects/ScriptableObject.h"


namespace Dolce
{
  class IDolce;
}

namespace Celeste::Settings
{
  class DolceSettings : public ScriptableObject
  {
    DECLARE_SCRIPTABLE_OBJECT(DolceSettings, CelesteDllExport)

    public:
      bool getEnabled() const { return m_enabled.getValue(); }
      const std::vector<std::string>& getOpenWindows() const { return m_openWindows; }

      CelesteDllExport void applyTo(Dolce::IDolce& dolce) const;
      CelesteDllExport void applyFrom(Dolce::IDolce& dolce);

      static const char* const ENABLED_ATTRIBUTE_NAME;
      static const char* const OPEN_WINDOWS_ELEMENT_NAME;
      static const char* const OPEN_WINDOW_ELEMENT_NAME;
      static const Path SETTINGS_RELATIVE_PATH;

    protected:
      bool doDeserialize(const tinyxml2::XMLElement* element) override;
      void doSerialize(tinyxml2::XMLElement* element) const override;

    private:
      using Inherited = ScriptableObject;

      ValueField<bool>& m_enabled;
      std::vector<std::string> m_openWindows;
  };
}

#endif