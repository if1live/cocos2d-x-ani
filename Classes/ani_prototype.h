// Ŭnicode please
#pragma once

#include "ani_frame.h"
#include "ani_resource.h"

namespace sora {;

class Ani;

//하나의 애니메이션 타입에 대해서 공유되는 속성 및 복제할 속성을 가지는 클래스이다
//이것을 기반으로 AniInstance를 찍어내면된다
class AniPrototype {
public:
	typedef std::map<int, AniFrame> FrameDictType;
	typedef std::map<int, AniResource> ResDictType;
	typedef std::multimap<int, int> MaskDataType;

public:
	AniPrototype();
	//외부에서 애니를 찍어낼때는 이거만 쓰면 된다
	Ani Create();

public:
	void SetResourceData(const std::map<int, AniResource> &res_dict);
	AniResource *GetResource(int resid);
	const AniResource *GetResource(int resid) const;

	void AddFrame(const AniFrame &frame);
	const AniFrame *GetFrame(int index) const;
	AniFrame *GetFrame(int index);
	FrameDictType::iterator FrameBegin() { return frame_dict_.begin(); }
	FrameDictType::iterator FrameEnd() { return frame_dict_.end(); }

	int fps() const { return fps_; }
	int num_frame() const { return num_frame_; }

	void set_fps(int fps) { fps_ = fps; }
	void set_num_frame(int n) { num_frame_ = n; }

	const std::string &plist_file() const { return plist_file_; }
	const std::string &tex_file() const { return tex_file_; }
	void set_plist_file(const std::string &s) { plist_file_ = s; }
	void set_tex_file(const std::string &s) { tex_file_ = s; }

	//resource는 각각의 내이가 공유하지 않는다
	//(각각의 애니마다 재생되는 프레임의 위치가 다르니까 visible도 다르고 기타 잡탕도 달라서)
	//그래서 const로 둬서 복제용으로만 쓰자
	const ResDictType &GetResourceDict() const { return res_dict_; }

	void SetMaskData(const std::multimap<int, int> &mask_dict);
	std::vector<int> GetMaskResIdList() const;
	bool IsMaskResId(int resid) const;
	bool IsClipedResid(int resid) const;

private:
	FrameDictType frame_dict_;
	ResDictType res_dict_;
	MaskDataType mask_dict_;

	std::string plist_file_;
	std::string tex_file_;

	int fps_;
	int num_frame_;		//몇장으로 애니가 구성되는가

	std::set<int> clip_res_list_;
};


}
