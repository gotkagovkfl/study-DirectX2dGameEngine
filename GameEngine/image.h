// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// image.h v1.2

#ifndef _IMAGE_H                // Prevent multiple definitions if this 
#define _IMAGE_H                // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "textureManager.h"
#include "constants.h"

class Image
{
protected:
	Graphics* graphics;
	TextureManager* textureManager;

	SpriteData spriteData;
	COLOR_ARGB colorFilter;
	int		cols;
	int		startFrame;
	int		endFrame;
	int		currentFrame;
	float	frameDelay;
	float	animTimer;
	HRESULT	hr;
	bool	loop;
	bool	visible;
	bool	initialized;
	bool	animComplete;

public:
	// 생성자
	Image();
	// 소멸자
	virtual ~Image();

	// ================================== Getter =======================================

    const virtual SpriteData& getSpriteInfo() { return spriteData; }

    virtual bool  getVisible() { return visible; }

    virtual float getX() { return spriteData.x; }
    virtual float getY() { return spriteData.y; }

    virtual float getScale() { return spriteData.scale; }

    virtual int   getWidth() { return spriteData.width; }
    virtual int   getHeight() { return spriteData.height; }


    virtual float getCenterX() { return spriteData.x + spriteData.width / 2 * getScale(); }
    virtual float getCenterY() { return spriteData.y + spriteData.height / 2 * getScale(); }

    virtual float getDegrees() { return spriteData.angle * (180.0f / (float)PI); }

    virtual float getRadians() { return spriteData.angle; }

    virtual float getFrameDelay() { return frameDelay; }
    virtual int   getStartFrame() { return startFrame; }
    virtual int   getEndFrame() { return endFrame; }
    virtual int   getCurrentFrame() { return currentFrame; }

    virtual RECT  getSpriteDataRect() { return spriteData.rect; }

    virtual bool  getAnimationComplete() { return animComplete; }

    virtual COLOR_ARGB getColorFilter() { return colorFilter; }

	// ================================== Setter ======================================= 
    
    virtual void setX(float newX) { spriteData.x = newX; }
    virtual void setY(float newY) { spriteData.y = newY; }

    virtual void setScale(float s) { spriteData.scale = s; }

    virtual void setDegrees(float deg) { spriteData.angle = deg * ((float)PI / 180.0f); }       // 도 반환 - 위쪽부터 시계방향
    virtual void setRadians(float rad) { spriteData.angle = rad; }                              // 라디안  - 위쪽부터 시계방향

    virtual void setVisible(bool v) { visible = v; }

    virtual void setFrameDelay(float d) { frameDelay = d; }
    virtual void setFrames(int s, int e) { startFrame = s; endFrame = e; }
    //virtual void setCurrentFrame(int c);

    //virtual void setRect();

    virtual void setSpriteDataRect(RECT r) { spriteData.rect = r; }

    virtual void setLoop(bool lp) { loop = lp; }

    virtual void setAnimationComplete(bool a) { animComplete = a; };

    virtual void setColorFilter(COLOR_ARGB color) { colorFilter = color; }          // 아무 변화를 주고 싶지 않다면 WHITE

    virtual void setTextureManager(TextureManager* textureM) { textureManager = textureM; }

    // ================================= 그 외 함수 ===========================================
    
    // Image 객체 초기화
    virtual bool initialize(Graphics* g, int width, int height, int ncols, TextureManager* textureM);

    // flip
    virtual void flipHorizontal(bool flip) { spriteData.flipHorizontal = flip; }
    virtual void flipVertical(bool flip) { spriteData.flipVertical = flip; }

    // 이미지 그리기
    virtual void draw(COLOR_ARGB color = graphicsNS::WHITE);
    virtual void draw(SpriteData sd, COLOR_ARGB color = graphicsNS::WHITE);

    // 이미지 애니메이션 업데이트 
    //virtual void update(float frameTime);
};


#endif
