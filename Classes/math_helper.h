// Ŭnicode please
#pragma once

#include <string>
#include "vector.h"
#include <cmath>
#include <cstdlib>
#include <vector>

namespace sora {;

inline int StringToInt(const std::string &str) {
	int result = atoi(str.c_str());
	return result;
}

inline float StringToFloat(const std::string &str) {
	//double이 큰 형태이고 원래 atof의 타입이 double더라
	double result = atof(str.c_str());
	return (float)(result);
}

//min, max는 std에도 선언되어있지만
//빌어먹을 윈도우는 min/max를 매크로로 기본 지원한다
//생각없이 min/max를 썻다가 골치아픈 사태가 발생하는것을 근본적으로 막기 위해서
//필요할떄 다른 형태의 min/max를 쓸수있도록 했다
template<typename T>
inline const T& Maximum(const T &a, const T &b) {
	return (a > b) ? a : b;
}
template<typename T>
inline const T& Minimum(const T &a, const T &b) {
	return (a < b) ? a : b;
}

template<typename T>
inline bool IsNormalNumber(T num) { return !(IsNaN(num) || IsINF(num)); }
template<typename T>
inline bool IsNaN(T num) { return (num != num); }
template<typename T>
bool IsINF(T num) {
	// TODO
	return false;
/*
	if (std::numeric_limits<T>::has_infinity == false) {
		return false;
	}
	if (num == std::numeric_limits<T>::infinity()) {
		return true;
	}
	//else..
	return false;
	*/
}

template<typename T>
bool IsPower(T base, T target) {
	if(target < 1) {
		//2*-1=0.5인데 이런 경우는 쓰지 않을듯해서 신경쓰지 않는다
		//어차피 위와같이 분수로 내려가는 경우, 정밀도 문제로
		//쓰지도 못할거같다
		return false;
	}
	T curr = base;
	while (curr <= target) {
		if (curr == target) {
			return true;
		}
		curr *= base;
	}
	return false;
}

template<typename T>
T CeilPower(T base, T target) {
	if (target < 1) {
		//소수점이하는 계산무시
		return base;
	}
	T curr = base;
	while (curr < target) {
		curr *= base;
	}
	return curr;
}

template<typename T>
void GetMinMax(const T &v1, const T &v2, T &min_value, T &max_value) {
	if (v1 < v2) {
		min_value = v1;
		max_value = v2;
	} else {
		min_value = v2;
		max_value = v1;
	}
}

//error 확인은 double, float에서만 쓰는데 double은 안쓰니까
//float용만 떄려박아도 문제 없다
bool EqualAbsError(float a, float b, float max_error);
bool EqualRelError(float a, float b, float max_error);
bool EqualUlps(float a, float b, int max_ulps);

///@brief MTMath와 중복되지 않는 일부 수학함수를 몰아서 적절히 처리하기 위해서 만든 클래스이다 
///@brief 두 벡터 사이의 각을 구해준다. return값은 -Pi ~ +Pi 사이.
float CalcRadian(const glm::vec2 &a, const glm::vec2 &b);
bool StringToBool(const std::string &str);

template<typename VecType>
void LineToCurveBezier(const std::vector<VecType> &line,std::vector<VecType> &curve,int devide = 10)
{
	IUASSERT(line.size()>=2);
	IUASSERT(curve.empty());
	IUASSERT(devide>0);

	std::vector<VecType> middle_points;
	auto it = line.begin();
	VecType pPrev = *it;
	it++;
	for(;it!=line.end();it++)
	{
		VecType pNow = *it;
		VecType tmp;
		tmp.x = (pPrev.x+pNow.x)/2.0f;
		tmp.y = (pPrev.y+pNow.y)/2.0f;
		middle_points.push_back(tmp);
		pPrev = pNow;
	}
	int i,j,size = middle_points.size();



	curve.push_back(line.front());
	for(i=0;i<size-1;i++)
	{
		VecType p1 = middle_points[i];
		VecType p2 = line[i+1];
		VecType p3 = middle_points[i+1];
		
		for(j=0;j<devide;j++)
		{
			float t = ((float)j) / devide;
			VecType p;
			p.x = p1.x*((1-t)*(1-t))
				+p2.x*(2*(1-t)*t)
				+p3.x*(t*t);
			p.y = p1.y*((1-t)*(1-t))
				+p2.y*(2*(1-t)*t)
				+p3.y*(t*t);
			curve.push_back(p);
		}
	}
	curve.push_back(middle_points.back());
	curve.push_back(line.back());
}
}