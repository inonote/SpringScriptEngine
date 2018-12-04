/*! @file CScriptEngine.cpp
@brief スクリプトエンジン本体のソースコード。
*/

#include "CScriptEngine.h"

using namespace std;

CScriptEngine::CScriptEngine() : 
	m_bWantExit(false),
	m_bWantBreak(false),
	m_PosInfo({0, 0}),
	m_uSPos(0),
	m_uBufSize(0),
	m_bSetPostion(0),
	m_nRunCount(0)
{
	
}

CScriptEngine::~CScriptEngine() {
}

/*! @brief スクリプトを読み込み
@param[in] sFilename 読み込むファイル名
@return 成功時true、失敗時false。
*/
bool CScriptEngine::Load(const char *sFilename) {
	ifstream ifs;
	stringstream ss;
	//スクリプトファイルを読み込み
	int nSize = 0;
	ifs.open(sFilename);
	if (!ifs) { //ファイル読み込みに失敗
		m_em.ChachError(ERRORID_FILENOTEXIST, __LINE__, "");
		return false;
	}
	ss << ifs.rdbuf();
	ifs.close();
	m_Buf = ss.str() + "\r\n";
	ss.clear();
	m_uBufSize = m_Buf.length();

	m_sFilename = sFilename;
	m_label.clear();
	CheckAllLabels();
	return true;
}

/*! @brief メモリからスクリプトを読み込む
@param[in] buf 文字列バッファ
@return 成功時true、失敗時false。
*/
bool CScriptEngine::LoadFromMem(const char *buf) {
	if (buf == 0)
		return false;

	m_Buf = buf;
	m_Buf += string("\r\n");
	m_uBufSize = m_Buf.length();

	m_sFilename = "";
	m_label.clear();
	CheckAllLabels();
	return true;
}

/*! @brief スクリプトを実行
@return 常にtrue
*/
bool CScriptEngine::Run() {
	size_t &at = m_PosInfo.uPos;
	size_t &spos = m_uSPos;	//行の開始位置
	CommandArg cmdInfo;		//コマンド情報を格納する
	char cNowChar = 0;		//現在の文字
	char cPrevChar = 0;		//前の文字
	if (m_nRunCount == 0) {
		at = 0;
		spos = 0;
		m_PosInfo.uLine = 0;
	}
	m_bWantBreak = false;

	m_nRunCount++;	//実行カウンタを一つ増やす

	while (at < m_uBufSize) {
		if (m_bWantExit || m_bWantBreak)
			break;
		cNowChar = m_Buf[at];
		if (cNowChar == 0x0D || cNowChar == 0x0A) { //改行文字が来た時に行を解析させる
			if (at > 0 && !(cNowChar ==0x0A && cPrevChar==0x0D)) { //改行コードがCRLFの時の2バイト目回避処理
				AnalyzeCommand(m_Buf.substr(spos, at - spos), cmdInfo);
				if (cmdInfo.commandName != "" && cmdInfo.commandName[0] != '*') {
					CallCommand(cmdInfo);
					if (m_bSetPostion) {
						m_bSetPostion = false;
						if (m_PosInfo.uPos > 0)
							cPrevChar == m_Buf[m_PosInfo.uPos - 1];
						continue;
					}
				}
				m_PosInfo.uLine++;
			}
			spos = at + 1;
		}
		cPrevChar = cNowChar;
		at++;
	}

	m_nRunCount--;	//実行カウンタを減らす
	if (m_nRunCount == 0)
		m_bWantExit = false;
	m_bWantBreak = false;
	return true;
}

