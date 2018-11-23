/*! @file CErrorMan.h
@brief エラーキャッチ用クラス。
*/

#pragma once

/*! @param 第一引数 nErrorID
@param 第二引数 nLine
@param 第三引数 sFilePath
@param 第四引数 pUserData
*/
typedef int(*ErrorCallbk)(int, int, const char*, void*);

#define ERRORID_NONE				0x0000	//!< エラーなし
#define ERRORID_UNKNOWNCOMMAND		0x0001	//!< 不明なコマンド
#define ERRORID_LABELNOTEXIST		0x0002	//!< ラベルが見つからない
#define ERRORID_FILENOTEXIST		0x1000	//!< ファイルが見つからない
#define ERRORID_PARAMETERILLEGAL	0x2000	//!< パラメーターが不正
#define ERRORID_PARAMETERNOTENOUGH	0x2001	//!< パラメーターが不足している
#define ERRORID_DIVIDEBYZERO		0x2002	//!< 0で除算しようとした
#define ERRORID_OUTOFINDEX			0x2003	//!< インデックス値が範囲外
#define ERRORID_COULDNTALLOCMEMORY	0xF000	//!< メモリの確保に失敗
#define ERRORID_UNKNOWNERROR		0xFFFF	//!< 不明なエラー

class CErrorMan {
public:
	CErrorMan(HWND hOwnerWnd = NULL);
	bool ChachError(int nErrorID, int nLine, const char *sFilePath, const char* sOptionalText = NULL);

private:
	ErrorCallbk m_pCallBk;
	HWND m_hWnd;		//!< オーナーウィンドウハンドル
	void *m_pUserData;	//!< ユーザー定義値

public:
	/*! @brief エラーをキャッチしたときに呼び出されるコールバック関数を設定します。
	@param[in] pFunc コールバック関数へのポインタ
	@param[in] pUserData ユーザー定義値　
	*/
	inline void SetCallBk(ErrorCallbk pFunc, void* pUserData) {
		m_pCallBk = pFunc;
		m_pUserData = pUserData;
	};

	//!< 設定したコールバック関数を取得
	inline ErrorCallbk GetCallBk() {
		return m_pCallBk;
	};

	//! @brief オーナーウィンドウを設定
	inline void SetOwnerWnd(HWND hOwnerWnd) {
		m_hWnd = hOwnerWnd;
	};

	//! @brief オーナーウィンドウを取得
	inline HWND GetOwnerWnd() {
		return m_hWnd;
	};
};