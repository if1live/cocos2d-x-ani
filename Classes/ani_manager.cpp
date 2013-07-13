// Ŭnicode please
#include "stdafx.h"
#include "ani_manager.h"
#include "ani_parser.h"
#include "file_helper.h"

using namespace std;

namespace sora {;

AniManager::AniManager()
{
}
AniManager::~AniManager()
{
}

const AniPrototype *AniManager::Get(const std::string &key) const
{
    auto found = ani_dict_.find(key);
    if(found == ani_dict_.end()) {
        return NULL;
    } else {
        return &found->second;
    }
}
AniPrototype *AniManager::Get(const std::string &key)
{
    auto found = ani_dict_.find(key);
    if(found == ani_dict_.end()) {
        return NULL;
    } else {
        return &found->second;
    }
}
bool AniManager::Set(const std::string &key, const AniPrototype &ani)
{
    auto found = ani_dict_.find(key);
    bool retval = false;
    if(found == ani_dict_.end()) {
        retval = false;
    } else {
        retval = true;
    }
    ani_dict_[key] = ani;
    return retval;
}

AniParser AniManager::GetByteParser()
{
    AniDeviceType dev_type = GetAniDeviceType();
    return AniParser::CreateByteAniParser(dev_type);
}
AniParser AniManager::GetXmlParser()
{
    AniDeviceType dev_type = GetAniDeviceType();
    return AniParser::CreateXmlAniParser(dev_type);
}

AniDeviceType AniManager::GetAniDeviceType()
{
    /*XXX
    //장치의 해상도에 맞추서 어떤 애니를 쓸지 결정하기
    int dev_code = Device::GetInstance().GetDeviceCode();
    AniDeviceType dev_type = kAniDeviceSD;
    switch(dev_code) {
    case Device::kiPhone3GS:
    dev_type = kAniDeviceSD;
    break;
    case Device::kiPhone4:
    dev_type = kAniDeviceHD;
    break;
    case Device::kiPad:
    dev_type = kAniDeviceIPad;
    break;
    case Device::kiPadHD:
    dev_type = kAniDeviceIPadHD;
    break;
    default:
    dev_type = kAniDeviceHD;
    break;
    }
    return dev_type;
    */
    return kAniDeviceHD;
}

AniPrototype *AniManager::LoadXmlFile(const std::string &file)
{
    AniPrototype *prev = Get(file);
    if(prev == NULL) {
        AniParser xml_parser = GetXmlParser();
        bool read_result = false;
        AniPrototype prototype = xml_parser.LoadFile(file, &read_result);
        Set(file, prototype);
        AniPrototype *ani = Get(file);
        return ani;
    } else {
        return prev;
    }
}
AniPrototype *AniManager::LoadByteFile(const std::string &file)
{
    AniPrototype *prev = Get(file);
    if(prev == NULL) {
        AniParser byte_parser = GetByteParser();
        bool read_result = false;
        AniPrototype prototype = byte_parser.LoadFile(file, &read_result);
        Set(file, prototype);
        AniPrototype *ani = Get(file);
        return ani;
    } else {
        return prev;
    }
}


AniPrototype *AniManager::LoadFile(const std::string &file)
{
    AniPrototype *prev = Get(file);
    if(prev == NULL) {
        //최초 2바이트 읽어서 바이트일지 xml일지를 예측하자
        vector<unsigned char> data = FileHelper::ReadByteFile(file);
        if(data.size() < 2) {
            IUASSERT(!"Not valid");
            return NULL;
        }

        AniParser *parser = NULL;
        AniParser byte_parser = GetByteParser();
        AniParser xml_parser = GetXmlParser();
        if(data[0] == 'q' && data[1] == 'b') {
            parser = &byte_parser;
        } else {
            parser = &xml_parser;
        }

        bool read_result = false;
        AniPrototype prototype = parser->LoadData(data, &read_result);
        Set(file, prototype);
        AniPrototype *ani = Get(file);
        return ani;
    } else {
        return prev;
    }
}
}	//namespace qb

