// Ŭnicode please
#pragma once

#if SR_USE_PCH==0
#include <sstream>
#include <string>
#endif

namespace sora {;
class XmlNode;

class XmlWriter {
public:
	///@param use_indent xml노드마다 들여쓰기
	///@param use_new_line 노드끝마다 엔터를 넣을것인가?
	XmlWriter(bool use_indent, bool use_newline);
	~XmlWriter();

	std::string Write(XmlNode *root);
	std::string AttributeToString(XmlNode *node);
private:
	//재귀적으로 호출되는 함수. oss에 계속 덧붙인다
	///@param level 들여쓰기 단계, 탭이 앞에 몇개 들어가는지를 결정
	void WriteNode(XmlNode *node, std::ostringstream &oss, int level);
	void InsertTab(std::ostringstream &oss, int level);
	void InsertNewline(std::ostringstream &oss);

	bool use_indent_;
	bool use_newline_;
};
}