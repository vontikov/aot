#pragma once 

#include "Resource.h"       // main symbols
#include "Synan_i.h"

class CSyntaxOpt;
class CGroup;

class ATL_NO_VTABLE CCOMGroup : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCOMGroup, &CLSID_Group>,
	public IDispatchImpl<IGroup, &IID_IGroup, &LIBID_SYNANLib>
{
public:
	CCOMGroup() {};

	~CCOMGroup() {};


DECLARE_REGISTRY_RESOURCEID(IDR_GROUP)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CCOMGroup)
	COM_INTERFACE_ENTRY(IGroup)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IGroup
public:
	STDMETHOD(get_Grammems)(/*[out, retval]*/ hyper *pVal);
	STDMETHOD(get_MainGroupLastWord)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_MainGroupFirstWord)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_LastWord)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_FirstWord)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_TypeStr)(/*[out, retval]*/ BSTR* pVal);
	STDMETHOD(get_MainWord)(/*[out, retval]*/ long *pVal);


	BOOL Init(const CGroup* pGroup, const CSyntaxOpt* pOptions);

protected:
	const CGroup* m_pGroup;
	const CSyntaxOpt*		m_pOptions;
	const CSyntaxOpt* GetOpt() const;
};


