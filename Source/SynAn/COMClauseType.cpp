// COMClauseType.cpp : Implementation of CCOMClauseType
#include "stdafx.h"
#include "Synan_i.h"
#include "COMClauseType.h"


STDMETHODIMP CCOMClauseType::get_Type(int *pVal)
{
	*pVal = m_pType->m_Type;
	return S_OK;
}


STDMETHODIMP CCOMClauseType::get_RootWordNo(long *pVal)
{
	*pVal = m_pType->m_Root.m_WordNo;
	return S_OK;
}

STDMETHODIMP CCOMClauseType::get_RootHomonymNo(long *pVal)
{
	*pVal = m_pType->m_Root.m_HomonymNo;
	return S_OK;
}

STDMETHODIMP CCOMClauseType::get_Description(BSTR *pVal)
{
	try
	{
		std::string  str;
		if (m_pType->m_Type == UnknownSyntaxElement) 
			if (m_pOptions->m_Language == morphRussian)
				str = _R("ПУСТЫХА");
			else
				str = "EMPTY";
		else
			str = m_pOptions->GetGramTab()->GetClauseNameByType((long)m_pType->m_Type);
		str = convert_to_utf8(str.c_str(), GetOpt()->m_Language);
		*pVal = _bstr_t(str.c_str()).copy();		
	}
	catch(...)
	{
		return E_FAIL;
	}

	return S_OK;
}
