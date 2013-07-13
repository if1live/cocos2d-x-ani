// Ŭnicode please
#include "stdafx.h"
#include "xml_writer.h"
#include "xml_node.h"

#include <string>
#include <sstream>


using namespace std;

namespace sora {;
XmlWriter::XmlWriter(bool use_indent, bool use_newline)
	: use_indent_(use_indent),
	use_newline_(use_newline)
{
}
XmlWriter::~XmlWriter()
{
}
std::string XmlWriter::Write(XmlNode *root)
{
	ostringstream oss;
	//헤더가 없어야 딴데서도 쓰기 쉽다
	//oss << "<?xml version='1.0'?>\n";
	WriteNode(root, oss, 0);
	return oss.str();
}
std::string XmlWriter::AttributeToString(XmlNode *node)
{
	ostringstream oss;
	XmlAttributeIterator it = node->AttributeBegin();
	XmlAttributeIterator endit = node->AttributeEnd();
	for( ; it != endit ; it++) {
		const string &key = it->first;
		const string &value = it->second;

		// 첫글자에 공백을 넣어준다. 그래야 연속으로할때 잘 이어짐
		// key='value'
		oss << " ";
		oss << key;
		oss <<  "=\'";
		oss << value;
		oss << "\'";
	}
	return oss.str();
}
void XmlWriter::WriteNode(XmlNode *node, std::ostringstream &oss, int level)
{
	if(node->ChildSize() == 0 && node->content.empty() == true) {
		//begin tag, no child, no content
		string attr_str = AttributeToString(node);
		InsertTab(oss, level);
		oss << "<" << node->name << attr_str << "/>";
		InsertNewline(oss);
	} else if(node->ChildSize() == 0 && node->content.empty() == false) {
		//no child, content, <as>fsd</as>
		string attr_str = AttributeToString(node);
		InsertTab(oss, level);
		oss << "<" << node->name << attr_str << ">";
		if(node->content.empty() == false) {
			oss << node->content;
		}
		oss << "</" << node->name << ">";
		InsertNewline(oss);
	} else {
		string attr_str = AttributeToString(node);
		InsertTab(oss, level);
		oss << "<" << node->name << attr_str << ">";
		InsertNewline(oss);

		if(node->content.empty() == false) {
			oss << node->content;
			InsertNewline(oss);
		}
		//write child
		XmlNodeListIterator it = node->ChildBegin();
		XmlNodeListIterator endit = node->ChildEnd();
		for( ; it != endit ; it++) {
			XmlNode *child = *it;
			WriteNode(child, oss, level + 1);
		}
		InsertTab(oss, level);
		oss << "</" << node->name << ">";
		InsertNewline(oss);
	}
}
void XmlWriter::InsertTab(std::ostringstream &oss, int level)
{
	if(use_indent_ == true) {
		for(int i = 0 ; i < level ; i++) {
			oss << "\t";
		}
	}
}
void XmlWriter::InsertNewline(std::ostringstream &oss)
{
	if(use_newline_ == true) {
		oss << "\n";
	}
}
}