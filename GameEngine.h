#pragma once

#include <windows.h>

//Windws function decleration
int WINAPI       WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//Game Engine Function Declaration
BOOL GameInitialize(HINSTANCE hInstance);
void GameStart(HWND hWindow);
void GameEnd();
void GameActivate(HWND hWindow);
void GameDeactivate(HWND hWindow);
void GamePaint(HDC hDC);
void GameCycle();

//mouse event Functions
void MouseButtonDown(int x, int y, BOOL bLeft);
void MouseButtonUp(int x, int y, BOOL bLeft);
void MouseMove(int x, int y);

//Handle keyboard input
void HandleKeys();

int wait(int iBeginTime);

class GameEngine
{
    protected:
    //Member Variables
    static GameEngine* m_pGameEngine;
    HINSTANCE          m_hInstance;
    HWND               m_hWindow;
    TCHAR              m_szWindowClass[32];
    TCHAR              m_szTitle[32];
    WORD               m_wIcon, m_wSmallIcon;
    WORD               m_wCursor;
    int                m_iWidth, m_iHeight;
    int                m_iFrameDelay;
    BOOL               m_bSleep;

    public:
    //Constructor/Destructor
    GameEngine(HINSTANCE hInstance, LPSTR szWindowClass, LPSTR szTitle,
               WORD wIcon, WORD wSmallIcon, WORD wCursor = NULL, int iWidth = 640, int iHeight = 480);
    virtual ~GameEngine();

    //Genaral Methods
    static GameEngine* GetEngine(){return m_pGameEngine;};
    BOOL               Initialize(int iCmdShow);
    LRESULT            HandleEvent(HWND hWindow, UINT msg, WPARAM, LPARAM lParam);

    //Accessor Methods
    HINSTANCE GetInstance() {return m_hInstance; };
    HWND      GetWindow(){return m_hWindow; };
    void      SetWindow(HWND hWindow){m_hWindow = hWindow;};
    LPSTR     GetTitle(){return m_szTitle; };
    WORD      GetIcon(){return m_wIcon; };
    WORD      GetSmallIcon(){return m_wSmallIcon; };
    WORD      GetCursor(){return m_wCursor; };
    int       GetWidth(){return m_iWidth; };
    int       GetHeight(){return m_iHeight; };
    void      SetFrameRate(int iFrameRate) {m_iFrameDelay = 1000 / iFrameRate;};
    int       GetFrameDelay(){return m_iFrameDelay;};
    BOOL      GetSleep(){return m_bSleep; };
    void      SetSleep(BOOL bSleep) {m_bSleep = bSleep; };
};
