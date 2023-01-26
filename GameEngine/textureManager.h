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
	LP_TEXTURE	texture;		// 텍스처 인터페이스
	const char* file;
	Graphics* graphics;
	bool	initialized;
	HRESULT	hr;
	
public:
	// 생성자
	TextureManager();

	// 소멸자
	virtual ~TextureManager();

	// get 텍스터 포인터
	LP_TEXTURE getTexture() const { return texture; }

	// get 텍스터 width
	UINT getWidth() const { return width; }

	// get 텍스처 height
	UINT getHeight() const { return height; }

	// ==============================================================
	// textureManager 초기화 - 디스크로부터 텍스처 파일을 불러온다.
	virtual bool initialize(Graphics* g, const char* file);

	//  ===============================================================
	
	// release resources  - 그래픽 디바이스가 로스트일 때 호출
	virtual void onLostDevice();

	// restore resource - 그래픽 디바이스가 리셋될 때 호출
	virtual void onResetDevice();

};

#endif