#pragma once

//変数を管理するクラス
class CVariableMan
{
public:
	typedef int				neInteger,		*nepInteger;
	typedef std::string		neString,		*nepString;
	typedef float			neFloat,		*nepFloat;
	typedef int				neHandle,		*nepHandle;
	typedef CVariableMan	*neVariable,	**nepVariable;

	enum VarType {
		VARTYPE_INTEGER,	//整数
		VARTYPE_STRING,		//文字列
		VARTYPE_FLOAT,		//実数
		VARTYPE_HANDLE,		//ハンドル
		VARTYPE_VARIABLE,	//変数(参照)
		VARTYPE_UNKNOWN,	//不明
		VARTYPE_LAST,
		VARTYPE_FIRST = VARTYPE_INTEGER
	};

	CVariableMan();
	CVariableMan(const std::string &name);
	CVariableMan(const CVariableMan &obj);
	~CVariableMan();
	
	//変数に代入
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

	//取得
	void* GetData();
	inline VarType GetType() { return m_type; };
	inline const std::string* GetName() { return &m_sVarName; };

private:
	void Release();

	VarType	m_type;						//変数の種類
	std::string	m_sVarName;					//変数名

	nepString	m_pData_str = NULL;		//文字列型変数用データー
	nepInteger	m_pData_int = NULL;		//整数型変数用データー
	nepFloat	m_pData_float = NULL;	//実数型変数用データー
	nepHandle	m_pData_handle = NULL;	//ハンドル型変数用データー
	neVariable	m_pData_var = NULL;		//変数参照型用データー
};

#define GetVarPtrFromArg(_arg, _index) ((CVariableMan::neVariable)(_arg).ArgList[_index].GetData())
#define GetIntPtrFromArg(_arg, _index) ((CVariableMan::nepInteger)(_arg).ArgList[_index].GetData())
#define GetFloatPtrFromArg(_arg, _index) ((CVariableMan::nepFloat)(_arg).ArgList[_index].GetData())
#define GetHndPtrFromArg(_arg, _index) ((CVariableMan::nepHandle)(_arg).ArgList[_index].GetData())
#define GetStrPtrFromArg(_arg, _index) ((CVariableMan::nepString)(_arg).ArgList[_index].GetData())
