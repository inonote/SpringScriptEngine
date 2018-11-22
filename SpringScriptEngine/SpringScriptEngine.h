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

/*! @brief SpringScriptEngine�̃C���X�^���X���쐬
@return ���������Ƃ���HSPRINGSCRIPT�^�̃n���h���B���s�����Ƃ���NULL�B
*/
DLLEXPORT HSPRINGSCRIPT SpringScript_CreateInstance();

/*! @brief SpringScriptEngine�̃C���X�^���X��j��
@param[in] hSpringScript SpringScriptEngine�̃n���h��
@return ���s�����Ƃ�FALSE�A���������Ƃ���FALSE�ȊO�B
*/
DLLEXPORT BOOL SpringScript_DestroyInstance(HSPRINGSCRIPT hSpringScript);

/*! @brief �G���[�����̌Ăяo���R�[���o�b�N�֐���ݒ�
@param[in] hSpringScript SpringScriptEngine�̃n���h��
@param[in] hWnd �e�E�B���h�E�n���h��(NULL��)
@param[in] pFunc �R�[���o�b�N�֐��ւ̃|�C���^(NULL��)
@param[in] pUserData �R�[���o�b�N�֐��̌Ăяo�����ɓn����郆�[�U�[��`�̒l(NULL��)
@return ���s�����Ƃ�FALSE�A���������Ƃ���FALSE�ȊO�B
*/
DLLEXPORT BOOL SpringScript_SetErrorCallbk(HSPRINGSCRIPT hSpringScript, HWND hWnd, ErrorCallbk pFunc, void *pUserData);

/*! @brief �X�N���v�g�t�@�C����ǂݍ���
@param[in] hSpringScript SpringScriptEngine�̃n���h��
@param[in] pszFileName �X�N���v�g�t�@�C���ւ̃p�X
@return ���s�����Ƃ�FALSE�A���������Ƃ���FALSE�ȊO�B
*/
DLLEXPORT BOOL SpringScript_Load(HSPRINGSCRIPT hSpringScript, const char *pszFileName);

/*! @brief �X�N���v�g�����s
@param[in] hSpringScript SpringScriptEngine�̃n���h��
@return ���s�����Ƃ�FALSE�A���������Ƃ���FALSE�ȊO�B
*/
DLLEXPORT BOOL SpringScript_Run(HSPRINGSCRIPT hSpringScript);

/*! @brief �X�N���v�g�̎��s�𒆎~
�X�N���v�g���s����SpringScript_Run�֐����Ăяo�����ꍇ�́A�Ō�ɌĂяo����SpringScript_Run�֐��̎��s�𒆎~���܂��B
@param[in] hSpringScript SpringScriptEngine�̃n���h��
@return ���s�����Ƃ�FALSE�A���������Ƃ���FALSE�ȊO�B
*/
DLLEXPORT BOOL SpringScript_Break(HSPRINGSCRIPT hSpringScript);

/*! @brief �X�N���v�g�̎��s�����ׂĒ��~
�X�N���v�g���s���ɌĂяo����SpringScript_Run�֐����܂߁A���ׂĂ̎��s�𒆎~���܂��B
@param[in] hSpringScript SpringScriptEngine�̃n���h��
@return ���s�����Ƃ�FALSE�A���������Ƃ���FALSE�ȊO�B
*/
DLLEXPORT BOOL SpringScript_Exit(HSPRINGSCRIPT hSpringScript);

/*! @brief ���߂�o�^
@param[in] hSpringScript SpringScriptEngine�̃n���h��
@param[in] pszName ���߂̖��O(NULL�s��)
@param[in] pszPrm ���߂̈���(NULL�s��)
�����̌^�������L��[i = Integer / f = Float / s = String / v = Variable / . = �ϒ�����]
��)�uInteger, String, Float, Integer, ...�v�̉ϒ������ɂ������ꍇ�́A�uisfi.�v�Ɛݒ肵�܂��B
@param[in] pFunc �Ăяo���R�[���o�b�N�֐�(NULL�s��)
@param[in] pUserData �R�[���o�b�N�֐��̌Ăяo�����ɓn����郆�[�U�[��`�̒l(NULL��)
@return ���s�����Ƃ�FALSE�A���������Ƃ���FALSE�ȊO�B
*/
DLLEXPORT BOOL SpringScript_RegisterCommand(HSPRINGSCRIPT hSpringScript, const char *pszName, const char *pszPrm, seCOMMAND pFunc, void *pUserData);

