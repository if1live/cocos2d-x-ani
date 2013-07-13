// Ŭnicode please
#pragma once

namespace sora {;

//애니메이션은 총 4종류로 볼수잇다
//sd/hd/ipadhd/ipad
//각각에 대해서 일부 연산이 미묘하게 달라진다(예를 들면 행렬)
//이 값을 시스템에서 직접 바로 갖다쓰는것보다는 함수인자같은 형태로 넘기면 테스트에 유용할거
//같아서 독립시켜놓음
//아이패드르HD와 같이 못보는 이유는 HD는 컨테츠 스케일2배로 뻥튀기 하기 떄문
//안드로이드의 경우는 가장 근접한 아이폰 스펙을 따라서 스케일링 하자
typedef enum {
    kAniDeviceSD,
    kAniDeviceHD,
    kAniDeviceIPad,
    kAniDeviceIPadHD,
} AniDeviceType;

struct AniColor4ub {
    AniColor4ub() : r(255), g(255), b(255), a(255) {}
    AniColor4ub(unsigned char r, unsigned char g, unsigned char b, unsigned char a) 
        : r(r), g(g), b(b), a(a) {}
    AniColor4ub(const unsigned char d[4])
        : r(d[0]), g(d[1]), b(d[2]), a(d[3]) {}

    union {
        struct {
            unsigned char r;
            unsigned char g;
            unsigned char b;
            unsigned char a;
        };
        unsigned char data[4];
    };

    static AniColor4ub Float(float r, float g, float b, float a) {
        unsigned char rvalue = (unsigned char)(r * 255);
        unsigned char gvalue = (unsigned char)(g * 255);
        unsigned char bvalue = (unsigned char)(b * 255);
        unsigned char avalue = (unsigned char)(a * 255);
        return AniColor4ub(rvalue, gvalue, bvalue, avalue);
    }
    static AniColor4ub Float(const float d[4]) {
        return AniColor4ub::Float(d[0], d[1], d[2], d[3]);
    }

    bool operator==(const AniColor4ub &o) const {
        return (r == o.r && g == o.g && b == o.b && a == o.a);
    }
    bool operator!=(const AniColor4ub &o) const {
        return !(*this == o);
    }
};


}
