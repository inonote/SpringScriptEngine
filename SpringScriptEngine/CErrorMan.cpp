#include "CErrorMan.h"

CErrorMan::CErrorMan(HWND hOwnerWnd) :
	m_pUserData(NULL),
	m_pCallBk(NULL)
{
	m_hWnd = hOwnerWnd;
}

//エラーをキャッチする関数
//Note: エラーメッセージ表示後、SetCallBk関数で登録したコールバック関数が呼び出されるので、
//      そこでプログラムを終了するかの判定を行う。

bool CErrorMan::ChachError(int nErrorID, int nLine, const char *sFilePath, const char *sOptionalText){
	char sErrorMsg[0x200];
	char sBuf[0x300];

	switch(nErrorID)
	{
	case ERRORID_UNKNOWNCOMMAND:
		strcpy_s(sErrorMsg, 0x200, "不明なコマンドです。");
		break;
	case ERRORID_LABELNOTEXIST:
		strcpy_s(sErrorMsg, 0x200, "ラベルが見つかりません。");
		break;
	case ERRORID_FILENOTEXIST:
		strcpy_s(sErrorMsg, 0x200, "ファイルが見つかりません。");
		break;
	case ERRORID_PARAMETERILLEGAL:
		strcpy_s(sErrorMsg, 0x200, "引数が不正です。");
		break;
	case ERRORID_PARAMETERNOTENOUGH:
		strcpy_s(sErrorMsg, 0x200, "引数が不足しています。");
		break;
	case ERRORID_DIVIDEBYZERO:
		strcpy_s(sErrorMsg, 0x200, "0で除算しようとしました。");
		break;
	case ERRORID_OUTOFINDEX:
		strcpy_s(sErrorMsg, 0x200, "インデックス値が範囲外です。");
		break;
	case ERRORID_COULDNTALLOCMEMORY:
		strcpy_s(sErrorMsg, 0x200, "メモリの確保に失敗しました。");
		break;
	case ERRORID_UNKNOWNERROR:
		strcpy_s(sErrorMsg, 0x200, "原因不明のエラーが発生しました。");
		break;
	}
	if (sOptionalText)
		sprintf_s(sBuf, 0x300, "%s\n#%d 行:%d(%s)\n\n詳細:\n%s", sErrorMsg, nErrorID, nLine + 1, sFilePath, sOptionalText);
	else
		sprintf_s(sBuf, 0x300, "%s\n#%d 行:%d(%s)\n\n", sErrorMsg, nErrorID, nLine+1, sFilePath);
	MessageBox(m_hWnd, sBuf, "Error", MB_ICONERROR);
	if (m_pCallBk) {
		m_pCallBk(nErrorID, nLine, sFilePath, m_pUserData); //エラー処理後用のコールバック関数を呼び出す
	}
	return true;
}