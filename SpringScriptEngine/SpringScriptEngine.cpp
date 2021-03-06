// SpringScriptEngine.cpp : Defines the exported functions for the DLL application.
//
#include "CScriptEngine.h"
#include "CVariableMan.h"

#define _DLLEXPORT
#include "SpringScriptEngine.h"

#define HANDLECHECK(p) if (p == NULL) { return FALSE; }
#define SS(p) ((CScriptEngine*)p)
#define VM(p) ((CVariableMan*)p)
using namespace std;

/*! @brief SpringScriptEngineのインスタンスを作成
@return 成功したときはHSPRINGSCRIPT型のハンドル。失敗したときはNULL。
*/
DLLFUNC HSPRINGSCRIPT SpringScript_CreateInstance() {
	//インスタンスを作成
	CScriptEngine* pIns = new(nothrow) CScriptEngine;
	return (HSPRINGSCRIPT)pIns;
}

/*! @brief SpringScriptEngineのインスタンスを破棄
@param[in] hSpringScript SpringScriptEngineのハンドル
@return 失敗したときFALSE、成功したときはFALSE以外。
*/
DLLFUNC BOOL SpringScript_DestroyInstance(HSPRINGSCRIPT hSpringScript) {
	HANDLECHECK(hSpringScript);

	//インスタンスを破棄
	delete hSpringScript;
	return TRUE;
}

/*! @brief エラー発生の呼び出すコールバック関数を設定
@param[in] hSpringScript SpringScriptEngineのハンドル
@param[in] hWnd 親ウィンドウハンドル(NULL可)
@param[in] pFunc コールバック関数へのポインタ(NULL可)
@param[in] pUserData コールバック関数の呼び出し時に渡されるユーザー定義の値(NULL可)
@return 失敗したときFALSE、成功したときはFALSE以外。
*/
DLLFUNC BOOL SpringScript_SetErrorCallbk(HSPRINGSCRIPT hSpringScript, HWND hWnd, ErrorCallbk pFunc, void *pUserData) {
	HANDLECHECK(hSpringScript);

	SS(hSpringScript)->m_em.SetOwnerWnd(hWnd);
	SS(hSpringScript)->m_em.SetCallBk(pFunc, pUserData);
	return TRUE;
}

/*! @brief スクリプトファイルを読み込む
@param[in] hSpringScript SpringScriptEngineのハンドル
@param[in] pszFileName スクリプトファイルへのパス
@return 失敗したときFALSE、成功したときはFALSE以外。
*/
DLLFUNC BOOL SpringScript_Load(HSPRINGSCRIPT hSpringScript, const char *pszFileName) {
	HANDLECHECK(hSpringScript);

	if (!SS(hSpringScript)->Load(pszFileName))
		return FALSE;
	return TRUE;
}

/*! @brief スクリプトファイルをメモリから読み込む
@param[in] hSpringScript SpringScriptEngineのハンドル
@param[in] buf 文字列バッファ
@return 失敗したときFALSE、成功したときはFALSE以外。
*/
DLLFUNC BOOL SpringScript_LoadFromMem(HSPRINGSCRIPT hSpringScript, const char *buf) {
	HANDLECHECK(hSpringScript);

	if (!SS(hSpringScript)->LoadFromMem(buf))
		return FALSE;
	return TRUE;
}

/*! @brief スクリプトを実行
@param[in] hSpringScript SpringScriptEngineのハンドル
@return 失敗したときFALSE、成功したときはFALSE以外。
*/
DLLFUNC BOOL SpringScript_Run(HSPRINGSCRIPT hSpringScript) {
	HANDLECHECK(hSpringScript);

	SS(hSpringScript)->Run();
	return TRUE;
}

/*! @brief スクリプトの実行を中止<br>
スクリプト実行中にSpringScript_Run関数を呼び出した場合は、最後に呼び出したSpringScript_Run関数の実行を中止します。
@param[in] hSpringScript SpringScriptEngineのハンドル
@return 失敗したときFALSE、成功したときはFALSE以外。
*/
DLLFUNC BOOL SpringScript_Break(HSPRINGSCRIPT hSpringScript) {
	HANDLECHECK(hSpringScript);

	SS(hSpringScript)->Break();
	return TRUE;
}

