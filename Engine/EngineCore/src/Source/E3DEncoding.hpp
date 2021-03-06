﻿//
//  E3DEncodine.hpp
//
//  Created by 潘庆云 on 2017/5/17.
//

#ifndef __E3D_ENCODING_HPP__
#define __E3D_ENCODING_HPP__

#include "Interface.h"

namespace E3DEngine
{
	class Encoding : public IObject
	{
	public:
		static bool IsNumber(const char * cstring)
		{
			if (strcmp(cstring, "true") == 0 || strcmp(cstring, "false") == 0)
			{
				return true;
			}
			DWORD pointNumber = 0;
			DWORD start = ((cstring[0] == '+') || (cstring[0] == '-')) ? 1 : 0;
			for (DWORD i = start; i < strlen(cstring); i++)
			{
				if ((cstring[i] < '0') || (cstring[i] > '9'))
				{
					if (cstring[i] == '.')
					{
						++pointNumber;
					}
					else
					{
						return false;
					}
				}
			}
			
			if (pointNumber > 1)
			{
				return false;
			}
			
			return true;
		}
	protected:
	private:
	};
}
#endif /* E3DEncodine_hpp */