/*! @brief 全てのラベルをチェック
@return 常にtrue
@todo std::mapに変更したい
*/
bool CScriptEngine::CheckAllLabels() {
	size_t at = 0;
	size_t spos = 0;		//行の開始位置
	size_t uLine = 0;		//行番号
	CommandArg cmdInfo;		//コマンド情報を格納する
	PostionInfo pi;
	char cPrevChar = 0;		//前の文字

	m_label.clear();
	while (at < m_uBufSize) {
		if (m_Buf[at] == 0x0D || m_Buf[at] == 0x0A) { //改行文字が来た時に行を解析させる
			if (at > 0 && !(m_Buf[at] == 0x0A && cPrevChar == 0x0D)) { //改行コードがCRLFの時の2バイト目回避処理
				AnalyzeCommand(m_Buf.substr(spos, at - spos), cmdInfo);
				if (cmdInfo.commandName.length() > 1 && cmdInfo.commandName[0] == '*') {
					//ラベルリストに登録
					pi.uPos = spos;
					pi.uLine = uLine;
					m_label.insert(make_pair(cmdInfo.commandName.substr(1, string::npos), pi));
				}
				uLine++;
			}
			spos = at + 1;
		}
		cPrevChar = m_Buf[at];
		at++;
	}
	return true;
}

/*! @brief 命令を登録
@param[in] name 命令の名前
@param[in] prm 引数リスト(i = Integer / f = Float / s = String / v = Variable / . = 可変長引数)
@param[in] pFunc 呼び出される関数
@param[in] arg ユーザー定義値
@return 成功時true、失敗時false。
*/
bool CScriptEngine::RegisterCommand(const string &name, const string &prm, seCOMMAND pFunc, void *arg) {
	static CommandInfo ci;

	//関数名が名無し、関数ポインタがNullの時は弾く
	if (name.length() == 0)
		return false;
	if (pFunc == NULL)
		return false;

	ci.name = name;
	ci.prm = prm;
	ci.pFunc = pFunc;
	ci.arg = arg;
	m_cmdlist.push_back(ci); //登録
	return true;
}

/*! @brief 命令を削除
@param[in] name 削除する命令の名前
@return 成功時true、失敗時false。
@note 全ての同じ名前の命令が削除されることに注意
*/
bool CScriptEngine::UnregisterCommand(const string &name) {
	//関数名が名無しの時は弾く
	if (name.length() == 0)
		return false;
	
	//同名の命令を削除する
	auto result = remove_if(m_cmdlist.begin(), m_cmdlist.end(),
		[&name](const CommandInfo &o) -> bool {return o.name == name;});

	//末尾に余ったアイテムを削除
	m_cmdlist.erase(result, m_cmdlist.end());

	return true;
}

/*! @brief ラベルジャンプ
@param[in] sLabel ラベル名
@param[in] bEvent イベント発火によるジャンプかどうか
@return 成功時true、失敗時false。
@note waitなどの待機命令内でイベントを処理する時でラベルジャンプした後に
CScriptEngine::Run()を実行する場合に、bEventをtrueにします。
*/
bool CScriptEngine::GoTo(const string &sLabel, bool bEvent) {
	if (sLabel.length() == 0)
		return false;

	PostionInfo pi;
	
	try {
		pi = m_label.at(sLabel);
	}
	catch (std::out_of_range) { //ラベルが見つからない
		m_em.ChachError(ERRORID_LABELNOTEXIST, m_PosInfo.uLine, m_sFilename.c_str());
		return false;
	}

	if (bEvent)
		SetPostionForce(pi);
	else
		SetPostion(pi);
	return true;
}

