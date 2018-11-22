#pragma once
typedef int(*ErrorCallbk)(int, int, const char*, void*);

class CErrorMan {
public:
	CErrorMan(HWND hOwnerWnd = NULL);
	inline void SetWindow(HWND hOwnerWnd = NULL) { m_hWnd = hOwnerWnd; };
	bool ChachError(int nErrorID, int nLine, const char *sFilePath, const char* sOptionalText = NULL);

#define ERRORID_NONE				0x0000	// �G���[�Ȃ�
#define ERRORID_UNKNOWNCOMMAND		0x0001	// �s���ȃR�}���h
#define ERRORID_LABELNOTEXIST		0x0002	// ���x����������Ȃ�
#define ERRORID_FILENOTEXIST		0x1000	// �t�@�C����������Ȃ�
#define ERRORID_PARAMETERILLEGAL	0x2000	// �p�����[�^�[���s��
#define ERRORID_PARAMETERNOTENOUGH	0x2001	// �p�����[�^�[���s�����Ă���
#define ERRORID_DIVIDEBYZERO		0x2002	// 0�ŏ��Z���悤�Ƃ���
#define ERRORID_OUTOFINDEX			0x2003	// �C���f�b�N�X�l���͈͊O
#define ERRORID_COULDNTALLOCMEMORY	0xF000	// �������̊m�ۂɎ��s
#define ERRORID_UNKNOWNERROR		0xFFFF	// �s���ȃG���[
	
private:
	ErrorCallbk m_pCallBk;
	HWND m_hWnd;
	void *m_pUserData;

public:
	inline void SetCallBk(ErrorCallbk pFunc, void* pUserData) {
		m_pCallBk = pFunc;
		m_pUserData = pUserData;
	};
	inline ErrorCallbk GetCallBk() {
		return m_pCallBk;
	};
	inline void SetOwnerWnd(HWND hOwnerWnd) {
		m_hWnd = hOwnerWnd;
	};
	inline HWND GetOwnerWnd() {
		return m_hWnd;
	};
};