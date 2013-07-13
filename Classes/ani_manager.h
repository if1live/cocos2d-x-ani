// Ŭnicode please
#pragma once

#include "ani_prototype.h"
#include "ani.h"
#include "singleton.h"

#if SR_USE_PCH == 0
#include <memory>
#endif

namespace sora {;

class AniParser;

class AniManager : public Singleton<AniManager> {
public:
    AniParser GetByteParser();
    AniParser GetXmlParser();
    static AniDeviceType GetAniDeviceType();
public:
    AniManager();
    ~AniManager();

    //파일명이 곧 키이름
    AniPrototype *LoadFile(const std::string &file);

    void Clear() { ani_dict_.clear(); }
    const AniPrototype *Get(const std::string &key) const;
    AniPrototype *Get(const std::string &key);
    //같은 이름으로 2번 등록하면 false, 그리고 새거로 바뀜
    bool Set(const std::string &key, const AniPrototype &ani);

private:
    std::map<std::string, AniPrototype> ani_dict_;

    std::unique_ptr<AniParser> byte_parser_;
    std::unique_ptr<AniParser> xml_parser_;	

    AniPrototype *LoadXmlFile(const std::string &file);
    AniPrototype *LoadByteFile(const std::string &file);
};
}
