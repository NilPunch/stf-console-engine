#pragma once
#pragma comment(lib, "winmm.lib")

#ifndef UNICODE
#error Please enable UNICODE for your compiler! VS: Project Properties -> General -> \
Character Set -> Use Unicode.
#endif

#include <Windows.h>

#include <string>
#include <assert.h>

#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>

#include <stf/Containers.h>
#include <stf/Random.h>
#include <stf/Vector.h>
#include <stf/Matrix.h>

#define PI 3.141592653589793
#define TWO_PI 6.283185307179586
#define HALF_PI 1.570796326794896

#define CE_INIT_SUCCESS false
#define CE_INIT_FAILURE true
#define CE_UPDATE_CONTINUE false
#define CE_UPDATE_EXIT true
#define CE_EXIT_SUCCESS false
#define CE_EXIT_FAILURE true


// #define CE_NO_FPS_LIMIT // Uncomment this to unlock FPS
#define CE_DEFAULT_FPS_LIM 60.0
#define CE_MOUSE_MAX_BUTTONS 5
#define CE_AVERAGE_FRAMELIST_SIZE 10

// In case this is wheel roll up and down event
#define CE_MOUSE_ADDITIONAL_EVENTS 2

// In this namespace defined a lot of cool (my own) usefull classes 
using namespace stf;

// Using time literals
using namespace std::chrono_literals;

// Character color attributes
enum COLOR : short
{
    FG_BLACK = 0x0000, BG_BLACK = 0x0000,
    FG_DARK_BLUE = 0x0001, BG_DARK_BLUE = 0x0010,
    FG_DARK_GREEN = 0x0002, BG_DARK_GREEN = 0x0020,
    FG_DARK_CYAN = 0x0003, BG_DARK_CYAN = 0x0030,
    FG_DARK_RED = 0x0004, BG_DARK_RED = 0x0040,
    FG_DARK_MAGENTA = 0x0005, BG_DARK_MAGENTA = 0x0050,
    FG_DARK_YELLOW = 0x0006, BG_DARK_YELLOW = 0x0060,
    FG_GREY = 0x0007, BG_GREY = 0x0070,
    FG_DARK_GREY = 0x0008, BG_DARK_GREY = 0x0080,
    FG_BLUE = 0x0009, BG_BLUE = 0x0090,
    FG_GREEN = 0x000A, BG_GREEN = 0x00A0,
    FG_CYAN = 0x000B, BG_CYAN = 0x00B0,
    FG_RED = 0x000C, BG_RED = 0x00C0,
    FG_MAGENTA = 0x000D, BG_MAGENTA = 0x00D0,
    FG_YELLOW = 0x000E, BG_YELLOW = 0x00E0,
    FG_WHITE = 0x000F, BG_WHITE = 0x00F0
};

// UNICODE characters with filled quads
enum QUAD : short
{
    SOLID = 0x2588,
    THREEQUARTERS = 0x2593,
    HALF = 0x2592,
    QUARTER = 0x2591
};

// UNICODE characters for draw command-line shapes
enum BOXSHAPE : short
{
    L_UP = 0x250c, R_UP = 0x2510,
    L_DOWN = 0x2514, R_DOWN = 0x2518,
    L_UP2 = 0x2554, R_UP2 = 0x2557,
    L_DOWN2 = 0x255a, R_DOWN2 = 0x255d,
    HOR = 0x2500, VER = 0x2502,
    HOR2 = 0x2550, VER2 = 0x2551,
    T_R = 0x251c, T_L = 0x2524, T_D = 0x252c, T_U = 0x2534,
    T_R2 = 0x2560, T_L2 = 0x2563, T_D2 = 0x2566, T_U2 = 0x2569,
    B_CROSS = 0x2573,
    A_U = 0x25b2, A_D = 0x25bc, A_L = 0x25c4, A_R = 0x25ba
};

// Keyboard virtual-key codes
enum class KEY : size_t
{
    EMPTY = 0x40,
    A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    K0 = 0x30, K1, K2, K3, K4, K5, K6, K7, K8, K9,
    F1 = 0x70, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
    LEFT = 0x25, UP, RIGHT, DOWN,
    SPACE = 0x20, PAGE_UP, PAGE_DOWN, END, HOME, TAB = 0x09, LSHIFT = 0xA0, RSHIFT, LCTRL, RCTRL, INS = 0x2D, DEL,
    BACKSPACE = 0x08, ESC = 0x1B, ENTER = 0x0D, CTRL, ALT, PAUSE,
    NP0 = 0x60, NP1, NP2, NP3, NP4, NP5, NP6, NP7, NP8, NP9,
    NP_MUL, NP_ADD, NP_SEPARATOR, NP_SUB, NP_DECIMAL
};

// Mouse buttons virtual-key codes
enum class BUTTON : size_t
{
    LEFT = 0x0,
    RIGHT = 0x1,
    MIDDLE = 0x2,
    M1, M2, M3,
    // Last two constant reserved for wheel registration
    WH_FORWARD = CE_MOUSE_MAX_BUTTONS + 0x0,
    WH_BACKWARD = CE_MOUSE_MAX_BUTTONS + 0x1
};

// Cursor images enumeration
enum class CURSOR : short
{
    ARROW = 0x0,
    ARROW_WATCH,
    WAIT_WATCH,
    CROSS,
    TEXT,
    SIZE_WIDTH,
    SIZE_HEIGHT,
    SIZE_DIAGONAL_LEFT,
    SIZE_DIAGONAL_RIGHT,
    SIZE_ALL,
    HAND,
    PIN
};

// Supported languages
enum KB_LAYOUT
{
    RU,
    EN
};

// Struct that handle state of button
struct KeyState
{
    bool Pressed = false;
    bool Released = false;
    bool Held = false;
};

// Struct that handle keys update info & states between frames
struct KeyInfo
{
    union
    {
        size_t Code;
        KEY Key = KEY::EMPTY;
    };
    KeyState State;
};

