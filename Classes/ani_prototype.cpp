// Ŭnicode please
#include "stdafx.h"
#include "ani_prototype.h"
#include "ani.h"

using namespace std;

namespace sora {;

AniPrototype::AniPrototype()
	: num_frame_(0), fps_(60)
{
}


void AniPrototype::SetResourceData(const std::map<int, AniResource> &res_dict)
{
	res_dict_ = res_dict;
}

void AniPrototype::AddFrame(const AniFrame &frame)
{
	int frame_idx = frame.frame_idx();
	frame_dict_[frame_idx] = frame;
}
const AniFrame *AniPrototype::GetFrame(int index) const
{
	auto found = frame_dict_.find(index);
	if(found == frame_dict_.end()) {
		return NULL;
	} else {
		return &found->second;
	}
}
AniFrame *AniPrototype::GetFrame(int index)
{
	auto found = frame_dict_.find(index);
	if(found == frame_dict_.end()) {
		return NULL;
	} else {
		return &found->second;
	}
}

Ani AniPrototype::Create()
{
	Ani ani(this);
	return ani;
}

AniResource *AniPrototype::GetResource(int resid)
{
	auto found = res_dict_.find(resid);
	if(found == res_dict_.end()) {
		return NULL;
	} else {
		return &found->second;
	}
}
const AniResource *AniPrototype::GetResource(int resid) const
{
	auto found = res_dict_.find(resid);
	if(found == res_dict_.end()) {
		return NULL;
	} else {
		return &found->second;
	}
}

void AniPrototype::SetMaskData(const std::multimap<int, int> &mask_dict)
{
	mask_dict_ = mask_dict;

	clip_res_list_.clear();
	for(auto it = mask_dict.begin(), e = mask_dict.end() ; it != e ; ++it) {
		clip_res_list_.insert(it->second);
	}
}
std::vector<int> AniPrototype::GetMaskResIdList() const
{
	set<int> mask_set;
	auto it = mask_dict_.begin();
	auto endit = mask_dict_.end();
	for( ; it != endit ; ++it) {
		mask_set.insert(it->first);
	}

	vector<int> mask_list(mask_set.begin(), mask_set.end());
	return mask_list;
}
bool AniPrototype::IsMaskResId(int resid) const
{
	auto found = mask_dict_.find(resid);
	if(found == mask_dict_.end()) {
		return false;
	} else {
		return true;
	}
}

bool AniPrototype::IsClipedResid(int resid) const
{
	auto found = clip_res_list_.find(resid);
	if(found == clip_res_list_.end()) {
		return false;
	} else {
		return true;
	}
}

}