#ifndef _GAMEPLAYING_H
#define _GAMEPLAYING_H
class GamePlaying {
private:
    HBITMAP hPlayingBackground; //����ͼƬ
    const int wideBackground = 1350;
    const int highBackground = 700;
    HBITMAP hRedChessboard; //�췽����
    HBITMAP hBlackChessboard; //�ڷ�����
    const int WideChessboard = 628;
    const int HighChessboard = 700;
    HBITMAP hFocus; //ѡ�����ӱ��
    HBITMAP hPiece; //�����ز�
    HBITMAP hPieceMask; //��������
    HBITMAP hDaohang; //����
    HFONT hChessFont; //�Ծ�����
    TCHAR duiju[9]; //�洢��ǰ�ߵ�һ��


    enum Pieces { Jiang, Shi, Xiang, Ma, Ju, Pao, Bing }; //ö������

    struct Piece {
        bool qizi; //�Ƿ��������
        bool honghei; //�Ǻ췽���ӻ��Ǻڷ����� ��true��false
        bool daohang; //�Ƿ񱻵�������
        Pieces bianshi; //ָ�����ĸ�����
    };

    Piece redChess[9][10]; //�Ժ췽Ϊ�ο�������
    Piece blackChess[9][10]; //�Ժڷ�Ϊ�ο�������
    POINT mouseClick; //�洢�����λ�õ��߼�����
    bool mouseYouxiao; //ָʾ��ǰ�Ƿ��Ѿ�����Ч���
    bool mouseHonghei; //ָʾ���ķ����� ��true��false
    POINT pieceSelected; //�洢�Ѿ��������ӵ��߼�����
public:
    void Start(HWND hWnd);
    void Update(HWND hWnd);
    void End(HWND hWnd);
    void OnWindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    POINT MouseClick(LPARAM lParam);  //�ͻ�������ת���߼�����
    POINT PieceDown(POINT point, bool honghei); //�߼�����ת���ͻ�������
    void PieceDaohang(POINT point); //ѡ�����Ӳ��������ܵ�����֮��
    void PieceUnDaohang(); //������������
    void PieceMove(POINT point); //�ƶ����� ��������ѡ������ ���߰���������������������
    void FlipChess(bool honghei); //��ת����
    void Duiju(POINT oldPoint, POINT newPoint);
};
#endif