// Screen buffer pixel data
typedef CHAR_INFO Pixel;

/* You must publicly inheritated from this */
class ConsoleEngine abstract
{
public:
    ConsoleEngine()
    {
        m_AppName = L"Console Application";

        m_MouseX = 0;
        m_MouseY = 0;

        hConsoleInput = GetStdHandle(STD_INPUT_HANDLE);
        hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

        hCursor = GetCursor();
    }

private:
    bool m_IsConsoleFullscreen = false;

public:
    BOOL ConstructConsole(int screen_width, int screen_height, int font_width, int font_height)
    {
        m_Screen.x = screen_width;
        m_Screen.y = screen_height;

        m_FontSize = { font_width, font_height };

        m_MouseX = m_Screen.x / 2;
        m_MouseY = m_Screen.y / 2;

        if (hConsoleOutput == INVALID_HANDLE_VALUE)
            return Error(L"Bad Handle");

        // Change console visual size to a minimum so m_ScreenBuffer can shrink
        // below the actual visual size
        rectWindow = { 0, 0, 1, 1 };
        SetConsoleWindowInfo(hConsoleOutput, TRUE, &rectWindow);

        // Set the size of the screen buffer
        COORD coord = { (short)m_Screen.x, (short)m_Screen.y };
        if (!SetConsoleScreenBufferSize(hConsoleOutput, coord))
            return Error(L"Invalid SetConsoleScreenBufferSize");

        // Assign screen buffer to the console
        if (!SetConsoleActiveScreenBuffer(hConsoleOutput))
            return Error(L"Invalid SetConsoleActiveScreenBuffer");

        // Set font
        CONSOLE_FONT_INFOEX cfi;
        cfi.cbSize = sizeof(cfi);
        cfi.nFont = 0;
        cfi.dwFontSize = { (short)font_width, (short)font_height };
        cfi.FontFamily = FF_DONTCARE;
        cfi.FontWeight = FW_NORMAL;
        wcscpy_s(cfi.FaceName, L"Consolas");
        if (!SetCurrentConsoleFontEx(hConsoleOutput, false, &cfi))
            return Error(L"Invalid SetCurrentConsoleFontEx");

        // Get screen buffer info and check the maximum allowed window size. Return
        // error if exceeded, so user knows their dimensions/fontsize are too large
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (!GetConsoleScreenBufferInfo(hConsoleOutput, &csbi))
            return Error(L"Invalid GetConsoleScreenBufferInfo");
        if ((short)m_Screen.y > csbi.dwMaximumWindowSize.Y)
            return Error(L"Screen Height / Font Height Too Big");
        if ((short)m_Screen.x > csbi.dwMaximumWindowSize.X)
            return Error(L"Screen Width / Font Width Too Big");

        // Set physical console window size
        rectWindow = { 0, 0, (short)m_Screen.x - 1, (short)m_Screen.y - 1 };
        if (!SetConsoleWindowInfo(hConsoleOutput, TRUE, &rectWindow))
            return Error(L"Invalid SetConsoleWindowInfo");

        // Set flags to allow console input
        if (!SetConsoleMode(hConsoleInput, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT))
            return Error(L"Invalid SetConsoleMode");

        // Disable cursore
        CONSOLE_CURSOR_INFO cursor;
        cursor.bVisible = false;
        cursor.dwSize = 1;
        if (!SetConsoleCursorInfo(hConsoleOutput, &cursor))
            return Error(L"Invalid SetConsoleCursorInfo");

        // Allocate memory for screen buffer
        m_ScreenBuffer = new CHAR_INFO[m_Screen.x*m_Screen.y];
        memset(m_ScreenBuffer, 0, sizeof(CHAR_INFO) * m_Screen.x * m_Screen.y);

        // Handler for Close Envent
        SetConsoleCtrlHandler((PHANDLER_ROUTINE)CloseEvent, TRUE);
        return 1;
    }

