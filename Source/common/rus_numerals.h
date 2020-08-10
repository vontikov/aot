#pragma once
#include  "utilit.h"

struct CNumeralToNumber
{
	const char*	m_Cardinal; // "два"
	const char* m_Ordinal; // "второй"
	const char* m_Adverb; // "ВДВОЕМ"
	const char* m_CoollectiveNumber; //  "ДВОЕ"
	const char* m_RomanNumber; // II
	double		m_Number; // 2
    bool        m_bNoun; // false
	const char* m_GenitForm; // "двух" (двухламповый)
};

const size_t NumeralToNumberCount = 42;
const CNumeralToNumber NumeralToNumber[NumeralToNumberCount+1] = 
{
{"ОДИН","ПЕРВЫЙ","","","I",1,false,"ОДНО"}, 
{"ПОЛТОРА","","","","",1.5,false,"ПОЛУТОРО"}, 
{"ДВА","ВТОРОЙ","ВДВОЕМ","ДВОЕ","II",2,false,"ДВУХ"}, 
{"ТРИ","ТРЕТИЙ","ВТРОЕМ","ТРОЕ","III",3,false,"ТРЕХ"}, 
{"ЧЕТЫРЕ","ЧЕТВЕРТЫЙ","ВЧЕТВЕРОМ","ЧЕТВЕРО","IV",4,false,"ЧЕТЫРЕХ"}, 
{"ПЯТЬ","ПЯТЫЙ","ВПЯТЕРОМ","ПЯТЕРО","V",5,false,"ПЯТИ"}, 
{"ШЕСТЬ","ШЕСТОЙ","ВШЕСТЕРОМ","ШЕСТЕРО","VI",6,false,"ШЕСТИ"}, 
{"СЕМЬ","СЕДЬМОЙ","ВСЕМЕРОМ","СЕМЕРО","VII",7,false,"СЕМИ"}, 
{"ВОСЕМЬ","ВОСЬМОЙ","ВВОСЬМЕРОМ","ВОСЬМЕРО","VIII",8,false,"ВОСЕМИ"}, 
{"ДЕВЯТЬ","ДЕВЯТЫЙ","ВДЕВЯТЕРОМ","ДЕВЯТЕРО","IX",9 ,false,"ДЕВЯТИ"}, 
{"ДЕСЯТЬ","ДЕСЯТЫЙ","ВДЕСЯТЕРОМ","ДЕСЯТЕРО","X",10,false,"ДЕСЯТИ"}, 
{"ОДИННАДЦАТЬ","ОДИННАДЦАТЫЙ","ВОДИННАДЦАТЕРОМ","","XI",11,false,"ОДИННАДЦАТИ"}, 
{"ДВЕНАДЦАТЬ","ДВЕНАДЦАТЫЙ", "ВДВЕНАДЦАТЕРОМ","","XII",12,false,"ДВЕНАДЦАТИ"}, 
{"ТРИНАДЦАТЬ","ТРИНАДЦАТЫЙ", "ВТРИНАДЦАТЕРОМ","","XIII",13,false,"ТРИНАДЦАТИ"}, 
{"ЧЕТЫРНАДЦАТЬ","ЧЕТЫРНАДЦАТЫЙ","ВЧЕТЫРНАДЦАТЕРОМ","","XIV",14,false,"ЧЕТЫРНАДЦАТИ"}, 
{"ПЯТНАДЦАТЬ","ПЯТНАДЦАТЫЙ", "ВПЯТНАДЦАТЕРОМ","","XV",15,false,"ПЯТНАДЦАТИ"}, 
{"ШЕСТНАДЦАТЬ","ШЕСТНАДЦАТЫЙ", "ВШЕСТНАДЦАТЕРОМ","","XVI",16,false,"ШЕСТНАДЦАТИ"}, 
{"СЕМНАДЦАТЬ","СЕМНАДЦАТЫЙ", "ВСЕМНАДЦАТЕРОМ","","XVII",17,false,"СЕМНАДЦАТИ"},  
{"ВОСЕМНАДЦАТЬ","ВОСЕМНАДЦАТЫЙ", "ВВОСЕМНАДЦАТЕРОМ","","XIII",18,false,"ВОСЕМНАДЦАТИ"},  
{"ДЕВЯТНАДЦАТЬ","ДЕВЯТНАДЦАТЫЙ", "ВДЕВЯТНАДЦАТЕРОМ","","XIX",19,false,"ДЕВЯТНАДЦАТИ"}, 
{"ДВАДЦАТЬ","ДВАДЦАТЫЙ", "ВДВАДЦАТЕРОМ","","XX",20,false,"ДВАДЦАТИ"}, 
{"ТРИДЦАТЬ","ТРИДЦАТЫЙ", "ВТРИДЦАТЕРОМ","","XXX",30,false,"ТРИДЦАТИ"}, 
{"СОРОК","СОРОКОВОЙ", "","","XL",40,false,"СОРОКА"}, 
{"ПЯТЬДЕСЯТ","ПЯТИДЕСЯТЫЙ", "ВПЯТИДЕСЯТЕРОМ","","L",50,false,"ПЯТИДЕСЯТИ"},  
{"ШЕСТЬДЕСЯТ","ШЕСТИДЕСЯТЫЙ", "ВШЕСТИДЕСЯТЕРОМ","","LX",60,false,"ШЕСТИДЕСЯТИ"}, 
{"СЕМЬДЕСЯТ","СЕМИДЕСЯТЫЙ", "ВСЕМИДЕСЯТЕРОМ","","LXX",70,false,"СЕМИДЕСЯТИ"},  
{"ВОСЕМЬДЕСЯТ","ВОСЬМИДЕСЯТЫЙ", "ВВОСЬМИДЕСЯТЕРОМ","","LXXX",80,false,"ВОСЬМИДЕСЯТИ"}, 
{"ДЕВЯНОСТО","ДЕВЯНОСТЫЙ", "","","XC",90,false,"ДЕВЯНОСТО"}, 
{"СТО","СОТЫЙ","","","C",100,false,"СТО"}, 
{"ДВЕСТИ","ДВУХСОТЫЙ","","","CC",200,false,"ДВУХСОТ"}, 
{"ТРИСТА","ТРЕХСОТЫЙ","","","CCC",300,false,"ТРЕХСОТ"}, 
{"ЧЕТЫРЕСТА","ЧЕТЫРЕХСОТЫЙ","","CD","I",400,false,"ЧЕТЫРЕХСОТ"}, 
{"ПЯТЬСОТ","ПЯТИСОТЫЙ", "","","D",500,false,"ПЯТИСОТ"}, 
{"ШЕСТЬСОТ","ШЕСТИСОТЫЙ", "","","DC",600,false,"ШЕСТИСОТ"}, 
{"СЕМЬСОТ","СЕМИСОТЫЙ", "","","DCC",700,false,"СЕМИСОТ"}, 
{"ВОСЕМЬСОТ","ВОСЬМИСОТЫЙ", "","","DCCC",800,false,"ВОСЬМИСОТ"}, 
{"ДЕВЯТЬСОТ","ДЕВЯТИСОТЫЙ","","","CM",900,false,"ДЕВЯТИСОТ"}, 
{"ТЫСЯЧА","ТЫСЯЧНЫЙ","","","M",1000,true,""}, 
{"МИЛЛИОН","МИЛЛИОННЫЙ","","","",1000000,true,""},
{"МИЛЛИАРД","МИЛЛИАРДНЫЙ","","","",1000000000,true,""},
{"ТРИЛЛИОН","ТРИЛЛИОННЫЙ","","","",1000000000000.0,true,""},
{"КВАДРИЛЛИОН","КВАДРИЛЛИОННЫЙ","","","",1000000000000000.0,true,""},
{"НОЛЬ","НУЛЕВОЙ","","","",0,true,""} //не должен быть включен в NumeralToNumberCount
};


extern const CNumeralToNumber NumeralToNumber[];

extern std::string IntToStr (double i);
extern std::string FindByNumber(QWORD Number);
extern int IsAdverbRule(const std::string& Lemma);
extern double GetCardinalNumeral(const std::string& word);
extern QWORD GetOrdinalNumeral(const std::string& word);
extern QWORD GetNounNumeral(const std::string& word);
extern std::string spellout_number_ru(QWORD x, BYTE IsOrdinal); // 0 - no , 1 - yes, 10 - yes, for thousands

