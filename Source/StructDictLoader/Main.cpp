#include "../common/utilit.h"
#include "../StructDictLib/Ross.h"
#include "../StructDictLib/TempArticle.h"

void PrintUsage()
{
	std::cout << "StructDict Loader\n";
	std::cout << "This program converts text version of structural dictionary to binary\n";
	std::cout << "Usage: StructDictLoader <ToTxt|FromTxt> <Inputfile> <OutputCatalog>\n";
	std::cout << "Example: StructDictLoader ToTxt ross.txt c:/rml/dicts/ross \n";
	exit(-1);
};

void export_dict(std::string fileName, std::string folder) {
	CDictionary Dict;
	if (!Dict.Load(folder.c_str()) || !Dict.ReadUnitComments())
	{
		throw CExpc("Cannot load dictionary from %s", folder.c_str());
	};
	std::ofstream outf (fileName, std::ios::binary);
	if (!outf.is_open())
	{
		throw CExpc("Cannot write to %s", fileName.c_str());
	};

	CTempArticle A;
	A.m_pRoss = &Dict;

	for (WORD i = 0; i < Dict.m_Units.size(); i++)
	{
		outf << "============\n" << convert_to_utf8(Dict.GetUnitTextHeader(i), Dict.m_Language);
		try
		{
			A.ReadFromDictionary(i, false, true);
			if (!A.ArticleToText())
			{
				throw CExpc("Error! Cannot get the entry No %i", i);
			};
			outf << convert_to_utf8(A.GetArticleStr(), Dict.m_Language);
		}
		catch (...)
		{
			throw CExpc("Error! Cannot get the entry No %i", i);
		}

	};
}

void import_dict(std::string fileName, std::string folder) {
	CDictionary Dict;
	if (!FileExists(fileName.c_str()))
	{
		throw CExpc("Cannot read %s", fileName.c_str());
	};

	if (!Dict.LoadOnlyConstants(folder.c_str()))
	{
		throw CExpc("Cannot load an empty dictionary from %s", folder.c_str());
	};
	Dict.m_bShouldSaveComments = true;

	std::string Messages;
	if (!Dict.ImportFromText(fileName, false, iceSkip, 1, Messages)) {
		throw CExpc(Messages);
	}
	Dict.Save();
}

int main(int argc, char** argv)
{
	if (argc != 4) {
		PrintUsage();
	}

	std::string Action = argv[1];
	if ((Action != "ToTxt") && (Action != "FromTxt")) {
		PrintUsage();
	}

	try {
		if (Action == "FromTxt")
		{
			import_dict(argv[2], argv[3]);
		}
		else
		{
			export_dict(argv[2], argv[3]);
		};
	}
	catch (CExpc c)
	{
		std::cerr << c.m_strCause << std::endl;
	}
	catch (const std::exception& ex)
	{
		std::cerr << ex.what() << std::endl;
	}



}