    BOOL ConstructConsole(int font_width, int font_height)
    {
        if (hConsoleOutput == INVALID_HANDLE_VALUE)
            return Error(L"Bad Handle");

        m_IsConsoleFullscreen = true;

        m_FontSize = { font_width, font_height };

        // Change console visual size to a minimum so m_ScreenBuffer can shrink
        // below the actual visual size
        rectWindow = { 0, 0, 1, 1 };
        SetConsoleWindowInfo(hConsoleOutput, TRUE, &rectWindow);

        // Set font
        CONSOLE_FONT_INFOEX cfi;
        cfi.cbSize = sizeof(cfi);
        cfi.nFont = 0;
        cfi.dwFontSize = { (short)font_width, (short)font_height };
        cfi.FontFamily = FF_DONTCARE;
        cfi.FontWeight = FW_NORMAL;
        wcscpy_s(cfi.FaceName, L"Consolas");
        if (!SetCurrentConsoleFontEx(hConsoleOutput, false, &cfi))
            return Error(L"Invalid SetCurrentConsoleFontEx");


        m_Screen.x = GetSystemMetrics(SM_CXSCREEN) / font_width;
        m_Screen.y = GetSystemMetrics(SM_CYSCREEN) / font_height;

        m_MouseX = m_Screen.x / 2;
        m_MouseY = m_Screen.y / 2;

        // Set the size of the screen buffer
        COORD coord = { (short)m_Screen.x, (short)m_Screen.y };

        rectWindow = { 0, 0, coord.X, coord.Y };
        SetConsoleWindowInfo(hConsoleOutput, TRUE, &rectWindow);

        //int scsbs = SetConsoleScreenBufferSize(hConsoleOutput, coord);
        if (!SetConsoleScreenBufferSize(hConsoleOutput, coord))
            return Error((std::wstring(L"Invalid SetConsoleScreenBufferSize: ") + std::to_wstring(GetLastError())).c_str());

        // Set fullscreen display mode
        if (!SetConsoleDisplayMode(hConsoleOutput, CONSOLE_FULLSCREEN_MODE, &coord))
            return Error(L"Invalid SetConsoleDisplayMode");

        // Assign screen buffer to the console
        if (!SetConsoleActiveScreenBuffer(hConsoleOutput))
            return Error(L"Invalid SetConsoleActiveScreenBuffer");


        // Get screen buffer info and check the maximum allowed window size. Return
        // error if exceeded, so user knows their dimensions/fontsize are too large
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (!GetConsoleScreenBufferInfo(hConsoleOutput, &csbi))
            return Error(L"Invalid GetConsoleScreenBufferInfo");
        if ((short)m_Screen.y > csbi.dwMaximumWindowSize.Y)
            return Error(L"Screen Height / Font Height Too Big");
        if ((short)m_Screen.x > csbi.dwMaximumWindowSize.X)
            return Error(L"Screen Width / Font Width Too Big");

        // Set physical console window size
        rectWindow = { 0, 0, (short)m_Screen.x - 1, (short)m_Screen.y - 1 };
        if (!SetConsoleWindowInfo(hConsoleOutput, TRUE, &rectWindow))
            return Error(L"Invalid SetConsoleWindowInfo");

        // Set flags to allow console input
        if (!SetConsoleMode(hConsoleInput, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT))
            return Error(L"Invalid SetConsoleMode");

        // Disable cursore
        CONSOLE_CURSOR_INFO cursor;
        cursor.bVisible = false;
        cursor.dwSize = 1;
        if (!SetConsoleCursorInfo(hConsoleOutput, &cursor))
            return Error(L"Invalid SetConsoleCursorInfo");

        // Allocate memory for screen buffer
        m_ScreenBuffer = new CHAR_INFO[m_Screen.x*m_Screen.y];
        memset(m_ScreenBuffer, 0, sizeof(CHAR_INFO) * m_Screen.x * m_Screen.y);

        // Handler for Close Envent
        SetConsoleCtrlHandler((PHANDLER_ROUTINE)CloseEvent, TRUE);
        return 1;
    }

    /* Drawing routine */
public:
    void DrawPixel(int x, int y, short Character = 0x2588, short Color = FG_WHITE)
    {
        if (x >= 0 && x < (int)m_Screen.x && y >= 0 && y < (int)m_Screen.y)
        {
            m_ScreenBuffer[y * m_Screen.x + x].Char.UnicodeChar = Character;
            m_ScreenBuffer[y * m_Screen.x + x].Attributes = Color;
        }
    }
    void DrawPixel(iVec2 Point, short Character = 0x2588, short Color = FG_WHITE)
    {
        DrawPixel(Point.x, Point.y, Character, Color);
    }

    void DrawPixelUnsafe(int x, int y, short Character = 0x2588, short Color = FG_WHITE)
    {
        m_ScreenBuffer[y * m_Screen.x + x].Char.UnicodeChar = Character;
        m_ScreenBuffer[y * m_Screen.x + x].Attributes = Color;
    }
    void DrawPixelUnsafe(iVec2 Point, short Character = 0x2588, short Color = FG_WHITE)
    {
        DrawPixelUnsafe(Point.x, Point.y, Character, Color);
    }

    void DrawRect(int x1, int y1, int x2, int y2, short Character = 0x2588, short Color = FG_BLACK)
    {
        // Clamp values
        if (x1 < 0) x1 = 0;
        if (y1 < 0) y1 = 0;
        if (x2 > ScreenWidth()) x2 = ScreenWidth();
        if (y2 > ScreenHeight()) y2 = ScreenHeight();

        for (int x = x1; x < x2; x++)
            for (int y = y1; y < y2; y++)
            {
                m_ScreenBuffer[y * m_Screen.x + x].Char.UnicodeChar = Character;
                m_ScreenBuffer[y * m_Screen.x + x].Attributes = Color;
            }
    }
    void DrawRect(iVec2 TopLeft, iVec2 DownRight, short Character = 0x2588, short Color = FG_BLACK)
    {
        DrawRect(TopLeft.x, TopLeft.y, DownRight.x, DownRight.y, Character, Color);
    }
    void DrawBox(int pos_x, int pos_y, int size_x, int size_y, short Character = 0x2588, short Color = FG_BLACK)
    {
        // Clamp values
        if (pos_x < 0) pos_x = 0;
        if (pos_y < 0) pos_y = 0;
        if (size_x + pos_x > ScreenWidth()) size_x = ScreenWidth() - pos_x;
        if (size_y + pos_y > ScreenHeight()) size_y = ScreenHeight() - pos_y;

        for (int x = 0; x < size_x; x++)
            for (int y = 0; y < size_y; y++)
            {
                m_ScreenBuffer[(y + pos_y) * m_Screen.x + x + pos_x].Char.UnicodeChar = Character;
                m_ScreenBuffer[(y + pos_y) * m_Screen.x + x + pos_x].Attributes = Color;
            }
    }
    void DrawBox(iVec2 Position, iVec2 Size, short Character = 0x2588, short Color = FG_BLACK)
    {
        DrawBox(Position.x, Position.y, Size.x, Size.y, Character, Color);
    }


    ///<summary> Drawing multicolor string start with position (x,y) </summary>
    ///<param name="string"> Use simbol '$' to pop next Color from ...  </param>
    void DrawString(int x, int y, const std::wstring& string, short Color = FG_WHITE, ...)
    {
        ptrdiff_t addr_delta = 1;
        short current_color = Color;
        size_t index_offset = 0;
        for (size_t i = 0; i < string.size(); ++i)
        {
            if (string[i] == L'$')
            {
                current_color = *(&Color + (addr_delta++) * sizeof(short));
                ++i, ++index_offset;
            }
            DrawPixel(x + i - index_offset, y, string[i], current_color);
        }
    }
    ///<summary> Drawing multicolor string start with position (x,y) </summary>
    ///<param name="string"> Use simbol '$' to pop next Color from ...  </param>
    template < typename ... ColorFormat>
    void DrawString(iVec2 Position, const std::wstring& string, short Color = FG_WHITE, ColorFormat... format)
    {
        DrawString(Position.x, Position.y, string, Color, format...);
    }

