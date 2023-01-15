// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// gameError.h v1.3
// Error class thrown by game engine.

/*
 게임 엔진 실행 중 발생하는 오류 내용을 표시한다.
 에러 발생시 gameError 객체를 생성함.
 exception로부터 기능을 상속받음.
*/

#ifndef _GAMEERROR_H            // Prevent multiple definitions if this 
#define _GAMEERROR_H            // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

class GameError;

#include <string>
#include <exception>

namespace gameErrorNS
{
    // Error codes
    // Negative numbers are fatal errors that may require the game to be shutdown.
    // Positive numbers are warnings that do not require the game to be shutdown.
    const int FATAL_ERROR = -1;
    const int WARNING = 1;
}

// Game Error class. Thrown when an error is detected by the game engine.
// Inherits from std::exception
class GameError : public std::exception
{
private:
    int     errorCode; 
    std::string message;

public:
    // 기본 생성자
    GameError() :errorCode(gameErrorNS::FATAL_ERROR), message("Undefined Error in game.") {}
    // 복사 생성자
    GameError(const GameError& e) :std::exception(e), errorCode(e.errorCode), message(e.message) {}
    // 매개변수 생성자
    GameError(int code, const std::string& s) :errorCode(code), message(s) {}

    // 소멸자
    virtual ~GameError() {};

    // --------------------------------------------------
    // 할당 연산자 (=)
    GameError& operator= (const GameError& rhs)
    {
        std::exception::operator=(rhs);
        this->errorCode = rhs.errorCode;
        this->message = rhs.message;
    }
    // 부모 클래스로부터 재정의 
    virtual const char* what() const { return this->getMessage(); }
    const char* getMessage() const { return message.c_str(); }
    int getErrorCode() const { return errorCode; }
};

#endif
