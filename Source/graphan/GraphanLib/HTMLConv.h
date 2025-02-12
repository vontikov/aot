// ==========  This file is under  LGPL, the GNU Lesser General Public Licence
// ==========  Dialing Graphematical Module (www.aot.ru)
// ==========  Copyright by Alexey Sokirko  and Andrey Kalinin(1996-2001)

#ifndef  HTMLConv_h
#define HTMLConv_h


#include "morph_dict/common/utilit.h"

class HTML 
{
public:
		bool	m_bCollectOffsets;

		HTML()
		{
			m_bCollectOffsets = false;
		};
		std::string GetTextFromHtmlFile(std::string FileName);
		std::string GetTextFromHTMLBuffer(const char* Buffer, size_t BufferLen);
		unsigned long getOffset(unsigned long off);

private:
      bool checkTag(const std::string& str, const char* tag);
      void addOffset(unsigned long);
      
      struct offset_range
      {
		unsigned long low;
		unsigned long high;
		offset_range(unsigned long l = 0L, unsigned long h = 0L) : low(l), high(h){};
      };

      std::vector<offset_range> offsets;
};


#endif
