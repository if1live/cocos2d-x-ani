// Ŭnicode please
#include "stdafx.h"
#include "ani_parser.h"
#include "string_util.h"
#include "math_helper.h"
#include "ani.h"
#include "xml_node.h"
#include "xml_reader.h"
#include "ani_prototype.h"
#include "ani_data.h"

#include "ani_reader.h"
#include "ani_writer.h"

#include "file_helper.h"

using namespace std;
using namespace glm;
using namespace cocos2d;

namespace sora {;
/*
//공통부분
void AniParser::ConvertToFlashMatrix(qb::mat4 &mat)
{
	//3*3의 형태
	//x는 기준행렬일떄의 값
	//0 1 2
	//3 4 5
	//6 7 8
	//5번항목을 내림/올림해서 적절히 플래시와 똑같이 보이도록 조정하기
	//플래시 자체의 렌더러의 경우 y좌표를 내림으로처리하는듯하는 문제가 있어서
	//플래시 플레이어와 다르게 렌더링 된다. 이를 애니툴에서 내림 처리해서 문제를 고친다

	//일단 꺼놓기
	//float y = mat.GetElem(1, 3);
	//y = floorf(y);
	//mat.SetElem(1, 3, y);
}
*/
void AniParser::ReverseYDirection(glm::mat3 &m)
{
    m[2][1] = -m[2][1];
}


AniPrototype AniParser::LoadData(const std::vector<unsigned char> &data, bool *retcode) const
{
	switch(parser_type_) {
	case kXmlParser:
		return LoadXmlData(data, retcode);
		break;
	case kByteParser:
		return LoadByteData(data, retcode);
		break;
	default:
		IUASSERT(!"do not reach");
		return LoadXmlData(data, retcode);
		break;
	}
}
AniPrototype AniParser::LoadFile(const std::string &file, bool *retcode)
{
	switch(parser_type_) {
	case kXmlParser:
		return LoadXmlFile(file, retcode);
		break;
	case kByteParser:
		return LoadByteFile(file, retcode);
		break;
	default:
		IUASSERT(!"do not reach");
		return LoadXmlFile(file, retcode);
		break;
	}
}

AniPrototype AniParser::LoadByteData(const std::vector<unsigned char> &data, bool *retcode) const
{
	//default = true
	if(retcode != NULL) {
		*retcode = true;
	}

    unique_ptr<AniReader> reader(AniReader::ByteVersion());
	if(data.empty() == true) {
		if(retcode != NULL) {
			*retcode = false;
		}
		AniPrototype empty;
		return empty;
	}

	AniData ani_data = reader->Read(data);
	AniPrototype ani_prototype;
	Convert(&ani_prototype, ani_data);
	return ani_prototype;
}
AniPrototype AniParser::LoadXmlData(const std::vector<unsigned char> &data, bool *retcode) const
{
	//default = true
	if(retcode != NULL) {
		*retcode = true;
	}

    unique_ptr<AniReader> reader(AniReader::XmlVersion());
    SR_ASSERT(reader.get() != NULL);
	if(data.empty() == true) {
		if(retcode != NULL) {
			*retcode = false;
		}
		AniPrototype empty;
		return empty;
	}
	AniData ani_data = reader->Read(data);
	AniPrototype ani_prototype;
	Convert(&ani_prototype, ani_data);
	return ani_prototype;
}

AniPrototype AniParser::LoadByteFile(const std::string &file, bool *retcode) const
{
	vector<unsigned char> data = FileHelper::ReadByteFile(file);
	return LoadByteData(data, retcode);
}

AniPrototype AniParser::LoadXmlFile(const std::string &file, bool *retcode) const
{
	vector<unsigned char> data = FileHelper::ReadByteFile(file);
	return LoadXmlData(data, retcode);
}

std::string AniParser::ConvertFilename(const std::string &orig) const
{
	//데이터에 내장된 파일명과 실제 파일명은 경로과 suffix에서 좀 다르다
	//suffix를 뗀것을 로직다룰때 쓴다
	//공용이름으로 넣어주면 시스템에 맞는 파일 로딩은 엔진이 알아서 해주시겟지
	string pure_name = FileHelper::RemoveSuffix(orig);

	//애니메이션은 하나의 폴더에 전부 몰아서 넣어놧다
	string path_prefix = "ani/";
	string output = path_prefix + pure_name;
	return output;
}

void AniParser::Convert(AniPrototype *ani, const AniData &data) const
{
	//header 구성
	ani->set_fps(data.fps);
	ani->set_num_frame(data.num_frame);

	string tex_file = ConvertFilename(data.texture_file_name);
	string plist_file = ConvertFilename(data.frame_file_name);
	ani->set_tex_file(tex_file);
	ani->set_plist_file(plist_file);
        
	//texture, plist 로딩. 이것은 최소한 1번은 실행 되어야 ccsprite frame cache가 사용 가능
	CCTextureCache::sharedTextureCache()->addImage(tex_file.c_str());
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(plist_file.c_str(), tex_file.c_str());

	//resource 구성
	std::map<int, AniResource> res_dict;
	for(auto it = data.frame_list.begin(), e = data.frame_list.end() ; it != e ; ++it) {
		for(auto i = it->cmd_list.begin(), end = it->cmd_list.end() ; i != end ; ++i) {
			if(i->cmd_type == kFrameCmd_Show) {
				const string &name = i->res_name;
				CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name.c_str());
				AniResource res(i->res_id, i->z, frame);
				res_dict[i->res_id] = res;
			}
		}
	}
	ani->SetResourceData(res_dict);
	
