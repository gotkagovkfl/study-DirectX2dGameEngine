// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// input.h v1.8
// Last modified April-12-2013

#ifndef _INPUT_H                
#define _INPUT_H                
#define WIN32_LEAN_AND_MEAN

class Input;

#include <windows.h>
#include <windowsx.h>
#include <string>
#include "constants.h"
#include "gameError.h"



// for high-definition mouse
#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC      ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE     ((USHORT) 0x02)
#endif

namespace inputNS
{
    const int KEYS_ARRAY_LEN = 256;     // size of key arrays

    // what values for clear(), bit flag
    const UCHAR KEYS_DOWN = 1;
    const UCHAR KEYS_PRESSED = 2;
    const UCHAR MOUSE = 4;
    const UCHAR TEXT_IN = 8;
    const UCHAR KEYS_MOUSE_TEXT = KEYS_DOWN + KEYS_PRESSED + MOUSE + TEXT_IN;
}

class Input
{
private:
    bool keysDown[inputNS::KEYS_ARRAY_LEN];         // Ư�� Ű�� ���� ��� true
    bool keysPressed[inputNS::KEYS_ARRAY_LEN];      // ���� ������ �����ϴ� ���� Ư�� Ű�� ������ �ִ� ��� true

    std::string textIn;                     // ����ڰ� �Է��� �ؽ�Ʈ
    char charIn;                            // �������� �Է��� ����
    bool newLine;                           // ���ο� ���� ������ �� true

    int mouseX, mouseY;                     // ȭ����� ���콺 ��ġ
    int mouseRawX, mouseRawY;               // ������ ���콺 ������
    int mouseWheel;                         // ���콺 �� ������

    RAWINPUTDEVICE Rid[1];                  // ������ ���콺��
    bool mouseCaptured;                     // ���콺�� ĸó�� ��� true
    bool mouseLButton;                      // ���� ���� ��� true
    bool mouseMButton;
    bool mouseRButton;
    bool mouseX1Button;
    bool mouseX2Button;
   
public:
    // ������
    Input();

    // �Ҹ���
    virtual ~Input();

    // ���콺 �Է� �ʱ�ȭ
    void initialize(HWND hwnd, bool capture);

    // Save key down state
    void keyDown(WPARAM);
    // Save key up state
    void keyUp(WPARAM);
    // Save the char just entered in textIn string
    void keyIn(WPARAM);

    // Returns true if the specified VIRTUAL KEY is down, otherwise false.
    bool isKeyDown(UCHAR vkey) const;
    // Return true if the specified VIRTUAL KEY has been pressed in the most recent frame. // Key presses are erased at the end of each frame.
    bool wasKeyPressed(UCHAR vkey) const;
    // Return true if any key was pressed in the most recent frame. // Key presses are erased at the end of each frame. 
    bool anyKeyPressed() const;

    // Ư�� Ű ������ ���� ���� �����
    void clearKeyPress(UCHAR vkey);
    // Clear specified input buffers 
    void clear(UCHAR what);
    // Clears key, mouse and text input data
    void clearAll() { clear(inputNS::KEYS_MOUSE_TEXT); }
    // Clear text input buffer
    void clearTextIn() { textIn.clear(); }
    // Clear last character entered
    void clearCharIn() { charIn = 0; }

    // Return text input as a string
    std::string getTextIn() { return textIn; }
    // Set text input string
    void setTextIn(std::string str) { textIn = str; }
    // Return last character entered
    char getCharIn() { return charIn; }

    // ȭ����� ���콺 ��ġ �б�
    void mouseIn(LPARAM);
    //ó������ ���� ���콺 �����͸� �о� mouseRawX, mouseRawY�� ����
    void mouseRawIn(LPARAM);
    // Reads mouse wheel movement.
    void mouseWheelIn(WPARAM);

    // Save state of mouse button
    void setMouseLButton(bool b) { mouseLButton = b; }
    // Save state of mouse button
    void setMouseMButton(bool b) { mouseMButton = b; }
    // Save state of mouse button
    void setMouseRButton(bool b) { mouseRButton = b; }
    // Save state of mouse button
    void setMouseXButton(WPARAM wParam) 
    {
        mouseX1Button = (wParam & MK_XBUTTON1) ? true : false;
        mouseX2Button = (wParam & MK_XBUTTON2) ? true : false;
    }

    // Return mouse X position
    int  getMouseX()        const { return mouseX; }
    // Return mouse Y position
    int  getMouseY()        const { return mouseY; }
    // Return raw mouse X movement relative to previous position.     // Left is <0, Right is >0
    int  getMouseRawX()
    {
        int rawX = mouseRawX;
        mouseRawX = 0;
        return rawX;
    }
    // Return raw mouse Y movement relative to previous position. // Up is <0, Down is >0
    int  getMouseRawY()
    {
        int rawY = mouseRawY;
        mouseRawY = 0;
        return rawY;
    }
    // Return mouse wheel movement relative to previous position. // Forward is >0, Backward is <0, movement is in multiples of WHEEL_DATA (120).
    int  getMouseWheel()
    {
        int wheel = mouseWheel;
        mouseWheel = 0;
        return wheel;
    }
    // Return state of mouse button.
    bool getMouseLButton()  const { return mouseLButton; }
    bool getMouseMButton()  const { return mouseMButton; }
    bool getMouseRButton()  const { return mouseRButton; }
    bool getMouseX1Button() const { return mouseX1Button; }
    bool getMouseX2Button() const { return mouseX2Button; }
    
};



#endif