    /* Impementation of Brezenhem algorithms for drawing */
    void DrawCircle(int X, int Y, int R, short Character = 0x2588, short Color = FG_WHITE)
    {
        int x = R;
        int y = 0;
        int p = 1 - x;
        while (x >= y)
        {
            DrawPixel(x + X, y + Y, Character, Color); DrawPixel(x + X, -y + Y, Character, Color);
            DrawPixel(-y + X, x + Y, Character, Color); DrawPixel(-y + X, -x + Y, Character, Color);
            DrawPixel(-x + X, -y + Y, Character, Color); DrawPixel(-x + X, y + Y, Character, Color);
            DrawPixel(y + X, -x + Y, Character, Color); DrawPixel(y + X, x + Y, Character, Color);
            ++y;
            if (p < 0)
            {
                p += 2 * y + 1;
            }
            else
            {
                --x;
                p += 2 * (y - x + 1);
            }
        }
    }
    void DrawCircle(iVec2 Center, int R, short Character = 0x2588, short Color = FG_WHITE)
    {
        DrawCircle(Center.x, Center.y, R, Character, Color);
    }
    void DrawFillCircle(int X, int Y, int R, short Character = 0x2588, short Color = FG_WHITE)
    {
        int x = R;
        int y = 0;
        int p = 1 - x;
        while (x >= y)
        {
            DrawLine(x + X, y + Y, x + X, -y + Y, Character, Color);
            DrawLine(-y + X, x + Y, -y + X, -x + Y, Character, Color);
            DrawLine(-x + X, -y + Y, -x + X, y + Y, Character, Color);
            DrawLine(y + X, -x + Y, y + X, x + Y, Character, Color);
            ++y;
            if (p < 0)
            {
                p += 2 * y + 1;
            }
            else
            {
                --x;
                p += 2 * (y - x + 1);
            }
        }
    }
    void DrawLine(int x1, int y1, int x2, int y2, short Character = 0x2588, short Color = FG_WHITE)
    {
        int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
        dx = x2 - x1; dy = y2 - y1;
        dx1 = abs(dx); dy1 = abs(dy);
        px = 2 * dy1 - dx1;	py = 2 * dx1 - dy1;
        if (dy1 <= dx1)
        {
            if (dx >= 0)
            {
                x = x1; y = y1; xe = x2;
            }
            else
            {
                x = x2; y = y2; xe = x1;
            }

            DrawPixel(x, y, Character, Color);

            for (i = 0; x < xe; i++)
            {
                x = x + 1;
                if (px < 0)
                    px = px + 2 * dy1;
                else
                {
                    if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) y = y + 1; else y = y - 1;
                    px = px + 2 * (dy1 - dx1);
                }
                DrawPixel(x, y, Character, Color);
            }
        }
        else
        {
            if (dy >= 0)
            {
                x = x1; y = y1; ye = y2;
            }
            else
            {
                x = x2; y = y2; ye = y1;
            }

            DrawPixel(x, y, Character, Color);

            for (i = 0; y < ye; i++)
            {
                y = y + 1;
                if (py <= 0)
                    py = py + 2 * dx1;
                else
                {
                    if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) x = x + 1; else x = x - 1;
                    py = py + 2 * (dx1 - dy1);
                }
                DrawPixel(x, y, Character, Color);
            }
        }
    }
    void DrawLine(iVec2 First, iVec2 Second, short Character = 0x2588, short Color = FG_WHITE)
    {
        DrawLine(First.x, First.y, Second.x, Second.y, Character, Color);
    }

    void DrawScreenBuffer(int pos_x, int pos_y, int size_x, int size_y, const Pixel* Buffer)
    {
        for (int x = pos_x; x < pos_x + size_x; ++x)
            for (int y = pos_y; x < pos_y + size_y; ++y)
                DrawPixel(x, y,
                    Buffer[(y - pos_y)*size_x + (pos_x - x)].Char.UnicodeChar,
                    Buffer[(y - pos_y)*size_x + (pos_x - x)].Attributes
                );
    }
    void DrawScreenBuffer(iVec2 Position, iVec2 Size, const Pixel* Buffer)
    {
        DrawScreenBuffer(Position.x, Position.y, Size.x, Size.y, Buffer);
    }

    // Return screen buffer for direct lookup
    const Pixel* const GetScreenBuffer() const
    {
        return m_ScreenBuffer;
    }

    std::wstring GetString(int x, int y, int Lenght)
    {
        std::wstring result;
        if (x + Lenght >= 0 && x + Lenght < (int)m_Screen.x && y >= 0 && y < (int)m_Screen.y)
        {
            result.reserve(Lenght);
            for (int i = 0; i < Lenght; ++i)
                result += m_ScreenBuffer[y * m_Screen.x + x + i].Char.UnicodeChar;
            return result;
        }
        return result;
    }
    std::wstring GetString(iVec2 Position, int Lenght) { GetString(Position.x, Position.y, Lenght); }

    Pixel GetPixel(int x, int y)
    {
        if (x >= 0 && x < (int)m_Screen.x && y >= 0 && y < (int)m_Screen.y)
        {
            return m_ScreenBuffer[y * m_Screen.x + x];
        }
        else
            return {};
    }
    Pixel GetPixel(iVec2 Point) { return GetPixel(Point.x, Point.y); }

protected:
    /* Must override this */
    virtual void Init() = 0;       // Proc once on start of program                     
    virtual void Update() = 0;     // Run code in function with FPS lock, must handle drawing routine

    /* Not necessary to override */
    virtual void Destroy() { Quit(); }     // Proc once on exit from Update()

    ~ConsoleEngine() = default;

    /* Screen info */
