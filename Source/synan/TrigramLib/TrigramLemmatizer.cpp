
#include "TrigramModel.h"
#include "morph_dict/AgramtabLib/RusGramTab.h"
#include "morph_dict/AgramtabLib/EngGramTab.h"
#include "morph_dict/AgramtabLib/GerGramTab.h"
#include "morph_dict/AgramtabLib/rus_consts.h"

#ifdef  USE_TRIGRAM_LEMMATIZER

template <class  T, class Y>
bool InitMorphologySystem(CTrigramModel& M)
{
	std::string langua_str = GetStringByLanguage(M.m_Language);
	M.m_pLemmatizerPrivate = new T;
    
	M.m_pLemmatizerPrivate->LoadDictionariesRegistry();
    M.m_pLemmatizer = M.m_pLemmatizerPrivate;

	M.m_pAgramtabPrivate = new Y;
	M.m_pAgramtabPrivate->LoadFromRegistry();
    M.m_pAgramtab = M.m_pAgramtabPrivate;
	return true;
};


bool CTrigramModel::CheckTagsForFormInfo(const std::vector<CTag>& Tags, const CFormInfo& F) const
{
	uint64_t CommonGrammems = 0;
	m_pAgramtab->GetGrammems(F.GetCommonAncode().c_str(), CommonGrammems);
	std::string SrcAncode = F.GetSrcAncode();
    BYTE SrcPoS = m_pAgramtab->GetPartOfSpeech(SrcAncode.c_str());
    uint64_t Grammems = m_pAgramtab->GetAllGrammems(SrcAncode.c_str());
    return    FindGramTabLineInTags(Tags, 1<<SrcPoS, Grammems | CommonGrammems);
}

bool CTrigramModel::FindGramTabLineInTags(const std::vector<CTag>& Tags, part_of_speech_mask_t Poses, uint64_t AllGrammems) const
{
    for (int j=0; j < Tags.size(); j++)
	{
		if (		(Tags[j].m_Pos != RUSSIAN_PART_OF_SPEECH_COUNT)
				&&	(((1<<Tags[j].m_Pos)&Poses) == 0)
			)
			continue;

		if (Tags[j].m_Pos == PREP) // не проверяем падеж у предлогов
			return true;
		else
			if (Tags[j].m_Grammems != (Tags[j].m_Grammems&AllGrammems))
					continue;

			return true;
	}
    return false;
}

size_t CTrigramModel::GetLemmasCount(std::string Word) const
{
	try 
	{
		std::set<std::string> lemmas;
		std::vector<CFormInfo> Paradigms;
		m_pLemmatizer->CreateParadigmCollection(false, Word, is_upper_alpha((BYTE)Word[0], m_Language), true,  Paradigms);
		for (size_t i=0; i < Paradigms.size(); i++)
			lemmas.insert(Paradigms[i].GetWordForm(0));
		return  lemmas.size();
	}
	catch (...)
	{
		return 0;
	}

}

std::string CTrigramModel::GetParticipleLemma(const CFormInfo& F) const 
{
	std::string SrcAncode = F.GetSrcAncode();
	BYTE SrcPoS = m_pAgramtab->GetPartOfSpeech(SrcAncode.c_str());
	if (SrcPoS != PARTICIPLE) return "";

	uint64_t CommonGrammems = 0;
    m_pAgramtab->GetGrammems(F.GetCommonAncode().c_str(), CommonGrammems);

	
	uint64_t G = m_pAgramtab->GetAllGrammems(SrcAncode.c_str()) | CommonGrammems;
	for (uint16_t k=0; k < F.GetCount(); k++)
	{
		BYTE PoS = m_pAgramtab->GetPartOfSpeech(F.GetAncode(k).c_str());
		if (PoS != PARTICIPLE) continue;
		uint64_t G_curr = m_pAgramtab->GetAllGrammems(F.GetAncode(k).c_str()) | CommonGrammems;
		if (G_curr & _QM(rSingular))
			if (G_curr & _QM(rNominativ))
				if (G_curr & _QM(rMasculinum))
					if ( !(G & rPassiveVoice) || (G_curr & rPassiveVoice))
						if ( !(G & rActiveVoice) || (G_curr & rActiveVoice))
							if ( !(G & rPresentTense) || (G_curr & rPresentTense))
								if ( !(G & rPastTense) || (G_curr & rPastTense))
								{
									return F.GetWordForm(k);
								}
	};
	return "";
}


