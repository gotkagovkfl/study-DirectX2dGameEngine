// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// input.cpp v1.6
// Last modified Feb-7-2013

#include "input.h"

// ================================================================================================
// ����Ʈ ������
// ================================================================================================
Input::Input()
{
	// �迭 �ʱ�ȭ
	for (size_t i = 0; i < inputNS::KEYS_ARRAY_LEN; i++)
		keysDown[i] = false;
	for (size_t i = 0; i < inputNS::KEYS_ARRAY_LEN; i++)
		keysPressed[i] = false;

	newLine = true;
	textIn = "";
	charIn = 0;

    // mouse data
    mouseX = 0;                      
    mouseY = 0;                
    mouseRawX = 0;                 
    mouseRawY = 0;                     
    mouseWheel = 0;                    
    mouseLButton = false;              
    mouseMButton = false;               
    mouseRButton = false;               
    mouseX1Button = false;              
    mouseX2Button = false;              

}

// ================================================================================================
// �Ҹ���
// ================================================================================================
Input::~Input()
{
    if (mouseCaptured)
        ReleaseCapture();       // ���콺 ĸó ����
}

// ================================================================================================
// ���콺 �Է��� �ʱ�ȭ 
// ================================================================================================
void Input::initialize(HWND hwnd, bool capture)
{
    try
    {
        mouseCaptured = capture;

        Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
        Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
        Rid[0].dwFlags = RIDEV_INPUTSINK;
        Rid[0].hwndTarget = hwnd;
        RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]));
        
        if (mouseCaptured)
            SetCapture(hwnd);
    }
    catch (...)
    {
        throw (GameError(gameErrorNS::FATAL_ERROR, "Error initializing input system"));
    }
}

// ================================================================================================
// Ű ����
// ================================================================================================
void Input::keyDown(WPARAM wParam)
{
    if (wParam < inputNS::KEYS_ARRAY_LEN)
    {
        keysDown[wParam] = true;
        keysPressed[wParam] = true;
    }
}

// ================================================================================================
// Ű �E
// ================================================================================================
void Input::keyUp(WPARAM wParam)
{
    if (wParam < inputNS::KEYS_ARRAY_LEN)
    {
        keysDown[wParam] = false;
    }
}

// ================================================================================================
// �ؽ�Ʈ �Է�
// ================================================================================================
void Input::keyIn(WPARAM wParam)
{
    // ���ο� ���� �����϶�,
    if (newLine)
    {
        textIn.clear();
        newLine = false;
    }
    // �齺���̽� �϶�
    if (wParam == '\b')
    {
        if (textIn.length() > 0)
            textIn.erase(textIn.size() - 1);
    }
    // �⺻ - textIn�� �Է� ���� �߰�
    else
    {
        textIn += wParam;
        charIn = wParam;
    }
    // ���� - ���ο� �� ����
    if ((char)wParam == '\r')
        newLine = true;
}

// ================================================================================================
// Ư�� ����Ű�� ������ ���,
// ================================================================================================
bool Input::isKeyDown(UCHAR vkey) const
{
    if (vkey < inputNS::KEYS_ARRAY_LEN)
        return keysDown[vkey];
    else
        return false;
}

// ================================================================================================
// ���� �ֱ� �����ӿ� Ư�� ���� Ű�� �����ٸ� true, �� ������ ���� Ű ������ ���� ���� ������
// ================================================================================================
bool Input::wasKeyPressed(UCHAR vkey) const
{
    if (vkey < inputNS::KEYS_ARRAY_LEN)
        return keysPressed[vkey];
    else
        return false;
}

// ================================================================================================
// ���� �ֱ� �����ӿ� �ƹ� Ű�� �����ٸ� true, �� ������ ���� Ű ������ ���� ���� ������
// ================================================================================================
bool Input::anyKeyPressed() const
{
    for (size_t i = 0; i < inputNS::KEYS_ARRAY_LEN; i++)
        if (keysPressed[i] == true)
            return true;
    return false;
}

// ================================================================================================
// Ư�� Ű ������ ���� ���� �����
// ================================================================================================
void Input::clearKeyPress(UCHAR vkey)
{
    if (vkey < inputNS::KEYS_ARRAY_LEN)
        keysPressed[vkey] = false;
}

//=============================================================================
// Ư�� �Է� ���� ���� - input.h �� �� ����
//=============================================================================
void Input::clear(UCHAR what)
{
    if (what & inputNS::KEYS_DOWN)       // if clear keys down
    {
        for (size_t i = 0; i < inputNS::KEYS_ARRAY_LEN; i++)
            keysDown[i] = false;
    }
    if (what & inputNS::KEYS_PRESSED)    // if clear keys pressed
    {
        for (size_t i = 0; i < inputNS::KEYS_ARRAY_LEN; i++)
            keysPressed[i] = false;
    }
    if (what & inputNS::MOUSE)           // if clear mouse
    {
        mouseX = 0;
        mouseY = 0;
        mouseRawX = 0;
        mouseRawY = 0;
        mouseWheel = 0;
    }
    if (what & inputNS::TEXT_IN)
    {
        clearTextIn();
        clearCharIn();
    }
}

//=============================================================================
// ȭ����� ���콺 ��ġ �б�
//=============================================================================
void Input::mouseIn(LPARAM lParam)
{
    mouseX = GET_X_LPARAM(lParam);
    mouseY = GET_Y_LPARAM(lParam);
}

//=============================================================================
// ó������ ���� ���콺 �����͸� �о� mouseRawX, mouseRawY�� ����
//=============================================================================
void Input::mouseRawIn(LPARAM lParam)
{
    UINT dwSize = 40;
    static BYTE lpb[40];

    GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));

    RAWINPUT* raw = (RAWINPUT*)lpb;

    if (raw->header.dwType == RIM_TYPEMOUSE)
    {
        mouseRawX = raw->data.mouse.lLastX;
        mouseRawY = raw->data.mouse.lLastY;
    }
}

//=============================================================================
// Reads mouse wheel movement expressed in multiples of WHEEL_DELTA, which
// is 120. A positive value indicates that the wheel was rotated away from the
// user, a negative value indicates that the wheel was rotated toward the user.
//=============================================================================
void Input::mouseWheelIn(WPARAM wParam)
{
    mouseWheel = GET_WHEEL_DELTA_WPARAM(wParam);
}