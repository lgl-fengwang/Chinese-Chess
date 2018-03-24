#include <Windows.h>
#include <mmsystem.h>
#include <tchar.h>
#include <time.h>
#include "GameMain.h"
#include "GameMenu.h"

void GameMenu::Start(HWND hWnd) {
    g_ClientWidth = wideBackground; g_ClientHeight = highBackground;
    MoveWindow(hWnd, 0, 0, g_ClientWidth + widthFrame, g_ClientHeight + heightFrame, true);
    mciSendString(TEXT("open sound/menubackground.wav"), NULL, NULL, NULL);
    mciSendString(TEXT("play sound/menubackground.wav"), NULL, NULL, NULL);
    hMenuBackground = (HBITMAP)LoadImage(hInst, TEXT("image/menubackground.bmp"), IMAGE_BITMAP, wideBackground, highBackground, LR_LOADFROMFILE);
    hGameStart = (HBITMAP)LoadImage(hInst, TEXT("image/gamestart.bmp"), IMAGE_BITMAP, wideGameStart, highGameStart * 3, LR_LOADFROMFILE);
    mouseInNow = false;
    mouseInPre = false;
}

void GameMenu::Update(HWND hWnd) {
    SelectObject(g_bufdc, hMenuBackground);
    BitBlt(g_mdc, 0, 0, wideBackground, highBackground, g_bufdc, 0, 0, SRCCOPY);
    SelectObject(g_bufdc, hGameStart);
    BitBlt(g_mdc, wideBackground / 2 - (wideGameStart / 2), highBackground / 2 - (highGameStart / 2), wideGameStart, highGameStart, g_bufdc, 0, highGameStart * 2, SRCAND);
    if (mouseInNow == false) {
        BitBlt(g_mdc, wideBackground / 2 - (wideGameStart / 2), highBackground / 2 - (highGameStart / 2), wideGameStart, highGameStart, g_bufdc, 0, 0, SRCPAINT);
    }
    else {
        BitBlt(g_mdc, wideBackground / 2 - (wideGameStart / 2), highBackground / 2 - (highGameStart / 2), wideGameStart, highGameStart, g_bufdc, 0, highGameStart, SRCPAINT);
    }

    if (mouseInNow != mouseInPre) {
        PlaySound(TEXT("sound/mousemove.wav"), NULL, SND_FILENAME | SND_ASYNC);
    }
    mouseInPre = mouseInNow;
    SetStretchBltMode(g_hdc, STRETCH_HALFTONE);
    SetBrushOrgEx(g_hdc, 0, 0, NULL);
    StretchBlt(g_hdc, 0, 0, g_ClientWidth, g_ClientHeight, g_mdc, 0, 0, wideBackground, highBackground,  SRCCOPY);
    g_tPre = GetTickCount();
}

void GameMenu::OnWindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_MOUSEMOVE: {
            if ((LOWORD(lParam) > g_ClientWidth / 2 - (wideGameStart * proportion / 2)) && (LOWORD(lParam) < g_ClientWidth / 2 + (wideGameStart * proportion / 2)) && (HIWORD(lParam) > g_ClientHeight / 2 - (highGameStart * proportion / 2)) & (HIWORD(lParam) < g_ClientHeight / 2 + (highGameStart * proportion / 2))) {
                mouseInNow = true;
            }
            else {
                mouseInNow = false;
            }
            break;
        }
        case WM_LBUTTONDOWN: {
            if ((LOWORD(lParam) > g_ClientWidth / 2 - (wideGameStart * proportion / 2)) && (LOWORD(lParam) < g_ClientWidth / 2 + (wideGameStart * proportion / 2)) && (HIWORD(lParam) > g_ClientHeight / 2 - (highGameStart * proportion / 2)) & (HIWORD(lParam) < g_ClientHeight / 2 + (highGameStart * proportion / 2))) {
                GameEnd(hWnd);
                gameState = GS_Playing;
                GameStart(hWnd);
            }
        }
    }
}

void GameMenu::End(HWND hWnd) {
    mciSendString(TEXT("stop sound/menubackground.wav"), NULL, NULL, NULL);
    mciSendString(TEXT("close sound/menubackground.wav"), NULL, NULL, NULL);
    DeleteObject(hGameStart);
    DeleteObject(hMenuBackground);
}