std::set<std::string> CTrigramModel::GetLemmaSetByTagAndWordStr(std::string Word, std::string TagStr, bool bOnlyWithMaxWeight) const
{
try {
	std::set<std::string> Lemmas;
	std::vector<CTag> Tags = m_TagSet.DecipherTagStr(TagStr,m_pAgramtab);
	//fprintf (stderr, "\t Tags count %i for %s (Tagstr=%s) (tagsetsize = %i)\n",  Tags.size(), Word.c_str(), TagStr.c_str(), m_TagSet.m_Tags.size());
	std::vector<CFormInfo> Paradigms;
	m_pLemmatizer->CreateParadigmCollection(false, Word, is_upper_alpha((BYTE)Word[0], m_Language), true, Paradigms);
	int MaxWeight = 0;
	int MaxWeightForAnyLemma = 0;
	std::string MostHeavyLemma;

	for (int i=0; i < Paradigms.size(); i++)
	{
		const CFormInfo& F = Paradigms[i];
		if (MaxWeightForAnyLemma  < F.GetWordWeight())
		{
			MostHeavyLemma = F.GetWordForm(0);
			MaxWeightForAnyLemma  =  F.GetWordWeight();
		}
		if (CheckTagsForFormInfo(Tags, F))
		{
			if (bOnlyWithMaxWeight)
			{
				if (F.GetWordWeight() > MaxWeight)
				{
					MaxWeight = F.GetWordWeight();
					Lemmas.clear();
					Lemmas.insert(F.GetWordForm(0));
				}
				else
					if (F.GetWordWeight() == MaxWeight)
						Lemmas.insert(F.GetWordForm(0));
			}
			else
			{
				Lemmas.insert(F.GetWordForm(0));
				std::string  ParticipleLemma = GetParticipleLemma(F);
				if (!ParticipleLemma.empty())
					Lemmas.insert(ParticipleLemma);
			}

		}

	};

	if (Lemmas.empty())
	{
		if (bOnlyWithMaxWeight && (MostHeavyLemma != ""))
		{
			// вызывается только из lemmatize_sentence, поэтому выдаем самую тяжелую лемму
			Lemmas.insert(MostHeavyLemma);
		}
		else
		{
			if (		(TagStr != "UNK") 
					&& CheckLanguage(Word, m_Language)
					&& ((BYTE)Word[0] != '-')
				)
				fprintf (stderr, "\tword form %s is not recognized by the lemmatizer with tag %s (probably different tag sets!)\n",  Word.c_str(), TagStr.c_str());
			std::string s = Word;
			RmlMakeUpper(s, m_Language);
			Lemmas.insert(s);
		}
	}

	return Lemmas;
}

catch (...)
{
	fprintf (stderr, "exception in CTrigramModel::GetLemmaSetByTagAndWordStr\n");
	return std::set<std::string>();
}

}

bool CTrigramModel::CheckLemma(std::string Word, std::string TagStr1, std::string TagStr2) const
{
try {
	if (TagStr1 == TagStr2) return true;

	std::set<std::string> Lemmas1 = GetLemmaSetByTagAndWordStr(Word, TagStr1, false);
	std::set<std::string> Lemmas2 = GetLemmaSetByTagAndWordStr(Word, TagStr2, false);

	std::vector<std::string> Out(std::min(Lemmas1.size(),Lemmas2.size()));
	std::vector<std::string>::iterator it = set_intersection(Lemmas1.begin(), Lemmas1.end(), Lemmas2.begin(), Lemmas2.end(),Out.begin());

	return it !=  Out.begin();
}
catch (...)
{
	fprintf (stderr, "exception in CTrigramModel::CheckLemma\n");
	return false;
}
};


