// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// textureManager.h v1.0

#ifndef _TEXTUREMANAGER_H       // Prevent multiple definitions if this 
#define _TEXTUREMANAGER_H       // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "graphics.h"
#include "constants.h"

class TextureManager
{
private:
	UINT	width;
	UINT	height;
	LP_TEXTURE	texture;		// �ؽ�ó �������̽�
	const char* file;
	Graphics* graphics;
	bool	initialized;
	HRESULT	hr;
	
public:
	// ������
	TextureManager();

	// �Ҹ���
	virtual ~TextureManager();

	// get �ؽ��� ������
	LP_TEXTURE getTexture() const { return texture; }

	// get �ؽ��� width
	UINT getWidth() const { return width; }

	// get �ؽ�ó height
	UINT getHeight() const { return height; }

	// ==============================================================
	// textureManager �ʱ�ȭ - ��ũ�κ��� �ؽ�ó ������ �ҷ��´�.
	virtual bool initialize(Graphics* g, const char* file);

	//  ===============================================================
	
	// release resources  - �׷��� ����̽��� �ν�Ʈ�� �� ȣ��
	virtual void onLostDevice();

	// restore resource - �׷��� ����̽��� ���µ� �� ȣ��
	virtual void onResetDevice();

};

#endif