#define IsSpace(_c) (_c == ' ' || _c == '\t')
/*! @brief 行を解析
@param[in] sLine 1行の文字列
@param[out] cmdInfo 命令データー
@return 常にtrue
*/
bool CScriptEngine::AnalyzeCommand(const string &sLine, CommandArg &cmdInfo) {
	size_t at = 0;
	size_t uLength = sLine.length();
	size_t spos = 0;
	bool bGotCommandName = false;	//コマンド名を取得できたかどうか
	bool bIsInString = false;		//文字列リテラル内に居るかどうか
	bool bIsReadingArg = false;		//引数を処理している最中か
	bool bStartedAnalyze = false;	//解析を始めたかどうか
	const char sep = ',';			//区切り文字
	char cNow = 0;					//現在の位置の文字
	static CVariableMan cArg;		//引数処理済みデーター
	char cPrevChar = 0;				//前の文字
	unsigned char nMultiByte = 0;	//マルチバイト文字処理状況

	//引数リストをリセット
	cmdInfo.ArgList.clear();
	//文字を走査して分解
	while (at < uLength) {
		cNow = sLine[at];
		nMultiByte = CheckMultiByte(nMultiByte, cNow);

		if (!bStartedAnalyze) {
			if (!IsSpace(cNow)) {
				bStartedAnalyze = true; //行頭の空白を飛ばし終えたら解析を開始する
				spos = at;
			}
		}

		if (bStartedAnalyze) {
			if (!bGotCommandName) { //コマンド名を取得
				if (IsSpace(cNow)) {
					cmdInfo.commandName = sLine.substr(spos, at - spos);
					bGotCommandName = true;
					spos = at + 1;
				}
			}
			else {
				//文字列リテラル処理
				if (bIsInString) {
					if (cNow == '\"' && cPrevChar != '\\' && nMultiByte==0)
						bIsInString = false;
				}
				else {
					if (cNow == '\"' && nMultiByte==0)
						bIsInString = true;
				}

				if (!bIsInString && cNow == sep) {
					AnalyzeArgument(sLine.substr(spos, at - spos), cArg);
					cmdInfo.ArgList.push_back(cArg);
					spos = at + 1;
				}
			}
		}
		if (!bIsInString && cNow==';' && nMultiByte==0) {
			//コメント文字が出現した場合は解析をやめる
			break;
		}

		cPrevChar = cNow;
		at++;
	}

	if (!bGotCommandName) { //コマンド名を取得
		cmdInfo.commandName = sLine.substr(spos, at - spos);
	}
	else {
		//残った引数を処理
		if (AnalyzeArgument(sLine.substr(spos, at - spos), cArg)) cmdInfo.ArgList.push_back(cArg);
	}
	return true;
}

