#include <omp.h>
#include <windows.h>
#include <stdio.h>

class Timer{
private:
	HWND hwnd;
	int framesCount;
	int REFRESH_FRAMES_COUNT;
	double startTime;
	double endTime;
	wchar_t buff[256];

public:
	Timer::Timer(HWND hwnd, int frames){
		this->hwnd = hwnd;
		this->startTime = 0;
		this->endTime = 0;
		this->framesCount = 0;
		this->REFRESH_FRAMES_COUNT = frames;
	}

	void Timer::countTime(){
		if (this->startTime == 0){
			this->startTime = omp_get_wtime();
			this->framesCount = 1;
		}
		else{
			framesCount++;
		}
	}
	void Timer::refreshWindowIfShouldBe(wchar_t* windowType){
		if (framesCount == REFRESH_FRAMES_COUNT){
			this->endTime = omp_get_wtime();
			swprintf(buff, 256,L"%s %d frames displayed in %lf", windowType, this->REFRESH_FRAMES_COUNT, this->endTime - this->startTime);
			SetWindowText(hwnd, buff);
			this->startTime = 0;
		}
	}
};
