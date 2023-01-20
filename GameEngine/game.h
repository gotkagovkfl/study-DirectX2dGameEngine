// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 4 game.h v1.1

#ifndef _GAME_H
#define _GAME_H
#define WIN32_LEAN_AND_MEAN

class Game;

#include <windows.h>
#include <Mmsystem.h>
#include "graphics.h"
#include "input.h"
#include "constants.h"
#include "gameError.h"

class Game
{
protected:
	// 게임 공통 속성
	Graphics* graphics;		// Graphics 객체를 가리키는 포인터
	Input* input;			// Input 객체를 가리키는 포인터
	HWND hwnd;				// 윈도우 핸들
	HRESULT hr;				// 표준 반환 타입
	LARGE_INTEGER timeStart;
	LARGE_INTEGER timeEnd;
	LARGE_INTEGER timerFreq;
	float frameTime;
	float fps;
	DWORD sleepTime;
	bool paused;
	bool initialized;

public:
	// 생성자
	Game();
	// 소멸자
	virtual ~Game();
	
	//========================================================== 멤버함수 =============================================================
	
	// 윈도우 메세지 핸들러
	LRESULT messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
	// 게임 초기화
	virtual void initialize(HWND hwnd);
	
	// Winmain의 메인 메세지 루프에서 반복적으로 run  함수가 호출된다.
	virtual void run(HWND);

	// ========== 그래픽 디바이스가 로스트 상태가 되었을 때 호출 ==================

	// 예약된 모든 비디오 메모리를 해제하고 그래픽 디바이스를 리셋
	virtual void releaseAll();

	// 모든 표면을 재생성하고 모든 개체를 리셋
	virtual void resetAll();

	// 예약된 모든 메모리 삭제
	virtual void deleteAll();

	// 게임아이템을 렌더링
	virtual void renderGame();

	// 로스트 상태가 된 그래픽 디바이스를 처리한다.
	virtual void handleLostGraphicsDevice();

	//==========================================================================

	// Graphics 객체를 가리키는 포인터를 반환 
	Graphics* getGraphics() { return graphics;  }

	//Input 객체를 가리키는 포인터를 반환
	Input* getInput() { return input;}

	// 게임 종료
	void exitGame() { PostMessage(hwnd, WM_DESTROY, 0, 0); }

	// =========================================== 순수가상함수 ================================================
	// 게임 아이템 갱신
	virtual void update() = 0;

	// AI 계산 
	virtual void ai() = 0;

	// 충돌 확인
	virtual void collision() = 0;

	// 그래픽 렌더
	virtual void render() = 0;

};

#endif