/*! @brief 引数を解析
@param[in] rawData 引数の生データー
@param[out] v 処理された引数データー
@return 空引数だった場合はfalse、それ以外はtrue
*/
bool CScriptEngine::AnalyzeArgument(const string &rawData, CVariableMan &v) {
	size_t at = 0;
	size_t uLength = rawData.length();
	CVariableMan::VarType vType = CVariableMan::VARTYPE_UNKNOWN;	//引数の型
	bool bStarted = false;			//解析を開始したかどうか
	char cNow = 0;					//現在の文字
	bool bIsInString = false;		//文字列リテラル内に居るかどうか
	bool bIsEscapeMode = false;		//エスケープ文字として処理する必要があるか
	static string d_str;			//文字列
	int d_int;						//整数
	float d_float;					//実数
	bool bMinus = false;			//負数
	bool bIsFloat = false;			//実数として処理するか
	bool bIsHex = false;			//16進数として処理するか
	unsigned char nMultiByte = 0;	//マルチバイト文字処理状況
	size_t uPointAt = 0;			//小数点の位置
	
	while (at < uLength) {
		cNow = rawData[at];
		nMultiByte = CheckMultiByte(nMultiByte, cNow);

		if (!bStarted) {
			if (!IsSpace(cNow)) {
				bStarted = true;
			}
		}

		if (bStarted) {
			if (nMultiByte == 0) { //Shift_JIS特有の半角文字処理の問題を回避する
				if (vType == CVariableMan::VARTYPE_UNKNOWN) {
					if (cNow == '\"') {
						vType = CVariableMan::VARTYPE_STRING;
						d_str = "";
						at++;
						continue;
					}
					else if (cNow == '$') {
						vType = CVariableMan::VARTYPE_INTEGER;
						bIsHex = true;
						d_int = 0;
						bMinus = false;
					}
					else if (cNow >= '0' && cNow <= '9') {
						vType = CVariableMan::VARTYPE_INTEGER;
						d_int = 0;
						bMinus = false;
					}
					else if (cNow == '-') {
						vType = CVariableMan::VARTYPE_INTEGER;
						d_int = 0;
						bMinus = true;
					}
					else {
						vType = CVariableMan::VARTYPE_VARIABLE;
						d_str = "";
					}
				}
			}
			if (vType == CVariableMan::VARTYPE_INTEGER && nMultiByte==0){ //数値を処理
				if (cNow == '.' && !bIsHex) {
					bIsFloat = true;
					d_float = d_int; //型変換
					uPointAt = 10;
				}
				else if (!bIsFloat) {
					if (bIsHex) { //16進数
						if (cNow >= '0' && cNow <= '9')
							d_int = (d_int << 4) + (cNow - '0');
						if (cNow >= 'A' && cNow <= 'F')
							d_int = (d_int << 4) + (cNow - 'A' + 10);
						if (cNow >= 'a' && cNow <= 'f')
							d_int = (d_int << 4) + (cNow - 'a' + 10);
					}
					else { //10進数
						if (cNow >= '0' && cNow <= '9')
							d_int = (d_int * 10) + (cNow - '0');
					}
				}
				else { //実数
					if (cNow >= '0' && cNow <= '9') {
						d_float += (float)(cNow - '0') / uPointAt;
						uPointAt *= 10;
					}
				}
			}
			else if (vType == CVariableMan::VARTYPE_STRING) { //文字列を処理
				if (!bIsEscapeMode) {
					if (cNow == '\\' && nMultiByte==0)
						bIsEscapeMode = true;
					else {
						if (cNow == '\"' && nMultiByte==0) {
							//文字列リテラルが終了したらループから脱出
							break;
						}
						d_str += cNow;
					}
				}
				else {
					//エスケープ文字として処理
					switch (cNow)
					{
					case 'n':
						d_str += '\n';
						break;
					case 'r':
						d_str += '\r';
						break;
					case '\"':
						d_str += '\"';
						break;
					case '\\':
						d_str += '\\';
						break;
					}
					bIsEscapeMode = false;
				}
			}
			else if (vType == CVariableMan::VARTYPE_VARIABLE) {
				d_str += cNow;
			}
			
			if (vType != CVariableMan::VARTYPE_STRING && IsSpace(cNow)) { //空白が出現したらループから脱出
				break;
			}

		}
		at++;
	}
	
	//引数の処理
	switch (vType)
	{
	case CVariableMan::VARTYPE_INTEGER:
		if (bIsFloat) {
			if (bMinus)
				d_float *= -1;
			v.Set(&d_float);
		}
		else {
			if (bMinus)
				d_int *= -1;
			v.Set(&d_int);
		}
		break;
	case CVariableMan::VARTYPE_STRING:
		v.Set(&d_str);
		break;
	case CVariableMan::VARTYPE_VARIABLE:
		//変数を走査
		bool bFound = false;
		for (auto itr = m_varList.begin(); itr != m_varList.end(); itr++) {
			if (*itr->GetName() == d_str) { //変数名が一致したら、そのクラスへのポインタを参照型として引数リストにセット
				v.Set(&m_varList[itr - m_varList.begin()]);
				bFound = true;
				break;
			}
		}
		if (!bFound) { //見つからなかったら新しく作る
			CVariableMan temp_var(d_str);
			m_varList.push_back(temp_var);
			v.Set(&m_varList[m_varList.size() - 1]);
		}
		break;
	}
	return bStarted;
}