/*! @brief ���߂��폜
@param[in] hSpringScript SpringScriptEngine�̃n���h��
@param[in] pszName ���߂̖��O(NULL�s��)
@return ���s�����Ƃ�FALSE�A���������Ƃ���FALSE�ȊO�B
*/
DLLEXPORT BOOL SpringScript_UnregisterCommand(HSPRINGSCRIPT hSpringScript, const char *pszName);

/*! @brief ���x���փW�����v
@param[in] hSpringScript SpringScriptEngine�̃n���h��
@param[in] pszName ���x���̖��O(NULL�s��)
@return ���s�����Ƃ�FALSE�A���������Ƃ���FALSE�ȊO�B
*/
DLLEXPORT BOOL SpringScript_GoTo(HSPRINGSCRIPT hSpringScript, const char *pszLabel);

/*! @brief ��͒��̃X�N���v�g�̈ʒu���擾
@param[in] hSpringScript SpringScriptEngine�̃n���h��
@param[out] pi PostionInfo�\���̂̃|�C���^(NULL�s��)
@return ���s�����Ƃ�FALSE�A���������Ƃ���FALSE�ȊO�B
*/
DLLEXPORT BOOL SpringScript_GetPostion(HSPRINGSCRIPT hSpringScript, PostionInfo *pi);

/*! @brief ��͒��̃X�N���v�g�̈ʒu���擾2
SpringScript_GetPostion�֐��ƈႢ�A�擪����̃o�b�t�@�ʒu������PostionInfo.uPos�ɍs�̓��̈ʒu���i�[���܂��B
@param[in] hSpringScript SpringScriptEngine�̃n���h��
@param[out] pi PostionInfo�\���̂̃|�C���^(NULL�s��)
@return ���s�����Ƃ�FALSE�A���������Ƃ���FALSE�ȊO�B
*/
DLLEXPORT BOOL SpringScript_GetPostionLH(HSPRINGSCRIPT hSpringScript, PostionInfo *pi);

/*! @brief ��͒��̃X�N���v�g�̈ʒu���ړ�
�T�u���[�`���W�����v�̕��A�ȂǂɎg�p���܂��B
@param[in] hSpringScript SpringScriptEngine�̃n���h��
@param[in] pi PostionInfo�\���̂̃|�C���^(NULL�s��)
@return ���s�����Ƃ�FALSE�A���������Ƃ���FALSE�ȊO�B
*/
DLLEXPORT BOOL SpringScript_SetPostion(HSPRINGSCRIPT hSpringScript, const PostionInfo *pi);

/*! @brief ��͒��̃X�N���v�g�̈ʒu���ړ�
SpringScript_SetPostion�֐��ƈႢ�A��񂾐�̍s�𖳎����Ď��̍s����J�n���܂��B
(PostionInfo�\���̂̒l�����̂܂܎g����)
@param[in] hSpringScript SpringScriptEngine�̃n���h��
@param[in] pi PostionInfo�\���̂̃|�C���^(NULL�s��)
@return ���s�����Ƃ�FALSE�A���������Ƃ���FALSE�ȊO�B
*/
DLLEXPORT BOOL SpringScript_SetPostionForce(HSPRINGSCRIPT hSpringScript, const PostionInfo *pi);

/*! @brief �s�ʒu���擾
@param[in] hSpringScript SpringScriptEngine�̃n���h��
@param[out] val �l��������ϐ��̃|�C���^(NULL�s��)
@return ���s�����Ƃ�FALSE�A���������Ƃ���FALSE�ȊO�B
*/
DLLEXPORT BOOL SpringScript_GetLineNum(HSPRINGSCRIPT hSpringScript, size_t *pVal);

/*! @brief �������v�Z
@param[in] hSpringScript SpringScriptEngine�̃n���h��
@param[in] pszExpression �v�Z��
@param[out] out ���ʂ�������ϐ��̃|�C���^(NULL�s��)
@return ���s�����Ƃ�FALSE�A���������Ƃ���FALSE�ȊO�B
*/
DLLEXPORT BOOL SpringScript_Eval(HSPRINGSCRIPT hSpringScript, const char *pszExpression, int *out);

/*! @brief �������v�Z(float)
SpringScript_Eval�֐��ƈႢ�A���ʂ�float�ŏo�͂��܂��B
@param[in] hSpringScript SpringScriptEngine�̃n���h��
@param[in] pszExpression �v�Z��
@param[out] out ���ʂ�������ϐ��̃|�C���^(NULL�s��)
@return ���s�����Ƃ�FALSE�A���������Ƃ���FALSE�ȊO�B
*/
DLLEXPORT BOOL SpringScript_Evalf(HSPRINGSCRIPT hSpringScript, const char *pszExpression, float *out);

