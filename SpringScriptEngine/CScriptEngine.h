#pragma once
#include "CScriptEngine.h"
#include "CVariableMan.h"
#include "CErrorMan.h"

//スクリプトエンジン本体のクラス。

//構造体
typedef struct tagCommandArg {
	std::string commandName;
	std::vector<CVariableMan> ArgList;
} CommandArg;

typedef int(*seCOMMAND)(void *, CommandArg *);

typedef struct tagCommandInfo {
	std::string name;
	std::string prm;
	seCOMMAND pFunc;
	void *arg;
} CommandInfo;

typedef struct tagPostionInfo {
	size_t uPos;
	size_t uLine;
} PostionInfo;

class CScriptEngine
{
public:
	CScriptEngine();
	~CScriptEngine();
	bool Load(const char *sFilename);
	bool Run();
	inline void Exit() { m_bWantExit = true; };
	inline void Break() { m_bWantBreak = true; };

	bool RegisterCommand(const std::string &name, const std::string &prm, seCOMMAND pFunc, void *arg);
	bool UnregisterCommand(const std::string &name);

	bool GoTo(const std::string &sLabel, bool bEvent = false);

	inline void SetPostion(const PostionInfo &o) { m_PosInfo = o; m_uSPos = o.uPos; m_bSetPostion = true; };
	inline void SetPostionForce(const PostionInfo &o) { m_PosInfo = o; m_uSPos = o.uPos; };
	inline const PostionInfo GetPostion() { return m_PosInfo; };
	inline const PostionInfo GetPostionLH() {
		static PostionInfo pi;
		pi.uLine = m_PosInfo.uLine;
		pi.uPos = m_uSPos;
		return pi;
	};

	inline const std::string *GetScriptFName() { return &m_sFilename; };
	inline size_t GetLine() { return m_PosInfo.uLine; };

	int Eval_i(const std::string &sExpression);
	float Eval_f(const std::string &sExpression);

	CErrorMan m_em;

private:
	std::string m_Buf;
	size_t m_uBufSize;
	PostionInfo m_PosInfo;	//現在位置
	size_t m_uSPos;			//行の先頭への位置
	std::string m_sFilename;

	bool m_bSetPostion;
	bool m_bWantExit, m_bWantBreak;

	int m_nRunCount;

	std::vector<CVariableMan> m_varList;
	std::vector<CommandInfo> m_cmdlist;

	typedef struct {
		PostionInfo pos;
		std::string sLabel;
	} LabelInfo;
	std::vector<LabelInfo> m_labelList;

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