/*! @brief 命令を呼び出す
@param[in] cmdInfo 呼び出す命令のデーター
@return 成功時true、失敗時false。
*/
bool CScriptEngine::CallCommand(CommandArg &cmdInfo) {
	bool bMatched = false;
	size_t index = 0;

	//全てのコマンドリストから命令の名前と引数一覧を比較し、一致した命令のIndexを取得する
	for (auto itr = m_cmdlist.begin(); itr != m_cmdlist.end(); itr++) {
		if (itr->name == cmdInfo.commandName && 
			((itr->prm.length() == cmdInfo.ArgList.size()) || 
				(itr->prm.length()>0 && itr->prm[itr->prm.length()-1]=='.' && (itr->prm.length() <= cmdInfo.ArgList.size())) )) {
			bMatched = true;
			for (size_t i = 0; i < itr->prm.length() && i < cmdInfo.ArgList.size(); i++){
				switch (itr->prm[i])
				{
				case 'i': //Integer
					if (cmdInfo.ArgList[i].GetType() != CVariableMan::VARTYPE_INTEGER)
						bMatched = false;
					if (cmdInfo.ArgList[i].GetType() == CVariableMan::VARTYPE_VARIABLE && 
						GetVarPtrFromArg(cmdInfo, i)->GetType() == CVariableMan::VARTYPE_INTEGER) {
						//整数型変数の場合は中身を取り出して引数に整数型定数を指定したことにする。
						cmdInfo.ArgList[i].Set((CVariableMan::nepInteger)GetVarPtrFromArg(cmdInfo, i)->GetData());
						bMatched = true;
					}
					if (cmdInfo.ArgList[i].GetType() == CVariableMan::VARTYPE_FLOAT) {
						//実数型の場合は引数に整数型を指定したことにする。
						cmdInfo.ArgList[i].Set((CVariableMan::neInteger)*(CVariableMan::nepFloat)cmdInfo.ArgList[i].GetData());
						bMatched = true;
					}
					break;
				case 's': //String
					if (cmdInfo.ArgList[i].GetType() != CVariableMan::VARTYPE_STRING)
						bMatched = false;
					if (cmdInfo.ArgList[i].GetType() == CVariableMan::VARTYPE_VARIABLE &&
						GetVarPtrFromArg(cmdInfo, i)->GetType() == CVariableMan::VARTYPE_STRING) {
						//整数型変数の場合は中身を取り出して引数に文字列定数を指定したことにする。
						cmdInfo.ArgList[i].Set((CVariableMan::nepString)GetVarPtrFromArg(cmdInfo, i)->GetData());
						bMatched = true;
					}
					break;
				case 'f': //Float
					if (cmdInfo.ArgList[i].GetType() != CVariableMan::VARTYPE_FLOAT)
						bMatched = false;
					if (cmdInfo.ArgList[i].GetType() == CVariableMan::VARTYPE_VARIABLE &&
						GetVarPtrFromArg(cmdInfo, i)->GetType() == CVariableMan::VARTYPE_FLOAT) {
						//整数型変数の場合は中身を取り出して引数に実数型定数を指定したことにする。
						cmdInfo.ArgList[i].Set((CVariableMan::nepFloat)GetVarPtrFromArg(cmdInfo, i)->GetData());
						bMatched = true;
					}
					if (cmdInfo.ArgList[i].GetType() == CVariableMan::VARTYPE_INTEGER) {
						//整数型の場合は引数に実数型を指定したことにする。
						cmdInfo.ArgList[i].Set((CVariableMan::neFloat)*(CVariableMan::nepInteger)cmdInfo.ArgList[i].GetData());
						bMatched = true;
					}
					break;
				case 'h': //Handle
					if (cmdInfo.ArgList[i].GetType() != CVariableMan::VARTYPE_HANDLE)
						bMatched = false;
					if (cmdInfo.ArgList[i].GetType() == CVariableMan::VARTYPE_VARIABLE &&
						GetVarPtrFromArg(cmdInfo, i)->GetType() == CVariableMan::VARTYPE_HANDLE) {
						//整数型変数の場合は中身を取り出して引数にハンドル型の定数を指定したことにする。
						cmdInfo.ArgList[i].SetAsHandle((CVariableMan::nepHandle)GetVarPtrFromArg(cmdInfo, i)->GetData());
						bMatched = true;
					}
					break;
				case 'v': //Variable
					if (cmdInfo.ArgList[i].GetType() != CVariableMan::VARTYPE_VARIABLE)
						bMatched = false;
					break;
				case '.': //可変長引数用
					i = cmdInfo.ArgList.size(); //ループを抜ける
					break;
				}
				if (!bMatched)
					break;
			}
			if (bMatched) {
				index = itr - m_cmdlist.begin();
				break;
			}
		}
	}

	if (bMatched)
		m_cmdlist[index].pFunc(m_cmdlist[index].arg, &cmdInfo); //関数を呼び出す
	else 
	{
		m_em.ChachError(ERRORID_UNKNOWNCOMMAND, m_PosInfo.uLine, m_sFilename.c_str());
		return false;
	}

	return true;
}

typedef struct {
	int pri; //!< 優先度
	char op; //!< 演算子
	bool eq; //!< 等号が付いているか
} RPN_op;

