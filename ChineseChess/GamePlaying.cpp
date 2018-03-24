#include <Windows.h>
#include <cmath>
#include <mmsystem.h>
#include <tchar.h>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "GameMain.h"
#include "GamePlaying.h"

void GamePlaying::Start(HWND hWnd) {
    g_ClientWidth = wideBackground; g_ClientHeight = highBackground;
    MoveWindow(hWnd, 0, 0, g_ClientWidth + widthFrame, g_ClientHeight + heightFrame, true);
    SetWindowLong(hWnd, GWL_STYLE, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPCHILDREN | WS_SIZEBOX | WS_VISIBLE);
    hPlayingBackground = (HBITMAP)LoadImage(hInst, TEXT("image/playingbackground.bmp"), IMAGE_BITMAP, wideBackground, highBackground, LR_LOADFROMFILE);
    hRedChessboard = (HBITMAP)LoadImage(hInst, TEXT("image/redchessboard.bmp"), IMAGE_BITMAP, WideChessboard * 2, HighChessboard, LR_LOADFROMFILE);
    hBlackChessboard = (HBITMAP)LoadImage(hInst, TEXT("image/blackchessboard.bmp"), IMAGE_BITMAP, WideChessboard * 2, HighChessboard, LR_LOADFROMFILE);
    hFocus = (HBITMAP)LoadImage(hInst, TEXT("image/focus.bmp"), IMAGE_BITMAP, 124, 62, LR_LOADFROMFILE);
    hPiece = (HBITMAP)LoadImage(hInst, TEXT("image/piece.bmp"), IMAGE_BITMAP, 434, 124, LR_LOADFROMFILE);
    hPieceMask = (HBITMAP)LoadImage(hInst, TEXT("image/piecemask.bmp"), IMAGE_BITMAP, 62, 62, LR_LOADFROMFILE);
    hDaohang = (HBITMAP)LoadImage(hInst, TEXT("image/daohang.bmp"), IMAGE_BITMAP, 124, 62, LR_LOADFROMFILE);
    hChessFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, TEXT("华文楷体"));
    SetWindowLong(hChessList, GWL_STYLE, WS_CHILD | WS_VISIBLE);
    SendMessage(hChessList, WM_SETFONT, (WPARAM)hChessFont, NULL);
    SendMessage(hChessList, LB_ADDSTRING, NULL, (LPARAM)TEXT("对局开始"));
    mouseYouxiao = false;
    pieceSelected.x = -1;
    pieceSelected.y = -1;
    HANDLE hFile = CreateFile(TEXT("chess/blackfirst.kj"), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        TCHAR error[40];
        wsprintf(error, TEXT("打开文件出错：%d"), (int)GetLastError());
        MessageBox(NULL,  error, TEXT("提示"), MB_ICONWARNING);
        GameEnd(hWnd);
        gameState = GS_Menu;
        GameStart(hWnd);
        return;
    }
    int fileSize = GetFileSize(hFile, NULL);
    TCHAR *buff;
    buff = (TCHAR*)malloc(fileSize);
    DWORD dwRead;
    if (!ReadFile(hFile, buff, fileSize, &dwRead, NULL))
    {
        MessageBox(NULL, TEXT("读取文件出错"), TEXT("提示"), MB_ICONWARNING);
        GameEnd(hWnd);
        gameState = GS_Menu;
        GameStart(hWnd);
        return;
    }
    buff[fileSize] = '\0';
    CloseHandle(hFile);
    mouseHonghei = (buff[0] == 'S');
    TCHAR str;
    int n = 0;
    if (mouseHonghei) {
        for (int i = 1; i < fileSize; ++i) {
            str = buff[i];
            switch (str) {
                case '0': {
                    redChess[n % 9][n / 9].qizi = false; redChess[n % 9][n / 9].honghei = true; redChess[n % 9][n / 9].daohang = false; redChess[n % 9][n / 9].bianshi = Jiang; ++n;
                    break;
                }
                case 'a': {
                    redChess[n % 9][n / 9].qizi = true; redChess[n % 9][n / 9].honghei = true; redChess[n % 9][n / 9].daohang = false; redChess[n % 9][n / 9].bianshi = Jiang; ++n;
                    break;
                }
                case 'b': {
                    redChess[n % 9][n / 9].qizi = true; redChess[n % 9][n / 9].honghei = true; redChess[n % 9][n / 9].daohang = false; redChess[n % 9][n / 9].bianshi = Shi; ++n;
                    break;
                }
                case 'c': {
                    redChess[n % 9][n / 9].qizi = true; redChess[n % 9][n / 9].honghei = true; redChess[n % 9][n / 9].daohang = false; redChess[n % 9][n / 9].bianshi = Xiang; ++n;
                    break;
                }
                case 'd': {
                    redChess[n % 9][n / 9].qizi = true; redChess[n % 9][n / 9].honghei = true; redChess[n % 9][n / 9].daohang = false; redChess[n % 9][n / 9].bianshi = Ma; ++n;
                    break;
                }
                case 'e': {
                    redChess[n % 9][n / 9].qizi = true; redChess[n % 9][n / 9].honghei = true; redChess[n % 9][n / 9].daohang = false; redChess[n % 9][n / 9].bianshi = Ju; ++n;
                    break;
                }
                case 'f': {
                    redChess[n % 9][n / 9].qizi = true; redChess[n % 9][n / 9].honghei = true; redChess[n % 9][n / 9].daohang = false; redChess[n % 9][n / 9].bianshi = Pao; ++n;
                    break;
                }
                case 'g': {
                    redChess[n % 9][n / 9].qizi = true; redChess[n % 9][n / 9].honghei = true; redChess[n % 9][n / 9].daohang = false; redChess[n % 9][n / 9].bianshi = Bing; ++n;
                    break;
                }
                case 'A': {
                    redChess[n % 9][n / 9].qizi = true; redChess[n % 9][n / 9].honghei = false; redChess[n % 9][n / 9].daohang = false; redChess[n % 9][n / 9].bianshi = Jiang; ++n;
                    break;
                }
                case 'B': {
                    redChess[n % 9][n / 9].qizi = true; redChess[n % 9][n / 9].honghei = false; redChess[n % 9][n / 9].daohang = false; redChess[n % 9][n / 9].bianshi = Shi; ++n;
                    break;
                }
                case 'C': {
                    redChess[n % 9][n / 9].qizi = true; redChess[n % 9][n / 9].honghei = false; redChess[n % 9][n / 9].daohang = false; redChess[n % 9][n / 9].bianshi = Xiang; ++n;
                    break;
                }
                case 'D': {
                    redChess[n % 9][n / 9].qizi = true; redChess[n % 9][n / 9].honghei = false; redChess[n % 9][n / 9].daohang = false; redChess[n % 9][n / 9].bianshi = Ma; ++n;
                    break;
                }
                case 'E': {
                    redChess[n % 9][n / 9].qizi = true; redChess[n % 9][n / 9].honghei = false; redChess[n % 9][n / 9].daohang = false; redChess[n % 9][n / 9].bianshi = Ju; ++n;
                    break;
                }
                case 'F': {
                    redChess[n % 9][n / 9].qizi = true; redChess[n % 9][n / 9].honghei = false; redChess[n % 9][n / 9].daohang = false; redChess[n % 9][n / 9].bianshi = Pao; ++n;
                    break;
                }
                case 'G': {
                    redChess[n % 9][n / 9].qizi = true; redChess[n % 9][n / 9].honghei = false; redChess[n % 9][n / 9].daohang = false; redChess[n % 9][n / 9].bianshi = Bing; ++n;
                    break;
                }
            }
        }
    }
    else {
        for (int i = 1; i < fileSize; ++i) {
            str = buff[i];
            switch (str) {
                case '0': {
                    blackChess[n % 9][n / 9].qizi = false; blackChess[n % 9][n / 9].honghei = true; blackChess[n % 9][n / 9].daohang = false; blackChess[n % 9][n / 9].bianshi = Jiang; ++n;
                    break;
                }
                case 'a': {
                    blackChess[n % 9][n / 9].qizi = true; blackChess[n % 9][n / 9].honghei = true; blackChess[n % 9][n / 9].daohang = false; blackChess[n % 9][n / 9].bianshi = Jiang; ++n;
                    break;
                }
                case 'b': {
                    blackChess[n % 9][n / 9].qizi = true; blackChess[n % 9][n / 9].honghei = true; blackChess[n % 9][n / 9].daohang = false; blackChess[n % 9][n / 9].bianshi = Shi; ++n;
                    break;
                }
                case 'c': {
                    blackChess[n % 9][n / 9].qizi = true; blackChess[n % 9][n / 9].honghei = true; blackChess[n % 9][n / 9].daohang = false; blackChess[n % 9][n / 9].bianshi = Xiang; ++n;
                    break;
                }
                case 'd': {
                    blackChess[n % 9][n / 9].qizi = true; blackChess[n % 9][n / 9].honghei = true; blackChess[n % 9][n / 9].daohang = false; blackChess[n % 9][n / 9].bianshi = Ma; ++n;
                    break;
                }
                case 'e': {
                    blackChess[n % 9][n / 9].qizi = true; blackChess[n % 9][n / 9].honghei = true; blackChess[n % 9][n / 9].daohang = false; blackChess[n % 9][n / 9].bianshi = Ju; ++n;
                    break;
                }
                case 'f': {
                    blackChess[n % 9][n / 9].qizi = true; blackChess[n % 9][n / 9].honghei = true; blackChess[n % 9][n / 9].daohang = false; blackChess[n % 9][n / 9].bianshi = Pao; ++n;
                    break;
                }
                case 'g': {
                    blackChess[n % 9][n / 9].qizi = true; blackChess[n % 9][n / 9].honghei = true; blackChess[n % 9][n / 9].daohang = false; blackChess[n % 9][n / 9].bianshi = Bing; ++n;
                    break;
                }
                case 'A': {
                    blackChess[n % 9][n / 9].qizi = true; blackChess[n % 9][n / 9].honghei = false; blackChess[n % 9][n / 9].daohang = false; blackChess[n % 9][n / 9].bianshi = Jiang; ++n;
                    break;
                }
                case 'B': {
                    blackChess[n % 9][n / 9].qizi = true; blackChess[n % 9][n / 9].honghei = false; blackChess[n % 9][n / 9].daohang = false; blackChess[n % 9][n / 9].bianshi = Shi; ++n;
                    break;
                }
                case 'C': {
                    blackChess[n % 9][n / 9].qizi = true; blackChess[n % 9][n / 9].honghei = false; blackChess[n % 9][n / 9].daohang = false; blackChess[n % 9][n / 9].bianshi = Xiang; ++n;
                    break;
                }
                case 'D': {
                    blackChess[n % 9][n / 9].qizi = true; blackChess[n % 9][n / 9].honghei = false; blackChess[n % 9][n / 9].daohang = false; blackChess[n % 9][n / 9].bianshi = Ma; ++n;
                    break;
                }
                case 'E': {
                    blackChess[n % 9][n / 9].qizi = true; blackChess[n % 9][n / 9].honghei = false; blackChess[n % 9][n / 9].daohang = false; blackChess[n % 9][n / 9].bianshi = Ju; ++n;
                    break;
                }
                case 'F': {
                    blackChess[n % 9][n / 9].qizi = true; blackChess[n % 9][n / 9].honghei = false; blackChess[n % 9][n / 9].daohang = false; blackChess[n % 9][n / 9].bianshi = Pao; ++n;
                    break;
                }
                case 'G': {
                    blackChess[n % 9][n / 9].qizi = true; blackChess[n % 9][n / 9].honghei = false; blackChess[n % 9][n / 9].daohang = false; blackChess[n % 9][n / 9].bianshi = Bing; ++n;
                    break;
                }
            }
        }
    }
    FlipChess(mouseHonghei);
}

