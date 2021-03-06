#pragma once

#include "CelesteDllExport.h"
#include "FileSystem/Path.h"
#include "tinyxml2.h"
#include "Objects/ScriptableObject.h"
#include "Reflection/Type.h"

#include <functional>
#include <unordered_map>


namespace Celeste
{
  class CelesteDllExport ScriptableObjectRegistry
  {
    public:
      using CreateFactoryFunction = std::function<std::unique_ptr<ScriptableObject>(const std::string&)>;
      using LoadFactoryFunction = std::function<std::unique_ptr<ScriptableObject>(const Path&)>;

    private:
      using InstantiationMapKey = std::string;
      using InstantiationMapValue = std::tuple<CreateFactoryFunction, LoadFactoryFunction>;
      using InstantiationMap = std::unordered_map<InstantiationMapKey, InstantiationMapValue>;
      using InstantiationMapPair = std::pair<InstantiationMapKey, InstantiationMapValue>;

    public:
      ScriptableObjectRegistry() = delete;
      ScriptableObjectRegistry(const ScriptableObjectRegistry&) = delete;
      ScriptableObjectRegistry& operator=(const ScriptableObjectRegistry&) = delete;

      template <typename T>
      static bool addScriptableObject();

      template <typename T>
      static void removeScriptableObject() { removeScriptableObject(T::type_name()); }

      template <typename T>
      static bool hasScriptableObject() { return hasScriptableObject(T::type_name()); }

      template <typename T>
      static std::unique_ptr<T> createScriptableObject(const std::string& name)
      {
        std::unique_ptr<ScriptableObject> scriptableObject = createScriptableObject(T::type_name(), name);
        return std::move(std::unique_ptr<T>(static_cast<T*>(scriptableObject.release())));
      }

      template <typename T>
      static std::unique_ptr<T> loadScriptableObject(const Path& path)
      { 
        std::unique_ptr<ScriptableObject> scriptableObject = loadScriptableObject(path);
        return std::move(std::unique_ptr<T>(static_cast<T*>(scriptableObject.release())));
      }

      static std::unique_ptr<ScriptableObject> loadScriptableObject(const Path& path);

      static void removeScriptableObject(const std::string& objectName)
      {
        if (!hasScriptableObject(objectName))
        {
          return;
        }

        getInstantiationMap().erase(objectName);
      }

      static bool hasScriptableObject(const std::string& objectName) 
      { 
        return !getInstantiationMap().empty() && getInstantiationMap().find(objectName) != getInstantiationMap().end();
      }

      static std::unique_ptr<ScriptableObject> createScriptableObject(const std::string& typeName, const std::string& name)
      {
        if (hasScriptableObject(typeName))
        {
          return std::move(std::get<0>(getInstantiationMap()[typeName])(name));
        }

        return std::unique_ptr<ScriptableObject>();
      }

    private:
      static InstantiationMap& getInstantiationMap();
  };

  //------------------------------------------------------------------------------------------------
  template <typename T>
  bool ScriptableObjectRegistry::addScriptableObject()
  {
    const Reflection::Type<T>& typeInfo = Reflection::Type<T>();

    if (!hasScriptableObject(typeInfo.getName()))
    {
      getInstantiationMap().emplace(typeInfo.getName(), std::make_pair(
        &ScriptableObject::create<T>,
        &ScriptableObject::load<T>));
      return true;
    }

    ASSERT_FAIL();
    return false;
  }
}