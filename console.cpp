#include <iostream>
#include <windows.h>
#include "console.h"

using namespace std;
using namespace con;

/// Global console output handler
HANDLE stdOutputHandle = GetStdHandle( STD_OUTPUT_HANDLE );

/// System time (ms)
unsigned int con::ticks()
{
    return GetTickCount();
}

///************************************************************
///
/// Stream operators overloads
///
///************************************************************

/// Apply modifier to output stream
std::ostream& operator<<( std::ostream &out, const con::modifier &mod )
{
    mod.pass( out );
    return out;
}

///************************************************************
///
/// Moddifiers definitions
///
///************************************************************

/// Pass dimmension modifier
void dimm::pass( std::ostream &out ) const
{
    SMALL_RECT windowSize = {
        0, 0,
        m_width - 1,
        m_height - 1,
    };

    SetConsoleWindowInfo( stdOutputHandle, true, &windowSize );
    SetConsoleScreenBufferSize( stdOutputHandle, *((COORD*)&m_width) );

    // show/hide cursor
    CONSOLE_CURSOR_INFO curInfo = {
        100, !m_hideCursor
    };

    SetConsoleCursorInfo( stdOutputHandle, &curInfo );
}

/// Pass output color modifier
void color::pass( std::ostream &out ) const
{
    SetConsoleTextAttribute( stdOutputHandle, m_color );
}

/// Pass console cursor modifier
void cursor::pass( std::ostream &out ) const
{
    SetConsoleCursorPosition( stdOutputHandle, *((COORD*)&m_x) );
}

/// Pass console clear screen modifier
void cls::pass( std::ostream &out ) const
{
    CONSOLE_SCREEN_BUFFER_INFO screenInfo;
    GetConsoleScreenBufferInfo( stdOutputHandle, &screenInfo );

    long unsigned int length = screenInfo.dwSize.X * screenInfo.dwSize.Y,
                      outLength = 0;

    COORD startPos = { 0, 0 };

    FillConsoleOutputCharacterA(
        stdOutputHandle, m_fillBy, length, startPos, &outLength );

    FillConsoleOutputAttribute(
        stdOutputHandle, m_attr, length, startPos, &outLength );
}

///************************************************************
///
/// Keyboard interface
///
///************************************************************

/// Retrieve the key state
bool kb::keyDown( int vk )
{
    return GetKeyState( vk )&0x8000;
}










