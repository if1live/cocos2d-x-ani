// Ŭnicode please
#include "stdafx.h"
#include "string_util.h"

namespace sora {;

const std::wstring &StringUtil::WhiteSpace(const std::wstring &s)
{
	static const wchar_t *whitespace = L"\t\n\x0b\x0c\r ";
	static std::wstring whitespace_str(whitespace);
	return whitespace_str;
}
const std::string &StringUtil::WhiteSpace(const std::string &s)
{
	static const char *whitespace = "\t\n\x0b\x0c\r ";
	static std::string whitespace_str(whitespace);
	return whitespace_str;
}

}