#pragma once

//�ϐ����Ǘ�����N���X
class CVariableMan
{
public:
	typedef int				neInteger,		*nepInteger;
	typedef std::string		neString,		*nepString;
	typedef float			neFloat,		*nepFloat;
	typedef int				neHandle,		*nepHandle;
	typedef CVariableMan	*neVariable,	**nepVariable;

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

	CVariableMan();
	CVariableMan(const std::string &name);
	CVariableMan(const CVariableMan &obj);
	~CVariableMan();
	
	//�ϐ��ɑ��
	bool Set(const nepInteger pData);
	bool Set(const nepString pData);
	bool Set(const nepFloat pData);
	bool Set(const neVariable pData);
	bool SetAsHandle(const nepHandle pData);

	inline bool Set(neInteger Data) { return Set(&Data); };
	inline bool Set(neFloat Data) { return Set(&Data); };
	inline bool SetAsHandle(neHandle Data) { return Set(&Data); };
	
	const CVariableMan& operator=(const CVariableMan &o);
	const CVariableMan& operator=(const nepInteger &o);
	const CVariableMan& operator=(const nepString &o);
	const CVariableMan& operator=(const nepFloat &o);
	const CVariableMan& operator=(const neVariable &o);
	bool operator==(const CVariableMan &o);
	bool operator!=(const CVariableMan &o);

	//�擾
	void* GetData();
	inline VarType GetType() { return m_type; };
	inline const std::string* GetName() { return &m_sVarName; };

private:
	void Release();

	VarType	m_type;						//�ϐ��̎��
	std::string	m_sVarName;					//�ϐ���

	nepString	m_pData_str = NULL;		//������^�ϐ��p�f�[�^�[
	nepInteger	m_pData_int = NULL;		//�����^�ϐ��p�f�[�^�[
	nepFloat	m_pData_float = NULL;	//�����^�ϐ��p�f�[�^�[
	nepHandle	m_pData_handle = NULL;	//�n���h���^�ϐ��p�f�[�^�[
	neVariable	m_pData_var = NULL;		//�ϐ��Q�ƌ^�p�f�[�^�[
};

#define GetVarPtrFromArg(_arg, _index) ((CVariableMan::neVariable)(_arg).ArgList[_index].GetData())
#define GetIntPtrFromArg(_arg, _index) ((CVariableMan::nepInteger)(_arg).ArgList[_index].GetData())
#define GetFloatPtrFromArg(_arg, _index) ((CVariableMan::nepFloat)(_arg).ArgList[_index].GetData())
#define GetHndPtrFromArg(_arg, _index) ((CVariableMan::nepHandle)(_arg).ArgList[_index].GetData())
#define GetStrPtrFromArg(_arg, _index) ((CVariableMan::nepString)(_arg).ArgList[_index].GetData())
