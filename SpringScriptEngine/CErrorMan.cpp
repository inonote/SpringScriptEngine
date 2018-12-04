/*! @file CErrorMan.cpp
@brief �G���[�L���b�`�p�N���X�B
*/

#include "CErrorMan.h"

CErrorMan::CErrorMan(HWND hOwnerWnd) :
	m_pUserData(NULL),
	m_pCallBk(NULL)
{
	m_hWnd = hOwnerWnd;
}

/*! @brief �G���[���L���b�`����֐�
@param[in] nErrorID �G���[ID(ERRORID_*)
@param[in] nLine �G���[�����s
@param[in] sFilePath �G���[�����������t�@�C��
@param[in] sOptionalText �I�v�V�����e�L�X�g(NULL��)
@return ���true
@note �G���[���b�Z�[�W�\����ASetCallBk�֐��œo�^�����R�[���o�b�N�֐����Ăяo�����̂ŁA�����Ńv���O�������I�����邩�̔�����s���B
*/

bool CErrorMan::ChachError(int nErrorID, int nLine, const char *sFilePath, const char *sOptionalText){
	char sErrorMsg[0x200];
	char sBuf[0x300];

	switch(nErrorID)
	{
	case ERRORID_UNKNOWNCOMMAND:
		strcpy_s(sErrorMsg, 0x200, "�s���ȃR�}���h�ł��B");
		break;
	case ERRORID_LABELNOTEXIST:
		strcpy_s(sErrorMsg, 0x200, "���x����������܂���B");
		break;
	case ERRORID_FILENOTEXIST:
		strcpy_s(sErrorMsg, 0x200, "�t�@�C����������܂���B");
		break;
	case ERRORID_PARAMETERILLEGAL:
		strcpy_s(sErrorMsg, 0x200, "�������s���ł��B");
		break;
	case ERRORID_PARAMETERNOTENOUGH:
		strcpy_s(sErrorMsg, 0x200, "�������s�����Ă��܂��B");
		break;
	case ERRORID_DIVIDEBYZERO:
		strcpy_s(sErrorMsg, 0x200, "0�ŏ��Z���悤�Ƃ��܂����B");
		break;
	case ERRORID_OUTOFINDEX:
		strcpy_s(sErrorMsg, 0x200, "�C���f�b�N�X�l���͈͊O�ł��B");
		break;
	case ERRORID_COULDNTALLOCMEMORY:
		strcpy_s(sErrorMsg, 0x200, "�������̊m�ۂɎ��s���܂����B");
		break;
	case ERRORID_UNKNOWNERROR:
		strcpy_s(sErrorMsg, 0x200, "�����s���̃G���[���������܂����B");
		break;
	}
	if (sOptionalText)
		sprintf_s(sBuf, 0x300, "%s\n#%d �s:%d(%s)\n\n�ڍ�:\n%s", sErrorMsg, nErrorID, nLine + 1, sFilePath, sOptionalText);
	else
		sprintf_s(sBuf, 0x300, "%s\n#%d �s:%d(%s)\n\n", sErrorMsg, nErrorID, nLine+1, sFilePath);

#ifdef _SSE_WINDOWS
	MessageBox(m_hWnd, sBuf, "Error", MB_ICONERROR);
#else
	printf("%s", sBuf);
#endif // _SSE_WINDOWS

	if (m_pCallBk) {
		m_pCallBk(nErrorID, nLine, sFilePath, m_pUserData); //�G���[������p�̃R�[���o�b�N�֐����Ăяo��
	}
	return true;
}