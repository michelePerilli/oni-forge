#pragma once

#include <pugixml.hpp>
#include <functional>
#include <string>
#include <vector>

/**
 * @brief Self-describing XML field mapping.
 *
 * Each field knows how to read itself from an XML node
 * and how to write itself to an XML node.
 * The xmlName is kept for documentation purposes only.
 */
template<typename T>
struct XmlField {
    const char* xmlName;
    std::function<void(pugi::xml_node&, const T&)> write;
    std::function<void(const pugi::xml_node&, T&)> read;
};

// ---------------------------------------------------------------------------
// Macros for all field patterns
// ---------------------------------------------------------------------------

// Simple string element: <Tag>value</Tag>
#define STR_FIELD(Tag, Member) \
    { Tag, \
      [](pugi::xml_node& n, const auto& o) { \
          n.append_child(Tag).text().set(o.Member.c_str()); \
      }, \
      [](const pugi::xml_node& n, auto& o) { \
          if (const auto c = n.child(Tag)) o.Member = c.text().as_string(); \
      } \
    }

// String attribute: <Node attr="value">
#define ATTR_FIELD(Attr, Member) \
    { Attr, \
      [](pugi::xml_node& n, const auto& o) { \
          n.append_attribute(Attr) = o.Member.c_str(); \
      }, \
      [](const pugi::xml_node& n, auto& o) { \
          o.Member = n.attribute(Attr).as_string(); \
      } \
    }

// Float vector: <Tag><Float>v</Float>...</Tag>
#define FLOAT_VEC_FIELD(Tag, Member) \
    { Tag, \
      [](pugi::xml_node& n, const auto& o) { \
          auto vec = n.append_child(Tag); \
          for (const auto& f : o.Member) \
              vec.append_child("Float").text().set(f.c_str()); \
      }, \
      [](const pugi::xml_node& n, auto& o) { \
          for (const auto& f : n.child(Tag).children("Float")) \
              o.Member.push_back(f.text().as_string()); \
      } \
    }

// Link list: <Tag><Link>v</Link>...</Tag>
#define LINK_LIST_FIELD(Tag, Member) \
    { Tag, \
      [](pugi::xml_node& n, const auto& o) { \
          auto list = n.append_child(Tag); \
          for (const auto& l : o.Member) \
              list.append_child("Link").text().set(l.c_str()); \
      }, \
      [](const pugi::xml_node& n, auto& o) { \
          for (const auto& l : n.child(Tag).children("Link")) \
              o.Member.push_back(l.text().as_string()); \
      } \
    }

// Sub-struct: <Tag><field1>v1</field1>...</Tag>
#define SUB_STRUCT_FIELD(Tag, Member, FieldsArray) \
    { Tag, \
      [](pugi::xml_node& n, const auto& o) { \
          auto s = n.append_child(Tag); \
          for (const auto& f : FieldsArray) f.write(s, o.Member); \
      }, \
      [](const pugi::xml_node& n, auto& o) { \
          const auto s = n.child(Tag); \
          for (const auto& f : FieldsArray) f.read(s, o.Member); \
      } \
    }

// List of sub-structs: <Tag><ItemTag>...</ItemTag></Tag>
#define STRUCT_LIST_FIELD(Tag, ItemTag, Member, ItemType, FieldsArray) \
    { Tag, \
      [](pugi::xml_node& n, const auto& o) { \
          auto list = n.append_child(Tag); \
          for (const auto& item : o.Member) { \
              auto s = list.append_child(ItemTag); \
              for (const auto& f : FieldsArray) f.write(s, item); \
          } \
      }, \
      [](const pugi::xml_node& n, auto& o) { \
          for (const auto& s : n.child(Tag).children(ItemTag)) { \
              ItemType item{}; \
              for (const auto& f : FieldsArray) f.read(s, item); \
              o.Member.push_back(item); \
          } \
      } \
    }

// Dual <Impacts>: first is a ref string, second is an inline ONCCImpact list
#define DUAL_IMPACTS_FIELD(RefMember, ImpactsMember) \
    { "Impacts", \
      [](pugi::xml_node& n, const ONCC::ONCC& o) { \
          auto impacts = n.append_child("Impacts"); \
          for (const auto& imp : o.ImpactsMember) \
              impacts.append_child("ONCCImpact").append_child("Name").text().set(imp.name.c_str()); \
      }, \
      [](const pugi::xml_node& n, ONCC::ONCC& o) { \
          for (const pugi::xml_node& impNode : n.children("Impacts")) { \
              bool hasElement = false; \
              for (const pugi::xml_node& child : impNode.children()) \
                  if (child.type() == pugi::node_element) { hasElement = true; break; } \
              if (hasElement) \
                  for (const pugi::xml_node& imp : impNode.children("ONCCImpact")) \
                      o.ImpactsMember.push_back({ imp.child_value("Name") }); \
              else if (const std::string val = impNode.text().as_string(); !val.empty()) \
                  o.RefMember = val; \
          } \
      } \
    }
