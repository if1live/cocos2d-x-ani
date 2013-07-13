// Ŭnicode please
#pragma once

#ifndef MT_PI
#define MT_PI 3.141592f
#endif

namespace sora {;

//const + etc
const float kPi = MT_PI;
const float kPiOver2 = MT_PI / 2.0f;
const float kPiOver4 = MT_PI / 4.0f;

//60분법의 구현체
//-180~180사이에서 유지되도록 항상 유지
class AngleDeg {
public:
	AngleDeg();
	AngleDeg(float deg);
	~AngleDeg();
	
	float CalcPlusMinus180(float v);

	float To_0_360() const;	//0~360도로 각도를 반환
	float ToPlusMinus180() const;	//-180~180도로 각도 변환

	//operator
	AngleDeg operator+(const AngleDeg &o) const;
	AngleDeg& operator+=(const AngleDeg &o);
	
	AngleDeg operator-(const AngleDeg &o) const;
	AngleDeg& operator-=(const AngleDeg &o);

	AngleDeg operator-() const;

	AngleDeg operator*(float s) const;
	AngleDeg &operator*=(float s);

public:
	static bool IsInside(const AngleDeg &range_a, const AngleDeg &range_b, const AngleDeg &a);
private:
	//-180~180사이에서 유지되도록 항상 유지
	float deg_;
};

//deg to rad같은함수는 타입별로 있으면 좋겠다라는 생각을 했지만
//정수값을 그냥 넣으면 정수로 인식하고 정수로 뱉는 바람에
//변환결과가 0이 나올때가 많았다
//그리고 굳이 타입을 여러개 지원하도록 만드려고 해봤자
//float, double뿐인데 double은 쓰지 않고있으니까
//float로 충분한거같아서 직접 때려박았다
inline float DegreeToRadian(float degree) { return degree * kPi / 180; }
inline float RadianToDegree(float radian) {	return radian * 180 / kPi; }
inline float DegToRad(float deg) { return DegreeToRadian(deg); }
inline float RadToDeg(float rad) { return RadianToDegree(rad); }
}	//namespace qb