/*! @brief ���߂̈����̐����擾
@param[in] arg CommandArg�\���̂̃|�C���^
@return �����̐�
*/
DLLEXPORT UINT SpringScript_GetCmdArgCount(const CommandArg *arg);

/*! @brief ���߂̈����̌^���擾
@param[in] arg CommandArg�\���̂̃|�C���^
@param[in] index �����̈ʒu
@return VarType�񋓑̂Ŏ����������̌^(���s��:VARTYPE_UNKNOWN)
*/
DLLEXPORT CVariableMan::VarType SpringScript_GetCmdArgType(CommandArg *arg, int index);

/*! @brief ���߂̈����ɃA�N�Z�X�邽�߂̃n���h�����擾
���̊֐����g�p���āASpringScript_GetVarVal�n��SpringScript_SetVarVal�֐��ň����𑀍�ł���悤�ɂȂ�܂��B
@param[in] arg CommandArg�\���̂̃|�C���^
@param[in] index �����̈ʒu
@return HSPRINGSCRIPTVAR�n���h��(���s��:NULL)
*/
DLLEXPORT HSPRINGSCRIPTVAR SpringScript_GetCmdArgPtr(CommandArg *arg, int index);

/*! @brief Integer�^�̕ϐ��̒l���擾
SpringScript_GetCmdArgPtr�֐��Ŏ擾���������̑���ɂ��g�p�ł��܂��B
@param[in] var HSPRINGSCRIPTVAR�n���h��(NULL�s��)
@return �擾�����l
*/
DLLEXPORT INT SpringScript_GetVarValInt(HSPRINGSCRIPTVAR *var);

/*! @brief String�^�̕ϐ��̓��e���擾
SpringScript_GetCmdArgPtr�֐��Ŏ擾���������̑���ɂ��g�p�ł��܂��B
@param[in] var HSPRINGSCRIPTVAR�n���h��(NULL�s��)
@param[out] lpString ������������̂���o�b�t�@�̃|�C���^(NULL�s��)
@param[in] nMaxCount �o�b�t�@�̃T�C�Y
@return ���s�����Ƃ�FALSE�A���������Ƃ���FALSE�ȊO�B
*/
DLLEXPORT BOOL SpringScript_GetVarValStr(HSPRINGSCRIPTVAR *var, char *lpString, int nMaxCount);

/*! @brief String�^�̕ϐ��̓��e�̒������擾
SpringScript_GetCmdArgPtr�֐��Ŏ擾���������̑���ɂ��g�p�ł��܂��B
@param[in] var HSPRINGSCRIPTVAR�n���h��(NULL�s��)
@return ������̒���
*/
DLLEXPORT INT SpringScript_GetVarValStrLen(HSPRINGSCRIPTVAR *var);

/*! @brief Float�^�̕ϐ��̒l���擾
SpringScript_GetCmdArgPtr�֐��Ŏ擾���������̑���ɂ��g�p�ł��܂��B
@param[in] var HSPRINGSCRIPTVAR�n���h��(NULL�s��)
@return �擾�����l
*/
DLLEXPORT FLOAT SpringScript_GetVarValFloat(HSPRINGSCRIPTVAR *var);

/*! @brief Handle�^�̕ϐ��̒l���擾
SpringScript_GetCmdArgPtr�֐��Ŏ擾���������̑���ɂ��g�p�ł��܂��B
@param[in] var HSPRINGSCRIPTVAR�n���h��(NULL�s��)
@return �擾�����l
*/
DLLEXPORT INT SpringScript_GetVarValHnd(HSPRINGSCRIPTVAR *var);

/*! @brief Variable�^(�Q��)�̕ϐ��̒l���擾
SpringScript_GetCmdArgPtr�֐��Ŏ擾���������̑���ɂ��g�p�ł��܂��B
@param[in] var HSPRINGSCRIPTVAR�n���h��(NULL�s��)
@return HSPRINGSCRIPTVAR�n���h��(���s��:NULL)
*/
DLLEXPORT HSPRINGSCRIPTVAR SpringScript_GetVarValRef(HSPRINGSCRIPTVAR *var);

/*! @brief �ϐ��̒l��ݒ�
SpringScript_GetCmdArgPtr�֐��Ŏ擾���������̑���ɂ��g�p�ł��܂��B
@param[in] var HSPRINGSCRIPTVAR�n���h��(NULL�s��)
@param[in] val �l�̃|�C���^
@param[in] type CVariableMan::VarType�񋓑̂Ŏ������ϐ��̌^
@return HSPRINGSCRIPTVAR�n���h��(���s��:NULL)
*/
DLLEXPORT BOOL SpringScript_SetVarVal(HSPRINGSCRIPTVAR *var, const void *val, CVariableMan::VarType type);