#pragma once

#include <vector>
#include <string>

namespace sora {;

class StringUtil {
public:
	template<typename T>
	static T Trim(const T &str);

	template<typename T>
	static T LeftTrim(const T &str);

	template<typename T>
	static T RightTrim(const T &str);

	template<typename T>
	static int Split(const T &str, std::vector<T> &tokens, const T &delim);

	template<typename T>
	static T Join(const T &str, const std::vector<T> &tokens);

	template<typename T>
	static int Tokenize(const T &str, std::vector<T> &tokens, const T &delim);

	template<typename T>
	static std::vector<T*> SplitLine(T *str);

	template<typename T>
	static T ToLower(const T &str);

	template<typename T>
	static T ToUpper(const T &str);

	static const std::wstring &WhiteSpace(const std::wstring &s);
	static const std::string &WhiteSpace(const std::string &s);
};


template<typename T>
T StringUtil::Trim(const T &str)
{
	//do no use boost to compile speed + dependenty
	std::size_t leftFound = str.find_first_not_of(WhiteSpace(str));
	//_Tstring left(n == _Tstring::npos ? tmp : tmp.substr(n, tmp.length()));
	std::size_t rightFound = str.find_last_not_of(WhiteSpace(str));
	//_Tstring right(n == _Tstring::npos ? tmp : tmp.substr(0, n + 1));

	if(leftFound == std::string::npos) {
		leftFound = 0;
	}
	if(rightFound == std::string::npos) {
		rightFound = str.length();
	}
	const T result = str.substr(leftFound, rightFound-leftFound+1);
	return result;
}

template<typename T>
T StringUtil::LeftTrim(const T &str)
{
	std::size_t n = str.find_first_not_of(WhiteSpace(str));
	T left(n == std::string::npos ? str : str.substr(n, str.length()));
	//_Tstring left = boost::trim_left_copy(str_);
	return left;

}

template<typename T>
T StringUtil::RightTrim(const T &str)
{
	size_t n = str.find_last_not_of(WhiteSpace(str));
	T right(n == std::string::npos ? str : str.substr(0, n + 1));
	//_Tstring right = boost::trim_right_copy(str_);
	return right;
}

template<typename T>
int StringUtil::Split(const T &str, std::vector<T> &tokens, const T &delim)
{
	tokens.clear();

	//if no delimeter
	if(delim.length() == 0) {
		tokens.push_back(str);
		return -1;
	}
	if(str.length() == 0) {
		return 0;
	}

	//simple impl
	T token;
	for(std::size_t i = 0 ; i < str.length() ; i++) {
		const typename T::value_type ch = str[i];
		std::size_t found = delim.find_first_of(ch);
		if(found == T::npos) {
			//token get!
			token.push_back(ch);
		} else {
			if(token.length() > 0) {
				tokens.push_back(token);
			}
			token.clear();
		}
	}
	//reminder
	if(token.length() > 0 && token[0] != '\0') {
		tokens.push_back(token);
	}
	return tokens.size();
}

template<typename T>
T StringUtil::Join(const T &str, const std::vector<T> &tokens)
{
	T result;
	int tokenCnt = tokens.size();
	for(int i = 0 ; i < tokenCnt ; i++) {
		result += tokens[i];
		if(i == tokenCnt-1) {
			break;
		}
		result += str;
	}
	return result;
}

template<typename T>
int StringUtil::Tokenize(const T &str, std::vector<T> &tokens, const T &delim)
{
	tokens.clear();
	if(delim.size() == 0) {
		tokens.push_back(str);
		return -1;
	}
	if(str.size() == 0) {
		return 0;
	}
	//logit start
	std::size_t start = 0;
	while(true) {
		std::size_t found = str.find(delim, start);
		if(found == T::npos) {
			if(start < str.length()) {
				std::string tmp = str.substr(start);
				tokens.push_back(tmp);
			}
			return tokens.size();
		} else {
			std::string tmp = str.substr(start, found-start);
			tokens.push_back(tmp);

			start = found + delim.size();
		}
	}
	//do not reach
	IUASSERT(false && "do not reach");
	return -1;
}

template<typename T>
T StringUtil::ToLower(const T &str)
{
	T result(str.begin(), str.end());
	for(size_t i = 0 ; i < result.size() ; i++) {
		typename T::value_type ch = tolower(result[i]);
		result[i] = ch;
	}
	return result;
}

template<typename T>
T StringUtil::ToUpper(const T &str)
{
	T result(str.begin(), str.end());
	for(size_t i = 0 ; i < result.size() ; i++) {
		typename T::value_type ch = toupper(result[i]);
		result[i] = ch;
	}
	return result;
}

template<typename T>
std::vector<T*> StringUtil::SplitLine(T*str)
{
	std::vector<T*> line_list;
	if(str == NULL) {
		return line_list;
	}
	
	if(str[0] == '\0') {
		//0글자인 경우
		return line_list;
	}
	
	//첫줄은 항상 들어가겟지
	line_list.push_back(str);

	//줄단위로 잘릴수잇는 포인터를 반환하기
	for(int i = 1 ; str[i] != '\0' ; i++) {
		T ch = str[i];
		if(str[i] == '\n' || str[i] == '\r') {
			str[i] = '\0';
		}
		T curr_ch = str[i];
		T prev_ch = str[i-1];
		if(curr_ch != '\0' && prev_ch == '\0') {
			line_list.push_back(str+i);
		}
	}
	return line_list;	
}

}