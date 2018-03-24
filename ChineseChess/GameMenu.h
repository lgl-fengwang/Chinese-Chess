#ifndef _GAMEMENU_H
#define _GAMEMENU_H
class GameMenu {
private:
    HBITMAP hMenuBackground; //����ͼ
    const int wideBackground = 1350;
    const int highBackground = 700;
    HBITMAP hGameStart; //��ʼ��Ϸ��ťͼƬ
    const int wideGameStart = 150;
    const int highGameStart = 50;
    bool mouseInNow; //��ǰ����Ƿ��ڰ�ť��
    bool mouseInPre; //��һ����Ⱦ����Ƿ��ڰ�ť��
public:
    void Start(HWND hWnd);
    void Update(HWND hWnd);
    void End(HWND hWnd);
    void OnWindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};
#endif