/*! @brief スクリプトの実行をすべて中止<br>
スクリプト実行中に呼び出したSpringScript_Run関数を含め、すべての実行を中止します。
@param[in] hSpringScript SpringScriptEngineのハンドル
@return 失敗したときFALSE、成功したときはFALSE以外。
*/
DLLFUNC BOOL SpringScript_Exit(HSPRINGSCRIPT hSpringScript) {
	HANDLECHECK(hSpringScript);

	SS(hSpringScript)->Exit();
	return TRUE;
}

/*! @brief 命令を登録
@param[in] hSpringScript SpringScriptEngineのハンドル
@param[in] pszName 命令の名前(NULL不可)
@param[in] pszPrm 命令の引数(NULL不可)<br>
引数の型を示す記号[i = Integer / f = Float / s = String / v = Variable / . = 可変長引数]<br>
例)「Integer, String, Float, Integer, ...」の可変長引数にしたい場合は、「isfi.」と設定します。
@param[in] pFunc 呼び出すコールバック関数(NULL不可)
@param[in] pUserData コールバック関数の呼び出し時に渡されるユーザー定義の値(NULL可)
@return 失敗したときFALSE、成功したときはFALSE以外。
*/
DLLFUNC BOOL SpringScript_RegisterCommand(HSPRINGSCRIPT hSpringScript, const char *pszName, const char *pszPrm, CommandCallbk pFunc, void *pUserData) {
	HANDLECHECK(hSpringScript);

	if (!SS(hSpringScript)->RegisterCommand(string(pszName), string(pszPrm), (seCOMMAND)pFunc, pUserData))
		return FALSE;
	return TRUE;
}

/*! @brief 命令を削除
@param[in] hSpringScript SpringScriptEngineのハンドル
@param[in] pszName 命令の名前(NULL不可)
@return 失敗したときFALSE、成功したときはFALSE以外。
*/
DLLFUNC BOOL SpringScript_UnregisterCommand(HSPRINGSCRIPT hSpringScript, const char *pszName) {
	HANDLECHECK(hSpringScript);

	if (!SS(hSpringScript)->UnregisterCommand(string(pszName)))
		return FALSE;
	return TRUE;
}

/*! @brief ラベルへジャンプ
@param[in] hSpringScript SpringScriptEngineのハンドル
@param[in] pszName ラベルの名前(NULL不可)
@return 失敗したときFALSE、成功したときはFALSE以外。
*/
DLLFUNC BOOL SpringScript_GoTo(HSPRINGSCRIPT hSpringScript, const char *pszLabel) {
	HANDLECHECK(hSpringScript);

	if (!SS(hSpringScript)->GoTo(string(pszLabel), false))
		return FALSE;
	return TRUE;
}

/*! @brief ラベルへジャンプ2<br>
SpringScript_Run()を呼び出す前にラベルにジャンプしておきたい場合はこちらを使用します。
@param[in] hSpringScript SpringScriptEngineのハンドル
@param[in] pszName ラベルの名前(NULL不可)
@return 失敗したときFALSE、成功したときはFALSE以外。
*/
DLLFUNC BOOL SpringScript_GoTo_Event(HSPRINGSCRIPT hSpringScript, const char *pszLabel) {
	HANDLECHECK(hSpringScript);

	if (!SS(hSpringScript)->GoTo(string(pszLabel), true))
		return FALSE;
	return TRUE;
}

/*! @brief 解析中のスクリプトの位置を取得
@param[in] hSpringScript SpringScriptEngineのハンドル
@param[out] pi PostionInfo構造体のポインタ(NULL不可)
@return 失敗したときFALSE、成功したときはFALSE以外。
*/
DLLFUNC BOOL SpringScript_GetPostion(HSPRINGSCRIPT hSpringScript, PostionInfo *pi) {
	HANDLECHECK(hSpringScript);

	*pi = SS(hSpringScript)->GetPostion();
	return TRUE;
}

