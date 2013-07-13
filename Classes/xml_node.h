#pragma once

#include <vector>
#include <string>

namespace sora {;
class XmlNode;
class XmlNodeList;

typedef std::vector<XmlNode*> XmlNodeListType;
typedef XmlNodeListType::iterator XmlNodeListIterator;
typedef XmlNodeListType::const_iterator XmlNodeListConstIterator;
typedef std::unordered_map<std::string, std::string> XmlAttributeDictType;
typedef XmlAttributeDictType::iterator XmlAttributeIterator;

class XmlNode {
public:
	XmlNode();
	XmlNode(const char *name);
	XmlNode(const std::string &name);
	~XmlNode();

	//attribute
	void SetAttribute(const char *key, const char *value);
	void SetAttribute(const std::string &key, const std::string &value);
	const std::string &GetAttribute(const char *key) const;
	bool HasAttribute(const char *key) const;

	XmlAttributeIterator AttributeBegin() { return attribute_.begin(); }
	XmlAttributeIterator AttributeEnd() { return attribute_.end(); }

	//children
	void AddChild(XmlNode *child);
	XmlNode *Child(int index) const;
	int ChildCount() const { return children_.size(); }
	int ChildSize() const { return children_.size(); }
	XmlNode *FirstChild() const;
	XmlNode *LastChild() const;

	XmlNodeListIterator ChildBegin() { return children_.begin(); }
	XmlNodeListIterator ChildEnd() { return children_.end(); }
	XmlNodeListConstIterator ChildBegin() const { return children_.begin(); }
	XmlNodeListConstIterator ChildEnd() const { return children_.end(); }

	//getter + setter
	std::string content;
	std::string name;

	XmlNode *parent() const { return parent_; }

protected:
	//setter
	void set_parent(XmlNode *parent) { parent_ = parent; }


	XmlAttributeDictType attribute_;
	XmlNode *parent_;

	XmlNodeListType children_;

};
}
