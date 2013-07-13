// Ŭnicode please
#include "stdafx.h"
#include "angle.h"
#include <cmath>
#include "assert_include.h"
#include "vector.h"

using namespace std;
using namespace glm;

namespace sora {;
AngleDeg::AngleDeg()
	: deg_(0)
{
}

AngleDeg::AngleDeg(float deg)
	: deg_(deg)
{
	deg_ = CalcPlusMinus180(deg);
}

AngleDeg::~AngleDeg()
{
}

float AngleDeg::CalcPlusMinus180(float v)
{
	if(v >= -180.0f && v < 180.0f) {
		return v;	//아무것도 안해도 되는 올바른 구역
	} else if(v < -180.0f) {
		//범위보다 더 작은 경우, 올바른 구역에 집어넣기
		int loop_count = (int)((-v + 180.0f) / 360.0f);
		float retval = v + loop_count * 360.0f;

		IUASSERT(retval >= -180.0f);
		IUASSERT(retval < 180.0f);
		return retval;

	} else if(v >= 180.0f) {
		//범위보다 더 큰 경우, 올바른 구역에 집어넣기
		int loop_count = (int)((v + 180) / 360.0f);
		float retval = v - loop_count * 360.0f;
		IUASSERT(retval >= -180.0f);
		IUASSERT(retval < 180.0f);
		return retval;

	} else {
		IUASSERT(!"not valid");
	}
	return v;
}

float AngleDeg::To_0_360() const
{
	//0~360도로 각도를 반환
	float ret = deg_ + 180.0f;
	IUASSERT(ret >= 0.0f);
	IUASSERT(ret < 360.0f);
	return ret;
}
float AngleDeg::ToPlusMinus180() const
{
	//-180~180도로 각도 변환
	IUASSERT(deg_ >= -180.0f);
	IUASSERT(deg_ < 180.0f);
	return deg_;
}

//operator
AngleDeg AngleDeg::operator+(const AngleDeg &o) const
{
	AngleDeg tmp(*this);
	tmp += o;
	return tmp;
}
AngleDeg& AngleDeg::operator+=(const AngleDeg &o)
{
	float ang = deg_ + o.deg_;
	deg_ = CalcPlusMinus180(ang);
	return *this;
}
	
AngleDeg AngleDeg::operator-(const AngleDeg &o) const
{
	AngleDeg tmp(*this);
	tmp -= o;
	return tmp;
}
AngleDeg& AngleDeg::operator-=(const AngleDeg &o)
{
	float ang = deg_ - o.deg_;
	deg_ = CalcPlusMinus180(ang);
	return *this;
}

AngleDeg AngleDeg::operator-() const
{
	float ang = -deg_;
	return AngleDeg(ang);
}

AngleDeg AngleDeg::operator*(float s) const
{
	AngleDeg tmp(*this);
	tmp *= s;
	return tmp;
}
AngleDeg &AngleDeg::operator*=(float s)
{
	float ang = deg_ * s;
	deg_ = CalcPlusMinus180(ang);
	return *this;
}
bool AngleDeg::IsInside(const AngleDeg &range_a, const AngleDeg &range_b, const AngleDeg &a)
{
	vec2 vec_a(
		cos(DegreeToRadian(range_a.ToPlusMinus180())), 
		sin(DegreeToRadian(range_a.ToPlusMinus180()))
		);
	vec2 vec_b(
		cos(DegreeToRadian(range_b.ToPlusMinus180())),
		sin(DegreeToRadian(range_b.ToPlusMinus180()))
		);
	vec2 dir(
		cos(DegreeToRadian(a.ToPlusMinus180())), 
		sin(DegreeToRadian(a.ToPlusMinus180()))
		);
	
	float dot_a_b = glm::dot(vec_a, vec_b);
	float dot_a_dir = glm::dot(vec_a, dir);
	float dot_b_dir = glm::dot(vec_b, dir);

	float deg_a_b = RadianToDegree(acos(dot_a_b));
	float deg_a_dir = RadianToDegree(acos(dot_a_dir));
	float deg_b_dir = RadianToDegree(acos(dot_b_dir));

	if(fabs(deg_a_b - (deg_a_dir + deg_b_dir)) < 0.1) {
		return true;
	} else {
		return false;
	}
}
}	//namespace qb