	//frame + cmd 구성
	for(auto frame_it = data.frame_list.begin(), e = data.frame_list.end() ; frame_it != e ; ++frame_it) {
		AniFrame frame(frame_it->number, frame_it->cmd_list.size());
		for(auto i = frame_it->cmd_list.begin(), end = frame_it->cmd_list.end() ; i != end ; ++i) {
			if(i->cmd_type == kFrameCmd_Show) {
				int res_id = i->res_id;
				mat3 mat = Mat3ToMat4(i->matrix, dev_type_);
				ReverseYDirection(mat);

				AniColor4ub mul_color(i->mul_color);
				AniColor4ub add_color(i->add_color);

				AniFrameCommand cmd = AniFrameCommand::Show(res_id, mat, mul_color, add_color);
				frame.AddCmd(cmd);

			} else if(i->cmd_type == kFrameCmd_Apply) {
				int res_id = i->res_id;
				mat3 mat = Mat3ToMat4(i->matrix, dev_type_);
				ReverseYDirection(mat);

				AniColor4ub mul_color(i->mul_color);
				AniColor4ub add_color(i->add_color);

				AniFrameCommand cmd = AniFrameCommand::Apply(res_id, mat, mul_color, add_color);
				frame.AddCmd(cmd);

			} else if(i->cmd_type == kFrameCmd_Hide) {
				int res_id = i->res_id;
				AniFrameCommand cmd = AniFrameCommand::Hide(res_id);
				frame.AddCmd(cmd);

			}
		}
		ani->AddFrame(frame);
	}

	//mask 처리
	multimap<int, int> mask_dict;
	for(auto it = data.mask_list.begin(), e = data.mask_list.end() ; it != e ; ++it) {
		const AniMaskData &mask = *it;
		//마스크 정보 적절히 등록하기

		for(size_t i = 0 ; i < mask.clip_list.size() ; i++) {
			mask_dict.insert(make_pair(mask.mask, mask.clip_list[i]));
		}

		//마스크로 선언된 리소스는 보이지 않는다
		AniResource *res = ani->GetResource(mask.mask);
		if(res != NULL) {
			res->set_visible(false);
		}
	}
	ani->SetMaskData(mask_dict);

	//state 부분 복사
	for(auto it = data.state_list.begin(), e = data.state_list.end() ; it != e ; ++it) {
		const AniStateData &state = *it;
	}
}

glm::mat3 AniParser::Mat3ToMat4(const glm::mat3 &input, AniDeviceType dev_type)
{

	//3*3의 형태
	//0	1 2
	//3 4 5
	//6 7 8

	//입력으로 들어오는 행렬은 레티나 기준이다
	//2, 5번 항목은 retina가 아닌 경우는 절반으로 들어가야한다
	//2,5인 이유는 그것이 평행이동 항목과 연결되니까

    //디바이스 마다 행렬크기 미묘하게달라지는데 내가 뭘 짯나 기억이 안난다...
    //리펙토링 하면서 다시 확인하기
	float scale = 2.0f;
    /*XXX
	switch(dev_type) {
	case kAniDeviceSD:
		scale = 0.5f;
		break;
	case kAniDeviceHD:
		scale = 0.5f;	//for iphone
		break;
	case kAniDeviceIPad:
		scale = 1.0f;
		break;	
	case kAniDeviceIPadHD:
		//scale = 2.0f;
		scale = 1.0f;
		break;
	default:
		IUASSERT(!"Do not reach");
		break;
	}
    */

    mat3 m = input;
    m[2][0] *= scale;
    m[2][1] *= scale;
	return m;
}
}