private:
    iVec2 m_Screen;
    SMALL_RECT rectWindow;

    iVec2 m_FontSize;

public:
    constexpr const int ScreenWidth() const
    {
        return m_Screen.x;
    }
    constexpr const int ScreenHeight() const
    {
        return m_Screen.y;
    }
    const iVec2 Screen() const
    {
        return m_Screen;
    }

    const iVec2 FontSize() const
    {
        return m_FontSize;
    }

    bool FontResize(int font_width, int font_height)
    {
        if (!m_IsConsoleFullscreen)
            return Error(L"Console not in fullscreen");

        // If the same font size is already set in the cosnole then ignore
        if (m_FontSize == iVec2{ font_width, font_height })
            return 1;

        m_FontSize = { font_width, font_height };

        SetConsoleActiveScreenBuffer(hOriginalConsole);

        rectWindow = { 0, 0, (short)m_Screen.x, (short)m_Screen.x };
        SetConsoleWindowInfo(hConsoleOutput, TRUE, &rectWindow);

        // Set the size of the screen buffer
        COORD coord = { (short)m_Screen.x, (short)m_Screen.y };

        // Set window display mode
        if (!SetConsoleDisplayMode(hConsoleOutput, CONSOLE_WINDOWED_MODE, &coord))
            return Error(L"Invalid SetConsoleDisplayMode");

        // Set font
        CONSOLE_FONT_INFOEX cfi;
        cfi.cbSize = sizeof(cfi);
        cfi.nFont = 0;
        cfi.dwFontSize = { (short)font_width, (short)font_height };
        cfi.FontFamily = FF_DONTCARE;
        cfi.FontWeight = FW_NORMAL;
        wcscpy_s(cfi.FaceName, L"Consolas");
        if (!SetCurrentConsoleFontEx(hConsoleOutput, false, &cfi))
            return Error(L"Invalid SetCurrentConsoleFontEx");

        m_Screen.x = GetSystemMetrics(SM_CXSCREEN) / font_width;
        m_Screen.y = GetSystemMetrics(SM_CYSCREEN) / font_height;

        coord = { (short)m_Screen.x, (short)m_Screen.y };

        rectWindow = { 0, 0, coord.X, coord.Y };
        SetConsoleWindowInfo(hConsoleOutput, TRUE, &rectWindow);

        //int scsbs = SetConsoleScreenBufferSize(hConsoleOutput, coord);
        if (!SetConsoleScreenBufferSize(hConsoleOutput, coord))
            return Error((std::wstring(L"Invalid SetConsoleScreenBufferSize: ") + std::to_wstring(GetLastError())).c_str());

        // Set fullscreen display mode
        if (!SetConsoleDisplayMode(hConsoleOutput, CONSOLE_FULLSCREEN_MODE, &coord))
            return Error(L"Invalid SetConsoleDisplayMode");

        // Assign screen buffer to the console
        if (!SetConsoleActiveScreenBuffer(hConsoleOutput))
            return Error(L"Invalid SetConsoleActiveScreenBuffer");

        // Get screen buffer info and check the maximum allowed window size. Return
        // error if exceeded, so user knows their dimensions/fontsize are too large
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (!GetConsoleScreenBufferInfo(hConsoleOutput, &csbi))
            return Error(L"Invalid GetConsoleScreenBufferInfo");
        if ((short)m_Screen.y > csbi.dwMaximumWindowSize.Y)
            return Error(L"Screen Height / Font Height Too Big");
        if ((short)m_Screen.x > csbi.dwMaximumWindowSize.X)
            return Error(L"Screen Width / Font Width Too Big");

        // Set physical console window size
        rectWindow = { 0, 0, (short)m_Screen.x - 1, (short)m_Screen.y - 1 };
        if (!SetConsoleWindowInfo(hConsoleOutput, TRUE, &rectWindow))
            return Error(L"Invalid SetConsoleWindowInfo");

        // Set flags to allow console input
        if (!SetConsoleMode(hConsoleInput, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT))
            return Error(L"Invalid SetConsoleMode");

        // Reallocate memory for screen buffer
        delete[] m_ScreenBuffer;
        m_ScreenBuffer = new CHAR_INFO[m_Screen.x*m_Screen.y];
        memset(m_ScreenBuffer, 0, sizeof(CHAR_INFO) * m_Screen.x * m_Screen.y);

        // Disable cursore
        CONSOLE_CURSOR_INFO cursor;
        cursor.bVisible = false;
        cursor.dwSize = 1;
        if (!SetConsoleCursorInfo(hConsoleOutput, &cursor))
            return Error(L"Invalid SetConsoleCursorInfo");

        return 1;
    }

    /* Input */
private:
    KeyState m_Keys[256], m_Mouse[CE_MOUSE_MAX_BUTTONS + CE_MOUSE_ADDITIONAL_EVENTS];
    KEY m_last_key = KEY::EMPTY;

    int m_MouseX;
    int m_MouseY;

