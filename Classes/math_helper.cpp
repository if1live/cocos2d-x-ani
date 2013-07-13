// Ŭnicode please
#include "stdafx.h"
#if SR_USE_PCH == 0
#include "assert_include.h"
#endif
#include "math_helper.h"

#include "vector.h"
#include "angle.h"

using namespace std;

namespace sora {;
float CalcRadian(const glm::vec2 &a, const glm::vec2 &b)
{
    //0벡터가 들어오면 시망하니까 적절히 수정
    glm::vec3 aVec(a.x, a.y, 0);
    glm::vec3 bVec(b.x, b.y, 0);

    if (IsZeroVector(a) == true) {
        return 0;
    }
    if (IsZeroVector(b) == true) {
        return 0;
    }
    aVec = glm::normalize(aVec);
    bVec = glm::normalize(bVec);
    glm::vec3 zeroVec(0, 0, 0);

    glm::vec3 cVec = glm::cross(aVec, bVec);

    float crossValue = cVec.z;
    float dotValue = glm::dot(aVec, bVec);

    //crossValue, dotValue는 asin, acos로 들어가니까 -1~1사이로 맞춰준다
    //이렇게 처리하지 않으며 asin/acos의 값이 nan이 뜰수있따
    if (crossValue < -1.0f) {
        crossValue = -1.0f;
    } else if (crossValue > 1.0f) {
        crossValue = 1.0f;
    }
    if (dotValue < -1.0f) {
        dotValue = -1.0f;
    } else if (dotValue > 1.0f) {
        dotValue = 1.0f;
    }

    float crossRadian = (float)asin(crossValue);	//이 값은 -파이/2 ~ 파이/2 사이의 값으로 나온다.
    float dotRadian = (float)acos(dotValue);	//이 값은 0 ~ 파이 사이의 값으로 나온다.
    IUASSERT(crossRadian == crossRadian && "nan test");
    IUASSERT(dotRadian == dotRadian && "nan test");

    if (crossRadian > 0 && dotRadian <= kPi / 2) {
        return crossRadian;
    }
    if (crossRadian > 0 && dotRadian > kPi / 2) {
        return dotRadian;
    }
    if (crossRadian <= 0 && dotRadian <= kPi / 2) {
        return crossRadian;
    } else {
        return -dotRadian;
    }
    return -dotRadian;
}

bool StringToBool(const std::string &str)
{
    if(str == "true" || str == "True" || str == "t" || str == "T") {
        return true;
    } else if(str == "false" || str == "False" || str == "f" || str == "F") {
        return false;
    } else {
        IUASSERT(!"how to parse?");
        return false;
    }
}

bool EqualAbsError(float a, float b, float max_error)
{
    if (a == b) {
        return true;
    }
    if (abs(a - b) <= max_error) {
        return true;
    }
    return false;
}
bool EqualRelError(float a, float b, float max_error)
{
    if (fabs(a - b) < max_error) {
        return true;
    }
    float relative_error = 0;
    if (abs(b) > abs(a)) {
        relative_error = abs((a-b) / b);
    } else {
        relative_error = abs((a-b) / a);
    }
    if (relative_error <= max_error) {
        return true;
    }
    return false;
}
bool EqualUlps(float a, float b, int max_ulps)
{
    //http://www.cygnus-software.com/papers/comparingfloats/comparingfloats.htm
    IUASSERT(sizeof(float) == sizeof(int));
    if (a == b) {
        return true;
    }
    int int_diff = abs(*(int*)&a - *(int*)&b);
    if (int_diff <= max_ulps) {
        return true;
    }
    return false;
}

}