bool CTrigramModel::InitDicts () 
{
	if (m_pLemmatizerPrivate) delete m_pLemmatizerPrivate;
	if (m_pAgramtabPrivate) delete m_pAgramtabPrivate;
	switch (m_Language)
	{
		case morphRussian: 
					if (!InitMorphologySystem<CLemmatizerRussian, CRusGramTab>(*this)) return false; break;
		case morphEnglish : 
					if (!InitMorphologySystem<CLemmatizerEnglish, CEngGramTab>(*this)) return  false;break;
		case morphGerman: 
					if (!InitMorphologySystem<CLemmatizerGerman, CGerGramTab>(*this)) return  false;break;
		default: return false;;
	};


	m_Graphan.m_Language = m_Language;
	

	if (!m_Graphan.LoadDicts())
	{
		fprintf (stderr,"Cannot load graphan dictionaries \n");
		return 1;
	};

	m_Graphan.m_bUseParagraphTagToDivide = true;
	m_Graphan.m_bUseIndention = false;
	m_Graphan.m_MaxSentenceLength = 1000;

	return true;
	
};


void  CTrigramModel::get_tags_from_lemmatizer_but_not_preps(const std::string& WordStr, std::set<uint16_t>& tags) const
{
	if (!CheckLanguage(WordStr,m_Language)) return; 
	std::vector<CFormInfo> FormInfos;
	std::string  _WordStr = WordStr;
	if (!m_pLemmatizer->CreateParadigmCollection(false, _WordStr, is_upper_alpha(WordStr[0],m_Language), true, FormInfos) ) return;
	
	for (size_t i=0; i < FormInfos.size(); i++)
	{
		if (		(WordStr.size() == 1) 
				&&	!FormInfos[i].m_bFound
			) // игнорируем однобуквенные предсказанные слова
			continue;
		uint64_t CommonGrammems = 0;
        m_pAgramtab->GetGrammems(FormInfos[i].GetCommonAncode().c_str(), CommonGrammems);
		std::string  Ancodes = FormInfos[i].GetSrcAncode();
		std::string Lemma = FormInfos[i].GetWordForm(0);
		for (size_t j=0; j < Ancodes.size(); j+=2)
		{
			// игнорируем предлоги 
			if (m_pAgramtab->GetPartOfSpeech(Ancodes.c_str()+j) == PREP)
				continue;
			// получаем тег по анкоду 
			const CTag& tag  = m_TagSet.GetTagFromAncode(m_pAgramtab, Ancodes.c_str()+j, CommonGrammems, Lemma);
            if (!tag.IsEmptyTag())
            {
			    std::string TagStr = tag.m_Name;
			    int TagNo = find_tag(TagStr);
			    if (TagNo != UnknownTag)
			    {
					    tags.insert(TagNo);
			    }
			    else
                    if (!m_bQuiet)
				        fprintf (stderr, "Bad tag %s (ancode = %s) for %s\n",TagStr.c_str(), Ancodes.c_str()+j, WordStr.c_str());
            }
            else
            {
                // morph_dict contains interpretation that cannot be mapped to this tag set, for example some brand-new part of speech
            }
		}
	};
};


