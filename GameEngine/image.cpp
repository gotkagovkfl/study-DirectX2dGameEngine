// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Image.cpp v1.0

#include "image.h"


//=============================================================================
// �⺻ ������
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
// �Ҹ���
//=============================================================================
Image::~Image()
{}


//=============================================================================
// Image �ʱ�ȭ
//=============================================================================
bool Image::initialize(Graphics* g, int width, int height, int ncols, TextureManager* textureM)
{
    try
    {
        graphics = g;
        textureManager = textureM;

        // �ؽ�ó ������ �������� ũ�� ����
        spriteData.texture = textureManager->getTexture();
        if (width == 0)
            width = textureManager->getWidth();
        spriteData.width = width;
        if (height == 0)
            height = textureManager->getHeight();
        spriteData.height = height;

        // cols ����
        cols = ncols;
        if (cols == 0)
            cols = 1;

        // currentFrame�� �׸��� ���� spriteData.rect�� �����Ѵ�. 
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
// currentFrame�� �����ϰ� setRect�� ȣ���Ѵ�.
//=============================================================================
void Image::setCurrentFrame(int c)
{
    if (c >= 0)
    {
        currentFrame = c;
        animComplete = false;
        setRect();          
    }
}

//=============================================================================
// currentFrame�� ���� �ؽ�ó �̹����� ��踦 ������ ȭ�鿡 �������� �̹����� �����Ѵ�.
//=============================================================================
inline void Image::setRect()
{
    spriteData.rect.left = (currentFrame % cols) * spriteData.width;
    spriteData.rect.right = spriteData.rect.left + spriteData.width;
    spriteData.rect.top = (currentFrame / cols) * spriteData.height;
    spriteData.rect.bottom = spriteData.rect.top + spriteData.height;
}


//=============================================================================
// color�� ���ͷ� ����� �̹����� �׸���. 
//=============================================================================
void Image::draw(COLOR_ARGB color)
{
    // �̹����� �׸��� �ʴ� ���
    if (!visible || graphics == NULL)
        return;

    // onReset()�� ȣ��� ��츦 ����� ���ο� �ؽ�ó�� �����´�.
    spriteData.texture = textureManager->getTexture();

    // ��������Ʈ �׸���
    if (color == graphicsNS::FILTER)        // ���ͻ��
        graphics->drawSprite(spriteData, colorFilter);
    else                                    // color�� ���ͷ� ��� 
        graphics->drawSprite(spriteData, color); 
}

//=============================================================================
// color�� ���ͷ� ����� �̹����� �׸���. 
//=============================================================================
void Image::draw(SpriteData sd, COLOR_ARGB color)
{
    // �̹����� �׸��� �ʴ� ���
    if (!visible || graphics == NULL)
        return;

    // �ؽ�ó�� �����ϱ� ���� �̹����� rect�� ����Ѵ�.
    sd.rect = spriteData.rect;
    // onReset()�� ȣ��� ��츦 ����� ���ο� �ؽ�ó�� �����´�.
    sd.texture = textureManager->getTexture();

    // ��������Ʈ �׸���
    if (color == graphicsNS::FILTER)        // ���ͻ��
        graphics->drawSprite(sd, colorFilter);
    else                                    // color�� ���ͷ� ��� 
        graphics->drawSprite(sd, color);
}

//=============================================================================
// currentFrame�� ���� �ؽ�ó �̹����� ��踦 ������ ȭ�鿡 �������� �̹����� �����Ѵ�.
//=============================================================================
void Image::update(float frameTime)
{
    // �ִϸ��̼� ���� ���� ���� 
    if (endFrame - startFrame > 0) 
    {
        animTimer += frameTime;
        if (animTimer > frameDelay) // �ִϸ��̼��� ��ü�� ������
        {
            animTimer -= frameDelay;
            currentFrame++;
            if (currentFrame < startFrame || currentFrame > endFrame)  
            {
                if (loop == true)   // �ݺ� �ִϸ��̼��� ���
                    currentFrame = startFrame;
                else                // �ݺ����� �ʴ� �ִϸ��̼��� ���
                {
                    currentFrame = endFrame;
                    animComplete = true;
                }
            }
            // currFrame�� �ٲ�� �̹��� ��ü
            setRect();      
        }
    }
}



