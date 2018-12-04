/*! @file CScriptEngine.cpp
@brief �X�N���v�g�G���W���{�̂̃\�[�X�R�[�h�B
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

/*! @brief �X�N���v�g��ǂݍ���
@param[in] sFilename �ǂݍ��ރt�@�C����
@return ������true�A���s��false�B
*/
bool CScriptEngine::Load(const char *sFilename) {
	ifstream ifs;
	stringstream ss;
	//�X�N���v�g�t�@�C����ǂݍ���
	int nSize = 0;
	ifs.open(sFilename);
	if (!ifs) { //�t�@�C���ǂݍ��݂Ɏ��s
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

/*! @brief ����������X�N���v�g��ǂݍ���
@param[in] buf ������o�b�t�@
@return ������true�A���s��false�B
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

/*! @brief �X�N���v�g�����s
@return ���true
*/
bool CScriptEngine::Run() {
	size_t &at = m_PosInfo.uPos;
	size_t &spos = m_uSPos;	//�s�̊J�n�ʒu
	CommandArg cmdInfo;		//�R�}���h�����i�[����
	char cNowChar = 0;		//���݂̕���
	char cPrevChar = 0;		//�O�̕���
	if (m_nRunCount == 0) {
		at = 0;
		spos = 0;
		m_PosInfo.uLine = 0;
	}
	m_bWantBreak = false;

	m_nRunCount++;	//���s�J�E���^������₷

	while (at < m_uBufSize) {
		if (m_bWantExit || m_bWantBreak)
			break;
		cNowChar = m_Buf[at];
		if (cNowChar == 0x0D || cNowChar == 0x0A) { //���s�������������ɍs����͂�����
			if (at > 0 && !(cNowChar ==0x0A && cPrevChar==0x0D)) { //���s�R�[�h��CRLF�̎���2�o�C�g�ډ������
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

	m_nRunCount--;	//���s�J�E���^�����炷
	if (m_nRunCount == 0)
		m_bWantExit = false;
	m_bWantBreak = false;
	return true;
}

/*! @brief �S�Ẵ��x�����`�F�b�N
@return ���true
@todo std::map�ɕύX������
*/
bool CScriptEngine::CheckAllLabels() {
	size_t at = 0;
	size_t spos = 0;		//�s�̊J�n�ʒu
	size_t uLine = 0;		//�s�ԍ�
	CommandArg cmdInfo;		//�R�}���h�����i�[����
	PostionInfo pi;
	char cPrevChar = 0;		//�O�̕���

	m_label.clear();
	while (at < m_uBufSize) {
		if (m_Buf[at] == 0x0D || m_Buf[at] == 0x0A) { //���s�������������ɍs����͂�����
			if (at > 0 && !(m_Buf[at] == 0x0A && cPrevChar == 0x0D)) { //���s�R�[�h��CRLF�̎���2�o�C�g�ډ������
				AnalyzeCommand(m_Buf.substr(spos, at - spos), cmdInfo);
				if (cmdInfo.commandName.length() > 1 && cmdInfo.commandName[0] == '*') {
					//���x�����X�g�ɓo�^
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

/*! @brief ���߂�o�^
@param[in] name ���߂̖��O
@param[in] prm �������X�g(i = Integer / f = Float / s = String / v = Variable / . = �ϒ�����)
@param[in] pFunc �Ăяo�����֐�
@param[in] arg ���[�U�[��`�l
@return ������true�A���s��false�B
*/
bool CScriptEngine::RegisterCommand(const string &name, const string &prm, seCOMMAND pFunc, void *arg) {
	static CommandInfo ci;

	//�֐������������A�֐��|�C���^��Null�̎��͒e��
	if (name.length() == 0)
		return false;
	if (pFunc == NULL)
		return false;

	ci.name = name;
	ci.prm = prm;
	ci.pFunc = pFunc;
	ci.arg = arg;
	m_cmdlist.push_back(ci); //�o�^
	return true;
}

/*! @brief ���߂��폜
@param[in] name �폜���閽�߂̖��O
@return ������true�A���s��false�B
@note �S�Ă̓������O�̖��߂��폜����邱�Ƃɒ���
*/
bool CScriptEngine::UnregisterCommand(const string &name) {
	//�֐������������̎��͒e��
	if (name.length() == 0)
		return false;
	
	//�����̖��߂��폜����
	auto result = remove_if(m_cmdlist.begin(), m_cmdlist.end(),
		[&name](const CommandInfo &o) -> bool {return o.name == name;});

	//�����ɗ]�����A�C�e�����폜
	m_cmdlist.erase(result, m_cmdlist.end());

	return true;
}

/*! @brief ���x���W�����v
@param[in] sLabel ���x����
@param[in] bEvent �C�x���g���΂ɂ��W�����v���ǂ���
@return ������true�A���s��false�B
@note wait�Ȃǂ̑ҋ@���ߓ��ŃC�x���g���������鎞�Ń��x���W�����v�������
CScriptEngine::Run()�����s����ꍇ�ɁAbEvent��true�ɂ��܂��B
*/
bool CScriptEngine::GoTo(const string &sLabel, bool bEvent) {
	if (sLabel.length() == 0)
		return false;

	PostionInfo pi;
	
	try {
		pi = m_label.at(sLabel);
	}
	catch (std::out_of_range) { //���x����������Ȃ�
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
/*! @brief �s�����
@param[in] sLine 1�s�̕�����
@param[out] cmdInfo ���߃f�[�^�[
@return ���true
*/
bool CScriptEngine::AnalyzeCommand(const string &sLine, CommandArg &cmdInfo) {
	size_t at = 0;
	size_t uLength = sLine.length();
	size_t spos = 0;
	bool bGotCommandName = false;	//�R�}���h�����擾�ł������ǂ���
	bool bIsInString = false;		//�����񃊃e�������ɋ��邩�ǂ���
	bool bIsReadingArg = false;		//�������������Ă���Œ���
	bool bStartedAnalyze = false;	//��͂��n�߂����ǂ���
	const char sep = ',';			//��؂蕶��
	char cNow = 0;					//���݂̈ʒu�̕���
	static CVariableMan cArg;		//���������ς݃f�[�^�[
	char cPrevChar = 0;				//�O�̕���
	unsigned char nMultiByte = 0;	//�}���`�o�C�g����������

	//�������X�g�����Z�b�g
	cmdInfo.ArgList.clear();
	//�����𑖍����ĕ���
	while (at < uLength) {
		cNow = sLine[at];
		nMultiByte = CheckMultiByte(nMultiByte, cNow);

		if (!bStartedAnalyze) {
			if (!IsSpace(cNow)) {
				bStartedAnalyze = true; //�s���̋󔒂��΂��I�������͂��J�n����
				spos = at;
			}
		}

		if (bStartedAnalyze) {
			if (!bGotCommandName) { //�R�}���h�����擾
				if (IsSpace(cNow)) {
					cmdInfo.commandName = sLine.substr(spos, at - spos);
					bGotCommandName = true;
					spos = at + 1;
				}
			}
			else {
				//�����񃊃e��������
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
			//�R�����g�������o�������ꍇ�͉�͂���߂�
			break;
		}

		cPrevChar = cNow;
		at++;
	}

	if (!bGotCommandName) { //�R�}���h�����擾
		cmdInfo.commandName = sLine.substr(spos, at - spos);
	}
	else {
		//�c��������������
		if (AnalyzeArgument(sLine.substr(spos, at - spos), cArg)) cmdInfo.ArgList.push_back(cArg);
	}
	return true;
}

/*! @brief ���������
@param[in] rawData �����̐��f�[�^�[
@param[out] v �������ꂽ�����f�[�^�[
@return ������������ꍇ��false�A����ȊO��true
*/
bool CScriptEngine::AnalyzeArgument(const string &rawData, CVariableMan &v) {
	size_t at = 0;
	size_t uLength = rawData.length();
	CVariableMan::VarType vType = CVariableMan::VARTYPE_UNKNOWN;	//�����̌^
	bool bStarted = false;			//��͂��J�n�������ǂ���
	char cNow = 0;					//���݂̕���
	bool bIsInString = false;		//�����񃊃e�������ɋ��邩�ǂ���
	bool bIsEscapeMode = false;		//�G�X�P�[�v�����Ƃ��ď�������K�v�����邩
	static string d_str;			//������
	int d_int;						//����
	float d_float;					//����
	bool bMinus = false;			//����
	bool bIsFloat = false;			//�����Ƃ��ď������邩
	bool bIsHex = false;			//16�i���Ƃ��ď������邩
	unsigned char nMultiByte = 0;	//�}���`�o�C�g����������
	size_t uPointAt = 0;			//�����_�̈ʒu
	
	while (at < uLength) {
		cNow = rawData[at];
		nMultiByte = CheckMultiByte(nMultiByte, cNow);

		if (!bStarted) {
			if (!IsSpace(cNow)) {
				bStarted = true;
			}
		}

		if (bStarted) {
			if (nMultiByte == 0) { //Shift_JIS���L�̔��p���������̖����������
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
			if (vType == CVariableMan::VARTYPE_INTEGER && nMultiByte==0){ //���l������
				if (cNow == '.' && !bIsHex) {
					bIsFloat = true;
					d_float = d_int; //�^�ϊ�
					uPointAt = 10;
				}
				else if (!bIsFloat) {
					if (bIsHex) { //16�i��
						if (cNow >= '0' && cNow <= '9')
							d_int = (d_int << 4) + (cNow - '0');
						if (cNow >= 'A' && cNow <= 'F')
							d_int = (d_int << 4) + (cNow - 'A' + 10);
						if (cNow >= 'a' && cNow <= 'f')
							d_int = (d_int << 4) + (cNow - 'a' + 10);
					}
					else { //10�i��
						if (cNow >= '0' && cNow <= '9')
							d_int = (d_int * 10) + (cNow - '0');
					}
				}
				else { //����
					if (cNow >= '0' && cNow <= '9') {
						d_float += (float)(cNow - '0') / uPointAt;
						uPointAt *= 10;
					}
				}
			}
			else if (vType == CVariableMan::VARTYPE_STRING) { //�����������
				if (!bIsEscapeMode) {
					if (cNow == '\\' && nMultiByte==0)
						bIsEscapeMode = true;
					else {
						if (cNow == '\"' && nMultiByte==0) {
							//�����񃊃e�������I�������烋�[�v����E�o
							break;
						}
						d_str += cNow;
					}
				}
				else {
					//�G�X�P�[�v�����Ƃ��ď���
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
			
			if (vType != CVariableMan::VARTYPE_STRING && IsSpace(cNow)) { //�󔒂��o�������烋�[�v����E�o
				break;
			}

		}
		at++;
	}
	
	//�����̏���
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
		//�ϐ��𑖍�
		bool bFound = false;
		for (auto itr = m_varList.begin(); itr != m_varList.end(); itr++) {
			if (*itr->GetName() == d_str) { //�ϐ�������v������A���̃N���X�ւ̃|�C���^���Q�ƌ^�Ƃ��Ĉ������X�g�ɃZ�b�g
				v.Set(&m_varList[itr - m_varList.begin()]);
				bFound = true;
				break;
			}
		}
		if (!bFound) { //������Ȃ�������V�������
			CVariableMan temp_var(d_str);
			m_varList.push_back(temp_var);
			v.Set(&m_varList[m_varList.size() - 1]);
		}
		break;
	}
	return bStarted;
}

/*! @brief ���߂��Ăяo��
@param[in] cmdInfo �Ăяo�����߂̃f�[�^�[
@return ������true�A���s��false�B
*/
bool CScriptEngine::CallCommand(CommandArg &cmdInfo) {
	bool bMatched = false;
	size_t index = 0;

	//�S�ẴR�}���h���X�g���疽�߂̖��O�ƈ����ꗗ���r���A��v�������߂�Index���擾����
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
						//�����^�ϐ��̏ꍇ�͒��g�����o���Ĉ����ɐ����^�萔���w�肵�����Ƃɂ���B
						cmdInfo.ArgList[i].Set((CVariableMan::nepInteger)GetVarPtrFromArg(cmdInfo, i)->GetData());
						bMatched = true;
					}
					if (cmdInfo.ArgList[i].GetType() == CVariableMan::VARTYPE_FLOAT) {
						//�����^�̏ꍇ�͈����ɐ����^���w�肵�����Ƃɂ���B
						cmdInfo.ArgList[i].Set((CVariableMan::neInteger)*(CVariableMan::nepFloat)cmdInfo.ArgList[i].GetData());
						bMatched = true;
					}
					break;
				case 's': //String
					if (cmdInfo.ArgList[i].GetType() != CVariableMan::VARTYPE_STRING)
						bMatched = false;
					if (cmdInfo.ArgList[i].GetType() == CVariableMan::VARTYPE_VARIABLE &&
						GetVarPtrFromArg(cmdInfo, i)->GetType() == CVariableMan::VARTYPE_STRING) {
						//�����^�ϐ��̏ꍇ�͒��g�����o���Ĉ����ɕ�����萔���w�肵�����Ƃɂ���B
						cmdInfo.ArgList[i].Set((CVariableMan::nepString)GetVarPtrFromArg(cmdInfo, i)->GetData());
						bMatched = true;
					}
					break;
				case 'f': //Float
					if (cmdInfo.ArgList[i].GetType() != CVariableMan::VARTYPE_FLOAT)
						bMatched = false;
					if (cmdInfo.ArgList[i].GetType() == CVariableMan::VARTYPE_VARIABLE &&
						GetVarPtrFromArg(cmdInfo, i)->GetType() == CVariableMan::VARTYPE_FLOAT) {
						//�����^�ϐ��̏ꍇ�͒��g�����o���Ĉ����Ɏ����^�萔���w�肵�����Ƃɂ���B
						cmdInfo.ArgList[i].Set((CVariableMan::nepFloat)GetVarPtrFromArg(cmdInfo, i)->GetData());
						bMatched = true;
					}
					if (cmdInfo.ArgList[i].GetType() == CVariableMan::VARTYPE_INTEGER) {
						//�����^�̏ꍇ�͈����Ɏ����^���w�肵�����Ƃɂ���B
						cmdInfo.ArgList[i].Set((CVariableMan::neFloat)*(CVariableMan::nepInteger)cmdInfo.ArgList[i].GetData());
						bMatched = true;
					}
					break;
				case 'h': //Handle
					if (cmdInfo.ArgList[i].GetType() != CVariableMan::VARTYPE_HANDLE)
						bMatched = false;
					if (cmdInfo.ArgList[i].GetType() == CVariableMan::VARTYPE_VARIABLE &&
						GetVarPtrFromArg(cmdInfo, i)->GetType() == CVariableMan::VARTYPE_HANDLE) {
						//�����^�ϐ��̏ꍇ�͒��g�����o���Ĉ����Ƀn���h���^�̒萔���w�肵�����Ƃɂ���B
						cmdInfo.ArgList[i].SetAsHandle((CVariableMan::nepHandle)GetVarPtrFromArg(cmdInfo, i)->GetData());
						bMatched = true;
					}
					break;
				case 'v': //Variable
					if (cmdInfo.ArgList[i].GetType() != CVariableMan::VARTYPE_VARIABLE)
						bMatched = false;
					break;
				case '.': //�ϒ������p
					i = cmdInfo.ArgList.size(); //���[�v�𔲂���
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
		m_cmdlist[index].pFunc(m_cmdlist[index].arg, &cmdInfo); //�֐����Ăяo��
	else 
	{
		m_em.ChachError(ERRORID_UNKNOWNCOMMAND, m_PosInfo.uLine, m_sFilename.c_str());
		return false;
	}

	return true;
}

typedef struct {
	int pri; //!< �D��x
	char op; //!< ���Z�q
	bool eq; //!< �������t���Ă��邩
} RPN_op;

/*! @brief �t�|�[�����h�L�@�ɕϊ�
@param[in] expression ��
@param[out] output ����
*/
void MakeRPN(const string &expression, string &output) {
	string out;				//�o��
	vector<RPN_op> stack;	//�X�^�b�N
	size_t uLength = expression.length();
	char cNow = 0;			//���݂̕���
	char cNext = 0;			//���̕���
	int nBrkLv = 0;			//�J�b�R�̃��x��
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

				//���̉��Z�q���D�揇�ʂ��������̂͐�ɏ�������
				while (stack.size() && stack.back().pri >= lv) {
					out += stack.back().op;
					if (stack.back().eq)
						out += '='; //������ǉ�
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
			out += '='; //������ǉ�
		stack.pop_back();
	}
	output = out;

	return;
}

/*! @brief ���l���ǂ����𒲂ׂ� 
@param[in] s ������^�̐��l
@return ���l�ł���ꍇtrue�A����ȊOfalse
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

/*! @brief ������̐�����]��(int)
@param[in] ��
@return ����
*/
int CScriptEngine::Eval_i(const string &expression) {
	return (int)Eval_f(expression);
}

/*! @brief ������̐�����]��(float)
@param[in] ��
@return ����
*/
float CScriptEngine::Eval_f(const string &expression) {
	vector<float> stack;	//�X�^�b�N
	char cNow = 0;		//���݂̕���
	char cNext = 0;		//���̕���
	bool bHex = false;	//16�i���ŏ������邩
	bool bHex_ = false;
	float l = 0.0f, r = 0.0f;
	string rpn;
	MakeRPN(expression, rpn); //�t�|�[�����h�L�@�ɕϊ�
	size_t uLength = rpn.length();

	//�v�Z
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
				if (IsDigitString(s) || bHex_) { //���l
					if (bHex_) //16�i��
						stack.push_back((float)strtol(s.c_str(), NULL, 16));
					else //10�i��
						stack.push_back(stof(s));
				}
				else { //�ϐ�
					for (auto itr = m_varList.begin(); itr != m_varList.end(); itr++) {
						if (*itr->GetName() == s) {
							//�^�������łȂ��ꍇ�͌^�ϊ�
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