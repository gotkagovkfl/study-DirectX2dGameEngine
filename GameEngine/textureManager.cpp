// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// textureManager.cpp v1.0
// A TextureManager object loads and maintains one texture file. 
// Create a TextureManager object for each texture file in the game.

#include "textureManager.h"

//=============================================================================
// 기본 생성자
//=============================================================================
TextureManager::TextureManager()
{
	texture = NULL;
	width = 0;
	height = 0;
	file = NULL;
	graphics = NULL;
	initialized = false;
}

//=============================================================================
// 소멸자
//=============================================================================
TextureManager::~TextureManager()
{
	SAFE_RELEASE(texture);
}

// ----------------------------------------------------------------------------------------------------------------------

//=============================================================================
// textureManager 초기화 - 디스크로부터 텍스처 파일을 불러온다.
//=============================================================================
bool TextureManager::initialize(Graphics* g, const char* f)
{
	try
	{
		graphics = g;
		file = f;
		hr = graphics->loadTexture(file, TRANSCOLOR, width, height, texture);
		if (FAILED(hr))
		{
			SAFE_RELEASE(texture);
			return false;
		}
	}
	catch (...) { return false; }

	initialized = true;
	return true;
}


// -----------------------------------------------------------------------------------------------------------------

//=============================================================================
// 그래픽 디바이스가 로스트일 때 호출,
//=============================================================================
void TextureManager::onLostDevice()
{
	if (!initialized)
		return;
	SAFE_RELEASE(texture);
}

//=============================================================================
// 그래픽 디바이스가 리셋됐을 때 호출
//=============================================================================
void TextureManager::onResetDevice()
{
	if (!initialized)
		return;
	graphics->loadTexture(file, TRANSCOLOR, width, height, texture);
}