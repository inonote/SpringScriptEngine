#pragma once
#include "CVariableMan.h"
#include "CScriptEngine.h"

#ifdef DECLARE_HANDLE
DECLARE_HANDLE(HSPRINGSCRIPT);
DECLARE_HANDLE(HSPRINGSCRIPTVAR);
#else
typedef struct { int unused; } *HSPRINGSCRIPT;
typedef struct { int unused; } *HSPRINGSCRIPTVAR;
#endif

typedef int(*ErrorCallbk)(int, int, const char*, void*);

#define DLLEXPORT extern "C" __declspec(dllexport)

/*! @brief SpringScriptEngineのインスタンスを作成
@return 成功したときはHSPRINGSCRIPT型のハンドル。失敗したときはNULL。
*/
DLLEXPORT HSPRINGSCRIPT SpringScript_CreateInstance();

/*! @brief SpringScriptEngineのインスタンスを破棄
@param[in] hSpringScript SpringScriptEngineのハンドル
@return 失敗したときFALSE、成功したときはFALSE以外。
*/
DLLEXPORT BOOL SpringScript_DestroyInstance(HSPRINGSCRIPT hSpringScript);

/*! @brief エラー発生の呼び出すコールバック関数を設定
@param[in] hSpringScript SpringScriptEngineのハンドル
@param[in] hWnd 親ウィンドウハンドル(NULL可)
@param[in] pFunc コールバック関数へのポインタ(NULL可)
@param[in] pUserData コールバック関数の呼び出し時に渡されるユーザー定義の値(NULL可)
@return 失敗したときFALSE、成功したときはFALSE以外。
*/
DLLEXPORT BOOL SpringScript_SetErrorCallbk(HSPRINGSCRIPT hSpringScript, HWND hWnd, ErrorCallbk pFunc, void *pUserData);

/*! @brief スクリプトファイルを読み込む
@param[in] hSpringScript SpringScriptEngineのハンドル
@param[in] pszFileName スクリプトファイルへのパス
@return 失敗したときFALSE、成功したときはFALSE以外。
*/
DLLEXPORT BOOL SpringScript_Load(HSPRINGSCRIPT hSpringScript, const char *pszFileName);

/*! @brief スクリプトを実行
@param[in] hSpringScript SpringScriptEngineのハンドル
@return 失敗したときFALSE、成功したときはFALSE以外。
*/
DLLEXPORT BOOL SpringScript_Run(HSPRINGSCRIPT hSpringScript);

/*! @brief スクリプトの実行を中止
スクリプト実行中にSpringScript_Run関数を呼び出した場合は、最後に呼び出したSpringScript_Run関数の実行を中止します。
@param[in] hSpringScript SpringScriptEngineのハンドル
@return 失敗したときFALSE、成功したときはFALSE以外。
*/
DLLEXPORT BOOL SpringScript_Break(HSPRINGSCRIPT hSpringScript);

/*! @brief スクリプトの実行をすべて中止
スクリプト実行中に呼び出したSpringScript_Run関数を含め、すべての実行を中止します。
@param[in] hSpringScript SpringScriptEngineのハンドル
@return 失敗したときFALSE、成功したときはFALSE以外。
*/
DLLEXPORT BOOL SpringScript_Exit(HSPRINGSCRIPT hSpringScript);

/*! @brief 命令を登録
@param[in] hSpringScript SpringScriptEngineのハンドル
@param[in] pszName 命令の名前(NULL不可)
@param[in] pszPrm 命令の引数(NULL不可)
引数の型を示す記号[i = Integer / f = Float / s = String / v = Variable / . = 可変長引数]
例)「Integer, String, Float, Integer, ...」の可変長引数にしたい場合は、「isfi.」と設定します。
@param[in] pFunc 呼び出すコールバック関数(NULL不可)
@param[in] pUserData コールバック関数の呼び出し時に渡されるユーザー定義の値(NULL可)
@return 失敗したときFALSE、成功したときはFALSE以外。
*/
DLLEXPORT BOOL SpringScript_RegisterCommand(HSPRINGSCRIPT hSpringScript, const char *pszName, const char *pszPrm, seCOMMAND pFunc, void *pUserData);

