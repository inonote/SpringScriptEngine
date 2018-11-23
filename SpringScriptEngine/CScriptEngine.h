/*! @file CScriptEngine.h
@brief �X�N���v�g�G���W���{�̂̃w�b�_�R�[�h�B
*/

#pragma once
#include "CScriptEngine.h"
#include "CVariableMan.h"
#include "CErrorMan.h"

//�\����
typedef struct tagCommandArg {
	std::string commandName; //!< ���߂̖��O
	std::vector<CVariableMan> ArgList; //!< ����
} CommandArg;

typedef int(*seCOMMAND)(void *, CommandArg *);

typedef struct tagCommandInfo {
	std::string name;	//!< ���߂̖��O
	std::string prm;	//!< ����
	seCOMMAND pFunc;	//!< �Ăяo���֐��̃|�C���^
	void *arg;			//!< ���[�U�[��`�̒l
} CommandInfo;

typedef struct tagPostionInfo {
	size_t uPos;		//!< �t�@�C���̐擪����̈ʒu
	size_t uLine;		//!< �������̍s
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
	
	//! @brief �X�N���v�g�̎��s�𒆎~
	inline void Exit() { m_bWantExit = true; };
	//! @brief �S�ẴX�N���v�g�̎��s�𒆎~
	inline void Break() { m_bWantBreak = true; };
	//! @brief ��������t�@�C�����ʒu��ύX
	inline void SetPostion(const PostionInfo &o) { m_PosInfo = o; m_uSPos = o.uPos; m_bSetPostion = true; };
	//! @brief ��������t�@�C�����ʒu��ύX(�㏈���Ȃ�)
	inline void SetPostionForce(const PostionInfo &o) { m_PosInfo = o; m_uSPos = o.uPos; };
	//! @brief ���݂̃t�@�C�����ʒu���擾
	inline const PostionInfo GetPostion() { return m_PosInfo; };
	inline const PostionInfo GetPostionLH() { static PostionInfo pi; pi.uLine = m_PosInfo.uLine; pi.uPos = m_uSPos; return pi; };
	//! @brief �ǂݍ��񂾃X�N���v�g�t�@�C���̃p�X���擾
	inline const std::string *GetScriptFName() { return &m_sFilename; };
	//! @brief �������̍s
	inline size_t GetLine() { return m_PosInfo.uLine; };

	int Eval_i(const std::string &sExpression);
	float Eval_f(const std::string &sExpression);

	CErrorMan m_em;

private:
	std::string m_Buf;
	size_t m_uBufSize;
	PostionInfo m_PosInfo;	//!< ���݈ʒu
	size_t m_uSPos;			//!< �s�̐擪�ւ̈ʒu
	std::string m_sFilename;

	bool m_bSetPostion;		//!< �t�@�C�����ʒu���ύX���ꂽ��
	bool m_bWantExit, m_bWantBreak;

	int m_nRunCount;

	std::vector<CVariableMan> m_varList; //!< �ϐ��o�b�t�@
	std::vector<CommandInfo> m_cmdlist; //!< ���߃��X�g

	typedef struct {
		PostionInfo pos;	//!< �ʒu
		std::string sLabel;	//!< ���x���̖��O
	} LabelInfo;
	std::vector<LabelInfo> m_labelList; //!< ���x�����X�g

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