public:
    constexpr const int& GetMouseX() const
    {
        return m_MouseX;
    }
    constexpr const int& GetMouseY() const
    {
        return m_MouseY;
    }
    constexpr const KeyState& GetMouseButton(BUTTON mouse_button) const
    {
        return m_Mouse[(size_t)mouse_button];
    }
    iVec2 GetMouse() const
    {
        return iVec2{ m_MouseX, m_MouseY };
    }

    void SetMouseCursor(CURSOR cursor_type)
    {
        LPCWSTR CursorName = IDC_ARROW;
        switch (cursor_type)
        {
        case CURSOR::ARROW: CursorName = IDC_ARROW; break;
        case CURSOR::ARROW_WATCH: CursorName = IDC_APPSTARTING; break;
        case CURSOR::WAIT_WATCH: CursorName = IDC_WAIT; break;
        case CURSOR::CROSS: CursorName = IDC_CROSS; break;
        case CURSOR::TEXT: CursorName = IDC_IBEAM; break;
        case CURSOR::SIZE_WIDTH: CursorName = IDC_SIZEWE; break;
        case CURSOR::SIZE_HEIGHT: CursorName = IDC_SIZENS; break;
        case CURSOR::SIZE_DIAGONAL_LEFT: CursorName = IDC_SIZENESW; break;
        case CURSOR::SIZE_DIAGONAL_RIGHT: CursorName = IDC_SIZENWSE; break;
        case CURSOR::SIZE_ALL: CursorName = IDC_SIZEALL; break;
        case CURSOR::HAND: CursorName = IDC_HAND; break;
        case CURSOR::PIN: CursorName = IDC_PIN; break;
        }

        hCursor = LoadCursor(NULL, CursorName);
        SetCursor(hCursor);
    }

    constexpr const KeyState& GetKey(KEY key) const
    {
        return m_Keys[(size_t)key];
    }
    constexpr const KEY& GetLastKeyCode() const
    {
        return m_last_key;
    }
    constexpr const KeyState& GetLastKey() const
    {
        return m_Keys[(size_t)m_last_key];
    }

    // Get every keys that proc
    static const List<KeyInfo>& GetInputBuffer()
    {
        return CE_KeyBuffer;
    }

    // Check if console is focused window
    constexpr const bool& InFocus() const
    {
        return ConsoleInFocus;
    }

    /* Work with cursor */
private:
    bool m_IsCursorVis = false;
    iVec2 m_CursorPosition;

public:
    bool IsCursorVis() { return m_IsCursorVis; }
    iVec2 GetCursorPos() { return m_CursorPosition; }

    void SetCursorVis(bool IsVisible)
    {
        CONSOLE_CURSOR_INFO cursor;
        cursor.bVisible = IsVisible;
        cursor.dwSize = 1;
        SetConsoleCursorInfo(hConsoleOutput, &cursor);
    }
    void SetCursorPos(iVec2 Position)
    {
        if (Position.x >= 0 && Position.x < m_Screen.x && Position.y >= 0 && Position.y < m_Screen.y)
        {
            SetConsoleCursorPosition(hConsoleOutput, { (short)Position.x, (short)Position.y });
            m_CursorPosition = Position;
        }
    }

    /* Handle every keyboard inputs between frames */
private:
    static HHOOK CE_Hook;               // Just a hook to to callback keyboard function
    static KeyInfo CE_LastKUI;          // Info about last key that proc
    static List<KeyInfo> CE_KeyBuffer;  // Keys buffer, contains keys that change his states between frames

    static LRESULT CALLBACK KeyboardProc(_In_ int nCode, _In_ WPARAM wParam, _In_ LPARAM lParam)
    {
        PKBDLLHOOKSTRUCT KBD = (PKBDLLHOOKSTRUCT)lParam;
        KeyInfo KI;
        KI.Code = KBD->vkCode;
        if ((KBD->flags & LLKHF_EXTENDED) != 0) { // Check if it's the enter on the numpad
            // . . . maybe doing something, in case not in current implementation
        }
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
        {
            if (CE_LastKUI.Code == KI.Code && !CE_LastKUI.State.Released)
            {
                KI.State.Pressed = false;
                KI.State.Held = true;
            }
            else
            {
                KI.State.Pressed = true;
            }
        }
        else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP)
        {
            KI.State.Released = true;
        }
        CE_MutexInputModifying.lock();
        CE_KeyBuffer.push_back(KI);
        CE_LastKUI = KI;
        CE_MutexInputModifying.unlock();
        return CallNextHookEx(CE_Hook, nCode, wParam, lParam);
    }

    /* Threads & utilities */
