// Ŭnicode please
#pragma once
namespace sora {;
class Uncopyable {
protected:                                  // 파생된 클래스에 대해
	Uncopyable() {}                           // 생성과 소멸을
	~Uncopyable() {}                          // 허용합니다.

private:
	Uncopyable(const Uncopyable&);            // 하지만 복사는 방지합니다.
	Uncopyable& operator=(const Uncopyable&);
};
}