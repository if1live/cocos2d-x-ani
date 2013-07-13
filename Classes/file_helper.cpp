// Ŭnicode please
#include "stdafx.h"
#include "file_helper.h"

#if SR_USE_PCH == 0
#include "cocos2d.h"
#endif

using namespace cocos2d;
using namespace std;

namespace sora {;
std::string FileHelper::RemoveSuffix(const std::string &orig) 
{
	std::string str(orig);
	//CCFileUtils::ccRemoveHDSuffixFromFile 를 쓸 경우, 레티나 조건 검사를 하는데
	//그런거 없이 무조건 hd를 떼기 위해서 아래를 사용함
	// the begin index of last part of path
	std::string::size_type pos = str.rfind("/") + 1; 
	//TODO CC_RETINA_DISPLAY_FILENAME_SUFFIX는 0.13.에서 빠졋다. 이것에 의존하지않도록 수정하기
	//-hd가 잇으면 그거 제거, -ipadhd가 잇어도 그거 제거
	const char *hd_suffix = "-hd";
	const char *ipadhd_suffix = "-ipadhd";

	vector<string> suffix_list;
	suffix_list.push_back(hd_suffix);
	suffix_list.push_back(ipadhd_suffix);
	auto it = suffix_list.begin();
	auto endit = suffix_list.end();
	for( ; it != endit ; it++) {
		const string &suffix = *it;

		std::string::size_type suffixPos = str.rfind(suffix.c_str());
		if (std::string::npos != suffixPos && suffixPos > pos) {
			str.replace(suffixPos, suffix.length(), "");
		}
	}
	return str;
}
std::string FileHelper::ReadFile(const std::string &file)
{
    std::string full_ani_path = CCFileUtils::sharedFileUtils()->fullPathForFilename(file.c_str());
	IUASSERT(full_ani_path.empty() == false);	
    // cocos2dx 2.0에서 CCFileData가 사라져서 무효화되버렸다. 적절히 다른방법으로
	//CCFileData data(full_ani_path.c_str(), "rb");
    bool bRet = false;
    unsigned long nSize = 0;
    unsigned char* pBuffer = CCFileUtils::sharedFileUtils()->getFileData(full_ani_path.c_str(), "rb", &nSize);

    std::string str;
    if (pBuffer != NULL && nSize > 0) {
        std::string str;
        //마지막에 null을 하나 붙여서 str계열 함수에서 실수의 가능성을 배제
	    str.resize(nSize + 1, '\0');    
	    memcpy((char*)str.c_str(), (char*)pBuffer, nSize);
    }
    CC_SAFE_DELETE_ARRAY(pBuffer);

    //어차피 파일 자체가 몇메가씩 나가지 않을테니까 그냥 리턴
	//그리고 r-value만 믿고 리턴
    return str;
}

std::vector<unsigned char> FileHelper::ReadByteFile(const std::string &file)
{
	std::string full_ani_path = CCFileUtils::sharedFileUtils()->fullPathForFilename(file.c_str());
	IUASSERT(full_ani_path.empty() == false);	
	bool bRet = false;
    unsigned long nSize = 0;
    unsigned char* pBuffer = CCFileUtils::sharedFileUtils()->getFileData(full_ani_path.c_str(), "rb", &nSize);

    vector<unsigned char> retval;

    if (pBuffer != NULL && nSize > 0) {
        retval.resize(nSize);
	    memcpy(retval.data(), pBuffer, nSize);
    }
    CC_SAFE_DELETE_ARRAY(pBuffer);
	return retval;
}

}