private:
    void ManuallyKeysUpdate()
    {
        // If keys on lookup, then wait
        // Handle Keyboard Input
        for (size_t key_i = 0; key_i < 256; ++key_i)
        {
            keysNewState[key_i] = GetAsyncKeyState(key_i);
            m_Keys[key_i].Pressed = false;
            m_Keys[key_i].Released = false;
            if (keysNewState[key_i] != keysOldState[key_i])
            {
                m_last_key = (KEY)key_i;
                if (keysNewState[key_i] & 0x8000)
                {
                    m_Keys[key_i].Pressed = !m_Keys[key_i].Held;
                    m_Keys[key_i].Held = true;
                }
                else
                {
                    m_Keys[key_i].Released = true;
                    m_Keys[key_i].Held = false;
                }
            }
            keysOldState[key_i] = keysNewState[key_i];
        }

        // Check for window events
        INPUT_RECORD inBuffer[32];
        DWORD events = 0;
        GetNumberOfConsoleInputEvents(hConsoleInput, &events);
        if (events > 0)
            ReadConsoleInput(hConsoleInput, inBuffer, events, &events);

        for (DWORD i = 0; i < events; i++)
        {
            switch (inBuffer[i].EventType)
            {
            case FOCUS_EVENT:
            {
                ConsoleInFocus = inBuffer[i].Event.FocusEvent.bSetFocus;
            }
            break;

            case MOUSE_EVENT:
            {
                switch (inBuffer[i].Event.MouseEvent.dwEventFlags)
                {
                case MOUSE_MOVED:
                {
                    m_MouseX = inBuffer[i].Event.MouseEvent.dwMousePosition.X;
                    m_MouseY = inBuffer[i].Event.MouseEvent.dwMousePosition.Y;
                }
                break;


                case MOUSE_WHEELED:
                {
                    if ((short)(inBuffer[i].Event.MouseEvent.dwButtonState >> 16) > 0)
                        mouseNewState[(size_t)BUTTON::WH_FORWARD] = true;
                    else
                        mouseNewState[(size_t)BUTTON::WH_BACKWARD] = true;
                }

                default:
                    break;
                }
            case 0:
            {
                for (int m = 0; m < CE_MOUSE_MAX_BUTTONS; m++)
                    mouseNewState[m] = (inBuffer[i].Event.MouseEvent.dwButtonState & (1 << m)) > 0;
            }
            break;

            }
            break;

            default:
                break;
                // don't care just at the moment
            }
        }

        // Handle Mouse Input
        for (int mouse_i = 0; mouse_i < CE_MOUSE_MAX_BUTTONS + CE_MOUSE_ADDITIONAL_EVENTS; mouse_i++)
        {
            m_Mouse[mouse_i].Pressed = false;
            m_Mouse[mouse_i].Released = false;
            if (mouseNewState[mouse_i] != mouseOldState[mouse_i])
            {
                if (mouseNewState[mouse_i])
                {
                    m_Mouse[mouse_i].Pressed = true;
                    m_Mouse[mouse_i].Held = true;
                }
                else
                {
                    m_Mouse[mouse_i].Released = true;
                    m_Mouse[mouse_i].Held = false;
                }
            }
            mouseOldState[mouse_i] = mouseNewState[mouse_i];
        }

        // Reset Mouse Wheel proc
        mouseNewState[(size_t)BUTTON::WH_FORWARD] = false;
        mouseNewState[(size_t)BUTTON::WH_BACKWARD] = false;
    }

    void StableUpdateThread()
    {
        Init();

#ifndef CE_NO_FPS_LIMIT
        // FPS lock stuff
        auto tpBeginFrame = std::chrono::high_resolution_clock::now();
        auto tpEndFrame = tpBeginFrame + m_FPS;
#endif

        // Handle FPS
        auto tpPrevTime = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now());
        std::chrono::duration<double> tpElapsedTime;

        double TimingList[CE_AVERAGE_FRAMELIST_SIZE] = { 0.0 };
        size_t t_index{ 0 };

        // This cycle for the Destroy() return 1
        while (CE_ActiveMainThread) {
            while (CE_ActiveMainThread)
            {
                // Handle input
                ManuallyKeysUpdate();

                CE_MutexInputModifying.lock();
                // Update game states
                Update();

                // Clear between frame keys update
                CE_KeyBuffer.clear();
                CE_MutexInputModifying.unlock();

                // Draw console characters
                wchar_t TitleBuffer[256];
#ifndef CE_NO_FPS_LIMIT
                swprintf_s(TitleBuffer, 256, L"%s - FPS: %4.0f", m_AppName.c_str(), m_AverageFPS);
#else
                swprintf_s(TitleBuffer, 256, L"%s", m_AppName.c_str());
#endif
                SetConsoleTitle(TitleBuffer);
                WriteConsoleOutput(hConsoleOutput, m_ScreenBuffer, { (short)m_Screen.x, (short)m_Screen.y }, { 0,0 }, &rectWindow);

                auto tpCurrentTime = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now());

#ifndef CE_NO_FPS_LIMIT
                // Stable FPS
                if (tpCurrentTime < tpEndFrame)
                {
                    std::this_thread::sleep_until(tpEndFrame);
                    tpBeginFrame = tpEndFrame;
                }
                else
                {
                    tpBeginFrame = tpCurrentTime;
                }
                tpEndFrame = tpBeginFrame + m_FPS;
#endif

                // Handle framerate routine
                tpElapsedTime = tpCurrentTime - tpPrevTime;
                tpPrevTime = tpCurrentTime;
                m_StableDeltaTime = tpElapsedTime.count();

#ifndef CE_NO_FPS_LIMIT
                // Count average FPS
                TimingList[t_index] = m_StableDeltaTime;
                if (++t_index == CE_AVERAGE_FRAMELIST_SIZE)
                    t_index = 0U;
                m_AverageFPS = 0.0;
                for (auto& T : TimingList)
                    m_AverageFPS += T;
                m_AverageFPS = 1.0 / (m_AverageFPS / CE_AVERAGE_FRAMELIST_SIZE);
#endif
            }

            // Allow the user to free resources if they have overrided the destroy function
            Destroy();

            // Exit and clean up
            delete[] m_ScreenBuffer;
            SetConsoleActiveScreenBuffer(hOriginalConsole);
            CE_FinishedCondition.notify_one();
            return;
        }
    }

    void BufferedInputThread()
    {
        CE_Hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, NULL); // Set the keyboard hook
        if (!CE_Hook)
            return;
        MSG ThreadMsg;
        while (GetMessage(&ThreadMsg, NULL, WM_KEYFIRST, WM_KEYLAST)) {} // Proc message queue update
    }

public:
    void Start()
    {
        tpStartProgram = std::chrono::system_clock::now();
        CE_ActiveMainThread = true;
        std::thread KeysInputThread(&ConsoleEngine::BufferedInputThread, this);
        KeysInputThread.detach();
        std::thread UpdateThread(&ConsoleEngine::StableUpdateThread, this);
        UpdateThread.join();
        UnhookWindowsHookEx(CE_Hook);
    }

    // Force to exit from outside Update() function
    void Quit()
    {
        CE_ActiveMainThread = false;
    }

    void SetAppName(const wchar_t* AppName)
    {
        m_AppName = AppName;
    }
    void SetFramerate(double FPS)
    {
        m_FPS = std::chrono::duration_cast<std::chrono::system_clock::duration>(std::chrono::duration<double>{ 1.0 / FPS });
    }
    double GetFPS()
    {
        return m_AverageFPS;
    }

    /* Timing things */
private:
    std::chrono::duration<long long, std::ratio_multiply<std::hecto, std::nano>> m_FPS =
        std::chrono::duration_cast<std::chrono::system_clock::duration>(std::chrono::duration<double>{ 1.0 / CE_DEFAULT_FPS_LIM });
    double m_AverageFPS = 0.0;
    double m_StableDeltaTime = 0.0f;
    std::chrono::system_clock::time_point tpStartProgram;