/*! @brief 命令を削除
@param[in] hSpringScript SpringScriptEngineのハンドル
@param[in] pszName 命令の名前(NULL不可)
@return 失敗したときFALSE、成功したときはFALSE以外。
*/
DLLEXPORT BOOL SpringScript_UnregisterCommand(HSPRINGSCRIPT hSpringScript, const char *pszName);

/*! @brief ラベルへジャンプ
@param[in] hSpringScript SpringScriptEngineのハンドル
@param[in] pszName ラベルの名前(NULL不可)
@return 失敗したときFALSE、成功したときはFALSE以外。
*/
DLLEXPORT BOOL SpringScript_GoTo(HSPRINGSCRIPT hSpringScript, const char *pszLabel);

/*! @brief 解析中のスクリプトの位置を取得
@param[in] hSpringScript SpringScriptEngineのハンドル
@param[out] pi PostionInfo構造体のポインタ(NULL不可)
@return 失敗したときFALSE、成功したときはFALSE以外。
*/
DLLEXPORT BOOL SpringScript_GetPostion(HSPRINGSCRIPT hSpringScript, PostionInfo *pi);

/*! @brief 解析中のスクリプトの位置を取得2
SpringScript_GetPostion関数と違い、先頭からのバッファ位置を示すPostionInfo.uPosに行の頭の位置を格納します。
@param[in] hSpringScript SpringScriptEngineのハンドル
@param[out] pi PostionInfo構造体のポインタ(NULL不可)
@return 失敗したときFALSE、成功したときはFALSE以外。
*/
DLLEXPORT BOOL SpringScript_GetPostionLH(HSPRINGSCRIPT hSpringScript, PostionInfo *pi);

/*! @brief 解析中のスクリプトの位置を移動
サブルーチンジャンプの復帰などに使用します。
@param[in] hSpringScript SpringScriptEngineのハンドル
@param[in] pi PostionInfo構造体のポインタ(NULL不可)
@return 失敗したときFALSE、成功したときはFALSE以外。
*/
DLLEXPORT BOOL SpringScript_SetPostion(HSPRINGSCRIPT hSpringScript, const PostionInfo *pi);

/*! @brief 解析中のスクリプトの位置を移動
SpringScript_SetPostion関数と違い、飛んだ先の行を無視して次の行から開始します。
(PostionInfo構造体の値がそのまま使われる)
@param[in] hSpringScript SpringScriptEngineのハンドル
@param[in] pi PostionInfo構造体のポインタ(NULL不可)
@return 失敗したときFALSE、成功したときはFALSE以外。
*/
DLLEXPORT BOOL SpringScript_SetPostionForce(HSPRINGSCRIPT hSpringScript, const PostionInfo *pi);

/*! @brief 行位置を取得
@param[in] hSpringScript SpringScriptEngineのハンドル
@param[out] val 値を代入する変数のポインタ(NULL不可)
@return 失敗したときFALSE、成功したときはFALSE以外。
*/
DLLEXPORT BOOL SpringScript_GetLineNum(HSPRINGSCRIPT hSpringScript, size_t *pVal);

/*! @brief 数式を計算
@param[in] hSpringScript SpringScriptEngineのハンドル
@param[in] pszExpression 計算式
@param[out] out 結果を代入する変数のポインタ(NULL不可)
@return 失敗したときFALSE、成功したときはFALSE以外。
*/
DLLEXPORT BOOL SpringScript_Eval(HSPRINGSCRIPT hSpringScript, const char *pszExpression, int *out);

/*! @brief 数式を計算(float)
SpringScript_Eval関数と違い、結果をfloatで出力します。
@param[in] hSpringScript SpringScriptEngineのハンドル
@param[in] pszExpression 計算式
@param[out] out 結果を代入する変数のポインタ(NULL不可)
@return 失敗したときFALSE、成功したときはFALSE以外。
*/
DLLEXPORT BOOL SpringScript_Evalf(HSPRINGSCRIPT hSpringScript, const char *pszExpression, float *out);