void GamePlaying::Update(HWND hWnd) {
    SetStretchBltMode(g_mdc, HALFTONE);
    SelectObject(g_bufdc, hPlayingBackground);
    StretchBlt(g_mdc, 0, 0, g_ClientWidth, g_ClientHeight, g_bufdc, 0, 0, wideBackground, highBackground, SRCCOPY);
    SelectObject(g_bufdc, hRedChessboard);
    StretchBlt(g_mdc, 0, 0, (wideBackground - WideList) * proportion / 2, g_ClientHeight, g_bufdc, WideChessboard, 0, WideChessboard, HighChessboard, SRCAND);
    StretchBlt(g_mdc, 0, 0, (wideBackground - WideList) * proportion / 2, g_ClientHeight, g_bufdc, 0, 0, WideChessboard, HighChessboard, SRCPAINT);
    SelectObject(g_bufdc, hBlackChessboard);
    StretchBlt(g_mdc, g_ClientWidth - ((wideBackground - WideList) * proportion / 2), 0, (wideBackground - WideList) * proportion / 2, g_ClientHeight, g_bufdc, WideChessboard, 0, WideChessboard, HighChessboard, SRCAND);
    StretchBlt(g_mdc, g_ClientWidth - ((wideBackground - WideList) * proportion / 2), 0, (wideBackground - WideList) * proportion / 2, g_ClientHeight, g_bufdc, 0, 0, WideChessboard, HighChessboard, SRCPAINT);

    static POINT point;
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 10; ++j) {
            if (redChess[i][j].qizi) {
                point.x = i; point.y = j;
                point = PieceDown(point, true);
                SelectObject(g_bufdc, hPieceMask);
                StretchBlt(g_mdc, point.x, point.y, 62 * proportion, 62 * proportion, g_bufdc, 0, 0, 62, 62, SRCAND);
                SelectObject(g_bufdc, hPiece);
                if (redChess[i][j].honghei) {
                    switch (redChess[i][j].bianshi) {
                        case Ju: {
                            StretchBlt(g_mdc, point.x, point.y, 62 * proportion, 62 * proportion, g_bufdc, 248, 62, 62, 62, SRCPAINT);
                            break;
                        }
                        case Ma: {
                            StretchBlt(g_mdc, point.x, point.y, 62 * proportion, 62 * proportion, g_bufdc, 186, 62, 62, 62, SRCPAINT);
                            break;
                        }
                        case Xiang: {
                            StretchBlt(g_mdc, point.x, point.y, 62 * proportion, 62 * proportion, g_bufdc, 124, 62, 62, 62, SRCPAINT);
                            break;
                        }
                        case Shi: {
                            StretchBlt(g_mdc, point.x, point.y, 62 * proportion, 62 * proportion, g_bufdc, 62, 62, 62, 62, SRCPAINT);
                            break;
                        }
                        case Jiang: {
                            StretchBlt(g_mdc, point.x, point.y, 62 * proportion, 62 * proportion, g_bufdc, 0, 62, 62, 62, SRCPAINT);
                            break;
                        }
                        case Pao: {
                            StretchBlt(g_mdc, point.x, point.y, 62 * proportion, 62 * proportion, g_bufdc, 310, 62, 62, 62, SRCPAINT);
                            break;
                        }
                        case Bing: {
                            StretchBlt(g_mdc, point.x, point.y, 62 * proportion, 62 * proportion, g_bufdc, 372, 62, 62, 62, SRCPAINT);
                            break;
                        }
                    }
                }
                else {
                    switch (redChess[i][j].bianshi) {
                        case Ju: {
                            StretchBlt(g_mdc, point.x, point.y, 62 * proportion, 62 * proportion, g_bufdc, 248, 0, 62, 62, SRCPAINT);
                            break;
                        }
                        case Ma: {
                            StretchBlt(g_mdc, point.x, point.y, 62 * proportion, 62 * proportion, g_bufdc, 186, 0, 62, 62, SRCPAINT);
                            break;
                        }
                        case Xiang: {
                            StretchBlt(g_mdc, point.x, point.y, 62 * proportion, 62 * proportion, g_bufdc, 124, 0, 62, 62, SRCPAINT);
                            break;
                        }
                        case Shi: {
                            StretchBlt(g_mdc, point.x, point.y, 62 * proportion, 62 * proportion, g_bufdc, 62, 0, 62, 62, SRCPAINT);
                            break;
                        }
                        case Jiang: {
                            StretchBlt(g_mdc, point.x, point.y, 62 * proportion, 62 * proportion, g_bufdc, 0, 0, 62, 62, SRCPAINT);
                            break;
                        }
                        case Pao: {
                            StretchBlt(g_mdc, point.x, point.y, 62 * proportion, 62 * proportion, g_bufdc, 310, 0, 62, 62, SRCPAINT);
                            break;
                        }
                        case Bing: {
                            StretchBlt(g_mdc, point.x, point.y, 62 * proportion, 62 * proportion, g_bufdc, 372, 0, 62, 62, SRCPAINT);
                            break;
                        }
                    }
                }
            }
            if (blackChess[i][j].qizi) {
                point.x = i; point.y = j;
                point = PieceDown(point, false);
                SelectObject(g_bufdc, hPieceMask);
                StretchBlt(g_mdc, point.x, point.y, 62 * proportion, 62 * proportion, g_bufdc, 0, 0, 62, 62, SRCAND);
                SelectObject(g_bufdc, hPiece);
                if (blackChess[i][j].honghei) {
                    switch (blackChess[i][j].bianshi) {
                        case Ju: {
                            StretchBlt(g_mdc, point.x, point.y, 62 * proportion, 62 * proportion, g_bufdc, 248, 62, 62, 62, SRCPAINT);
                            break;
                        }
                        case Ma: {
                            StretchBlt(g_mdc, point.x, point.y, 62 * proportion, 62 * proportion, g_bufdc, 186, 62, 62, 62, SRCPAINT);
                            break;
                        }
                        case Xiang: {
                            StretchBlt(g_mdc, point.x, point.y, 62 * proportion, 62 * proportion, g_bufdc, 124, 62, 62, 62, SRCPAINT);
                            break;
                        }
                        case Shi: {
                            StretchBlt(g_mdc, point.x, point.y, 62 * proportion, 62 * proportion, g_bufdc, 62, 62, 62, 62, SRCPAINT);
                            break;
                        }
                        case Jiang: {
                            StretchBlt(g_mdc, point.x, point.y, 62 * proportion, 62 * proportion, g_bufdc, 0, 62, 62, 62, SRCPAINT);
                            break;
                        }
                        case Pao: {
                            StretchBlt(g_mdc, point.x, point.y, 62 * proportion, 62 * proportion, g_bufdc, 310, 62, 62, 62, SRCPAINT);
                            break;
                        }
                        case Bing: {
                            StretchBlt(g_mdc, point.x, point.y, 62 * proportion, 62 * proportion, g_bufdc, 372, 62, 62, 62, SRCPAINT);
                            break;
                        }
                    }
                }
                else {
                    switch (blackChess[i][j].bianshi) {
                        case Ju: {
                            StretchBlt(g_mdc, point.x, point.y, 62 * proportion, 62 * proportion, g_bufdc, 248, 0, 62, 62, SRCPAINT);
                            break;
                        }
                        case Ma: {
                            StretchBlt(g_mdc, point.x, point.y, 62 * proportion, 62 * proportion, g_bufdc, 186, 0, 62, 62, SRCPAINT);
                            break;
                        }
                        case Xiang: {
                            StretchBlt(g_mdc, point.x, point.y, 62 * proportion, 62 * proportion, g_bufdc, 124, 0, 62, 62, SRCPAINT);
                            break;
                        }
                        case Shi: {
                            StretchBlt(g_mdc, point.x, point.y, 62 * proportion, 62 * proportion, g_bufdc, 62, 0, 62, 62, SRCPAINT);
                            break;
                        }
                        case Jiang: {
                            StretchBlt(g_mdc, point.x, point.y, 62 * proportion, 62 * proportion, g_bufdc, 0, 0, 62, 62, SRCPAINT);
                            break;
                        }
                        case Pao: {
                            StretchBlt(g_mdc, point.x, point.y, 62 * proportion, 62 * proportion, g_bufdc, 310, 0, 62, 62, SRCPAINT);
                            break;
                        }
                        case Bing: {
                            StretchBlt(g_mdc, point.x, point.y, 62 * proportion, 62 * proportion, g_bufdc, 372, 0, 62, 62, SRCPAINT);
                            break;
                        }
                    }
                }
            }
            if (mouseHonghei) {
                if (redChess[i][j].daohang) {
                    point.x = i; point.y = j;
                    point = PieceDown(point, mouseHonghei);
                    SelectObject(g_bufdc, hDaohang);
                    StretchBlt(g_mdc, point.x, point.y, 62 * proportion, 62 * proportion, g_bufdc, 62, 0, 62, 62, SRCAND);
                    StretchBlt(g_mdc, point.x, point.y, 62 * proportion, 62 * proportion, g_bufdc, 0, 0, 62, 62, SRCPAINT);
                }
            }
            else {
                if (blackChess[i][j].daohang) {
                    point.x = i; point.y = j;
                    point = PieceDown(point, mouseHonghei);
                    SelectObject(g_bufdc, hDaohang);
                    StretchBlt(g_mdc, point.x, point.y, 62 * proportion, 62 * proportion, g_bufdc, 62, 0, 62, 62, SRCAND);
                    StretchBlt(g_mdc, point.x, point.y, 62 * proportion, 62 * proportion, g_bufdc, 0, 0, 62, 62, SRCPAINT);
                }
            }
        }
    }
    point = PieceDown(pieceSelected, mouseHonghei);
    SelectObject(g_bufdc, hFocus);
    StretchBlt(g_mdc, point.x, point.y, 62 * proportion, 62 * proportion, g_bufdc, 62, 0, 62, 62, SRCAND);
    StretchBlt(g_mdc, point.x, point.y, 62 * proportion, 62 * proportion, g_bufdc, 0, 0, 62, 62, SRCPAINT);
    BitBlt(g_hdc, 0, 0, g_ClientWidth, g_ClientHeight, g_mdc, 0, 0, SRCCOPY);
    g_tPre = GetTickCount();
    InvalidateRect(hChessList, NULL, false);
}

void GamePlaying::OnWindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    RECT *pRect;
    switch (message) {
        case WM_SIZING: {
            pRect = (RECT*)lParam;
            switch (wParam) {
                case WMSZ_TOPLEFT: {
                    pRect->left = pRect->right - g_ClientWidth - widthFrame;
                }
                case WMSZ_TOP:
                case WMSZ_TOPRIGHT: {
                    if (pRect->bottom - pRect->top > highBackground + heightFrame) {
                        pRect->top = pRect->bottom - highBackground - heightFrame;
                    }
                    else if (pRect->bottom - pRect->top < highBackground / 2 + heightFrame) {
                        pRect->top = pRect->bottom - highBackground / 2 - heightFrame;
                    }
                    g_ClientHeight = (pRect->bottom - pRect->top) - heightFrame;
                    proportion = (double)((pRect->bottom - pRect->top) - heightFrame) / highBackground;
                    g_ClientWidth = (int)((wideBackground - WideList) * proportion) + WideList;
                    pRect->right = g_ClientWidth + widthFrame;
                    break;
                }
                case WMSZ_BOTTOMLEFT: {
                    pRect->left = pRect->right - g_ClientWidth - widthFrame;
                }
                case WMSZ_BOTTOM:
                case WMSZ_BOTTOMRIGHT: {
                    if (pRect->bottom > highBackground + heightFrame) {
                        pRect->bottom = highBackground + heightFrame;
                    }
                    else if (pRect->bottom < highBackground / 2 + heightFrame) {
                        pRect->bottom = highBackground / 2 + heightFrame;
                    }
                    g_ClientHeight = (pRect->bottom) - heightFrame;
                    proportion = (double)((pRect->bottom) - heightFrame) / highBackground;
                    g_ClientWidth = (int)((wideBackground - WideList) * proportion) + WideList;
                    pRect->right = g_ClientWidth + widthFrame;
                    break;
                }
                case WMSZ_LEFT: {
                    if (pRect->right - pRect->left > wideBackground + widthFrame) {
                        pRect->left = pRect->right - wideBackground - widthFrame;
                    }
                    else if (pRect->right - pRect->left < wideBackground / 2 + widthFrame) {
                        pRect->left = pRect->right - wideBackground / 2 - widthFrame;
                    }
                    g_ClientWidth = (pRect->right - pRect->left) - widthFrame;
                    proportion = (double)(g_ClientWidth - WideList) / (wideBackground - WideList);
                    g_ClientHeight = (int)(highBackground * proportion);
                    pRect->bottom = g_ClientHeight + heightFrame;
                    break;
                }
                case WMSZ_RIGHT: {
                    if (pRect->right > wideBackground + widthFrame) {
                        pRect->right = wideBackground + widthFrame;
                    }
                    else if (pRect->right < wideBackground / 2 + widthFrame) {
                        pRect->right = wideBackground / 2 + widthFrame;
                    }
                    g_ClientWidth = (pRect->right) - widthFrame;
                    proportion = (double)(g_ClientWidth - WideList) / (wideBackground - WideList);
                    g_ClientHeight = (int)(highBackground * proportion);
                    pRect->bottom = g_ClientHeight + heightFrame;
                    break;
                }
            }
            MoveWindow(hChessList, (wideBackground - WideList) * proportion / 2, 62, WideList, highBackground * proportion - 62, true);
            break;
        }

        case WM_LBUTTONDOWN:
        {
            mouseClick = MouseClick(lParam);
            if (mouseYouxiao) {
                PieceMove(mouseClick);
            }
            else {
                PieceDaohang(mouseClick);
            }
        }
    }
}

void GamePlaying::End(HWND hWnd) {
    DeleteObject(hPlayingBackground);
    DeleteObject(hRedChessboard);
    DeleteObject(hBlackChessboard);
    DeleteObject(hFocus);
    DeleteObject(hPiece);
    DeleteObject(hPieceMask);
    DeleteObject(hDaohang);
    DeleteObject(hChessFont);
}

POINT GamePlaying::MouseClick(LPARAM lParam) {
    POINT point{ -1, -1 };
    if (mouseHonghei) {
        for (int i = 30; i < 600; i = i + 71) {
            for (int j = 30; j < 670; j = j + 71) {
                if (sqrt(pow(LOWORD(lParam) / proportion - i, 2) + pow(HIWORD(lParam) / proportion - j, 2)) <= 30) {
                    point.x = (i - 30) / 71;
                    point.y = (j - 30) / 71;
                }
            }
        }
    }
    else {
        for (int i = 30; i < 600; i = i + 71) {
            for (int j = 30; j < 670; j = j + 71) {
                if (sqrt(pow((LOWORD(lParam) - (WideChessboard * proportion) - WideList) / proportion - i, 2) + pow(HIWORD(lParam) / proportion - j, 2)) <= 30) {
                    point.x = (i - 30) / 71;
                    point.y = (j - 30) / 71;
                }
            }
        }
    }
    return point;
}

POINT GamePlaying::PieceDown(POINT point, bool honghei) {
    point.y = point.y * 71 * proportion;
    if (honghei) {
        point.x = point.x * 71 * proportion;
    }
    else {
        point.x = WideChessboard * proportion + WideList + point.x * 71 * proportion;
    }
    return point;
}

