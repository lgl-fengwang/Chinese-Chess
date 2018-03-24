#ifndef _GAMEMAIN_H
#define _GAMEMAIN_H
enum GameState { GS_Menu, GS_Playing, GS_Result }; //ö����Ϸ������״̬
extern HINSTANCE hInst;
extern int g_ClientWidth; //�ͻ�����
extern int g_ClientHeight; //�ͻ�����
extern int widthFrame; //���ڱ߿��
extern int heightFrame; //���ڱ߿��
extern const int WideList; //�б�ؼ���
extern double proportion; //�������ű���
extern DWORD g_tPre; //�ϴλ�ͼʱ��
extern DWORD g_tNow; //���λ�ͼʱ��
extern HDC g_hdc; //��Ϸ����
extern HDC g_mdc; //�ڴ滭��
extern HDC g_bufdc; //���滭��
extern GameState gameState;
extern HWND hChessList; //�Ծ��б�ؼ�


void GameStart(HWND hWnd); //��Ϸ״̬��ʼ��
void GameUpdate(HWND hWnd); //��Ϸ״̬ѭ��
void GameEnd(HWND hWnd); //��Ϸ״̬����
#endif
