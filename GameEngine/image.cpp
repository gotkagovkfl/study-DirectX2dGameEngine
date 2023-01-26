// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Image.cpp v1.0

#include "image.h"


//=============================================================================
// 기본 생성자
//=============================================================================
Image::Image()
{
    initialized = false;            // set true when successfully initialized
    spriteData.width = 2;
    spriteData.height = 2;
    spriteData.x = 0.0;
    spriteData.y = 0.0;
    spriteData.scale = 1.0;
    spriteData.angle = 0.0;
    spriteData.rect.left = 0;       // used to select one frame from multi-frame image
    spriteData.rect.top = 0;
    spriteData.rect.right = spriteData.width;
    spriteData.rect.bottom = spriteData.height;
    spriteData.texture = NULL;      // the sprite texture (picture)
    spriteData.flipHorizontal = false;
    spriteData.flipVertical = false;
    cols = 1;
    textureManager = NULL;
    startFrame = 0;
    endFrame = 0;
    currentFrame = 0;
    frameDelay = 1.0;               // default to 1 second per frame of animation
    animTimer = 0.0;
    visible = true;                 // the image is visible
    loop = true;                    // loop frames
    animComplete = false;
    graphics = NULL;                // link to graphics system
    colorFilter = graphicsNS::WHITE; // WHITE for no change
}

//=============================================================================
// 소멸자
//=============================================================================
Image::~Image()
{}


//=============================================================================
// Image 초기화
//=============================================================================
bool Image::initialize(Graphics* g, int width, int height, int ncols, TextureManager* textureM)
{
    try
    {
        graphics = g;
        textureManager = textureM;

        // 텍스처 정보를 바탕으로 크기 설정
        spriteData.texture = textureManager->getTexture();
        if (width == 0)
            width = textureManager->getWidth();
        spriteData.width = width;
        if (height == 0)
            height = textureManager->getHeight();
        spriteData.height = height;

        // cols 설정
        cols = ncols;
        if (cols == 0)
            cols = 1;

        // currentFrame을 그리기 위해 spriteData.rect을 구성한다. 
        spriteData.rect.left = (currentFrame % cols) * spriteData.width;
        spriteData.rect.right = spriteData.rect.left + spriteData.width;
        spriteData.rect.top = (currentFrame / cols) * spriteData.height;
        spriteData.rect.bottom = spriteData.rect.top + spriteData.height;
    }
    catch (...) { return false; }

    initialized = true;
    return true;
}


//=============================================================================
// color를 필터로 사용해 이미지를 그린다. 
//=============================================================================
void Image::draw(COLOR_ARGB color)
{
    // 이미지를 그리지 않는 경우
    if (!visible || graphics == NULL)
        return;

    // onReset()이 호출된 경우를 대비해 새로운 텍스처를 가져온다.
    spriteData.texture = textureManager->getTexture();

    // 스프라이트 그리기
    if (color == graphicsNS::FILTER)        // 필터사용
        graphics->drawSprite(spriteData, colorFilter);
    else                                    // color를 필터로 사용 
        graphics->drawSprite(spriteData, color); 
}

//=============================================================================
// color를 필터로 사용해 이미지를 그린다. 
//=============================================================================
void Image::draw(SpriteData sd, COLOR_ARGB color)
{
    // 이미지를 그리지 않는 경우
    if (!visible || graphics == NULL)
        return;

    // 텍스처를 선택하기 위해 이미지의 rect를 사용한다.
    sd.rect = spriteData.rect;
    // onReset()이 호출된 경우를 대비해 새로운 텍스처를 가져온다.
    sd.texture = textureManager->getTexture();

    // 스프라이트 그리기
    if (color == graphicsNS::FILTER)        // 필터사용
        graphics->drawSprite(spriteData, colorFilter);
    else                                    // color를 필터로 사용 
        graphics->drawSprite(spriteData, color);
}