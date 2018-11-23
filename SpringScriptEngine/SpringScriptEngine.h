#pragma once

#ifdef DECLARE_HANDLE
DECLARE_HANDLE(HSPRINGSCRIPT);
DECLARE_HANDLE(HSPRINGSCRIPTVAR);
DECLARE_HANDLE(HCOMMANDARG);
#else
typedef struct { int unused; } *HSPRINGSCRIPT;
typedef struct { int unused; } *HSPRINGSCRIPTVAR;
typedef struct { int unused; } *HCOMMANDARG;
#endif

#ifndef _DLLEXPORT

#ifndef BOOL
typedef int BOOL;
#endif
#ifndef HWND
typedef struct { int unused; } *HWND;
#endif
#ifndef UINT
typedef unsigned int UINT;
#endif
#ifndef FLOAT
typedef float FLOAT;
#endif
#ifndef INT
typedef int INT;
#endif
#ifndef PostionInfo
typedef struct tagPostionInfo {
	size_t uPos;		//!< �t�@�C���̐擪����̈ʒu
	size_t uLine;		//!< �������̍s
} PostionInfo;
#endif

#endif

//! @brief �ϐ��̌^
enum VarType {
	VARTYPE_INTEGER,	//����
	VARTYPE_STRING,		//������
	VARTYPE_FLOAT,		//����
	VARTYPE_HANDLE,		//�n���h��
	VARTYPE_VARIABLE,	//�ϐ�(�Q��)
	VARTYPE_UNKNOWN,	//�s��
	VARTYPE_LAST,
	VARTYPE_FIRST = VARTYPE_INTEGER
};

/*! �G���[�������ɌĂяo�����R�[���o�b�N�֐�
@param ������ �G���[ID
@param ������ �G���[�����s
@param ��O���� �G���[�����������t�@�C��
@param ��l���� ���[�U�[��`�l
*/
typedef int(*ErrorCallbk)(int, int, const char*, void*);

/*! @brief ���ߎ��s���ɌĂяo�����R�[���o�b�N�֐�
@param ������ ���[�U�[��`�l
@param ������ ������ێ�����HCOMMANDARG�n���h��
*/
typedef int(*CommandCallbk)(void *, HCOMMANDARG);

#ifdef _DLLEXPORT
#define DLLFUNC extern "C" __declspec(dllexport)
#else
#define DLLFUNC extern "C" __declspec(dllimport)
#endif

DLLFUNC HSPRINGSCRIPT SpringScript_CreateInstance();
DLLFUNC BOOL SpringScript_DestroyInstance(HSPRINGSCRIPT hSpringScript);
DLLFUNC BOOL SpringScript_SetErrorCallbk(HSPRINGSCRIPT hSpringScript, HWND hWnd, ErrorCallbk pFunc, void *pUserData);
DLLFUNC BOOL SpringScript_Load(HSPRINGSCRIPT hSpringScript, const char *pszFileName);
DLLFUNC BOOL SpringScript_Run(HSPRINGSCRIPT hSpringScript);
DLLFUNC BOOL SpringScript_Break(HSPRINGSCRIPT hSpringScript);
DLLFUNC BOOL SpringScript_Exit(HSPRINGSCRIPT hSpringScript);
DLLFUNC BOOL SpringScript_RegisterCommand(HSPRINGSCRIPT hSpringScript, const char *pszName, const char *pszPrm, CommandCallbk pFunc, void *pUserData);
DLLFUNC BOOL SpringScript_UnregisterCommand(HSPRINGSCRIPT hSpringScript, const char *pszName);
DLLFUNC BOOL SpringScript_GoTo(HSPRINGSCRIPT hSpringScript, const char *pszLabel);
DLLFUNC BOOL SpringScript_GetPostion(HSPRINGSCRIPT hSpringScript, PostionInfo *pi);
DLLFUNC BOOL SpringScript_GetPostionLH(HSPRINGSCRIPT hSpringScript, PostionInfo *pi);
DLLFUNC BOOL SpringScript_SetPostion(HSPRINGSCRIPT hSpringScript, const PostionInfo *pi);
DLLFUNC BOOL SpringScript_SetPostionForce(HSPRINGSCRIPT hSpringScript, const PostionInfo *pi);
DLLFUNC BOOL SpringScript_GetLineNum(HSPRINGSCRIPT hSpringScript, size_t *pVal);
DLLFUNC BOOL SpringScript_Eval(HSPRINGSCRIPT hSpringScript, const char *pszExpression, int *out);
DLLFUNC BOOL SpringScript_Evalf(HSPRINGSCRIPT hSpringScript, const char *pszExpression, float *out);
DLLFUNC UINT SpringScript_GetCmdArgCount(HCOMMANDARG hArg);
DLLFUNC VarType SpringScript_GetCmdArgType(HCOMMANDARG hArg, int index);
DLLFUNC HSPRINGSCRIPTVAR SpringScript_GetCmdArgPtr(HCOMMANDARG hArg, int index);
DLLFUNC INT SpringScript_GetVarValInt(HSPRINGSCRIPTVAR var);
DLLFUNC BOOL SpringScript_GetVarValStr(HSPRINGSCRIPTVAR var, char *lpString, int nMaxCount);
DLLFUNC INT SpringScript_GetVarValStrLen(HSPRINGSCRIPTVAR var);
DLLFUNC FLOAT SpringScript_GetVarValFloat(HSPRINGSCRIPTVAR var);
DLLFUNC INT SpringScript_GetVarValHnd(HSPRINGSCRIPTVAR var);
DLLFUNC HSPRINGSCRIPTVAR SpringScript_GetVarValRef(HSPRINGSCRIPTVAR var);
DLLFUNC BOOL SpringScript_SetVarVal(HSPRINGSCRIPTVAR var, const void *val, VarType type);