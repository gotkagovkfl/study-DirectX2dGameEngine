// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 4 spacewar.h v1.0

#ifndef _SPACEWAR_H
#define _SPACEWAR_H
#define WIN32_LEAN_AND_MEAN

#include "game.h"

class Spacewar : public Game
{
private:
	
public :
	// 持失切
	Spacewar();

	// 社瑚切
	virtual ~Spacewar();

	//
	void initialize(HWND hwnd);
	void update();
	void ai();
	void collision();
	void render();
	void releaseAll();
	void resetAll();
};

#endif
