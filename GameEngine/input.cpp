// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// input.cpp v1.6
// Last modified Feb-7-2013

#include "input.h"

// ================================================================================================
// 디폴트 생성자
// ================================================================================================
Input::Input()
{
	// 배열 초기화
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
// 소멸자
// ================================================================================================
Input::~Input()
{
    if (mouseCaptured)
        ReleaseCapture();       // 마우스 캡처 해제
}

// ================================================================================================
// 마우스 입력을 초기화 
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
// 키 누름
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
// 키 똄
// ================================================================================================
void Input::keyUp(WPARAM wParam)
{
    if (wParam < inputNS::KEYS_ARRAY_LEN)
    {
        keysDown[wParam] = false;
    }
}

// ================================================================================================
// 텍스트 입력
// ================================================================================================
void Input::keyIn(WPARAM wParam)
{
    // 새로운 줄의 시작일때,
    if (newLine)
    {
        textIn.clear();
        newLine = false;
    }
    // 백스페이스 일때
    if (wParam == '\b')
    {
        if (textIn.length() > 0)
            textIn.erase(textIn.size() - 1);
    }
    // 기본 - textIn에 입력 문자 추가
    else
    {
        textIn += wParam;
        charIn = wParam;
    }
    // 엔터 - 새로운 줄 시작
    if ((char)wParam == '\r')
        newLine = true;
}

// ================================================================================================
// 특정 가상키를 눌렀을 경우,
// ================================================================================================
bool Input::isKeyDown(UCHAR vkey) const
{
    if (vkey < inputNS::KEYS_ARRAY_LEN)
        return keysDown[vkey];
    else
        return false;
}

// ================================================================================================
// 가장 최근 프레임에 특정 가상 키를 눌렀다면 true, 각 프레임 끝에 키 누름에 관한 정보 지워짐
// ================================================================================================
bool Input::wasKeyPressed(UCHAR vkey) const
{
    if (vkey < inputNS::KEYS_ARRAY_LEN)
        return keysPressed[vkey];
    else
        return false;
}

// ================================================================================================
// 가장 최근 프레임에 아무 키를 눌렀다면 true, 각 프레임 끝에 키 누름에 관한 정보 지워짐
// ================================================================================================
bool Input::anyKeyPressed() const
{
    for (size_t i = 0; i < inputNS::KEYS_ARRAY_LEN; i++)
        if (keysPressed[i] == true)
            return true;
    return false;
}

// ================================================================================================
// 특정 키 누름에 관한 정보 지우기
// ================================================================================================
void Input::clearKeyPress(UCHAR vkey)
{
    if (vkey < inputNS::KEYS_ARRAY_LEN)
        keysPressed[vkey] = false;
}

//=============================================================================
// 특정 입력 버퍼 제거 - input.h 에 값 있음
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
// 화면상의 마우스 위치 읽기
//=============================================================================
void Input::mouseIn(LPARAM lParam)
{
    mouseX = GET_X_LPARAM(lParam);
    mouseY = GET_Y_LPARAM(lParam);
}

//=============================================================================
// 처리되지 않은 마우스 데이터를 읽어 mouseRawX, mouseRawY에 저장
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