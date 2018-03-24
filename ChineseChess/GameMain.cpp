#include <Windows.h>
#include <tchar.h>
#include <time.h>
#include "Resource.h"
#include "GameMain.h"
#include "GameMenu.h"
#include "GamePlaying.h"
#include "GameResult.h"

#pragma comment(lib, "winmm.lib")
extern const double MaxClientWidth = 1350;
extern const double MaxClientHeight = 700;
extern const int MinClientWidth = 722;
extern const int MinClientHeight = 350;
extern const int WideList = 94;
int widthFrame;
int heightFrame;
double proportion = 1;

int g_ClientWidth = 1350;
int g_ClientHeight = 700;
DWORD g_tPre = 0;
DWORD g_tNow = 0;
GameState gameState;
HWND hChessList;

HINSTANCE hInst;
HDC g_hdc;
HDC g_mdc;
HDC g_bufdc;

GameMenu gameMenu;
GamePlaying gamePlaying;
GameResult gameResult;


void GameOnWindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); //��Ϸ״̬����Windows��Ϣѭ��

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    hInst = hInstance;
    WNDCLASSEX wndClass = { 0 }; //���崰����
    const TCHAR ClassName[] = TEXT("WindowClass"); //��������

    wndClass.cbSize = sizeof(WNDCLASSEX); //���ýṹ����ֽ�����С
    wndClass.style = CS_HREDRAW | CS_VREDRAW; //���ô�����ʽ
    wndClass.lpfnWndProc = WndProc; //ָ�򴰿ڹ��̺�����ָ��
    wndClass.cbClsExtra = 0; //������ĸ����ڴ�
    wndClass.cbWndExtra = 0; //���ڵĸ����ڴ�
    wndClass.hInstance = hInstance; //ָ���������ڹ��̵ĳ����ʵ�����
    wndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON)); //ָ���������icoͼ����
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW); //ָ��������Ĺ����
    wndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH); //���ô��ڱ�����ˢ
    wndClass.lpszMenuName = NULL; //ָ���˵���Դ����
    wndClass.lpszClassName = ClassName; //ָ�������������

    if (!RegisterClassEx(&wndClass)) { //ע�ᴰ����
        MessageBox(NULL, TEXT("����RegisterClassExʧ��!"), TEXT("��ʾ"), MB_ICONWARNING);
        return 1;
    }

    RECT WindowPosition;
    WindowPosition = { 0, 0, g_ClientWidth, g_ClientHeight };
    AdjustWindowRect(&WindowPosition, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE); //���ݿͻ�����С���㴰�ڴ�С

    widthFrame = WindowPosition.right - WindowPosition.left - g_ClientWidth;
    heightFrame = WindowPosition.bottom - WindowPosition.top - g_ClientHeight;

    HWND hWnd;
    TCHAR WindowTitle[] = TEXT("�й�����(V1.0 ������ս��)���ߣ������"); //���ڱ���
    hWnd = CreateWindow(ClassName, WindowTitle, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPCHILDREN, CW_USEDEFAULT, CW_USEDEFAULT,
        WindowPosition.right - WindowPosition.left, WindowPosition.bottom - WindowPosition.top, NULL, NULL, hInstance, NULL); //��������

    if (!hWnd) {
        MessageBox(NULL, TEXT("����CreateWindowʧ��!"), TEXT("��ʾ"), MB_ICONWARNING);
        return 1;
    }

    MoveWindow(hWnd, 0, 0, WindowPosition.right - WindowPosition.left, WindowPosition.bottom - WindowPosition.top, true);  //���ô�����ʾʱ��λ��

    ShowWindow(hWnd, nShowCmd); //��ʾ����

    UpdateWindow(hWnd); //���´���

    g_hdc = GetDC(hWnd);
    g_mdc = CreateCompatibleDC(g_hdc);
    g_bufdc = CreateCompatibleDC(g_hdc);
    HBITMAP hBmp;
    hBmp = CreateCompatibleBitmap(g_hdc, g_ClientWidth, g_ClientHeight);
    SelectObject(g_mdc, hBmp);

    gameState = GS_Menu;

    GameStart(hWnd);

    MSG msg = { 0 };
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg); //������Ϣ
            DispatchMessage(&msg); //������Ϣ
        }

        g_tNow = GetTickCount(); //��ȡ��ǰϵͳʱ��
        if (g_tNow - g_tPre >= 5) {
            GameUpdate(hWnd);
        }

    }

    UnregisterClass(ClassName, wndClass.hInstance); //ע��������

    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    //HDC hdcListbox;
    switch (message) {
        case WM_CREATE: {
            hChessList = CreateWindow(TEXT("listbox"), TEXT(""), WS_CHILD | WS_VSCROLL, (MaxClientWidth - WideList) * proportion /2, 62, WideList, MaxClientHeight * proportion - 62, hWnd, NULL, hInst, NULL);
            break;
        }
        case WM_PAINT: {
            GameOnWindowMessage(hWnd, message, wParam, lParam);
            ValidateRect(hWnd, NULL);
            break;
        }
        case WM_SIZING: {
            GameOnWindowMessage(hWnd, message, wParam, lParam);
            break;
        }
        case WM_MOUSEMOVE: {
            GameOnWindowMessage(hWnd, message, wParam, lParam);
            break;
        }
        case WM_LBUTTONDOWN: {
            GameOnWindowMessage(hWnd, message, wParam, lParam);
            break;
        }
        //case WM_CTLCOLORLISTBOX: {
        //    hdcListbox = (HDC)wParam;
        //    SetTextColor(hdcListbox, RGB(0, 0, 0));
        //    SetBkMode(hdcListbox, TRANSPARENT);
        //    return (INT_PTR)GetStockObject(NULL_BRUSH);
        //}
        case WM_DESTROY: {
            PostQuitMessage(0);
            break;
        }
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void GameStart(HWND hWnd) {
    switch (gameState) {
        case GS_Menu:
            gameMenu.Start(hWnd);
            break;
        case GS_Playing:
            gamePlaying.Start(hWnd);
            break;
        case GS_Result:
            gameResult.Start(hWnd);
            break;
    }
}

void GameUpdate(HWND hWnd) {
    switch (gameState) {
        case GS_Menu:
            gameMenu.Update(hWnd);
            break;
        case GS_Playing:
            gamePlaying.Update(hWnd);
            break;
        case GS_Result:
            gameResult.Update(hWnd);
            break;
    }
}

void GameEnd(HWND hWnd) {
    switch (gameState) {
        case GS_Menu:
            gameMenu.End(hWnd);
            break;
        case GS_Playing:
            gamePlaying.End(hWnd);
            break;
        case GS_Result:
            gameResult.End(hWnd);
            break;
    }
}

void GameOnWindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (gameState) {
        case GS_Menu:
            gameMenu.OnWindowMessage(hWnd, message, wParam, lParam);
            break;
        case GS_Playing:
            gamePlaying.OnWindowMessage(hWnd, message, wParam, lParam);
            break;
        case GS_Result:
            gameResult.OnWindowMessage(hWnd, message, wParam, lParam);
            break;
    }
}
