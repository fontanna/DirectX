#define _XM_NO_INTRINSICS_ 
#include <windows.h>
#include <thread>
#include "WindowCreator.cpp"




void thread1(WindowCreator* windowCreator)
{
	windowCreator->initializeHardwareWindow();
	windowCreator->chceckEventMessages(1);
}
void  thread2(WindowCreator* windowCreator)
{
	windowCreator->initializeSoftwareWindow();
	windowCreator->chceckEventMessages(2);
}
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nShowCmd)
{
	WindowCreator* windowCreator = new WindowCreator(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
	std::thread t1(thread1, windowCreator);
	std::thread t2(thread2, windowCreator);
	while (1){
		if (windowCreator->ifAnyWindowClosed)
			ExitProcess(0);
	}
}
