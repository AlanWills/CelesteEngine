#pragma once

#include "SiblingXMLElementIterator.h"

#include <algorithm>


namespace Celeste::XML
{
  //------------------------------------------------------------------------------------------------
  template <bool is_const>
  class ChildXMLElementWalker
  {
    private:
      using iterator = SiblingXMLElementIterator<is_const>;

    public:
      ChildXMLElementWalker(typename iterator::pointer element, const std::string& name = "") :
        m_element(element),
        m_name(name)
      {
      }

      ~ChildXMLElementWalker() { }

      iterator begin() const { return iterator(m_element ? m_element->FirstChildElement(m_name.empty() ? nullptr : m_name.c_str()) : nullptr, m_name); }
      iterator end() const { return iterator(nullptr); }

    private:
      typename iterator::pointer m_element;
      std::string m_name;
  };

  //------------------------------------------------------------------------------------------------
  typedef ChildXMLElementWalker<true> const_walker;
  typedef ChildXMLElementWalker<false> walker;

  //------------------------------------------------------------------------------------------------
  inline size_t child_count(const tinyxml2::XMLElement* element, const std::string& name = "") 
  { 
    auto walker = const_walker(element, name);
    auto b = walker.begin();
    auto e = walker.end();

    size_t count = 0;
    while (b != e)
    {
      ++count;
      ++b;
    }

    return count;
  }

  //------------------------------------------------------------------------------------------------
  inline const_walker children(const tinyxml2::XMLElement* element, const std::string& name = "") { return const_walker(element, name); }

  //------------------------------------------------------------------------------------------------
  inline walker children(tinyxml2::XMLElement* element, const std::string& name = "") { return walker(element, name); }
}