bool CTrigramModel::tag_sentence(char* SentenceStr) const
{
	std::vector<std::string> words;
	std::vector<std::string> lemmas;
	std::vector<CWordIntepretation> tags;

	StringTokenizer tok(SentenceStr, " \t\r\n");
	while (tok())
	{
		std::string Token = tok.val();
		std::string Lemma = "";
		size_t index = Token.find('{');
		if (index > 0)
		{
			if (Token[Token.length()-1] != '}')
			{
				if (!m_bQuiet)
					fprintf (stderr,"No close curly bracket for %s\n",Token.c_str());
				return false;
			}
			Lemma = Token.substr(index+1, Token.length() - index - 2);
			RmlMakeUpper(Lemma, m_Language);
			Token = Token.substr(0, index);
		}

		words.push_back(Token);
		lemmas.push_back(Lemma);
	}

	if (!viterbi(words, tags))
		return false;

	for (size_t i=0; i<words.size(); i++)
	{
		if (i>0) 
			fprintf(stderr, " ");

		fprintf(stderr, "%s %s", words[i].c_str(), m_RegisteredTags[tags[i].m_TagId1].c_str());
		if (!lemmas[i].empty())
		{
			std::set<std::string> Lemmas1 = GetLemmaSetByTagAndWordStr(words[i], m_RegisteredTags[tags[i].m_TagId1], false);
			
			if (		Lemmas1.find(lemmas[i]) == Lemmas1.end()
					&&	!(Lemmas1.find(_R("ТОТ"))!= Lemmas1.end() && (lemmas[i] == _R("ТО")))
				)
			{
				
				fprintf(stderr, "\n<lemma_diff: %s != ", lemmas[i].c_str());
				for (std::set<std::string>::const_iterator it = Lemmas1.begin(); it != Lemmas1.end(); it++)
				{
					fprintf(stderr, " %s ", it->c_str());
				}
				fprintf(stderr, ">\n");
			}
		}

		if (tags[i].m_TagId2 != UnknownTag)
            fprintf(stderr, "[%s]", m_RegisteredTags[tags[i].m_TagId2].c_str());
	};

	fprintf(stderr, "\n");

	return true;
}


bool CTrigramModel::lemmatize_sentence(std::vector<std::string> words, std::vector<std::string>& lemmas) const
{
	std::vector<CWordIntepretation> tags;

	if (!viterbi(words, tags))
		return false;

	lemmas.clear();
	for (size_t i=0; i<words.size(); i++)
	{

		std::set<std::string> Lemmas = GetLemmaSetByTagAndWordStr(words[i], m_RegisteredTags[tags[i].m_TagId1], true);
		if (Lemmas.empty())
		{
			if (!m_bQuiet)
				fprintf(stderr, "Empty lemma std::set for \"%s\" \n", words[i].c_str());
			std::string UpperWord = words[i];
			RmlMakeUpper(UpperWord, morphRussian);
			lemmas.push_back(words[i]);
		}
		lemmas.push_back(*Lemmas.begin());
	};


	return true;
}

bool IsToken(const CGraLine& L)
{
	return !L.IsNotPrint()  && !L.IsSoft();
};

bool CheckWhetherUkranian(const std::vector<std::string>& tokens)
{
	for (size_t LineNo=1; LineNo < tokens.size(); LineNo++)
	{
		if ((tokens[LineNo] == "ї") || (tokens[LineNo] == "Ѕ"))
			return true;
	}
	return false;
}

bool CTrigramModel::TagRawText(std::string FileName)
{
	try {
		m_Graphan.LoadFileToGraphan(FileName);
	}
	catch (CExpc e)
	{
		std::cerr << "Cannot load file " << FileName << " ,exception: " << e.m_strCause << "\n";
		return false;
	};
	fprintf (stderr,"load file %s \n", FileName.c_str());

	size_t TokensCount = m_Graphan.GetUnits().size();
	std::vector<std::string> tokens;
	std::vector<CWordIntepretation> tags;
	

	for (uint32_t LineNo=1; LineNo < TokensCount; LineNo++)
	{
		tokens.clear();
		tags.clear();
		uint32_t LineNoEnd=LineNo;
		for (; LineNoEnd < TokensCount; LineNoEnd++)
		{
			if (IsToken(m_Graphan.GetUnit(LineNoEnd)) )
			{
				std::string TokenStr = m_Graphan.GetToken(LineNoEnd);
				assert (!TokenStr.empty());
				tokens.push_back( TokenStr  );
			}

			if (m_Graphan.HasDescr(LineNoEnd, OSentEnd)) 
					break;
		}
		if (tokens.size() > 200)
		{
			fprintf(stderr,"skipping sentence with %i words \n", tokens.size());
			continue;
		}
		if (CheckWhetherUkranian(tokens))
		{
			fprintf (stderr,"skip Ukranian sentence\n");
			continue;
		}

		if (!viterbi(tokens, tags))
			return false;

		for (size_t i=0; i<tokens.size(); i++)
		{
			printf("%s %s\n", tokens[i].c_str(), m_RegisteredTags[tags[i].m_TagId1].c_str());
            if (tags[i].m_TagId2 != UnknownTag)
                if (m_bUseSecondLocalMax)
                    printf("\t%s %s\n", tokens[i].c_str(), m_RegisteredTags[tags[i].m_TagId2].c_str());
		}

		LineNo = LineNoEnd;
	}
	return true;
}


