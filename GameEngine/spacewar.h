// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 4 spacewar.h v1.0

#ifndef _SPACEWAR_H
#define _SPACEWAR_H
#define WIN32_LEAN_AND_MEAN

#include "game.h"
#include "textureManager.h"
#include "image.h"

class Spacewar : public Game
{
private:
	// ���� ������
	TextureManager nebulaTexture;
	TextureManager planetTexture;
	TextureManager shipTexture;
	Image planet;
	Image nebula;
	Image ship;
	
public :
	// ������
	Spacewar();

	// �Ҹ���
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
