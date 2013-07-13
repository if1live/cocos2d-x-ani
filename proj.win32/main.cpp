#include "stdafx.h"
#include "main.h"
#include "AppDelegate.h"
#include "CCEGLView.h"

USING_NS_CC;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // create the application instance
    AppDelegate app;
    CCEGLView* eglView = CCEGLView::sharedOpenGLView();
    eglView->setViewName("HelloCpp");
    //eglView->setFrameSize(480, 320);

	//iphone 4
	//eglView->setFrameSize(640, 960);
	//eglView->setFrameZoomFactor(0.5f);

	eglView->setFrameSize(320, 480);
	eglView->setFrameZoomFactor(1.0f);

    return CCApplication::sharedApplication()->run();
}
