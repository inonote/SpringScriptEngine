/*! @file CVariableMan.cpp
@brief 変数を管理するクラス
*/

#include "CVariableMan.h"
using namespace std;
CVariableMan::CVariableMan() {
	m_type = VARTYPE_INTEGER;
	int val = 0;
	Set(&val);
	m_sVarName = "";
}

CVariableMan::CVariableMan(const string &name) {
	m_type = VARTYPE_INTEGER;
	int val = 0;
	Set(&val);
	m_sVarName = name;
}

CVariableMan::CVariableMan(const CVariableMan &obj) {
	m_sVarName = obj.m_sVarName;

	//データー型に合わせて代入
	switch (obj.m_type)
	{
	case CVariableMan::VARTYPE_INTEGER:
		Set(obj.m_pData_int);
		break;
	case CVariableMan::VARTYPE_STRING:
		Set(obj.m_pData_str);
		break;
	case CVariableMan::VARTYPE_FLOAT:
		Set(obj.m_pData_float);
		break;
	case CVariableMan::VARTYPE_HANDLE:
		Set(obj.m_pData_handle);
		break;
	case CVariableMan::VARTYPE_VARIABLE:
		Set(obj.m_pData_var);
		break;
	}
}

CVariableMan::~CVariableMan() {
	Release();
}

bool CVariableMan::Set(const nepInteger pData) {
	Release();
	m_type = VARTYPE_INTEGER;
	m_pData_int = new (nothrow)int;
	if (m_pData_int == NULL)
		return false;
	*m_pData_int = *pData;
	return true;
}

bool CVariableMan::Set(const nepString pData) {
	Release();
	m_type = VARTYPE_STRING;
	m_pData_str = new (nothrow)string;
	if (m_pData_str == NULL)
		return false;
	*m_pData_str = *pData;
	return true;
}

bool CVariableMan::Set(const nepFloat pData) {
	Release();
	m_type = VARTYPE_FLOAT;
	m_pData_float = new (nothrow)float;
	if (m_pData_float == NULL)
		return false;
	*m_pData_float = *pData;
	return true;
}

bool CVariableMan::SetAsHandle(const nepHandle pData) {
	Release();
	m_type = VARTYPE_HANDLE;
	m_pData_handle = new (nothrow)int;
	if (m_pData_handle == NULL)
		return false;
	*m_pData_handle = *pData;
	return true;
}

bool CVariableMan::Set(const neVariable pData) {
	Release();
	m_type = VARTYPE_VARIABLE;
	m_pData_var = pData;
	return true;
}

const CVariableMan& CVariableMan::operator=(const CVariableMan &o) {
	m_sVarName = o.m_sVarName;

	//データー型に合わせて代入
	switch (o.m_type)
	{
	case CVariableMan::VARTYPE_INTEGER:
		Set(o.m_pData_int);
		break;
	case CVariableMan::VARTYPE_STRING:
		Set(o.m_pData_str);
		break;
	case CVariableMan::VARTYPE_FLOAT:
		Set(o.m_pData_float);
		break;
	case CVariableMan::VARTYPE_HANDLE:
		Set(o.m_pData_handle);
		break;
	case CVariableMan::VARTYPE_VARIABLE:
		Set(o.m_pData_var);
		break;
	}
	return *this;
}

const CVariableMan& CVariableMan::operator=(const nepInteger &o) {
	Set(o);
	return *this;
}

const CVariableMan& CVariableMan::operator=(const nepString &o) {
	Set(o);
	return *this;
}

const CVariableMan& CVariableMan::operator=(const nepFloat &o) {
	Set(o);
	return *this;
}


const CVariableMan& CVariableMan::operator=(const neVariable &o) {
	Set(o);
	return *this;
}

bool CVariableMan::operator==(const CVariableMan &o) {
	if (m_type != o.m_type) //型のチェック
		return false;

	//データー型に合わせて比較
	switch (m_type)
	{
	case CVariableMan::VARTYPE_INTEGER:
		return (*m_pData_int == *o.m_pData_int);
		break;
	case CVariableMan::VARTYPE_STRING:
		return (*m_pData_str == *o.m_pData_str);
		break;
	case CVariableMan::VARTYPE_FLOAT:
		return (*m_pData_float == *o.m_pData_float);
		break;
	case CVariableMan::VARTYPE_HANDLE:
		return (*m_pData_handle == *o.m_pData_handle);
		break;
	case CVariableMan::VARTYPE_VARIABLE:
		return (*m_pData_var == *o.m_pData_var);
		break;
	}
	return false;
}

bool CVariableMan::operator!=(const CVariableMan &o) {
	return !(*this == o);
}

void* CVariableMan::GetData() {
	switch (m_type)
	{
	case CVariableMan::VARTYPE_INTEGER:
		return m_pData_int;
		break;
	case CVariableMan::VARTYPE_STRING:
		return m_pData_str;
		break;
	case CVariableMan::VARTYPE_FLOAT:
		return m_pData_float;
		break;
	case CVariableMan::VARTYPE_HANDLE:
		return m_pData_handle;
		break;
	case CVariableMan::VARTYPE_VARIABLE:
		return m_pData_var;
		break;
	}
}

void CVariableMan::Release() {
	switch (m_type)
	{
	case CVariableMan::VARTYPE_INTEGER:
		if (m_pData_int)
			delete m_pData_int;
		m_pData_int = NULL;
		break;
	case CVariableMan::VARTYPE_STRING:
		if (m_pData_str)
			delete m_pData_str;
		m_pData_str = NULL;
		break;
	case CVariableMan::VARTYPE_FLOAT:
		if (m_pData_float)
			delete m_pData_float;
		m_pData_float = NULL;
		break;
	case CVariableMan::VARTYPE_HANDLE:
		if (m_pData_handle)
			delete m_pData_handle;
		m_pData_handle = NULL;
		break;
	case CVariableMan::VARTYPE_VARIABLE:
		m_pData_var = NULL;
		break;
	}
}