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
	// ���� ���� �Ӽ�
	Graphics* graphics;		// Graphics ��ü�� ����Ű�� ������
	Input* input;			// Input ��ü�� ����Ű�� ������
	HWND hwnd;				// ������ �ڵ�
	HRESULT hr;				// ǥ�� ��ȯ Ÿ��
	LARGE_INTEGER timeStart;
	LARGE_INTEGER timeEnd;
	LARGE_INTEGER timerFreq;
	float frameTime;
	float fps;
	DWORD sleepTime;
	bool paused;
	bool initialized;

public:
	// ������
	Game();
	// �Ҹ���
	virtual ~Game();
	
	//========================================================== ����Լ� =============================================================
	
	// ������ �޼��� �ڵ鷯
	LRESULT messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
	// ���� �ʱ�ȭ
	virtual void initialize(HWND hwnd);
	
	// Winmain�� ���� �޼��� �������� �ݺ������� run  �Լ��� ȣ��ȴ�.
	virtual void run(HWND);

	// ========== �׷��� ����̽��� �ν�Ʈ ���°� �Ǿ��� �� ȣ�� ==================

	// ����� ��� ���� �޸𸮸� �����ϰ� �׷��� ����̽��� ����
	virtual void releaseAll();

	// ��� ǥ���� ������ϰ� ��� ��ü�� ����
	virtual void resetAll();

	// ����� ��� �޸� ����
	virtual void deleteAll();

	// ���Ӿ������� ������
	virtual void renderGame();

	// �ν�Ʈ ���°� �� �׷��� ����̽��� ó���Ѵ�.
	virtual void handleLostGraphicsDevice();

	//==========================================================================

	// Graphics ��ü�� ����Ű�� �����͸� ��ȯ 
	Graphics* getGraphics() { return graphics;  }

	//Input ��ü�� ����Ű�� �����͸� ��ȯ
	Input* getInput() { return input;}

	// ���� ����
	void exitGame() { PostMessage(hwnd, WM_DESTROY, 0, 0); }

	// =========================================== ���������Լ� ================================================
	// ���� ������ ����
	virtual void update() = 0;

	// AI ��� 
	virtual void ai() = 0;

	// �浹 Ȯ��
	virtual void collision() = 0;

	// �׷��� ����
	virtual void render() = 0;

};

#endif