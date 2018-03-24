#ifndef _GAMEPLAYING_H
#define _GAMEPLAYING_H
class GamePlaying {
private:
    HBITMAP hPlayingBackground; //背景图片
    const int wideBackground = 1350;
    const int highBackground = 700;
    HBITMAP hRedChessboard; //红方棋盘
    HBITMAP hBlackChessboard; //黑方棋盘
    const int WideChessboard = 628;
    const int HighChessboard = 700;
    HBITMAP hFocus; //选中棋子标记
    HBITMAP hPiece; //棋子素材
    HBITMAP hPieceMask; //棋子遮罩
    HBITMAP hDaohang; //导航
    HFONT hChessFont; //对局字体
    TCHAR duiju[9]; //存储当前走的一步


    enum Pieces { Jiang, Shi, Xiang, Ma, Ju, Pao, Bing }; //枚举棋子

    struct Piece {
        bool qizi; //是否存在棋子
        bool honghei; //是红方棋子还是黑方棋子 红true黑false
        bool daohang; //是否被导航覆盖
        Pieces bianshi; //指明是哪个棋子
    };

    Piece redChess[9][10]; //以红方为参考的棋谱
    Piece blackChess[9][10]; //以黑方为参考的棋谱
    POINT mouseClick; //存储鼠标点击位置的逻辑坐标
    bool mouseYouxiao; //指示当前是否已经有有效点击
    bool mouseHonghei; //指示该哪方行棋 红true黑false
    POINT pieceSelected; //存储已经激活棋子的逻辑坐标
public:
    void Start(HWND hWnd);
    void Update(HWND hWnd);
    void End(HWND hWnd);
    void OnWindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    POINT MouseClick(LPARAM lParam);  //客户区坐标转换逻辑坐标
    POINT PieceDown(POINT point, bool honghei); //逻辑坐标转换客户区坐标
    void PieceDaohang(POINT point); //选择棋子并标记其可能的落子之处
    void PieceUnDaohang(); //将导航标记清除
    void PieceMove(POINT point); //移动棋子 或者重新选择棋子 或者巴拉巴拉巴拉巴拉。。。
    void FlipChess(bool honghei); //翻转棋盘
    void Duiju(POINT oldPoint, POINT newPoint);
};
#endif