void GamePlaying::PieceDaohang(POINT point) {
    if (mouseHonghei) {
        if ((point.x >= 0) && (point.x <= 8) && (point.y >= 0) && (point.y <= 9) && redChess[point.x][point.y].qizi) {
            switch (redChess[point.x][point.y].bianshi) {
                case Jiang: {
                    if (redChess[point.x][point.y].honghei) {
                        pieceSelected = point;
                        switch (point.x) {
                            case 3: {
                                switch (point.y) {
                                    case 9: {
                                        if ((!redChess[4][9].qizi) || (redChess[4][9].qizi && (!redChess[4][9].honghei))) {
                                            mouseYouxiao = true;
                                            redChess[4][9].daohang = true;
                                        }
                                        if ((!redChess[3][8].qizi) || (redChess[3][8].qizi && (!redChess[3][8].honghei))) {
                                            mouseYouxiao = true;
                                            redChess[3][8].daohang = true;
                                        }
                                        break;
                                    }
                                    case 8: {
                                        if ((!redChess[4][8].qizi) || (redChess[4][8].qizi && (!redChess[4][8].honghei))) {
                                            mouseYouxiao = true;
                                            redChess[4][8].daohang = true;
                                        }
                                        if ((!redChess[3][9].qizi) || (redChess[3][9].qizi && (!redChess[3][9].honghei))) {
                                            mouseYouxiao = true;
                                            redChess[3][9].daohang = true;
                                        }
                                        if ((!redChess[3][7].qizi) || (redChess[3][7].qizi && (!redChess[3][7].honghei))) {
                                            mouseYouxiao = true;
                                            redChess[3][7].daohang = true;
                                        }
                                        break;
                                    }
                                    case 7: {
                                        if ((!redChess[3][8].qizi) || (redChess[3][8].qizi && (!redChess[3][8].honghei))) {
                                            mouseYouxiao = true;
                                            redChess[3][8].daohang = true;
                                        }
                                        if ((!redChess[4][7].qizi) || (redChess[4][7].qizi && (!redChess[4][7].honghei))) {
                                            mouseYouxiao = true;
                                            redChess[4][7].daohang = true;
                                        }
                                        break;
                                    }
                                }
                                break;
                            }
                            case 4: {
                                switch (point.y) {
                                    case 9: {
                                        if ((!redChess[3][9].qizi) || (redChess[3][9].qizi && (!redChess[3][9].honghei))) {
                                            mouseYouxiao = true;
                                            redChess[3][9].daohang = true;
                                        }
                                        if ((!redChess[5][9].qizi) || (redChess[5][9].qizi && (!redChess[5][9].honghei))) {
                                            mouseYouxiao = true;
                                            redChess[5][9].daohang = true;
                                        }
                                        if ((!redChess[4][8].qizi) || (redChess[4][8].qizi && (!redChess[4][8].honghei))) {
                                            mouseYouxiao = true;
                                            redChess[4][8].daohang = true;
                                        }
                                        break;
                                    }
                                    case 8: {
                                        if ((!redChess[3][8].qizi) || (redChess[3][8].qizi && (!redChess[3][8].honghei))) {
                                            mouseYouxiao = true;
                                            redChess[3][8].daohang = true;
                                        }
                                        if ((!redChess[5][8].qizi) || (redChess[5][8].qizi && (!redChess[5][8].honghei))) {
                                            mouseYouxiao = true;
                                            redChess[5][8].daohang = true;
                                        }
                                        if ((!redChess[4][9].qizi) || (redChess[4][9].qizi && (!redChess[4][9].honghei))) {
                                            mouseYouxiao = true;
                                            redChess[4][9].daohang = true;
                                        }
                                        if ((!redChess[4][7].qizi) || (redChess[4][7].qizi && (!redChess[4][7].honghei))) {
                                            mouseYouxiao = true;
                                            redChess[4][7].daohang = true;
                                        }
                                        break;
                                    }
                                    case 7: {
                                        if ((!redChess[3][7].qizi) || (redChess[3][7].qizi && (!redChess[3][7].honghei))) {
                                            mouseYouxiao = true;
                                            redChess[3][7].daohang = true;
                                        }
                                        if ((!redChess[5][7].qizi) || (redChess[5][7].qizi && (!redChess[5][7].honghei))) {
                                            mouseYouxiao = true;
                                            redChess[5][7].daohang = true;
                                        }
                                        if ((!redChess[4][8].qizi) || (redChess[4][8].qizi && (!redChess[4][8].honghei))) {
                                            mouseYouxiao = true;
                                            redChess[4][8].daohang = true;
                                        }
                                        break;
                                    }
                                }
                                break;
                            }
                            case 5: {
                                switch (point.y) {
                                    case 9: {
                                        if ((!redChess[4][9].qizi) || (redChess[4][9].qizi && (!redChess[4][9].honghei))) {
                                            mouseYouxiao = true;
                                            redChess[4][9].daohang = true;
                                        }
                                        if ((!redChess[5][8].qizi) || (redChess[5][8].qizi && (!redChess[5][8].honghei))) {
                                            mouseYouxiao = true;
                                            redChess[5][8].daohang = true;
                                        }
                                        break;
                                    }
                                    case 8: {
                                        if ((!redChess[4][8].qizi) || (redChess[4][8].qizi && (!redChess[4][8].honghei))) {
                                            mouseYouxiao = true;
                                            redChess[4][8].daohang = true;
                                        }
                                        if ((!redChess[5][9].qizi) || (redChess[5][9].qizi && (!redChess[5][9].honghei))) {
                                            mouseYouxiao = true;
                                            redChess[5][9].daohang = true;
                                        }
                                        if ((!redChess[5][7].qizi) || (redChess[5][7].qizi && (!redChess[5][7].honghei))) {
                                            mouseYouxiao = true;
                                            redChess[5][7].daohang = true;
                                        }
                                        break;
                                    }
                                    case 7: {
                                        if ((!redChess[4][7].qizi) || (redChess[4][7].qizi && (!redChess[4][7].honghei))) {
                                            mouseYouxiao = true;
                                            redChess[4][7].daohang = true;
                                        }
                                        if ((!redChess[5][8].qizi) || (redChess[5][8].qizi && (!redChess[5][8].honghei))) {
                                            mouseYouxiao = true;
                                            redChess[5][8].daohang = true;
                                        }
                                        break;
                                    }
                                }
                                break;
                            }
                        }

                    }
                    break;
                }
                case Shi: {
                    if (redChess[point.x][point.y].honghei) {
                        pieceSelected = point;
                        if (point.x == 4) {
                            if ((!redChess[3][9].qizi) || (redChess[3][9].qizi && (!redChess[3][9].honghei))) {
                                mouseYouxiao = true;
                                redChess[3][9].daohang = true;
                            }
                            if ((!redChess[3][7].qizi) || (redChess[3][7].qizi && (!redChess[3][7].honghei))) {
                                mouseYouxiao = true;
                                redChess[3][7].daohang = true;
                            }
                            if ((!redChess[5][9].qizi) || (redChess[5][9].qizi && (!redChess[5][9].honghei))) {
                                mouseYouxiao = true;
                                redChess[5][9].daohang = true;
                            }
                            if ((!redChess[5][7].qizi) || (redChess[5][7].qizi && (!redChess[5][7].honghei))) {
                                mouseYouxiao = true;
                                redChess[5][7].daohang = true;
                            }
                        }
                        else {
                            if ((!redChess[4][8].qizi) || (redChess[4][8].qizi && (!redChess[4][8].honghei))) {
                                mouseYouxiao = true;
                                redChess[4][8].daohang = true;
                            }
                        }
                    }
                    break;
                }
                case Xiang: {
                    if (redChess[point.x][point.y].honghei) {
                        pieceSelected = point;
                        switch (point.x) {
                            case 0: {
                                if ((!redChess[1][8].qizi) && ((!redChess[2][9].qizi) || (redChess[2][9].qizi && (!redChess[2][9].honghei)))) {
                                    mouseYouxiao = true;
                                    redChess[2][9].daohang = true;
                                }
                                if ((!redChess[1][6].qizi) && ((!redChess[2][5].qizi) || (redChess[2][5].qizi && (!redChess[2][5].honghei)))) {
                                    mouseYouxiao = true;
                                    redChess[2][5].daohang = true;
                                }
                                break;
                            }
                            case 2: {
                                if ((!redChess[0][7].qizi) || (redChess[0][7].qizi && (!redChess[0][7].honghei))) {
                                    if ((point.y == 9) && (!redChess[1][8].qizi)) {
                                        mouseYouxiao = true;
                                        redChess[0][7].daohang = true;
                                    }
                                    if ((point.y == 5) && (!redChess[1][6].qizi)) {
                                        mouseYouxiao = true;
                                        redChess[0][7].daohang = true;
                                    }
                                }
                                if ((!redChess[4][7].qizi) || (redChess[4][7].qizi && (!redChess[4][7].honghei))) {
                                    if ((point.y == 9) && (!redChess[3][8].qizi)) {
                                        mouseYouxiao = true;
                                        redChess[4][7].daohang = true;
                                    }
                                    if ((point.y == 5) && (!redChess[3][6].qizi)) {
                                        mouseYouxiao = true;
                                        redChess[4][7].daohang = true;
                                    }
                                }
                                break;
                            }
                            case 4: {
                                if ((!redChess[3][8].qizi) && ((!redChess[2][9].qizi) || (redChess[2][9].qizi && (!redChess[2][9].honghei)))) {
                                    mouseYouxiao = true;
                                    redChess[2][9].daohang = true;
                                }
                                if ((!redChess[3][6].qizi) && ((!redChess[2][5].qizi) || (redChess[2][5].qizi && (!redChess[2][5].honghei)))) {
                                    mouseYouxiao = true;
                                    redChess[2][5].daohang = true;
                                }
                                if ((!redChess[5][8].qizi) && ((!redChess[6][9].qizi) || (redChess[6][9].qizi && (!redChess[6][9].honghei)))) {
                                    mouseYouxiao = true;
                                    redChess[6][9].daohang = true;
                                }
                                if ((!redChess[5][6].qizi) && ((!redChess[6][5].qizi) || (redChess[6][5].qizi && (!redChess[6][5].honghei)))) {
                                    mouseYouxiao = true;
                                    redChess[6][5].daohang = true;
                                }
                                break;
                            }
                            case 6: {
                                if ((!redChess[8][7].qizi) || (redChess[8][7].qizi && (!redChess[8][7].honghei))) {
                                    if ((point.y == 9) && (!redChess[7][8].qizi)) {
                                        mouseYouxiao = true;
                                        redChess[8][7].daohang = true;
                                    }
                                    if ((point.y == 5) && (!redChess[7][6].qizi)) {
                                        mouseYouxiao = true;
                                        redChess[8][7].daohang = true;
                                    }
                                }
                                if ((!redChess[4][7].qizi) || (redChess[4][7].qizi && (!redChess[4][7].honghei))) {
                                    if ((point.y == 9) && (!redChess[5][8].qizi)) {
                                        mouseYouxiao = true;
                                        redChess[4][7].daohang = true;
                                    }
                                    if ((point.y == 5) && (!redChess[5][6].qizi)) {
                                        mouseYouxiao = true;
                                        redChess[4][7].daohang = true;
                                    }
                                }
                                break;
                            }
                            case 8: {
                                if ((!redChess[7][8].qizi) && ((!redChess[6][9].qizi) || (redChess[6][9].qizi && (!redChess[6][9].honghei)))) {
                                    mouseYouxiao = true;
                                    redChess[6][9].daohang = true;
                                }
                                if ((!redChess[7][6].qizi) && ((!redChess[6][5].qizi) || (redChess[6][5].qizi && (!redChess[6][5].honghei)))) {
                                    mouseYouxiao = true;
                                    redChess[6][5].daohang = true;
                                }
                                break;
                            }
                        }
                    }
                    break;
                }
                case Ma: {
                    if (redChess[point.x][point.y].honghei) {
                        pieceSelected = point;
                        if ((point.y > 1) && (!redChess[point.x][point.y - 1].qizi)) {
                            if ((point.x > 0) && ((!redChess[point.x - 1][point.y - 2].qizi) || (redChess[point.x - 1][point.y - 2].qizi && (!redChess[point.x - 1][point.y - 2].honghei)))) {
                                mouseYouxiao = true;
                                redChess[point.x - 1][point.y - 2].daohang = true;
                            }
                            if ((point.x < 8) && ((!redChess[point.x + 1][point.y - 2].qizi) || (redChess[point.x + 1][point.y - 2].qizi && (!redChess[point.x + 1][point.y - 2].honghei)))) {
                                mouseYouxiao = true;
                                redChess[point.x + 1][point.y - 2].daohang = true;
                            }
                        }
                        if ((point.y < 8) && (!redChess[point.x][point.y + 1].qizi)) {
                            if ((point.x > 0) && ((!redChess[point.x - 1][point.y + 2].qizi) || (redChess[point.x - 1][point.y + 2].qizi && (!redChess[point.x - 1][point.y + 2].honghei)))) {
                                mouseYouxiao = true;
                                redChess[point.x - 1][point.y + 2].daohang = true;
                            }
                            if ((point.x < 8) && ((!redChess[point.x + 1][point.y + 2].qizi) || (redChess[point.x + 1][point.y + 2].qizi && (!redChess[point.x + 1][point.y + 2].honghei)))) {
                                mouseYouxiao = true;
                                redChess[point.x + 1][point.y + 2].daohang = true;
                            }
                        }
                        if ((point.x > 1) && (!redChess[point.x - 1][point.y].qizi)) {
                            if ((point.y > 0) && ((!redChess[point.x - 2][point.y - 1].qizi) || (redChess[point.x - 2][point.y - 1].qizi && (!redChess[point.x - 2][point.y - 1].honghei)))) {
                                mouseYouxiao = true;
                                redChess[point.x - 2][point.y - 1].daohang = true;
                            }
                            if ((point.y < 9) && ((!redChess[point.x - 2][point.y + 1].qizi) || (redChess[point.x - 2][point.y + 1].qizi && (!redChess[point.x - 2][point.y + 1].honghei)))) {
                                mouseYouxiao = true;
                                redChess[point.x - 2][point.y + 1].daohang = true;
                            }
                        }
                        if ((point.x < 7) && (!redChess[point.x + 1][point.y].qizi)) {
                            if ((point.y > 0) && ((!redChess[point.x + 2][point.y - 1].qizi) || (redChess[point.x + 2][point.y - 1].qizi && (!redChess[point.x + 2][point.y - 1].honghei)))) {
                                mouseYouxiao = true;
                                redChess[point.x + 2][point.y - 1].daohang = true;
                            }
                            if ((point.y < 9) && ((!redChess[point.x + 2][point.y + 1].qizi) || (redChess[point.x + 2][point.y + 1].qizi && (!redChess[point.x + 2][point.y + 1].honghei)))) {
                                mouseYouxiao = true;
                                redChess[point.x + 2][point.y + 1].daohang = true;
                            }
                        }
                    }
                    break;
                }
                case Ju: {
                    if (redChess[point.x][point.y].honghei) {
                        pieceSelected = point;
                        for (int i = point.x - 1; i >= 0; --i) {
                            if (!redChess[i][point.y].qizi) {
                                mouseYouxiao = true;
                                redChess[i][point.y].daohang = true;
                            }
                            if (redChess[i][point.y].qizi) {
                                if (!redChess[i][point.y].honghei) {
                                    mouseYouxiao = true;
                                    redChess[i][point.y].daohang = true;
                                }
                                break;
                            }
                        }
                        for (int i = point.x + 1; i <= 8; ++i) {
                            if (!redChess[i][point.y].qizi) {
                                mouseYouxiao = true;
                                redChess[i][point.y].daohang = true;
                            }
                            if (redChess[i][point.y].qizi) {
                                if (!redChess[i][point.y].honghei) {
                                    mouseYouxiao = true;
                                    redChess[i][point.y].daohang = true;
                                }
                                break;
                            }
                        }
                        for (int i = point.y - 1; i >= 0; --i) {
                            if (!redChess[point.x][i].qizi) {
                                mouseYouxiao = true;
                                redChess[point.x][i].daohang = true;
                            }
                            if (redChess[point.x][i].qizi) {
                                if (!redChess[point.x][i].honghei) {
                                    mouseYouxiao = true;
                                    redChess[point.x][i].daohang = true;
                                }
                                break;
                            }
                        }
                        for (int i = point.y + 1; i <= 9; ++i) {
                            if (!redChess[point.x][i].qizi) {
                                mouseYouxiao = true;
                                redChess[point.x][i].daohang = true;
                            }
                            if (redChess[point.x][i].qizi) {
                                if (!redChess[point.x][i].honghei) {
                                    mouseYouxiao = true;
                                    redChess[point.x][i].daohang = true;
                                }
                                break;
                            }
                        }
                    }
                    break;
                }
                case Pao: {
                    static int paotai = -1; //表示炮台的x坐标或者y坐标
                    if (redChess[point.x][point.y].honghei) {
                        pieceSelected = point;
                        for (int i = point.x - 1; i >= 0; --i) {
                            if (redChess[i][point.y].qizi) {
                                paotai = i;
                                break;
                            }
                            mouseYouxiao = true;
                            redChess[i][point.y].daohang = true;
                        }
                        for (int i = paotai - 1; i >= 0; --i) {
                            if (redChess[i][point.y].qizi) {
                                if (!redChess[i][point.y].honghei) {
                                    mouseYouxiao = true;
                                    redChess[i][point.y].daohang = true;
                                }
                                break;
                            }
                        }
                        paotai = -1;
                        for (int i = point.x + 1; i <= 8; ++i) {
                            if (redChess[i][point.y].qizi) {
                                paotai = i;
                                break;
                            }
                            mouseYouxiao = true;
                            redChess[i][point.y].daohang = true;
                        }
                        for (int i = paotai + 1; i <= 8; ++i) {
                            if (redChess[i][point.y].qizi) {
                                if (!redChess[i][point.y].honghei) {
                                    mouseYouxiao = true;
                                    redChess[i][point.y].daohang = true;
                                }
                                break;
                            }
                        }
                        paotai = -1;
                        for (int i = point.y - 1; i >= 0; --i) {
                            if (redChess[point.x][i].qizi) {
                                paotai = i;
                                break;
                            }
                            mouseYouxiao = true;
                            redChess[point.x][i].daohang = true;
                        }
                        for (int i = paotai - 1; i >= 0; --i) {
                            if (redChess[point.x][i].qizi) {
                                if (!redChess[point.x][i].honghei) {
                                    mouseYouxiao = true;
                                    redChess[point.x][i].daohang = true;
                                }
                                break;
                            }
                        }
                        paotai = -1;
                        for (int i = point.y + 1; i <= 9; ++i) {
                            if (redChess[point.x][i].qizi) {
                                paotai = i;
                                break;
                            }
                            mouseYouxiao = true;
                            redChess[point.x][i].daohang = true;
                        }
                        for (int i = paotai + 1; i <= 9; ++i) {
                            if (redChess[point.x][i].qizi) {
                                if (!redChess[point.x][i].honghei) {
                                    mouseYouxiao = true;
                                    redChess[point.x][i].daohang = true;
                                }
                                break;
                            }
                        }
                        paotai = -1;
                    }
                    break;
                }
                case Bing: {
                    if (redChess[point.x][point.y].honghei) {
                        pieceSelected = point;
                        if (point.y >= 5) {
                            if ((!redChess[point.x][point.y - 1].qizi) || (redChess[point.x][point.y - 1].qizi && (!redChess[point.x][point.y - 1].honghei))) {
                                mouseYouxiao = true;
                                redChess[point.x][point.y - 1].daohang = true;
                            }
                        }
                        else {
                            if ((point.y - 1 >= 0) && ((!redChess[point.x][point.y - 1].qizi) || (redChess[point.x][point.y - 1].qizi && (!redChess[point.x][point.y - 1].honghei)))) {
                                mouseYouxiao = true;
                                redChess[point.x][point.y - 1].daohang = true;
                            }
                            if ((point.x - 1 >= 0) && ((!redChess[point.x - 1][point.y].qizi) || (redChess[point.x - 1][point.y].qizi && (!redChess[point.x - 1][point.y].honghei)))) {
                                mouseYouxiao = true;
                                redChess[point.x - 1][point.y].daohang = true;
                            }
                            if ((point.x + 1 <= 8) && ((!redChess[point.x + 1][point.y].qizi) || (redChess[point.x + 1][point.y].qizi && (!redChess[point.x + 1][point.y].honghei)))) {
                                mouseYouxiao = true;
                                redChess[point.x + 1][point.y].daohang = true;
                            }
                        }
                    }
                    break;
                }
            }
        }
    }
    else {
        if ((point.x >= 0) && (point.x <= 8) && (point.y >= 0) && (point.y <= 9) && blackChess[point.x][point.y].qizi) {
            switch (blackChess[point.x][point.y].bianshi) {
                case Jiang: {
                    if (!blackChess[point.x][point.y].honghei) {
                        pieceSelected = point;
                        switch (point.x) {
                            case 3: {
                                switch (point.y) {
                                    case 9: {
                                        if ((!blackChess[4][9].qizi) || (blackChess[4][9].qizi && blackChess[4][9].honghei)) {
                                            mouseYouxiao = true;
                                            blackChess[4][9].daohang = true;
                                        }
                                        if ((!blackChess[3][8].qizi) || (blackChess[3][8].qizi && blackChess[3][8].honghei)) {
                                            mouseYouxiao = true;
                                            blackChess[3][8].daohang = true;
                                        }
                                        break;
                                    }
                                    case 8: {
                                        if ((!blackChess[4][8].qizi) || (blackChess[4][8].qizi && blackChess[4][8].honghei)) {
                                            mouseYouxiao = true;
                                            blackChess[4][8].daohang = true;
                                        }
                                        if ((!blackChess[3][9].qizi) || (blackChess[3][9].qizi && blackChess[3][9].honghei)) {
                                            mouseYouxiao = true;
                                            blackChess[3][9].daohang = true;
                                        }
                                        if ((!blackChess[3][7].qizi) || (blackChess[3][7].qizi && blackChess[3][7].honghei)) {
                                            mouseYouxiao = true;
                                            blackChess[3][7].daohang = true;
                                        }
                                        break;
                                    }
                                    case 7: {
                                        if ((!blackChess[3][8].qizi) || (blackChess[3][8].qizi && blackChess[3][8].honghei)) {
                                            mouseYouxiao = true;
                                            blackChess[3][8].daohang = true;
                                        }
                                        if ((!blackChess[4][7].qizi) || (blackChess[4][7].qizi && blackChess[4][7].honghei)) {
                                            mouseYouxiao = true;
                                            blackChess[4][7].daohang = true;
                                        }
                                        break;
                                    }
                                }
                                break;
                            }
                            case 4: {
                                switch (point.y) {
                                    case 9: {
                                        if ((!blackChess[3][9].qizi) || (blackChess[3][9].qizi && blackChess[3][9].honghei)) {
                                            mouseYouxiao = true;
                                            blackChess[3][9].daohang = true;
                                        }
                                        if ((!blackChess[5][9].qizi) || (blackChess[5][0].qizi && blackChess[5][9].honghei)) {
                                            mouseYouxiao = true;
                                            blackChess[5][9].daohang = true;
                                        }
                                        if ((!blackChess[4][8].qizi) || (blackChess[4][8].qizi && blackChess[4][8].honghei)) {
                                            mouseYouxiao = true;
                                            blackChess[4][8].daohang = true;
                                        }
                                        break;
                                    }
                                    case 8: {
                                        if ((!blackChess[3][8].qizi) || (blackChess[3][8].qizi && blackChess[3][8].honghei)) {
                                            mouseYouxiao = true;
                                            blackChess[3][8].daohang = true;
                                        }
                                        if ((!blackChess[5][8].qizi) || (blackChess[5][8].qizi && blackChess[5][8].honghei)) {
                                            mouseYouxiao = true;
                                            blackChess[5][8].daohang = true;
                                        }
                                        if ((!blackChess[4][9].qizi) || (blackChess[4][9].qizi && blackChess[4][9].honghei)) {
                                            mouseYouxiao = true;
                                            blackChess[4][9].daohang = true;
                                        }
                                        if ((!blackChess[4][7].qizi) || (blackChess[4][7].qizi && blackChess[4][7].honghei)) {
                                            mouseYouxiao = true;
                                            blackChess[4][7].daohang = true;
                                        }
                                        break;
                                    }
                                    case 7: {
                                        if ((!blackChess[3][7].qizi) || (blackChess[3][7].qizi && blackChess[3][7].honghei)) {
                                            mouseYouxiao = true;
                                            blackChess[3][7].daohang = true;
                                        }
                                        if ((!blackChess[5][7].qizi) || (blackChess[5][7].qizi && blackChess[5][7].honghei)) {
                                            mouseYouxiao = true;
                                            blackChess[5][7].daohang = true;
                                        }
                                        if ((!blackChess[4][8].qizi) || (blackChess[4][8].qizi && blackChess[4][8].honghei)) {
                                            mouseYouxiao = true;
                                            blackChess[4][8].daohang = true;
                                        }
                                        break;
                                    }
                                }
                                break;
                            }
                            case 5: {
                                switch (point.y) {
                                    case 9: {
                                        if ((!blackChess[4][9].qizi) || (blackChess[4][9].qizi && blackChess[4][9].honghei)) {
                                            mouseYouxiao = true;
                                            blackChess[4][9].daohang = true;
                                        }
                                        if ((!blackChess[5][8].qizi) || (blackChess[5][8].qizi && blackChess[5][8].honghei)) {
                                            mouseYouxiao = true;
                                            blackChess[5][8].daohang = true;
                                        }
                                        break;
                                    }
                                    case 8: {
                                        if ((!blackChess[4][8].qizi) || (blackChess[4][8].qizi && blackChess[4][8].honghei)) {
                                            mouseYouxiao = true;
                                            blackChess[4][8].daohang = true;
                                        }
                                        if ((!blackChess[5][9].qizi) || (blackChess[5][9].qizi && blackChess[5][9].honghei)) {
                                            mouseYouxiao = true;
                                            blackChess[5][9].daohang = true;
                                        }
                                        if ((!blackChess[5][7].qizi) || (blackChess[5][7].qizi && blackChess[5][7].honghei)) {
                                            mouseYouxiao = true;
                                            blackChess[5][7].daohang = true;
                                        }
                                        break;
                                    }
                                    case 7: {
                                        if ((!blackChess[4][7].qizi) || (blackChess[4][7].qizi && blackChess[4][7].honghei)) {
                                            mouseYouxiao = true;
                                            blackChess[4][7].daohang = true;
                                        }
                                        if ((!blackChess[5][8].qizi) || (blackChess[5][8].qizi && blackChess[5][8].honghei)) {
                                            mouseYouxiao = true;
                                            blackChess[5][8].daohang = true;
                                        }
                                        break;
                                    }
                                }
                                break;
                            }
                        }
                    }
                    break;
                }
                case Shi: {
                    if (!blackChess[point.x][point.y].honghei) {
                        pieceSelected = point;
                        if (point.x == 4) {
                            if ((!blackChess[3][9].qizi) || (blackChess[3][9].qizi && blackChess[3][9].honghei)) {
                                mouseYouxiao = true;
                                blackChess[3][9].daohang = true;
                            }
                            if ((!blackChess[3][7].qizi) || (blackChess[3][7].qizi && blackChess[3][7].honghei)) {
                                mouseYouxiao = true;
                                blackChess[3][7].daohang = true;
                            }
                            if ((!blackChess[5][9].qizi) || (blackChess[5][9].qizi && blackChess[5][9].honghei)) {
                                mouseYouxiao = true;
                                blackChess[5][9].daohang = true;
                            }
                            if ((!blackChess[5][7].qizi) || (blackChess[5][7].qizi && blackChess[5][7].honghei)) {
                                mouseYouxiao = true;
                                blackChess[5][7].daohang = true;
                            }
                        }
                        else {
                            if ((!blackChess[4][8].qizi) || (blackChess[4][8].qizi && blackChess[4][8].honghei)) {
                                mouseYouxiao = true;
                                blackChess[4][8].daohang = true;
                            }
                        }
                    }
                    break;
                }
                case Xiang: {
                    if (!blackChess[point.x][point.y].honghei) {
                        pieceSelected = point;
                        switch (point.x) {
                            case 0: {
                                if ((!blackChess[1][8].qizi) && ((!blackChess[2][9].qizi) || (blackChess[2][9].qizi && blackChess[2][9].honghei))) {
                                    mouseYouxiao = true;
                                    blackChess[2][9].daohang = true;
                                }
                                if ((!blackChess[1][6].qizi) && ((!blackChess[2][5].qizi) || (blackChess[2][5].qizi && blackChess[2][5].honghei))) {
                                    mouseYouxiao = true;
                                    blackChess[2][5].daohang = true;
                                }
                                break;
                            }
                            case 2: {
                                if ((!blackChess[0][7].qizi) || (blackChess[0][7].qizi && blackChess[0][7].honghei)) {
                                    if ((point.y == 9) && (!blackChess[1][8].qizi)) {
                                        mouseYouxiao = true;
                                        blackChess[0][7].daohang = true;
                                    }
                                    if ((point.y == 5) && (!blackChess[1][6].qizi)) {
                                        mouseYouxiao = true;
                                        blackChess[0][7].daohang = true;
                                    }
                                }
                                if ((!blackChess[4][7].qizi) || (blackChess[4][7].qizi && blackChess[4][7].honghei)) {
                                    if ((point.y == 9) && (!blackChess[3][8].qizi)) {
                                        mouseYouxiao = true;
                                        blackChess[4][7].daohang = true;
                                    }
                                    if ((point.y == 5) && (!blackChess[3][6].qizi)) {
                                        mouseYouxiao = true;
                                        blackChess[4][7].daohang = true;
                                    }
                                }
                                break;
                            }
                            case 4: {
                                if ((!blackChess[3][8].qizi) && ((!blackChess[2][9].qizi) || (blackChess[2][9].qizi && blackChess[2][9].honghei))) {
                                    mouseYouxiao = true;
                                    blackChess[2][9].daohang = true;
                                }
                                if ((!blackChess[3][6].qizi) && ((!blackChess[2][5].qizi) || (blackChess[2][5].qizi && blackChess[2][5].honghei))) {
                                    mouseYouxiao = true;
                                    blackChess[2][5].daohang = true;
                                }
                                if ((!blackChess[5][8].qizi) && ((!blackChess[6][9].qizi) || (blackChess[6][9].qizi && blackChess[6][9].honghei))) {
                                    mouseYouxiao = true;
                                    blackChess[6][9].daohang = true;
                                }
                                if ((!blackChess[5][6].qizi) && ((!blackChess[6][5].qizi) || (blackChess[6][5].qizi && blackChess[6][5].honghei))) {
                                    mouseYouxiao = true;
                                    blackChess[6][5].daohang = true;
                                }
                                break;
                            }
                            case 6: {
                                if ((!blackChess[8][7].qizi) || (blackChess[8][7].qizi && blackChess[8][7].honghei)) {
                                    if ((point.y == 9) && (!blackChess[7][8].qizi)) {
                                        mouseYouxiao = true;
                                        blackChess[8][7].daohang = true;
                                    }
                                    if ((point.y == 5) && (!blackChess[7][6].qizi)) {
                                        mouseYouxiao = true;
                                        blackChess[8][7].daohang = true;
                                    }
                                }
                                if ((!blackChess[4][7].qizi) || (blackChess[4][7].qizi && blackChess[4][7].honghei)) {
                                    if ((point.y == 9) && (!blackChess[5][8].qizi)) {
                                        mouseYouxiao = true;
                                        blackChess[4][7].daohang = true;
                                    }
                                    if ((point.y == 5) && (!blackChess[5][6].qizi)) {
                                        mouseYouxiao = true;
                                        blackChess[4][7].daohang = true;
                                    }
                                }
                                break;
                            }
                            case 8: {
                                if ((!blackChess[7][8].qizi) && ((!blackChess[6][9].qizi) || (blackChess[6][9].qizi && blackChess[6][9].honghei))) {
                                    mouseYouxiao = true;
                                    blackChess[6][9].daohang = true;
                                }
                                if ((!blackChess[7][6].qizi) && ((!blackChess[6][5].qizi) || (blackChess[6][5].qizi && blackChess[6][5].honghei))) {
                                    mouseYouxiao = true;
                                    blackChess[6][5].daohang = true;
                                }
                                break;
                            }
                        }
                    }
                    break;
                }
                case Ma: {
                    if (!blackChess[point.x][point.y].honghei) {
                        pieceSelected = point;
                        if ((point.y > 1) && (!blackChess[point.x][point.y - 1].qizi)) {
                            if ((point.x > 0) && ((!blackChess[point.x - 1][point.y - 2].qizi) || (blackChess[point.x - 1][point.y - 2].qizi && blackChess[point.x - 1][point.y - 2].honghei))) {
                                mouseYouxiao = true;
                                blackChess[point.x - 1][point.y - 2].daohang = true;
                            }
                            if ((point.x < 8) && ((!blackChess[point.x + 1][point.y - 2].qizi) || (blackChess[point.x + 1][point.y - 2].qizi && blackChess[point.x + 1][point.y - 2].honghei))) {
                                mouseYouxiao = true;
                                blackChess[point.x + 1][point.y - 2].daohang = true;
                            }
                        }
                        if ((point.y < 8) && (!blackChess[point.x][point.y + 1].qizi)) {
                            if ((point.x > 0) && ((!blackChess[point.x - 1][point.y + 2].qizi) || (blackChess[point.x - 1][point.y + 2].qizi && blackChess[point.x - 1][point.y + 2].honghei))) {
                                mouseYouxiao = true;
                                blackChess[point.x - 1][point.y + 2].daohang = true;
                            }
                            if ((point.x < 8) && ((!blackChess[point.x + 1][point.y + 2].qizi) || (blackChess[point.x + 1][point.y + 2].qizi && blackChess[point.x + 1][point.y + 2].honghei))) {
                                mouseYouxiao = true;
                                blackChess[point.x + 1][point.y + 2].daohang = true;
                            }
                        }
                        if ((point.x > 1) && (!blackChess[point.x - 1][point.y].qizi)) {
                            if ((point.y > 0) && ((!blackChess[point.x - 2][point.y - 1].qizi) || (blackChess[point.x - 2][point.y - 1].qizi && blackChess[point.x - 2][point.y - 1].honghei))) {
                                mouseYouxiao = true;
                                blackChess[point.x - 2][point.y - 1].daohang = true;
                            }
                            if ((point.y < 9) && ((!blackChess[point.x - 2][point.y + 1].qizi) || (blackChess[point.x - 2][point.y + 1].qizi && blackChess[point.x - 2][point.y + 1].honghei))) {
                                mouseYouxiao = true;
                                blackChess[point.x - 2][point.y + 1].daohang = true;
                            }
                        }
                        if ((point.x < 7) && (!blackChess[point.x + 1][point.y].qizi)) {
                            if ((point.y > 0) && ((!blackChess[point.x + 2][point.y - 1].qizi) || (blackChess[point.x + 2][point.y - 1].qizi && blackChess[point.x + 2][point.y - 1].honghei))) {
                                mouseYouxiao = true;
                                blackChess[point.x + 2][point.y - 1].daohang = true;
                            }
                            if ((point.y < 9) && ((!blackChess[point.x + 2][point.y + 1].qizi) || (blackChess[point.x + 2][point.y + 1].qizi && blackChess[point.x + 2][point.y + 1].honghei))) {
                                mouseYouxiao = true;
                                blackChess[point.x + 2][point.y + 1].daohang = true;
                            }
                        }
                    }
                    break;
                }
                case Ju: {
                    if (!blackChess[point.x][point.y].honghei) {
                        pieceSelected = point;
                        for (int i = point.x - 1; i >= 0; --i) {
                            if (!blackChess[i][point.y].qizi) {
                                mouseYouxiao = true;
                                blackChess[i][point.y].daohang = true;
                            }
                            if (blackChess[i][point.y].qizi) {
                                if (blackChess[i][point.y].honghei) {
                                    mouseYouxiao = true;
                                    blackChess[i][point.y].daohang = true;
                                }
                                break;
                            }
                        }
                        for (int i = point.x + 1; i <= 8; ++i) {
                            if (!blackChess[i][point.y].qizi) {
                                mouseYouxiao = true;
                                blackChess[i][point.y].daohang = true;
                            }
                            if (blackChess[i][point.y].qizi) {
                                if (blackChess[i][point.y].honghei) {
                                    mouseYouxiao = true;
                                    blackChess[i][point.y].daohang = true;
                                }
                                break;
                            }
                        }
                        for (int i = point.y - 1; i >= 0; --i) {
                            if (!blackChess[point.x][i].qizi) {
                                mouseYouxiao = true;
                                blackChess[point.x][i].daohang = true;
                            }
                            if (blackChess[point.x][i].qizi) {
                                if (blackChess[point.x][i].honghei) {
                                    mouseYouxiao = true;
                                    blackChess[point.x][i].daohang = true;
                                }
                                break;
                            }
                        }
                        for (int i = point.y + 1; i <= 9; ++i) {
                            if (!blackChess[point.x][i].qizi) {
                                mouseYouxiao = true;
                                blackChess[point.x][i].daohang = true;
                            }
                            if (blackChess[point.x][i].qizi) {
                                if (blackChess[point.x][i].honghei) {
                                    mouseYouxiao = true;
                                    blackChess[point.x][i].daohang = true;
                                }
                                break;
                            }
                        }
                    }
                    break;
                }
                case Pao: {
                    static int paotai = -1;; //表示炮台的x坐标或者y坐标
                    if (!blackChess[point.x][point.y].honghei) {
                        pieceSelected = point;
                        for (int i = point.x - 1; i >= 0; --i) {
                            if (blackChess[i][point.y].qizi) {
                                paotai = i;
                                break;
                            }
                            mouseYouxiao = true;
                            blackChess[i][point.y].daohang = true;
                        }
                        for (int i = paotai - 1; i >= 0; --i) {
                            if (blackChess[i][point.y].qizi) {
                                if (blackChess[i][point.y].honghei) {
                                    mouseYouxiao = true;
                                    blackChess[i][point.y].daohang = true;
                                }
                                break;
                            }
                        }
                        paotai = -1;
                        for (int i = point.x + 1; i <= 8; ++i) {
                            if (blackChess[i][point.y].qizi) {
                                paotai = i;
                                break;
                            }
                            mouseYouxiao = true;
                            blackChess[i][point.y].daohang = true;
                        }
                        for (int i = paotai + 1; i <= 8; ++i) {
                            if (blackChess[i][point.y].qizi) {
                                if (blackChess[i][point.y].honghei) {
                                    mouseYouxiao = true;
                                    blackChess[i][point.y].daohang = true;
                                }
                                break;
                            }
                        }
                        paotai = -1;
                        for (int i = point.y - 1; i >= 0; --i) {
                            if (blackChess[point.x][i].qizi) {
                                paotai = i;
                                break;
                            }
                            mouseYouxiao = true;
                            blackChess[point.x][i].daohang = true;
                        }
                        for (int i = paotai - 1; i >= 0; --i) {
                            if (blackChess[point.x][i].qizi) {
                                if (blackChess[point.x][i].honghei) {
                                    mouseYouxiao = true;
                                    blackChess[point.x][i].daohang = true;
                                }
                                break;
                            }
                        }
                        paotai = -1;
                        for (int i = point.y + 1; i <= 9; ++i) {
                            if (blackChess[point.x][i].qizi) {
                                paotai = i;
                                break;
                            }
                            mouseYouxiao = true;
                            blackChess[point.x][i].daohang = true;
                        }
                        for (int i = paotai + 1; i <= 9; ++i) {
                            if (blackChess[point.x][i].qizi) {
                                if (blackChess[point.x][i].honghei) {
                                    mouseYouxiao = true;
                                    blackChess[point.x][i].daohang = true;
                                }
                                break;
                            }
                        }
                        paotai = -1;
                    }
                    break;
                }
                case Bing: {
                    if (!blackChess[point.x][point.y].honghei) {
                        pieceSelected = point;
                        if (point.y >= 5) {
                            if ((!blackChess[point.x][point.y - 1].qizi) || (blackChess[point.x][point.y - 1].qizi && blackChess[point.x][point.y - 1].honghei)) {
                                mouseYouxiao = true;
                                blackChess[point.x][point.y - 1].daohang = true;
                            }
                        }
                        else {
                            if ((point.y - 1 >= 0) && ((!blackChess[point.x][point.y - 1].qizi) || (blackChess[point.x][point.y - 1].qizi && blackChess[point.x][point.y - 1].honghei))) {
                                mouseYouxiao = true;
                                blackChess[point.x][point.y - 1].daohang = true;
                            }
                            if ((point.x - 1 >= 0) && ((!blackChess[point.x - 1][point.y].qizi) || (blackChess[point.x - 1][point.y].qizi && blackChess[point.x - 1][point.y].honghei))) {
                                mouseYouxiao = true;
                                blackChess[point.x - 1][point.y].daohang = true;
                            }
                            if ((point.x + 1 <= 8) && ((!blackChess[point.x + 1][point.y].qizi) || (blackChess[point.x + 1][point.y].qizi && blackChess[point.x + 1][point.y].honghei))) {
                                mouseYouxiao = true;
                                blackChess[point.x + 1][point.y].daohang = true;
                            }
                        }
                    }
                    break;
                }
            }
        }

    }
}