/*! @brief 逆ポーランド記法に変換
@param[in] expression 式
@param[out] output 結果
*/
void MakeRPN(const string &expression, string &output) {
	string out;				//出力
	vector<RPN_op> stack;	//スタック
	size_t uLength = expression.length();
	char cNow = 0;			//現在の文字
	char cNext = 0;			//次の文字
	int nBrkLv = 0;			//カッコのレベル
	RPN_op ro;
	output = "";

	for (size_t i = 0; i < uLength; i++) {
		cNow = expression[i];
		if (i + 1 < uLength)
			cNext = expression[i + 1];
		else
			cNext = 0;

		if ((cNow >= '0' && cNow <= '9') || (cNow >= 'a' && cNow <= 'z') || (cNow >= 'A' && cNow <= 'Z') || (cNow == '_')) {
			out += cNow;
		}
		else {
			int lv = 0;
			switch (cNow) {
			case '*':
			case '/':
			case '%':
				lv = nBrkLv - 3;
				ro.eq = false;
				goto case_op;
			case '+':
			case '-':
				lv = nBrkLv - 4;
				ro.eq = false;
				goto case_op;
			case '<':
			case '>':
				if (cNext == '=') {
					ro.eq = true;
					i++;
				}
				else
					ro.eq = false;
				lv = nBrkLv - 4;
				goto case_op;
			case '&':
				lv = nBrkLv - 8;
				ro.eq = false;
				goto case_op;
			case '^':
				lv = nBrkLv - 9;
				ro.eq = false;
				goto case_op;
			case '|':
				lv = nBrkLv - 10;
				ro.eq = false;
				goto case_op;
			case_op:
				out += ' ';

				//今の演算子より優先順位が高いものは先に処理する
				while (stack.size() && stack.back().pri >= lv) {
					out += stack.back().op;
					if (stack.back().eq)
						out += '='; //等号を追加
					stack.pop_back();
				}
				ro.op = cNow;
				ro.pri = lv;
				stack.push_back(ro);
				break;
			case '(':
			case '{':
				nBrkLv += 10;
				break;
			case ')':
			case '}':
				nBrkLv -= 10;
				break;
			case '$':
			case '#':
			case '.':
			case '_':
				out += cNow;
				break;
			case ' ':
				break;
			default:
				return;
				break;
			}
		}
	}
	out += ' ';
	while (stack.size()) {
		out += stack.back().op;
		if (stack.back().eq)
			out += '='; //等号を追加
		stack.pop_back();
	}
	output = out;

	return;
}

/*! @brief 数値かどうかを調べる 
@param[in] s 文字列型の数値
@return 数値である場合true、それ以外false
*/
bool IsDigitString(const string &s) {
	auto i = s.begin();
	while (i != s.end()) {
		if ((*i >= '0' && *i <= '9') ||
			*i == '.') {
		}
		else
			return false;
		i++;
	}
	return true;
}

/*! @brief 文字列の数式を評価(int)
@param[in] 式
@return 結果
*/
int CScriptEngine::Eval_i(const string &expression) {
	return (int)Eval_f(expression);
}