/*! @brief 解析中のスクリプトの位置を取得2<br>
SpringScript_GetPostion関数と違い、先頭からのバッファ位置を示すPostionInfo.uPosに行の頭の位置を格納します。
@param[in] hSpringScript SpringScriptEngineのハンドル
@param[out] pi PostionInfo構造体のポインタ(NULL不可)
@return 失敗したときFALSE、成功したときはFALSE以外。
*/
DLLFUNC BOOL SpringScript_GetPostionLH(HSPRINGSCRIPT hSpringScript, PostionInfo *pi) {
	HANDLECHECK(hSpringScript);

	*pi = SS(hSpringScript)->GetPostionLH();
	return TRUE;
}

/*! @brief 解析中のスクリプトの位置を移動<br>
サブルーチンジャンプの復帰などに使用します。
@param[in] hSpringScript SpringScriptEngineのハンドル
@param[in] pi PostionInfo構造体のポインタ(NULL不可)
@return 失敗したときFALSE、成功したときはFALSE以外。
*/
DLLFUNC BOOL SpringScript_SetPostion(HSPRINGSCRIPT hSpringScript, const PostionInfo *pi) {
	HANDLECHECK(hSpringScript);

	SS(hSpringScript)->SetPostion(*pi);
	return TRUE;
}

/*! @brief 解析中のスクリプトの位置を移動<br>
SpringScript_SetPostion関数と違い、飛んだ先の行を無視して次の行から開始します。<br>
(PostionInfo構造体の値がそのまま使われる)
@param[in] hSpringScript SpringScriptEngineのハンドル
@param[in] pi PostionInfo構造体のポインタ(NULL不可)
@return 失敗したときFALSE、成功したときはFALSE以外。
*/
DLLFUNC BOOL SpringScript_SetPostionForce(HSPRINGSCRIPT hSpringScript, const PostionInfo *pi) {
	HANDLECHECK(hSpringScript);

	SS(hSpringScript)->SetPostionForce(*pi);
	return TRUE;
}

/*! @brief 行位置を取得
@param[in] hSpringScript SpringScriptEngineのハンドル
@param[out] val 値を代入する変数のポインタ(NULL不可)
@return 失敗したときFALSE、成功したときはFALSE以外。
*/
DLLFUNC BOOL SpringScript_GetLineNum(HSPRINGSCRIPT hSpringScript, size_t *pVal) {
	HANDLECHECK(hSpringScript);
	if (pVal == NULL)
		return FALSE;
	*pVal = SS(hSpringScript)->GetLine();
	return TRUE;
}

/*! @brief 数式を計算
@param[in] hSpringScript SpringScriptEngineのハンドル
@param[in] pszExpression 計算式
@param[out] out 結果を代入する変数のポインタ(NULL不可)
@return 失敗したときFALSE、成功したときはFALSE以外。
*/
DLLFUNC BOOL SpringScript_Eval(HSPRINGSCRIPT hSpringScript, const char *pszExpression, int *out) {
	HANDLECHECK(hSpringScript);
	if (out == NULL)
		return FALSE;

	*out = SS(hSpringScript)->Eval_i(string(pszExpression));
	return TRUE;
}

/*! @brief 数式を計算(float)<br>
SpringScript_Eval関数と違い、結果をfloatで出力します。
@param[in] hSpringScript SpringScriptEngineのハンドル
@param[in] pszExpression 計算式
@param[out] out 結果を代入する変数のポインタ(NULL不可)
@return 失敗したときFALSE、成功したときはFALSE以外。
*/
DLLFUNC BOOL SpringScript_Evalf(HSPRINGSCRIPT hSpringScript, const char *pszExpression, float *out) {
	HANDLECHECK(hSpringScript);
	if (out == NULL)
		return FALSE;

	*out = SS(hSpringScript)->Eval_f(string(pszExpression));
	return TRUE;
}

