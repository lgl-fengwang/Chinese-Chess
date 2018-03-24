#ifndef _GAMERESULT_H
#define _GAMERESULT_H
class GameResult {
private:

public:
    void Start(HWND hWnd);
    void Update(HWND hWnd);
    void End(HWND hWnd);
    void OnWindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};
#endif
