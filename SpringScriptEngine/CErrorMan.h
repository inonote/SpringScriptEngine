#pragma once
typedef int(*ErrorCallbk)(int, int, const char*, void*);

class CErrorMan {
public:
	CErrorMan(HWND hOwnerWnd = NULL);
	inline void SetWindow(HWND hOwnerWnd = NULL) { m_hWnd = hOwnerWnd; };
	bool ChachError(int nErrorID, int nLine, const char *sFilePath, const char* sOptionalText = NULL);

#define ERRORID_NONE				0x0000	// エラーなし
#define ERRORID_UNKNOWNCOMMAND		0x0001	// 不明なコマンド
#define ERRORID_LABELNOTEXIST		0x0002	// ラベルが見つからない
#define ERRORID_FILENOTEXIST		0x1000	// ファイルが見つからない
#define ERRORID_PARAMETERILLEGAL	0x2000	// パラメーターが不正
#define ERRORID_PARAMETERNOTENOUGH	0x2001	// パラメーターが不足している
#define ERRORID_DIVIDEBYZERO		0x2002	// 0で除算しようとした
#define ERRORID_OUTOFINDEX			0x2003	// インデックス値が範囲外
#define ERRORID_COULDNTALLOCMEMORY	0xF000	// メモリの確保に失敗
#define ERRORID_UNKNOWNERROR		0xFFFF	// 不明なエラー
	
private:
	ErrorCallbk m_pCallBk;
	HWND m_hWnd;
	void *m_pUserData;

public:
	inline void SetCallBk(ErrorCallbk pFunc, void* pUserData) {
		m_pCallBk = pFunc;
		m_pUserData = pUserData;
	};
	inline ErrorCallbk GetCallBk() {
		return m_pCallBk;
	};
	inline void SetOwnerWnd(HWND hOwnerWnd) {
		m_hWnd = hOwnerWnd;
	};
	inline HWND GetOwnerWnd() {
		return m_hWnd;
	};
};