/*! @brief 命令の引数の数を取得
@param[in] hArg HCOMMANDARGハンドル
@return 引数の数
*/
DLLFUNC UINT SpringScript_GetCmdArgCount(HCOMMANDARG hArg) {
	if (hArg == NULL)
		return 0;

	return ((CommandArg*)hArg)->ArgList.size();
}

/*! @brief 呼び出された命令の名前を取得
@param[in] hArg HCOMMANDARGハンドル
@param[out] lpString 文字列を書き込むバッファのポインタ(NULL不可)
@param[in] nMaxCount バッファのサイズ
@return 失敗したときFALSE、成功したときはFALSE以外。
*/
DLLFUNC BOOL SpringScript_GetCmdName(HCOMMANDARG hArg, char *lpString, int nMaxCount) {
	if (hArg == NULL)
		return FALSE;

	strncpy_s(lpString, nMaxCount, ((CommandArg*)hArg)->commandName.c_str(), _TRUNCATE);
	return TRUE;
}

/*! @brief 呼び出された命令の名前の長さを取得
SpringScript_GetCmdArgPtr関数で取得した引数の操作にも使用できます。
@param[in] var HSPRINGSCRIPTVARハンドル(NULL不可)
@return 文字列の長さ
*/
DLLFUNC INT SpringScript_GetCmdNameLen(HCOMMANDARG hArg) {
	if (hArg == NULL)
		return 0;

	return ((CommandArg*)hArg)->commandName.length();
}

/*! @brief 命令の引数の型を取得
@param[in] hArg HCOMMANDARGハンドル
@param[in] index 引数の位置
@return VarType列挙体で示される引数の型(失敗時:VARTYPE_UNKNOWN)
*/
DLLFUNC VarType SpringScript_GetCmdArgType(HCOMMANDARG hArg, int index) {
	if (hArg == NULL)
		return VarType::VARTYPE_UNKNOWN;
	if (((CommandArg*)hArg)->ArgList.size() <= index || index < 0)
		return VarType::VARTYPE_UNKNOWN;

	return (VarType)((CommandArg*)hArg)->ArgList[index].GetType();
}

/*! @brief 命令の引数にアクセスるためのハンドルを取得<br>
この関数を使用して、SpringScript_GetVarVal系とSpringScript_SetVarVal関数で引数を操作できるようになります。
@param[in] hArg HCOMMANDARGハンドル
@param[in] index 引数の位置
@return HSPRINGSCRIPTVARハンドル(失敗時:NULL)
*/
DLLFUNC HSPRINGSCRIPTVAR SpringScript_GetCmdArgPtr(HCOMMANDARG hArg, int index) {
	if (hArg == NULL)
		return NULL;
	if (((CommandArg*)hArg)->ArgList.size() <= index || index < 0)
		return NULL;

	return (HSPRINGSCRIPTVAR)&((CommandArg*)hArg)->ArgList[index];
}

/*! @brief Integer型の変数の値を取得<br>
SpringScript_GetCmdArgPtr関数で取得した引数の操作にも使用できます。
@param[in] var HSPRINGSCRIPTVARハンドル(NULL不可)
@return 取得した値
*/
DLLFUNC INT SpringScript_GetVarValInt(HSPRINGSCRIPTVAR var) {
	if (var == NULL)
		return 0;
	if (VM(var)->GetType() !=VarType::VARTYPE_INTEGER)
		return 0;
	return *((INT*)VM(var)->GetData());
}

/*! @brief String型の変数の内容を取得<br>
SpringScript_GetCmdArgPtr関数で取得した引数の操作にも使用できます。
@param[in] var HSPRINGSCRIPTVARハンドル(NULL不可)
@param[out] lpString 文字列を書き込むバッファのポインタ(NULL不可)
@param[in] nMaxCount バッファのサイズ
@return 失敗したときFALSE、成功したときはFALSE以外。
*/
DLLFUNC BOOL SpringScript_GetVarValStr(HSPRINGSCRIPTVAR var, char *lpString, int nMaxCount) {
	if (var == NULL)
		return FALSE;
	if (VM(var)->GetType() !=VarType::VARTYPE_STRING)
		return FALSE;

	strncpy_s(lpString, nMaxCount, ((string*)VM(var)->GetData())->c_str(), _TRUNCATE);
	return TRUE;
}