void GamePlaying::PieceUnDaohang() {
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 10; ++j) {
            redChess[i][j].daohang = false;
            blackChess[i][j].daohang = false;
        }
    }
}

void GamePlaying::PieceMove(POINT point) {
    mouseYouxiao = false;
    if (mouseHonghei) {
        if (redChess[point.x][point.y].daohang) {
            Duiju(pieceSelected, point);
            SendMessage(hChessList, LB_ADDSTRING, NULL, (LPARAM)duiju);
            redChess[point.x][point.y].qizi = true;
            redChess[point.x][point.y].honghei = redChess[pieceSelected.x][pieceSelected.y].honghei;
            redChess[point.x][point.y].bianshi = redChess[pieceSelected.x][pieceSelected.y].bianshi;
            redChess[pieceSelected.x][pieceSelected.y].qizi = false;
            redChess[pieceSelected.x][pieceSelected.y].honghei = true;
            redChess[pieceSelected.x][pieceSelected.y].bianshi = Jiang;
            blackChess[8 - point.x][9 - point.y].qizi = true;
            blackChess[8 - point.x][9 - point.y].honghei = blackChess[8 - pieceSelected.x][9 - pieceSelected.y].honghei;
            blackChess[8 - point.x][9 - point.y].bianshi = blackChess[8 - pieceSelected.x][9 - pieceSelected.y].bianshi;
            blackChess[8 - pieceSelected.x][9 - pieceSelected.y].qizi = false;
            blackChess[8 - pieceSelected.x][9 - pieceSelected.y].honghei = true;
            blackChess[8 - pieceSelected.x][9 - pieceSelected.y].bianshi = Jiang;
            mouseHonghei = !mouseHonghei;
            PieceUnDaohang();
            pieceSelected.x = -1;
            pieceSelected.y = -1;
        }
        else if (redChess[point.x][point.y].qizi && (redChess[point.x][point.y].honghei == mouseHonghei)) {
            PieceUnDaohang();
            PieceDaohang(point);
        }
        else {
            pieceSelected.x = -1;
            pieceSelected.y = -1;
            PieceUnDaohang();
        }
    }
    else {
        if (blackChess[point.x][point.y].daohang) {
            Duiju(pieceSelected, point);
            SendMessage(hChessList, LB_ADDSTRING, NULL, (LPARAM)duiju);
            blackChess[point.x][point.y].qizi = true;
            blackChess[point.x][point.y].honghei = blackChess[pieceSelected.x][pieceSelected.y].honghei;
            blackChess[point.x][point.y].bianshi = blackChess[pieceSelected.x][pieceSelected.y].bianshi;
            blackChess[pieceSelected.x][pieceSelected.y].qizi = false;
            blackChess[pieceSelected.x][pieceSelected.y].honghei = true;
            blackChess[pieceSelected.x][pieceSelected.y].bianshi = Jiang;
            redChess[8 - point.x][9 - point.y].qizi = true;
            redChess[8 - point.x][9 - point.y].honghei = redChess[8 - pieceSelected.x][9 - pieceSelected.y].honghei;
            redChess[8 - point.x][9 - point.y].bianshi = redChess[8 - pieceSelected.x][9 - pieceSelected.y].bianshi;
            redChess[8 - pieceSelected.x][9 - pieceSelected.y].qizi = false;
            redChess[8 - pieceSelected.x][9 - pieceSelected.y].honghei = true;
            redChess[8 - pieceSelected.x][9 - pieceSelected.y].bianshi = Jiang;
            mouseHonghei = !mouseHonghei;
            PieceUnDaohang();
            pieceSelected.x = -1;
            pieceSelected.y = -1;
        }
        else if (blackChess[point.x][point.y].qizi && (blackChess[point.x][point.y].honghei == mouseHonghei)) {
            PieceUnDaohang();
            PieceDaohang(point);
        }
        else {
            pieceSelected.x = -1;
            pieceSelected.y = -1;
            PieceUnDaohang();
        }
    }
}