bool CTrigramModel::TagRawTexts(std::string FileName) 
{
	if (	(FileName.length() > 3)
				&&	(			(FileName.substr(FileName.length()-3) == "con")
						||		(FileName.substr(FileName.length()-3) == "lst")
					)
				)
	{
		FILE* fp = fopen(FileName.c_str(), "r");  
		if (!fp) 
		{
			fprintf (stderr, "cannot open %s\n", FileName.c_str());
			return false;
		};
		char buffer[10000];
		while (fgets(buffer, 10000, fp))
		{	
			rtrim(buffer);
			if (strlen(buffer) > 0)
				if (!TagRawText(buffer))
					return false;
			
		}
		fclose (fp);
		return true;
	}
	else
		return TagRawText(FileName);

}
/*
bool CTrigramModel::ConvertTagToMyStem(const CTag& tag, std::set<std::string>& Grammems, CMorphInterpretation& NewInterp) const
{
	std::string pos =  m_pAgramtab->GetPartOfSpeechStr(tag.m_Pos);
	std::string mystem_pos  = POS_Dialing2MyStem(pos);
	if (mystem_pos.empty())
	{
		if (!m_bQuiet)
			fprintf (stderr, "cannot convert part of speech \"%s\" to mystem\n",pos.c_str());
		return false;
	}
	for (int i = 0; i <sizeof(tag.m_Grammems)*8; i++)
	{
		if (_QM(i) & tag.m_Grammems)
		{
			std::string gr = Grammem_Dialing2MyStem(m_pAgramtab->GetGrammemStr(i));
			if (gr.empty())
			{
				if (!m_bQuiet)
					fprintf (stderr, "cannot convert grammem \"%s\" to mystem\n",m_pAgramtab->GetGrammemStr(i));
				return false;
			}
			Grammems.insert(gr);
		}
	}
	NewInterp.m_Pos = mystem_pos;
	NewInterp.m_Gr = "";	
	for (std::set<std::string>::const_iterator it = Grammems.begin(); it != Grammems.end(); it++)
	{
		NewInterp.m_Gr += *it;
		NewInterp.m_Gr += ",";
	}
	return true;
}

std::set<std::string> GetMystemSpecificGrammem(const std::vector<CMorphInterpretation>& v)
{
	std::vector<std::string> Result;
	for (size_t i=0; i < v.size(); i++)
	{
		StringTokenizer tok(v[i].m_Gr.c_str(), ", =");
		std::vector<std::string> s;
		while (tok())
		{
			std::string g = tok.val();
			if (IsMyStemSpecificGrammem(g))
				s.push_back(g);
		}
		if (i == 0)
			Result.swap(s);
		else
		{
			std::vector<std::string> aux;
			aux.resize(std::min(Result.size(),s.size()));
			std::vector<std::string>::iterator it = set_intersection(Result.begin(),Result.end(), 
				s.begin(),s.end(),aux.begin());
			aux.resize(it-aux.begin());
			Result.swap(aux);
		}
	}
	std::set<std::string> UltimateResult(Result.begin(), Result.end());
	return UltimateResult;
}
*/

//#pragma optimize( "", on)

