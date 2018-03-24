#ifndef _GAMEMAIN_H
#define _GAMEMAIN_H
enum GameState { GS_Menu, GS_Playing, GS_Result }; //枚举游戏的三种状态
extern HINSTANCE hInst;
extern int g_ClientWidth; //客户区宽
extern int g_ClientHeight; //客户区高
extern int widthFrame; //窗口边框宽
extern int heightFrame; //窗口边框高
extern const int WideList; //列表控件宽
extern double proportion; //窗口缩放比例
extern DWORD g_tPre; //上次绘图时间
extern DWORD g_tNow; //本次绘图时间
extern HDC g_hdc; //游戏画布
extern HDC g_mdc; //内存画布
extern HDC g_bufdc; //缓存画布
extern GameState gameState;
extern HWND hChessList; //对局列表控件


void GameStart(HWND hWnd); //游戏状态初始化
void GameUpdate(HWND hWnd); //游戏状态循环
void GameEnd(HWND hWnd); //游戏状态结束
#endif