void GamePlaying::FlipChess(bool honghei) {
    if (honghei) {
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 10; ++j) {
                blackChess[i][j] = redChess[8 - i][9 - j];
            }
        }
    }
    else {
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 10; ++j) {
               redChess[i][j] = blackChess[8 - i][9 - j];
            }
        }
    }
}

void GamePlaying::Duiju(POINT oldPoint, POINT newPoint) {
    strcpy_s(duiju, TEXT("\0"));
    TCHAR str1[3] = "出";
    TCHAR str2[3] = "错";
    TCHAR str3[3] = "了";
    TCHAR str4[3] = "吧";
    bool youmeiyou = false;
    if (mouseHonghei) {
        switch (redChess[oldPoint.x][oldPoint.y].bianshi) {
            case Jiang: {
                strcpy_s(str1, TEXT("帅\0"));
                switch (oldPoint.x) {
                    case 3: {
                        strcpy_s(str2, TEXT("六\0"));
                        break;
                    }
                    case 4: {
                        strcpy_s(str2, TEXT("五\0"));
                        break;
                    }
                    case 5: {
                        strcpy_s(str2, TEXT("四\0"));
                        break;
                    }
                }
                if (newPoint.y == oldPoint.y) {
                    strcpy_s(str3, TEXT("平\0"));
                    switch (newPoint.x) {
                        case 3: {
                            strcpy_s(str4, TEXT("六\0"));
                            break;
                        }
                        case 4: {
                            strcpy_s(str4, TEXT("五\0"));
                            break;
                        }
                        case 5: {
                            strcpy_s(str4, TEXT("四\0"));
                            break;
                        }
                    }
                }
                else if (newPoint.y < oldPoint.y) {
                    strcpy_s(str3, TEXT("进\0"));
                    switch (oldPoint.y - newPoint.y) {
                        case 1: {
                            strcpy_s(str4, TEXT("一\0"));
                            break;
                        }
                        case 5: {
                            strcpy_s(str4, TEXT("五\0"));
                            break;
                        }
                        case 6: {
                            strcpy_s(str4, TEXT("六\0"));
                            break;
                        }
                        case 7: {
                            strcpy_s(str4, TEXT("七\0"));
                            break;
                        }
                        case 8: {
                            strcpy_s(str4, TEXT("八\0"));
                            break;
                        }
                        case 9: {
                            strcpy_s(str4, TEXT("九\0"));
                            break;
                        }
                    }
                }
                else {
                    strcpy_s(str3, TEXT("退\0"));
                    strcpy_s(str4, TEXT("一\0"));
                }
                break;
            }
            case Shi: {
                strcpy_s(str1, TEXT("仕\0"));
                switch (oldPoint.x) {
                    case 3: {
                        strcpy_s(str2, TEXT("六\0"));
                        break;
                    }
                    case 4: {
                        strcpy_s(str2, TEXT("五\0"));
                        break;
                    }
                    case 5: {
                        strcpy_s(str2, TEXT("四\0"));
                        break;
                    }
                }
                if (newPoint.y < oldPoint.y) {
                    strcpy_s(str3, TEXT("进"));
                }
                else {
                    strcpy_s(str3, TEXT("退"));
                }
                switch (newPoint.x) {
                    case 3: {
                        strcpy_s(str4, TEXT("六\0"));
                        break;
                    }
                    case 4: {
                        strcpy_s(str4, TEXT("五\0"));
                        break;
                    }
                    case 5: {
                        strcpy_s(str4, TEXT("四\0"));
                        break;
                    }
                }
                break;
            }
            case Xiang: {
                strcpy_s(str1, TEXT("相\0"));
                switch (oldPoint.x) {
                    case 0: {
                        strcpy_s(str2, TEXT("九\0"));
                        break;
                    }
                    case 2: {
                        strcpy_s(str2, TEXT("七\0"));
                        break;
                    }
                    case 4: {
                        strcpy_s(str2, TEXT("五\0"));
                        break;
                    }
                    case 6: {
                        strcpy_s(str2, TEXT("三\0"));
                        break;
                    }
                    case 8: {
                        strcpy_s(str2, TEXT("一\0"));
                        break;
                    }
                }
                if (newPoint.y < oldPoint.y) {
                    strcpy_s(str3, TEXT("进"));
                }
                else {
                    strcpy_s(str3, TEXT("退"));
                }
                switch (newPoint.x) {
                    case 0: {
                        strcpy_s(str4, TEXT("九\0"));
                        break;
                    }
                    case 2: {
                        strcpy_s(str4, TEXT("七\0"));
                        break;
                    }
                    case 4: {
                        strcpy_s(str4, TEXT("五\0"));
                        break;
                    }
                    case 6: {
                        strcpy_s(str4, TEXT("三\0"));
                        break;
                    }
                    case 8: {
                        strcpy_s(str4, TEXT("一\0"));
                        break;
                    }
                }

                break;
            }
            case Ma: {
                for (int i = 0; i < 10; ++i) {
                    if (i == oldPoint.y) {
                        continue;
                    }
                    if (redChess[oldPoint.x][i].honghei && (redChess[oldPoint.x][i].bianshi == Ma)) {
                        youmeiyou = true;
                        if (i < oldPoint.y) {
                            strcpy_s(str1, TEXT("后"));
                        }
                        else {
                            strcpy_s(str1, TEXT("前"));
                        }
                        strcpy_s(str2, TEXT("马"));
                    }
                }
                if (!youmeiyou) {
                    strcpy_s(str1, TEXT("马"));
                    switch (oldPoint.x) {
                        case 0: {
                            strcpy_s(str2, TEXT("九"));
                            break;
                        }
                        case 1: {
                            strcpy_s(str2, TEXT("八"));
                            break;
                        }
                        case 2: {
                            strcpy_s(str2, TEXT("七"));
                            break;
                        }
                        case 3: {
                            strcpy_s(str2, TEXT("六"));
                            break;
                        }
                        case 4: {
                            strcpy_s(str2, TEXT("五"));
                            break;
                        }
                        case 5: {
                            strcpy_s(str2, TEXT("四"));
                            break;
                        }
                        case 6: {
                            strcpy_s(str2, TEXT("三"));
                            break;
                        }
                        case 7: {
                            strcpy_s(str2, TEXT("二"));
                            break;
                        }
                        case 8: {
                            strcpy_s(str2, TEXT("一"));
                            break;
                        }
                    }
                }
                if (newPoint.y < oldPoint.y) {
                    strcpy_s(str3, TEXT("进"));
                }
                else {
                    strcpy_s(str3, TEXT("退"));
                }
                switch (newPoint.x) {
                    case 0: {
                        strcpy_s(str4, TEXT("九"));
                        break;
                    }
                    case 1: {
                        strcpy_s(str4, TEXT("八"));
                        break;
                    }
                    case 2: {
                        strcpy_s(str4, TEXT("七"));
                        break;
                    }
                    case 3: {
                        strcpy_s(str4, TEXT("六"));
                        break;
                    }
                    case 4: {
                        strcpy_s(str4, TEXT("五"));
                        break;
                    }
                    case 5: {
                        strcpy_s(str4, TEXT("四"));
                        break;
                    }
                    case 6: {
                        strcpy_s(str4, TEXT("三"));
                        break;
                    }
                    case 7: {
                        strcpy_s(str4, TEXT("二"));
                        break;
                    }
                    case 8: {
                        strcpy_s(str4, TEXT("一"));
                        break;
                    }
                }
                break;
            }
            case Ju: {
                for (int i = 0; i < 10; ++i) {
                    if (i == oldPoint.y) {
                        continue;
                    }
                    if (redChess[oldPoint.x][i].honghei && (redChess[oldPoint.x][i].bianshi == Ju)) {
                        youmeiyou = true;
                        if (i < oldPoint.y) {
                            strcpy_s(str1, TEXT("后"));
                        }
                        else {
                            strcpy_s(str1, TEXT("前"));
                        }
                        strcpy_s(str2, TEXT("车"));
                    }
                }
                if (!youmeiyou) {
                    strcpy_s(str1, TEXT("车"));
                    switch (oldPoint.x) {
                        case 0: {
                            strcpy_s(str2, TEXT("九"));
                            break;
                        }
                        case 1: {
                            strcpy_s(str2, TEXT("八"));
                            break;
                        }
                        case 2: {
                            strcpy_s(str2, TEXT("七"));
                            break;
                        }
                        case 3: {
                            strcpy_s(str2, TEXT("六"));
                            break;
                        }
                        case 4: {
                            strcpy_s(str2, TEXT("五"));
                            break;
                        }
                        case 5: {
                            strcpy_s(str2, TEXT("四"));
                            break;
                        }
                        case 6: {
                            strcpy_s(str2, TEXT("三"));
                            break;
                        }
                        case 7: {
                            strcpy_s(str2, TEXT("二"));
                            break;
                        }
                        case 8: {
                            strcpy_s(str2, TEXT("一"));
                            break;
                        }
                    }
                }
                if (newPoint.y == oldPoint.y) {
                    strcpy_s(str3, TEXT("平"));
                    switch (newPoint.x) {
                        case 0: {
                            strcpy_s(str4, TEXT("九"));
                            break;
                        }
                        case 1: {
                            strcpy_s(str4, TEXT("八"));
                            break;
                        }
                        case 2: {
                            strcpy_s(str4, TEXT("七"));
                            break;
                        }
                        case 3: {
                            strcpy_s(str4, TEXT("六"));
                            break;
                        }
                        case 4: {
                            strcpy_s(str4, TEXT("五"));
                            break;
                        }
                        case 5: {
                            strcpy_s(str4, TEXT("四"));
                            break;
                        }
                        case 6: {
                            strcpy_s(str4, TEXT("三"));
                            break;
                        }
                        case 7: {
                            strcpy_s(str4, TEXT("二"));
                            break;
                        }
                        case 8: {
                            strcpy_s(str4, TEXT("一"));
                            break;
                        }
                    }
                }
                else if (newPoint.y < oldPoint.y) {
                    strcpy_s(str3, TEXT("进"));
                    switch (oldPoint.y - newPoint.y) {
                        case 1: {
                            strcpy_s(str4, TEXT("一"));
                            break;
                        }
                        case 2: {
                            strcpy_s(str4, TEXT("二"));
                            break;
                        }
                        case 3: {
                            strcpy_s(str4, TEXT("三"));
                            break;
                        }
                        case 4: {
                            strcpy_s(str4, TEXT("四"));
                            break;
                        }
                        case 5: {
                            strcpy_s(str4, TEXT("五"));
                            break;
                        }
                        case 6: {
                            strcpy_s(str4, TEXT("六"));
                            break;
                        }
                        case 7: {
                            strcpy_s(str4, TEXT("七"));
                            break;
                        }
                        case 8: {
                            strcpy_s(str4, TEXT("八"));
                            break;
                        }
                        case 9: {
                            strcpy_s(str4, TEXT("九"));
                            break;
                        }
                    }
                }
                else {
                    strcpy_s(str3, TEXT("退"));
                    switch (newPoint.y - oldPoint.y) {
                        case 1: {
                            strcpy_s(str4, TEXT("一"));
                            break;
                        }
                        case 2: {
                            strcpy_s(str4, TEXT("二"));
                            break;
                        }
                        case 3: {
                            strcpy_s(str4, TEXT("三"));
                            break;
                        }
                        case 4: {
                            strcpy_s(str4, TEXT("四"));
                            break;
                        }
                        case 5: {
                            strcpy_s(str4, TEXT("五"));
                            break;
                        }
                        case 6: {
                            strcpy_s(str4, TEXT("六"));
                            break;
                        }
                        case 7: {
                            strcpy_s(str4, TEXT("七"));
                            break;
                        }
                        case 8: {
                            strcpy_s(str4, TEXT("八"));
                            break;
                        }
                        case 9: {
                            strcpy_s(str4, TEXT("九"));
                            break;
                        }
                    }
                }
                break;
            }
            case Pao: {
                for (int i = 0; i < 10; ++i) {
                    if (i == oldPoint.y) {
                        continue;
                    }
                    if (redChess[oldPoint.x][i].honghei && (redChess[oldPoint.x][i].bianshi == Pao)) {
                        youmeiyou = true;
                        if (i < oldPoint.y) {
                            strcpy_s(str1, TEXT("后"));
                        }
                        else {
                            strcpy_s(str1, TEXT("前"));
                        }
                        strcpy_s(str2, TEXT("炮"));
                    }
                }
                if (!youmeiyou) {
                    strcpy_s(str1, TEXT("炮"));
                    switch (oldPoint.x) {
                        case 0: {
                            strcpy_s(str2, TEXT("九"));
                            break;
                        }
                        case 1: {
                            strcpy_s(str2, TEXT("八"));
                            break;
                        }
                        case 2: {
                            strcpy_s(str2, TEXT("七"));
                            break;
                        }
                        case 3: {
                            strcpy_s(str2, TEXT("六"));
                            break;
                        }
                        case 4: {
                            strcpy_s(str2, TEXT("五"));
                            break;
                        }
                        case 5: {
                            strcpy_s(str2, TEXT("四"));
                            break;
                        }
                        case 6: {
                            strcpy_s(str2, TEXT("三"));
                            break;
                        }
                        case 7: {
                            strcpy_s(str2, TEXT("二"));
                            break;
                        }
                        case 8: {
                            strcpy_s(str2, TEXT("一"));
                            break;
                        }
                    }
                }
                if (newPoint.y == oldPoint.y) {
                    strcpy_s(str3, TEXT("平"));
                    switch (newPoint.x) {
                        case 0: {
                            strcpy_s(str4, TEXT("九"));
                            break;
                        }
                        case 1: {
                            strcpy_s(str4, TEXT("八"));
                            break;
                        }
                        case 2: {
                            strcpy_s(str4, TEXT("七"));
                            break;
                        }
                        case 3: {
                            strcpy_s(str4, TEXT("六"));
                            break;
                        }
                        case 4: {
                            strcpy_s(str4, TEXT("五"));
                            break;
                        }
                        case 5: {
                            strcpy_s(str4, TEXT("四"));
                            break;
                        }
                        case 6: {
                            strcpy_s(str4, TEXT("三"));
                            break;
                        }
                        case 7: {
                            strcpy_s(str4, TEXT("二"));
                            break;
                        }
                        case 8: {
                            strcpy_s(str4, TEXT("一"));
                            break;
                        }
                    }
                }
                else if (newPoint.y < oldPoint.y) {
                    strcpy_s(str3, TEXT("进"));
                    switch (oldPoint.y - newPoint.y) {
                        case 1: {
                            strcpy_s(str4, TEXT("一"));
                            break;
                        }
                        case 2: {
                            strcpy_s(str4, TEXT("二"));
                            break;
                        }
                        case 3: {
                            strcpy_s(str4, TEXT("三"));
                            break;
                        }
                        case 4: {
                            strcpy_s(str4, TEXT("四"));
                            break;
                        }
                        case 5: {
                            strcpy_s(str4, TEXT("五"));
                            break;
                        }
                        case 6: {
                            strcpy_s(str4, TEXT("六"));
                            break;
                        }
                        case 7: {
                            strcpy_s(str4, TEXT("七"));
                            break;
                        }
                        case 8: {
                            strcpy_s(str4, TEXT("八"));
                            break;
                        }
                        case 9: {
                            strcpy_s(str4, TEXT("九"));
                            break;
                        }
                    }
                }
                else {
                    strcpy_s(str3, TEXT("退"));
                    switch (newPoint.y - oldPoint.y) {
                        case 1: {
                            strcpy_s(str4, TEXT("一"));
                            break;
                        }
                        case 2: {
                            strcpy_s(str4, TEXT("二"));
                            break;
                        }
                        case 3: {
                            strcpy_s(str4, TEXT("三"));
                            break;
                        }
                        case 4: {
                            strcpy_s(str4, TEXT("四"));
                            break;
                        }
                        case 5: {
                            strcpy_s(str4, TEXT("五"));
                            break;
                        }
                        case 6: {
                            strcpy_s(str4, TEXT("六"));
                            break;
                        }
                        case 7: {
                            strcpy_s(str4, TEXT("七"));
                            break;
                        }
                        case 8: {
                            strcpy_s(str4, TEXT("八"));
                            break;
                        }
                        case 9: {
                            strcpy_s(str4, TEXT("九"));
                            break;
                        }
                    }
                }
                break;
            }
            case Bing: {
                for (int i = 0; i < 10; ++i) {
                    if (i == oldPoint.y) {
                        continue;
                    }
                    if (redChess[oldPoint.x][i].honghei && (redChess[oldPoint.x][i].bianshi == Bing)) {
                        youmeiyou = true;
                        if (i < oldPoint.y) {
                            strcpy_s(str1, TEXT("后"));
                        }
                        else {
                            strcpy_s(str1, TEXT("前"));
                        }
                        strcpy_s(str2, TEXT("兵"));
                    }
                }
                if (!youmeiyou) {
                    strcpy_s(str1, TEXT("兵"));
                    switch (oldPoint.x) {
                        case 0: {
                            strcpy_s(str2, TEXT("九"));
                            break;
                        }
                        case 1: {
                            strcpy_s(str2, TEXT("八"));
                            break;
                        }
                        case 2: {
                            strcpy_s(str2, TEXT("七"));
                            break;
                        }
                        case 3: {
                            strcpy_s(str2, TEXT("六"));
                            break;
                        }
                        case 4: {
                            strcpy_s(str2, TEXT("五"));
                            break;
                        }
                        case 5: {
                            strcpy_s(str2, TEXT("四"));
                            break;
                        }
                        case 6: {
                            strcpy_s(str2, TEXT("三"));
                            break;
                        }
                        case 7: {
                            strcpy_s(str2, TEXT("二"));
                            break;
                        }
                        case 8: {
                            strcpy_s(str2, TEXT("一"));
                            break;
                        }
                    }
                }
                if (newPoint.y == oldPoint.y) {
                    strcpy_s(str3, TEXT("平"));
                    switch (newPoint.x) {
                        case 0: {
                            strcpy_s(str4, TEXT("九"));
                            break;
                        }
                        case 1: {
                            strcpy_s(str4, TEXT("八"));
                            break;
                        }
                        case 2: {
                            strcpy_s(str4, TEXT("七"));
                            break;
                        }
                        case 3: {
                            strcpy_s(str4, TEXT("六"));
                            break;
                        }
                        case 4: {
                            strcpy_s(str4, TEXT("五"));
                            break;
                        }
                        case 5: {
                            strcpy_s(str4, TEXT("四"));
                            break;
                        }
                        case 6: {
                            strcpy_s(str4, TEXT("三"));
                            break;
                        }
                        case 7: {
                            strcpy_s(str4, TEXT("二"));
                            break;
                        }
                        case 8: {
                            strcpy_s(str4, TEXT("一"));
                            break;
                        }
                    }
                }
                else {
                    strcpy_s(str3, TEXT("进"));
                    strcpy_s(str4, TEXT("一"));
                }
                break;
            }
        }
    }
    else {
        switch (blackChess[oldPoint.x][oldPoint.y].bianshi) {
            case Jiang: {
                strcpy_s(str1, TEXT("将\0"));
                switch (oldPoint.x) {
                    case 3: {
                        strcpy_s(str2, TEXT("6\0"));
                        break;
                    }
                    case 4: {
                        strcpy_s(str2, TEXT("5\0"));
                        break;
                    }
                    case 5: {
                        strcpy_s(str2, TEXT("4\0"));
                        break;
                    }
                }
                if (newPoint.y == oldPoint.y) {
                    strcpy_s(str3, TEXT("平\0"));
                    switch (newPoint.x) {
                        case 3: {
                            strcpy_s(str4, TEXT("6\0"));
                            break;
                        }
                        case 4: {
                            strcpy_s(str4, TEXT("5\0"));
                            break;
                        }
                        case 5: {
                            strcpy_s(str4, TEXT("4\0"));
                            break;
                        }
                    }
                }
                else if (newPoint.y < oldPoint.y) {
                    strcpy_s(str3, TEXT("进\0"));
                    switch (oldPoint.y - newPoint.y) {
                        case 1: {
                            strcpy_s(str4, TEXT("1\0"));
                            break;
                        }
                        case 5: {
                            strcpy_s(str4, TEXT("5\0"));
                            break;
                        }
                        case 6: {
                            strcpy_s(str4, TEXT("6\0"));
                            break;
                        }
                        case 7: {
                            strcpy_s(str4, TEXT("7\0"));
                            break;
                        }
                        case 8: {
                            strcpy_s(str4, TEXT("8\0"));
                            break;
                        }
                        case 9: {
                            strcpy_s(str4, TEXT("9\0"));
                            break;
                        }
                    }
                }
                else {
                    strcpy_s(str3, TEXT("退\0"));
                    strcpy_s(str4, TEXT("1\0"));
                }
                break;
            }
            case Shi: {
                strcpy_s(str1, TEXT("士\0"));
                switch (oldPoint.x) {
                    case 3: {
                        strcpy_s(str2, TEXT("6\0"));
                        break;
                    }
                    case 4: {
                        strcpy_s(str2, TEXT("5\0"));
                        break;
                    }
                    case 5: {
                        strcpy_s(str2, TEXT("4\0"));
                        break;
                    }
                }
                if (newPoint.y < oldPoint.y) {
                    strcpy_s(str3, TEXT("进"));
                }
                else {
                    strcpy_s(str3, TEXT("退"));
                }
                switch (newPoint.x) {
                    case 3: {
                        strcpy_s(str4, TEXT("6\0"));
                        break;
                    }
                    case 4: {
                        strcpy_s(str4, TEXT("5\0"));
                        break;
                    }
                    case 5: {
                        strcpy_s(str4, TEXT("4\0"));
                        break;
                    }
                }
                break;
            }
            case Xiang: {
                strcpy_s(str1, TEXT("象\0"));
                switch (oldPoint.x) {
                    case 0: {
                        strcpy_s(str2, TEXT("9\0"));
                        break;
                    }
                    case 2: {
                        strcpy_s(str2, TEXT("7\0"));
                        break;
                    }
                    case 4: {
                        strcpy_s(str2, TEXT("5\0"));
                        break;
                    }
                    case 6: {
                        strcpy_s(str2, TEXT("3\0"));
                        break;
                    }
                    case 8: {
                        strcpy_s(str2, TEXT("1\0"));
                        break;
                    }
                }
                if (newPoint.y < oldPoint.y) {
                    strcpy_s(str3, TEXT("进"));
                }
                else {
                    strcpy_s(str3, TEXT("退"));
                }
                switch (newPoint.x) {
                    case 0: {
                        strcpy_s(str4, TEXT("9\0"));
                        break;
                    }
                    case 2: {
                        strcpy_s(str4, TEXT("7\0"));
                        break;
                    }
                    case 4: {
                        strcpy_s(str4, TEXT("5\0"));
                        break;
                    }
                    case 6: {
                        strcpy_s(str4, TEXT("3\0"));
                        break;
                    }
                    case 8: {
                        strcpy_s(str4, TEXT("1\0"));
                        break;
                    }
                }

                break;
            }
            case Ma: {
                for (int i = 0; i < 10; ++i) {
                    if (i == oldPoint.y) {
                        continue;
                    }
                    if ((!blackChess[oldPoint.x][i].honghei) && (blackChess[oldPoint.x][i].bianshi == Ma)) {
                        youmeiyou = true;
                        if (i < oldPoint.y) {
                            strcpy_s(str1, TEXT("后"));
                        }
                        else {
                            strcpy_s(str1, TEXT("前"));
                        }
                        strcpy_s(str2, TEXT("马"));
                    }
                }
                if (!youmeiyou) {
                    strcpy_s(str1, TEXT("马"));
                    switch (oldPoint.x) {
                        case 0: {
                            strcpy_s(str2, TEXT("9"));
                            break;
                        }
                        case 1: {
                            strcpy_s(str2, TEXT("8"));
                            break;
                        }
                        case 2: {
                            strcpy_s(str2, TEXT("7"));
                            break;
                        }
                        case 3: {
                            strcpy_s(str2, TEXT("6"));
                            break;
                        }
                        case 4: {
                            strcpy_s(str2, TEXT("5"));
                            break;
                        }
                        case 5: {
                            strcpy_s(str2, TEXT("4"));
                            break;
                        }
                        case 6: {
                            strcpy_s(str2, TEXT("3"));
                            break;
                        }
                        case 7: {
                            strcpy_s(str2, TEXT("2"));
                            break;
                        }
                        case 8: {
                            strcpy_s(str2, TEXT("1"));
                            break;
                        }
                    }
                }
                if (newPoint.y < oldPoint.y) {
                    strcpy_s(str3, TEXT("进"));
                }
                else {
                    strcpy_s(str3, TEXT("退"));
                }
                switch (newPoint.x) {
                    case 0: {
                        strcpy_s(str4, TEXT("9"));
                        break;
                    }
                    case 1: {
                        strcpy_s(str4, TEXT("8"));
                        break;
                    }
                    case 2: {
                        strcpy_s(str4, TEXT("7"));
                        break;
                    }
                    case 3: {
                        strcpy_s(str4, TEXT("6"));
                        break;
                    }
                    case 4: {
                        strcpy_s(str4, TEXT("5"));
                        break;
                    }
                    case 5: {
                        strcpy_s(str4, TEXT("4"));
                        break;
                    }
                    case 6: {
                        strcpy_s(str4, TEXT("3"));
                        break;
                    }
                    case 7: {
                        strcpy_s(str4, TEXT("2"));
                        break;
                    }
                    case 8: {
                        strcpy_s(str4, TEXT("1"));
                        break;
                    }
                }
                break;
            }
            case Ju: {
                for (int i = 0; i < 10; ++i) {
                    if (i == oldPoint.y) {
                        continue;
                    }
                    if ((!blackChess[oldPoint.x][i].honghei) && (blackChess[oldPoint.x][i].bianshi == Ju)) {
                        youmeiyou = true;
                        if (i < oldPoint.y) {
                            strcpy_s(str1, TEXT("后"));
                        }
                        else {
                            strcpy_s(str1, TEXT("前"));
                        }
                        strcpy_s(str2, TEXT("车"));
                    }
                }
                if (!youmeiyou) {
                    strcpy_s(str1, TEXT("车"));
                    switch (oldPoint.x) {
                        case 0: {
                            strcpy_s(str2, TEXT("9"));
                            break;
                        }
                        case 1: {
                            strcpy_s(str2, TEXT("8"));
                            break;
                        }
                        case 2: {
                            strcpy_s(str2, TEXT("7"));
                            break;
                        }
                        case 3: {
                            strcpy_s(str2, TEXT("6"));
                            break;
                        }
                        case 4: {
                            strcpy_s(str2, TEXT("5"));
                            break;
                        }
                        case 5: {
                            strcpy_s(str2, TEXT("4"));
                            break;
                        }
                        case 6: {
                            strcpy_s(str2, TEXT("3"));
                            break;
                        }
                        case 7: {
                            strcpy_s(str2, TEXT("2"));
                            break;
                        }
                        case 8: {
                            strcpy_s(str2, TEXT("1"));
                            break;
                        }
                    }
                }
                if (newPoint.y == oldPoint.y) {
                    strcpy_s(str3, TEXT("平"));
                    switch (newPoint.x) {
                        case 0: {
                            strcpy_s(str4, TEXT("9"));
                            break;
                        }
                        case 1: {
                            strcpy_s(str4, TEXT("8"));
                            break;
                        }
                        case 2: {
                            strcpy_s(str4, TEXT("7"));
                            break;
                        }
                        case 3: {
                            strcpy_s(str4, TEXT("6"));
                            break;
                        }
                        case 4: {
                            strcpy_s(str4, TEXT("5"));
                            break;
                        }
                        case 5: {
                            strcpy_s(str4, TEXT("4"));
                            break;
                        }
                        case 6: {
                            strcpy_s(str4, TEXT("3"));
                            break;
                        }
                        case 7: {
                            strcpy_s(str4, TEXT("2"));
                            break;
                        }
                        case 8: {
                            strcpy_s(str4, TEXT("1"));
                            break;
                        }
                    }
                }
                else if (newPoint.y < oldPoint.y) {
                    strcpy_s(str3, TEXT("进"));
                    switch (oldPoint.y - newPoint.y) {
                        case 1: {
                            strcpy_s(str4, TEXT("1"));
                            break;
                        }
                        case 2: {
                            strcpy_s(str4, TEXT("2"));
                            break;
                        }
                        case 3: {
                            strcpy_s(str4, TEXT("3"));
                            break;
                        }
                        case 4: {
                            strcpy_s(str4, TEXT("4"));
                            break;
                        }
                        case 5: {
                            strcpy_s(str4, TEXT("5"));
                            break;
                        }
                        case 6: {
                            strcpy_s(str4, TEXT("6"));
                            break;
                        }
                        case 7: {
                            strcpy_s(str4, TEXT("7"));
                            break;
                        }
                        case 8: {
                            strcpy_s(str4, TEXT("8"));
                            break;
                        }
                        case 9: {
                            strcpy_s(str4, TEXT("9"));
                            break;
                        }
                    }
                }
                else {
                    strcpy_s(str3, TEXT("退"));
                    switch (newPoint.y - oldPoint.y) {
                        case 1: {
                            strcpy_s(str4, TEXT("1"));
                            break;
                        }
                        case 2: {
                            strcpy_s(str4, TEXT("2"));
                            break;
                        }
                        case 3: {
                            strcpy_s(str4, TEXT("3"));
                            break;
                        }
                        case 4: {
                            strcpy_s(str4, TEXT("4"));
                            break;
                        }
                        case 5: {
                            strcpy_s(str4, TEXT("5"));
                            break;
                        }
                        case 6: {
                            strcpy_s(str4, TEXT("6"));
                            break;
                        }
                        case 7: {
                            strcpy_s(str4, TEXT("7"));
                            break;
                        }
                        case 8: {
                            strcpy_s(str4, TEXT("8"));
                            break;
                        }
                        case 9: {
                            strcpy_s(str4, TEXT("9"));
                            break;
                        }
                    }
                }
                break;
            }
            case Pao: {
                for (int i = 0; i < 10; ++i) {
                                if (i == oldPoint.y) {
                                    continue;
                                }
                                if ((!blackChess[oldPoint.x][i].honghei) && (blackChess[oldPoint.x][i].bianshi == Pao)) {
                                    youmeiyou = true;
                                    if (i < oldPoint.y) {
                                        strcpy_s(str1, TEXT("后"));
                                    }
                                    else {
                                        strcpy_s(str1, TEXT("前"));
                                    }
                                    strcpy_s(str2, TEXT("炮"));
                                }
                            }
                if (!youmeiyou) {
                                strcpy_s(str1, TEXT("炮"));
                                switch (oldPoint.x) {
                                    case 0: {
                                        strcpy_s(str2, TEXT("9"));
                                        break;
                                    }
                                    case 1: {
                                        strcpy_s(str2, TEXT("8"));
                                        break;
                                    }
                                    case 2: {
                                        strcpy_s(str2, TEXT("7"));
                                        break;
                                    }
                                    case 3: {
                                        strcpy_s(str2, TEXT("6"));
                                        break;
                                    }
                                    case 4: {
                                        strcpy_s(str2, TEXT("5"));
                                        break;
                                    }
                                    case 5: {
                                        strcpy_s(str2, TEXT("4"));
                                        break;
                                    }
                                    case 6: {
                                        strcpy_s(str2, TEXT("3"));
                                        break;
                                    }
                                    case 7: {
                                        strcpy_s(str2, TEXT("2"));
                                        break;
                                    }
                                    case 8: {
                                        strcpy_s(str2, TEXT("1"));
                                        break;
                                    }
                                }
                            }
                if (newPoint.y == oldPoint.y) {
                                strcpy_s(str3, TEXT("平"));
                                switch (newPoint.x) {
                                    case 0: {
                                        strcpy_s(str4, TEXT("9"));
                                        break;
                                    }
                                    case 1: {
                                        strcpy_s(str4, TEXT("8"));
                                        break;
                                    }
                                    case 2: {
                                        strcpy_s(str4, TEXT("7"));
                                        break;
                                    }
                                    case 3: {
                                        strcpy_s(str4, TEXT("6"));
                                        break;
                                    }
                                    case 4: {
                                        strcpy_s(str4, TEXT("5"));
                                        break;
                                    }
                                    case 5: {
                                        strcpy_s(str4, TEXT("4"));
                                        break;
                                    }
                                    case 6: {
                                        strcpy_s(str4, TEXT("3"));
                                        break;
                                    }
                                    case 7: {
                                        strcpy_s(str4, TEXT("2"));
                                        break;
                                    }
                                    case 8: {
                                        strcpy_s(str4, TEXT("1"));
                                        break;
                                    }
                                }
                            }
                else if (newPoint.y < oldPoint.y) {
                                strcpy_s(str3, TEXT("进"));
                                switch (oldPoint.y - newPoint.y) {
                                    case 1: {
                                        strcpy_s(str4, TEXT("1"));
                                        break;
                                    }
                                    case 2: {
                                        strcpy_s(str4, TEXT("2"));
                                        break;
                                    }
                                    case 3: {
                                        strcpy_s(str4, TEXT("3"));
                                        break;
                                    }
                                    case 4: {
                                        strcpy_s(str4, TEXT("4"));
                                        break;
                                    }
                                    case 5: {
                                        strcpy_s(str4, TEXT("5"));
                                        break;
                                    }
                                    case 6: {
                                        strcpy_s(str4, TEXT("6"));
                                        break;
                                    }
                                    case 7: {
                                        strcpy_s(str4, TEXT("7"));
                                        break;
                                    }
                                    case 8: {
                                        strcpy_s(str4, TEXT("8"));
                                        break;
                                    }
                                    case 9: {
                                        strcpy_s(str4, TEXT("9"));
                                        break;
                                    }
                                }
                            }
                else {
                                strcpy_s(str3, TEXT("退"));
                                switch (newPoint.y - oldPoint.y) {
                                    case 1: {
                                        strcpy_s(str4, TEXT("1"));
                                        break;
                                    }
                                    case 2: {
                                        strcpy_s(str4, TEXT("2"));
                                        break;
                                    }
                                    case 3: {
                                        strcpy_s(str4, TEXT("3"));
                                        break;
                                    }
                                    case 4: {
                                        strcpy_s(str4, TEXT("4"));
                                        break;
                                    }
                                    case 5: {
                                        strcpy_s(str4, TEXT("5"));
                                        break;
                                    }
                                    case 6: {
                                        strcpy_s(str4, TEXT("6"));
                                        break;
                                    }
                                    case 7: {
                                        strcpy_s(str4, TEXT("7"));
                                        break;
                                    }
                                    case 8: {
                                        strcpy_s(str4, TEXT("8"));
                                        break;
                                    }
                                    case 9: {
                                        strcpy_s(str4, TEXT("9"));
                                        break;
                                    }
                                }
                            }
                break;
            }
            case Bing: {
                for (int i = 0; i < 10; ++i) {
                    if (i == oldPoint.y) {
                        continue;
                    }
                    if ((!blackChess[oldPoint.x][i].honghei) && (blackChess[oldPoint.x][i].bianshi == Bing)) {
                        youmeiyou = true;
                        if (i < oldPoint.y) {
                            strcpy_s(str1, TEXT("后"));
                        }
                        else {
                            strcpy_s(str1, TEXT("前"));
                        }
                        strcpy_s(str2, TEXT("卒"));
                    }
                }
                if (!youmeiyou) {
                    strcpy_s(str1, TEXT("卒"));
                    switch (oldPoint.x) {
                        case 0: {
                            strcpy_s(str2, TEXT("9"));
                            break;
                        }
                        case 1: {
                            strcpy_s(str2, TEXT("8"));
                            break;
                        }
                        case 2: {
                            strcpy_s(str2, TEXT("7"));
                            break;
                        }
                        case 3: {
                            strcpy_s(str2, TEXT("6"));
                            break;
                        }
                        case 4: {
                            strcpy_s(str2, TEXT("5"));
                            break;
                        }
                        case 5: {
                            strcpy_s(str2, TEXT("4"));
                            break;
                        }
                        case 6: {
                            strcpy_s(str2, TEXT("3"));
                            break;
                        }
                        case 7: {
                            strcpy_s(str2, TEXT("2"));
                            break;
                        }
                        case 8: {
                            strcpy_s(str2, TEXT("1"));
                            break;
                        }
                    }
                }
                if (newPoint.y == oldPoint.y) {
                    strcpy_s(str3, TEXT("平"));
                    switch (newPoint.x) {
                        case 0: {
                            strcpy_s(str4, TEXT("9"));
                            break;
                        }
                        case 1: {
                            strcpy_s(str4, TEXT("8"));
                            break;
                        }
                        case 2: {
                            strcpy_s(str4, TEXT("7"));
                            break;
                        }
                        case 3: {
                            strcpy_s(str4, TEXT("6"));
                            break;
                        }
                        case 4: {
                            strcpy_s(str4, TEXT("5"));
                            break;
                        }
                        case 5: {
                            strcpy_s(str4, TEXT("4"));
                            break;
                        }
                        case 6: {
                            strcpy_s(str4, TEXT("3"));
                            break;
                        }
                        case 7: {
                            strcpy_s(str4, TEXT("2"));
                            break;
                        }
                        case 8: {
                            strcpy_s(str4, TEXT("1"));
                            break;
                        }
                    }
                }
                else {
                    strcpy_s(str3, TEXT("进"));
                    strcpy_s(str4, TEXT("1"));
                }
                break;
                break;
            }
        }
    }
    strcat_s(duiju, str1);
    strcat_s(duiju, str2);
    strcat_s(duiju, str3);
    strcat_s(duiju, str4);
}
