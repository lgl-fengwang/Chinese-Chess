#ifndef _GAMEMENU_H
#define _GAMEMENU_H
class GameMenu {
private:
    HBITMAP hMenuBackground; //背景图
    const int wideBackground = 1350;
    const int highBackground = 700;
    HBITMAP hGameStart; //开始游戏按钮图片
    const int wideGameStart = 150;
    const int highGameStart = 50;
    bool mouseInNow; //当前鼠标是否在按钮里
    bool mouseInPre; //上一次渲染鼠标是否在按钮里
public:
    void Start(HWND hWnd);
    void Update(HWND hWnd);
    void End(HWND hWnd);
    void OnWindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};
#endif