/*! @brief String型の変数の内容の長さを取得<br>
SpringScript_GetCmdArgPtr関数で取得した引数の操作にも使用できます。
@param[in] var HSPRINGSCRIPTVARハンドル(NULL不可)
@return 文字列の長さ
*/
DLLFUNC INT SpringScript_GetVarValStrLen(HSPRINGSCRIPTVAR var) {
	if (var == NULL)
		return 0;
	if (VM(var)->GetType() !=VarType::VARTYPE_STRING)
		return 0;

	return ((string*)VM(var)->GetData())->length() + 1;
}

/*! @brief Float型の変数の値を取得<br>
SpringScript_GetCmdArgPtr関数で取得した引数の操作にも使用できます。
@param[in] var HSPRINGSCRIPTVARハンドル(NULL不可)
@return 取得した値
*/
DLLFUNC FLOAT SpringScript_GetVarValFloat(HSPRINGSCRIPTVAR var) {
	if (var == NULL)
		return 0;
	if (VM(var)->GetType() != VarType::VARTYPE_FLOAT)
		return 0;
	return *((FLOAT*)VM(var)->GetData());
}
DLLFUNC VOID SpringScript_GetVarValFloat2(HSPRINGSCRIPTVAR var, FLOAT *p) {
	*p = SpringScript_GetVarValFloat(var);
}

/*! @brief Handle型の変数の値を取得<br>
SpringScript_GetCmdArgPtr関数で取得した引数の操作にも使用できます。
@param[in] var HSPRINGSCRIPTVARハンドル(NULL不可)
@return 取得した値
*/
DLLFUNC INT SpringScript_GetVarValHnd(HSPRINGSCRIPTVAR var) {
	if (var == NULL)
		return 0;
	if (VM(var)->GetType() !=VarType::VARTYPE_HANDLE)
		return 0;
	return *((INT*)VM(var)->GetData());
}

/*! @brief Variable型(参照)の変数の値を取得<br>
SpringScript_GetCmdArgPtr関数で取得した引数の操作にも使用できます。
@param[in] var HSPRINGSCRIPTVARハンドル(NULL不可)
@return HSPRINGSCRIPTVARハンドル(失敗時:NULL)
*/
DLLFUNC HSPRINGSCRIPTVAR SpringScript_GetVarValRef(HSPRINGSCRIPTVAR var) {
	if (var == NULL)
		return NULL;
	if (VM(var)->GetType() !=VarType::VARTYPE_VARIABLE)
		return NULL;
	return (HSPRINGSCRIPTVAR)(CVariableMan*)VM(var)->GetData();
}

/*! @brief 変数の値を設定<br>
SpringScript_GetCmdArgPtr関数で取得した引数の操作にも使用できます。
@param[in] var HSPRINGSCRIPTVARハンドル(NULL不可)
@param[in] val 値のポインタ
@param[in] typeVarType::VARTYPE列挙体で示される変数の型
@return HSPRINGSCRIPTVARハンドル(失敗時:NULL)
*/
DLLFUNC BOOL SpringScript_SetVarVal(HSPRINGSCRIPTVAR var, const void *val, VarType type) {
	if (var == NULL)
		return FALSE;
	if (val == NULL)
		return FALSE;
	switch (type)
	{
	case VarType::VARTYPE_INTEGER:
		VM(var)->Set(*(int*)val);
		break;
	case VarType::VARTYPE_STRING:
		VM(var)->Set(&string((const char*)val));
		break;
	case VarType::VARTYPE_FLOAT:
		VM(var)->Set(*(float*)val);
		break;
	case VarType::VARTYPE_HANDLE:
		VM(var)->SetAsHandle(*(int*)val);
		break;
	case VarType::VARTYPE_VARIABLE:
		VM(var)->Set((CVariableMan*)val);
		break;
	default:
		return FALSE;

	}
	return TRUE;
}