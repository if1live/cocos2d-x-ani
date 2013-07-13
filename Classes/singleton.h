// Ŭnicode please 
//#ifndef SINGLETON_H_
//#define SINGLETON_H_
#pragma once

#include "assert_include.h"

#if SR_USE_PCH_==0
#include <cstdlib>
#endif

namespace sora {;
template<typename T>
class Singleton {
public:
	static T& GetInstance()
	{
		if(ctx_ == NULL) {
			ctx_ = new T;
		}
		return *ctx_;
	}

	static void DestroyInstance()
	{
		if(ctx_ != NULL) {
			delete(ctx_);
			ctx_ = NULL;
		}
	}
	static bool IsCreated()
	{
		if(ctx_ == NULL) {
			return false;
		} else {
			return true;
		}

	}
protected:
	Singleton() 
	{
		IUASSERT(ctx_ == NULL);
		long offset = (long)(T*)1 - (long)(Singleton*)(T*)1;
		ctx_ = (T*)(long)(this + offset);
	}
	~Singleton()
	{
		IUASSERT(ctx_ != NULL);
		ctx_ = NULL;
	}
	Singleton(const Singleton &o);
	Singleton operator=(const Singleton &o);

	static T *ctx_;
};

template<typename T>
T* Singleton<T>::ctx_ = NULL;
}

//#endif