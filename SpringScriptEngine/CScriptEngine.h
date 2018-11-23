/*! @file CScriptEngine.h
@brief スクリプトエンジン本体のヘッダコード。
*/

#pragma once
#include "CScriptEngine.h"
#include "CVariableMan.h"
#include "CErrorMan.h"

//構造体
typedef struct tagCommandArg {
	std::string commandName; //!< 命令の名前
	std::vector<CVariableMan> ArgList; //!< 引数
} CommandArg;

typedef int(*seCOMMAND)(void *, CommandArg *);

typedef struct tagCommandInfo {
	std::string name;	//!< 命令の名前
	std::string prm;	//!< 引数
	seCOMMAND pFunc;	//!< 呼び出す関数のポインタ
	void *arg;			//!< ユーザー定義の値
} CommandInfo;

typedef struct tagPostionInfo {
	size_t uPos;		//!< ファイルの先頭からの位置
	size_t uLine;		//!< 処理中の行
} PostionInfo;

class CScriptEngine
{
public:
	CScriptEngine();
	~CScriptEngine();
	bool Load(const char *sFilename);
	bool Run();
	bool RegisterCommand(const std::string &name, const std::string &prm, seCOMMAND pFunc, void *arg);
	bool UnregisterCommand(const std::string &name);
	bool GoTo(const std::string &sLabel, bool bEvent = false);
	
	//! @brief スクリプトの実行を中止
	inline void Exit() { m_bWantExit = true; };
	//! @brief 全てのスクリプトの実行を中止
	inline void Break() { m_bWantBreak = true; };
	//! @brief 処理するファイル内位置を変更
	inline void SetPostion(const PostionInfo &o) { m_PosInfo = o; m_uSPos = o.uPos; m_bSetPostion = true; };
	//! @brief 処理するファイル内位置を変更(後処理なし)
	inline void SetPostionForce(const PostionInfo &o) { m_PosInfo = o; m_uSPos = o.uPos; };
	//! @brief 現在のファイル内位置を取得
	inline const PostionInfo GetPostion() { return m_PosInfo; };
	inline const PostionInfo GetPostionLH() { static PostionInfo pi; pi.uLine = m_PosInfo.uLine; pi.uPos = m_uSPos; return pi; };
	//! @brief 読み込んだスクリプトファイルのパスを取得
	inline const std::string *GetScriptFName() { return &m_sFilename; };
	//! @brief 処理中の行
	inline size_t GetLine() { return m_PosInfo.uLine; };

	int Eval_i(const std::string &sExpression);
	float Eval_f(const std::string &sExpression);

	CErrorMan m_em;

private:
	std::string m_Buf;
	size_t m_uBufSize;
	PostionInfo m_PosInfo;	//!< 現在位置
	size_t m_uSPos;			//!< 行の先頭への位置
	std::string m_sFilename;

	bool m_bSetPostion;		//!< ファイル内位置が変更されたか
	bool m_bWantExit, m_bWantBreak;

	int m_nRunCount;

	std::vector<CVariableMan> m_varList; //!< 変数バッファ
	std::vector<CommandInfo> m_cmdlist; //!< 命令リスト

	typedef struct {
		PostionInfo pos;	//!< 位置
		std::string sLabel;	//!< ラベルの名前
	} LabelInfo;
	std::vector<LabelInfo> m_labelList; //!< ラベルリスト

	bool CheckAllLabels();
	bool AnalyzeCommand(const std::string &sLine, CommandArg &cmdInfo);
	bool AnalyzeArgument(const std::string &rawData, CVariableMan &v);
	bool CallCommand(CommandArg &cmdInfo);
	
	unsigned char CheckMultiByte(unsigned char in, char Char) {
		if (in != 1 && _ismbblead(Char))
			return 1;
		else if (in == 1)
			return 2;
		return 0;
	};
};
