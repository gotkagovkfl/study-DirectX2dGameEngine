// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// textureManager.cpp v1.0
// A TextureManager object loads and maintains one texture file. 
// Create a TextureManager object for each texture file in the game.

#include "textureManager.h"

//=============================================================================
// �⺻ ������
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
// �Ҹ���
//=============================================================================
TextureManager::~TextureManager()
{
	SAFE_RELEASE(texture);
}

// ----------------------------------------------------------------------------------------------------------------------

//=============================================================================
// textureManager �ʱ�ȭ - ��ũ�κ��� �ؽ�ó ������ �ҷ��´�.
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
// �׷��� ����̽��� �ν�Ʈ�� �� ȣ��,
//=============================================================================
void TextureManager::onLostDevice()
{
	if (!initialized)
		return;
	SAFE_RELEASE(texture);
}

//=============================================================================
// �׷��� ����̽��� ���µ��� �� ȣ��
//=============================================================================
void TextureManager::onResetDevice()
{
	if (!initialized)
		return;
	graphics->loadTexture(file, TRANSCOLOR, width, height, texture);
}