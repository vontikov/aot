// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#ifndef stdsynancommon_h
#define stdsynancommon_h


#pragma warning (disable : 4267)

#include "morph_dict/common/util_classes.h"

#define MAX_WORD_LEN                    100

#pragma warning (disable : 4786)
#include "morph_dict/LemmatizerBaseLib/Lemmatizers.h"
#include "morphan/LemmatizerLib/PLMLineCollection.h"

#include "morph_dict/AgramtabLib/agramtab_.h"
#include "dicts/RmlThesLib/Thesaurus.h"
#include "dicts/StructDictLib/Ross.h"


#include <functional>
#include <map>
#include <list>




template<class II, class Pr> inline
	II microsoft_find_if(II F, II L, Pr P)
{
	for (; F != L; ++F)
		if (P(*F))
			break;

	return (F); 
}

typedef  size_t EClauseType;
typedef size_t EGroupType;

#endif