public:
    // Return elapsed time between since the last frame
    constexpr const double& DeltaTime() const
    {
        return m_StableDeltaTime;
    }

    // Return elapsed time since the start of the program in ms
    constexpr double RunTime() const
    {
        return std::chrono::duration<double>(std::chrono::system_clock::now() - tpStartProgram).count();
    }

    /* Call on console close */
private:
    static BOOL CloseEvent(DWORD evt)
    {
        // Note this gets called in a seperate OS thread, so it must
        // only exit when the game has finished cleaning up, or else
        // the process will be killed before OnUserDestroy() has finished
        if (evt == CTRL_CLOSE_EVENT)
        {
            CE_ActiveMainThread = false;
            // Wait for thread to be exited
            std::unique_lock<std::mutex> ul(CE_MutexFC);
            CE_FinishedCondition.wait(ul);
        }
        return true;
    }

public:
    BOOL Error(const wchar_t *msg)
    {
        wchar_t buf[256];
        FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buf, 256, NULL);
        wchar_t full_msg[256];
        swprintf_s(full_msg, L"ERROR: %s\n\t%s\n", msg, buf);
        MessageBox(NULL, full_msg, L"Console Engine Error", MB_OK);
        return 0;
    }

private:
    std::wstring m_AppName;

    CHAR_INFO *m_ScreenBuffer;

    HANDLE hOriginalConsole;
    CONSOLE_SCREEN_BUFFER_INFO hOriginalConsoleInfo;
    HANDLE hConsoleOutput;
    HANDLE hConsoleInput;

    HCURSOR hCursor;

    uint16_t keysOldState[256] = { 0 };
    uint16_t keysNewState[256] = { 0 };
    bool mouseOldState[CE_MOUSE_MAX_BUTTONS + CE_MOUSE_ADDITIONAL_EVENTS] = { 0 };
    bool mouseNewState[CE_MOUSE_MAX_BUTTONS + CE_MOUSE_ADDITIONAL_EVENTS] = { 0 };

    bool ConsoleInFocus = true;

    static std::atomic<bool> CE_ActiveMainThread;

    static std::condition_variable CE_FinishedCondition;
    static std::mutex CE_MutexFC;

    static std::mutex CE_MutexInputModifying;
};

std::atomic<bool> ConsoleEngine::CE_ActiveMainThread(false);
std::condition_variable ConsoleEngine::CE_FinishedCondition;
std::mutex ConsoleEngine::CE_MutexFC;
std::mutex ConsoleEngine::CE_MutexInputModifying;

List<KeyInfo> ConsoleEngine::CE_KeyBuffer;
HHOOK ConsoleEngine::CE_Hook;
KeyInfo ConsoleEngine::CE_LastKUI;

//------- Useful utilities ---------------------------------------------------------------------------

// Greyscale combos
constexpr const std::pair<short, short>& GetGrayscale(int index)
{
    constexpr const std::pair<short, short> table[] =
    {
        { QUAD::SOLID, COLOR::FG_BLACK },
        { QUAD::HALF, COLOR::FG_BLACK | COLOR::BG_DARK_GREY },
        { QUAD::SOLID, COLOR::FG_DARK_GREY },
        { QUAD::HALF, COLOR::FG_BLACK | COLOR::BG_GREY },
        { QUAD::HALF, COLOR::FG_DARK_GREY | COLOR::BG_GREY },
        { QUAD::SOLID, COLOR::FG_GREY },
        { QUAD::HALF, COLOR::FG_GREY | COLOR::BG_WHITE },
        { QUAD::SOLID, COLOR::FG_WHITE }
    };

    return table[index];
}

/* Multy-threading */

template<class T, class U>
struct is_same_function : std::false_type {};
template<class T>
struct is_same_function<T, T> : std::true_type {};

///<summary>Create async corutine</summary>
///<param name="Routine">Reference to function with signature: void(args...). DON'T CALL DRAW FUNCS IN COROUTINE!!!</param>
///<param name="Inst">Pointer to calling class (this)</param>
///<param name="Args">Args that match to function signature</param>
template < typename Foo, typename CE_Class, typename ... In_Params  >
static void StartCoroutine(Foo Routine, CE_Class* Inst, In_Params... Args)
{
    static_assert(is_same_function< Foo, void(__thiscall CE_Class::*)(In_Params...)>::value,
        "Foo must have void(args...) signature, please check return type and args match");
    std::thread(Routine, Inst, Args...).detach();
}

static void WaitFor(std::chrono::milliseconds Msec) { std::this_thread::sleep_for(Msec); }
static void WaitFor(std::chrono::seconds Sec) { std::this_thread::sleep_for(Sec); }

// Just a timer
class Timer
{
private:
    std::chrono::milliseconds m_CountdownTime = 1000ms;
    std::chrono::time_point<std::chrono::steady_clock> m_TimerStartPoint;

public:
    Timer() { m_TimerStartPoint = std::chrono::steady_clock::now(); }
    Timer(std::chrono::milliseconds Ms) : m_CountdownTime(Ms) {}

    void SetTimer(std::chrono::milliseconds Ms) { m_CountdownTime = Ms;  m_TimerStartPoint = std::chrono::steady_clock::now(); }

    void Start()
    {
        m_TimerStartPoint = std::chrono::steady_clock::now();
    }

	void Stop()
	{
		m_TimerStartPoint = m_CountdownTime + std::chrono::steady_clock::now();
	}

    double Time() const
    {
        double TimeRemaining = std::chrono::duration<double>(m_TimerStartPoint + m_CountdownTime - std::chrono::steady_clock::now()).count();
        if (TimeRemaining > 0.0)
            return TimeRemaining;
        else
            return 0.0;
    }

    bool Ready() const
    {
        return Time() == 0.0;
    }
};