/*! @brief 命令の引数の数を取得
@param[in] arg CommandArg構造体のポインタ
@return 引数の数
*/
DLLEXPORT UINT SpringScript_GetCmdArgCount(const CommandArg *arg);

/*! @brief 命令の引数の型を取得
@param[in] arg CommandArg構造体のポインタ
@param[in] index 引数の位置
@return VarType列挙体で示される引数の型(失敗時:VARTYPE_UNKNOWN)
*/
DLLEXPORT CVariableMan::VarType SpringScript_GetCmdArgType(CommandArg *arg, int index);

/*! @brief 命令の引数にアクセスるためのハンドルを取得
この関数を使用して、SpringScript_GetVarVal系とSpringScript_SetVarVal関数で引数を操作できるようになります。
@param[in] arg CommandArg構造体のポインタ
@param[in] index 引数の位置
@return HSPRINGSCRIPTVARハンドル(失敗時:NULL)
*/
DLLEXPORT HSPRINGSCRIPTVAR SpringScript_GetCmdArgPtr(CommandArg *arg, int index);

/*! @brief Integer型の変数の値を取得
SpringScript_GetCmdArgPtr関数で取得した引数の操作にも使用できます。
@param[in] var HSPRINGSCRIPTVARハンドル(NULL不可)
@return 取得した値
*/
DLLEXPORT INT SpringScript_GetVarValInt(HSPRINGSCRIPTVAR *var);

/*! @brief String型の変数の内容を取得
SpringScript_GetCmdArgPtr関数で取得した引数の操作にも使用できます。
@param[in] var HSPRINGSCRIPTVARハンドル(NULL不可)
@param[out] lpString 文字列を書くのするバッファのポインタ(NULL不可)
@param[in] nMaxCount バッファのサイズ
@return 失敗したときFALSE、成功したときはFALSE以外。
*/
DLLEXPORT BOOL SpringScript_GetVarValStr(HSPRINGSCRIPTVAR *var, char *lpString, int nMaxCount);

/*! @brief String型の変数の内容の長さを取得
SpringScript_GetCmdArgPtr関数で取得した引数の操作にも使用できます。
@param[in] var HSPRINGSCRIPTVARハンドル(NULL不可)
@return 文字列の長さ
*/
DLLEXPORT INT SpringScript_GetVarValStrLen(HSPRINGSCRIPTVAR *var);

/*! @brief Float型の変数の値を取得
SpringScript_GetCmdArgPtr関数で取得した引数の操作にも使用できます。
@param[in] var HSPRINGSCRIPTVARハンドル(NULL不可)
@return 取得した値
*/
DLLEXPORT FLOAT SpringScript_GetVarValFloat(HSPRINGSCRIPTVAR *var);

/*! @brief Handle型の変数の値を取得
SpringScript_GetCmdArgPtr関数で取得した引数の操作にも使用できます。
@param[in] var HSPRINGSCRIPTVARハンドル(NULL不可)
@return 取得した値
*/
DLLEXPORT INT SpringScript_GetVarValHnd(HSPRINGSCRIPTVAR *var);

/*! @brief Variable型(参照)の変数の値を取得
SpringScript_GetCmdArgPtr関数で取得した引数の操作にも使用できます。
@param[in] var HSPRINGSCRIPTVARハンドル(NULL不可)
@return HSPRINGSCRIPTVARハンドル(失敗時:NULL)
*/
DLLEXPORT HSPRINGSCRIPTVAR SpringScript_GetVarValRef(HSPRINGSCRIPTVAR *var);

/*! @brief 変数の値を設定
SpringScript_GetCmdArgPtr関数で取得した引数の操作にも使用できます。
@param[in] var HSPRINGSCRIPTVARハンドル(NULL不可)
@param[in] val 値のポインタ
@param[in] type CVariableMan::VarType列挙体で示される変数の型
@return HSPRINGSCRIPTVARハンドル(失敗時:NULL)
*/
DLLEXPORT BOOL SpringScript_SetVarVal(HSPRINGSCRIPTVAR *var, const void *val, CVariableMan::VarType type);