std::vector<std::string> CTrigramModel::get_tokens_from_graphan(std::string InputStr)
{
	std::vector<std::string> tokens;
	m_Graphan.LoadStringToGraphan(InputStr);
	size_t TokensCount = m_Graphan.GetUnits().size();
	
	
	
	for (uint32_t LineNo=1; LineNo < TokensCount; LineNo++)
	{
		if (IsToken(m_Graphan.GetUnit(LineNo)) )
		{
			std::string TokenStr = m_Graphan.GetToken(LineNo);
			tokens.push_back( TokenStr  );
		}
	}
	return tokens;
}

bool CTrigramModel::tagging_string_with_end_of_sents(std::string InputStr, std::string& Result) 
{
	Result = "";
	m_Graphan.LoadStringToGraphan(InputStr);
	size_t TokensCount = m_Graphan.GetUnits().size();
	std::vector<std::string> tokens;
	std::vector<CWordIntepretation> tags;
	
	for (uint32_t LineNo=1; LineNo < TokensCount; LineNo++)
	{
		tokens.clear();
		tags.clear();
		uint32_t LineNoEnd=LineNo;
		for (; LineNoEnd < TokensCount; LineNoEnd++)
		{
			if (IsToken(m_Graphan.GetUnit(LineNoEnd)) )
			{
				std::string TokenStr = m_Graphan.GetToken(LineNoEnd);
				assert (!TokenStr.empty());
				tokens.push_back( TokenStr  );
			}

			if (m_Graphan.HasDescr(LineNoEnd, OSentEnd)) 
					break;
		}
		if (tokens.size() > 200)
		{
			Result = fprintf(stderr,"skipping sentence with %i words \n", tokens.size());
			continue;
		}
		if (!viterbi(tokens, tags))
			return false;

		for (size_t i=0; i<tokens.size(); i++)
			Result += Format("%s %s\n", tokens[i].c_str(), m_RegisteredTags[tags[i].m_TagId1].c_str());

		LineNo = LineNoEnd;
	}
	return true;
}

//  ------------------------------------------------------------ 
bool CTrigramModel::tagging(std::string FileName) 
{
	if (m_bRawTexts)
		return TagRawTexts(FileName);

	FILE* fp = fopen(FileName.c_str(), "r");  
	if (!fp) 
	{
		fprintf (stderr, "cannot open %s\n", FileName.c_str());
		return false;
	};
	
	char buffer[10000];
	while (fgets(buffer, 10000, fp))
	{ 
		if (!tag_sentence(buffer))
			return false;
	}
	

	fclose (fp);
	return true;
}

bool CTrigramModel::print_disamb_lemmas(std::vector<std::string> words) const
{	
	std::vector<CWordIntepretation> tags;	
	if (!viterbi(words, tags))		
		return false;	
	for (size_t i=0; i<words.size(); i++)	
	{		
		printf ("%s",words[i].c_str());		
		//if (GetLemmasCount(words[i]) > 1)		
		if (is_russian_alpha(words[i][0]))
		{			
			std::set<std::string> Lemmas = GetLemmaSetByTagAndWordStr(words[i], m_RegisteredTags[tags[i].m_TagId1], true);			
			std::string lemm = words[i];			
			if (!Lemmas.empty())			  
				lemm  = *Lemmas.begin();			
			EngRusMakeLower(lemm);			
			printf ("(%s)", lemm.c_str());		
		}		
		printf (" ");	
	};	
	printf ("\n");	
	return true;
}

bool CTrigramModel::lemmatize_file(std::string FileName) 
{
	FILE* fp = fopen(FileName.c_str(), "r");  
	if (!fp) 
	{
		fprintf (stderr, "cannot open %s\n", FileName.c_str());
		return false;
	};
	
	char buffer[10000];
	while (fgets(buffer, 10000, fp))
	{ 
		if (!print_disamb_lemmas(get_tokens_from_graphan(buffer)))
			return false;
	}
	

	fclose (fp);
	return true;

}



#endif





