/*! @file CErrorMan.h
@brief �G���[�L���b�`�p�N���X�B
*/

#pragma once

/*! @param ������ nErrorID
@param ������ nLine
@param ��O���� sFilePath
@param ��l���� pUserData
*/
typedef int(*ErrorCallbk)(int, int, const char*, void*);

#define ERRORID_NONE				0x0000	//!< �G���[�Ȃ�
#define ERRORID_UNKNOWNCOMMAND		0x0001	//!< �s���ȃR�}���h
#define ERRORID_LABELNOTEXIST		0x0002	//!< ���x����������Ȃ�
#define ERRORID_FILENOTEXIST		0x1000	//!< �t�@�C����������Ȃ�
#define ERRORID_PARAMETERILLEGAL	0x2000	//!< �p�����[�^�[���s��
#define ERRORID_PARAMETERNOTENOUGH	0x2001	//!< �p�����[�^�[���s�����Ă���
#define ERRORID_DIVIDEBYZERO		0x2002	//!< 0�ŏ��Z���悤�Ƃ���
#define ERRORID_OUTOFINDEX			0x2003	//!< �C���f�b�N�X�l���͈͊O
#define ERRORID_COULDNTALLOCMEMORY	0xF000	//!< �������̊m�ۂɎ��s
#define ERRORID_UNKNOWNERROR		0xFFFF	//!< �s���ȃG���[

class CErrorMan {
public:
	CErrorMan(HWND hOwnerWnd = NULL);
	bool ChachError(int nErrorID, int nLine, const char *sFilePath, const char* sOptionalText = NULL);

private:
	ErrorCallbk m_pCallBk;
	HWND m_hWnd;		//!< �I�[�i�[�E�B���h�E�n���h��
	void *m_pUserData;	//!< ���[�U�[��`�l

public:
	/*! @brief �G���[���L���b�`�����Ƃ��ɌĂяo�����R�[���o�b�N�֐���ݒ肵�܂��B
	@param[in] pFunc �R�[���o�b�N�֐��ւ̃|�C���^
	@param[in] pUserData ���[�U�[��`�l�@
	*/
	inline void SetCallBk(ErrorCallbk pFunc, void* pUserData) {
		m_pCallBk = pFunc;
		m_pUserData = pUserData;
	};

	//!< �ݒ肵���R�[���o�b�N�֐����擾
	inline ErrorCallbk GetCallBk() {
		return m_pCallBk;
	};

	//! @brief �I�[�i�[�E�B���h�E��ݒ�
	inline void SetOwnerWnd(HWND hOwnerWnd) {
		m_hWnd = hOwnerWnd;
	};

	//! @brief �I�[�i�[�E�B���h�E���擾
	inline HWND GetOwnerWnd() {
		return m_hWnd;
	};
};