/*! @brief 文字列の数式を評価(float)
@param[in] 式
@return 結果
*/
float CScriptEngine::Eval_f(const string &expression) {
	vector<float> stack;	//スタック
	char cNow = 0;		//現在の文字
	char cNext = 0;		//次の文字
	bool bHex = false;	//16進数で処理するか
	bool bHex_ = false;
	float l = 0.0f, r = 0.0f;
	string rpn;
	MakeRPN(expression, rpn); //逆ポーランド記法に変換
	size_t uLength = rpn.length();

	//計算
	for (size_t i = 0; i < uLength; i++) {
		cNow = rpn[i];
		if (i + 1 < uLength)
			cNext = rpn[i + 1];
		else
			cNext = 0;
		bHex_ = bHex;
		bHex = false;

		switch (cNow)
		{
		case '+':
			if (stack.size() <= 1)
				stack.insert(stack.begin(), 0);
			r = stack.back(); stack.pop_back();
			l = stack.back(); stack.pop_back();
			stack.push_back(l + r);
			break;
		case '-':
			if (stack.size() <= 1)
				stack.insert(stack.begin(), 0);
			r = stack.back(); stack.pop_back();
			l = stack.back(); stack.pop_back();
			stack.push_back(l - r);
			break;
		case '*':
			if (stack.size() <= 1)
				stack.insert(stack.begin(), 0);
			r = stack.back(); stack.pop_back();
			l = stack.back(); stack.pop_back();
			stack.push_back(l * r);
			break;
		case '/':
			if (stack.size() <= 1)
				stack.insert(stack.begin(), 0);
			r = stack.back(); stack.pop_back();
			l = stack.back(); stack.pop_back();
			if (r == 0) {
				m_em.ChachError(ERRORID_DIVIDEBYZERO, m_PosInfo.uLine, m_sFilename.c_str());
				return 0.0f;
			}
			stack.push_back(l / r);
			break;
		case '%':
			if (stack.size() <= 1)
				stack.insert(stack.begin(), 0);
			r = stack.back(); stack.pop_back();
			l = stack.back(); stack.pop_back();
			if (r == 0) {
				m_em.ChachError(ERRORID_DIVIDEBYZERO, m_PosInfo.uLine, m_sFilename.c_str());
				return 0.0f;
			}
			stack.push_back(fmodf(l, r));
			break;
		case '^':
			if (stack.size() <= 1)
				stack.insert(stack.begin(), 0);
			r = stack.back(); stack.pop_back();
			l = stack.back(); stack.pop_back();
			stack.push_back((float)((long)l ^ (long)r));
			break;
		case '&':
			if (stack.size() <= 1)
				stack.insert(stack.begin(), 0);
			r = stack.back(); stack.pop_back();
			l = stack.back(); stack.pop_back();
			stack.push_back((float)((long)l & (long)r));
			break;
		case '|':
			if (stack.size() <= 1)
				stack.insert(stack.begin(), 0);
			r = stack.back(); stack.pop_back();
			l = stack.back(); stack.pop_back();
			stack.push_back((float)((long)l | (long)r));
			break;
		case '>':
			if (stack.size() <= 1)
				stack.insert(stack.begin(), 0);
			r = stack.back(); stack.pop_back();
			l = stack.back(); stack.pop_back();
			if (cNext == '=') {
				stack.push_back(l >= r);
				i++;
			}
			else
				stack.push_back(l > r);
			break;
		case '<':
			if (stack.size() <= 1)
				stack.insert(stack.begin(), 0);
			r = stack.back(); stack.pop_back();
			l = stack.back(); stack.pop_back();
			if (cNext == '=') {
				stack.push_back(l <= r);
				i++;
			}
			else
				stack.push_back(l < r);
			break;
		case '=':
			if (stack.size() <= 1)
				stack.insert(stack.begin(), 0);
			r = stack.back(); stack.pop_back();
			l = stack.back(); stack.pop_back();
			stack.push_back(l == r);
			if (cNext == '=')
				i++;
			break;
		case '$':
			bHex = true;
			break;
		case ' ':
			break;
		default: {
			size_t ofs = rpn.find(' ', i);
			if (ofs != string::npos) {
				string s = rpn.substr(i, ofs - i);
				if (IsDigitString(s) || bHex_) { //数値
					if (bHex_) //16進数
						stack.push_back((float)strtol(s.c_str(), NULL, 16));
					else //10進数
						stack.push_back(stof(s));
				}
				else { //変数
					for (auto itr = m_varList.begin(); itr != m_varList.end(); itr++) {
						if (*itr->GetName() == s) {
							//型が実数でない場合は型変換
							switch (itr->GetType()) {
							case CVariableMan::VARTYPE_INTEGER:
								stack.push_back((float)*(CVariableMan::nepInteger)itr->GetData());
								break;
							case CVariableMan::VARTYPE_STRING:
								stack.push_back(stof(*(CVariableMan::nepString)itr->GetData()));
								break;
							case CVariableMan::VARTYPE_FLOAT:
								stack.push_back(*(CVariableMan::nepFloat)itr->GetData());
								break;
							case CVariableMan::VARTYPE_HANDLE:
								stack.push_back((float)*(CVariableMan::nepHandle)itr->GetData());
								break;
							}
						}
					}
				}
				i = ofs - 1;
			}
		}
		}
	}
	if (stack.size() == 0)
		return 0;

	return stack[0];
}