#include "E3DResource.h"

void E3DEngine::Resource::GetFiles(std::string path, std::vector<FileInfo>& files, std::string extension)
{
#ifdef WIN32
	//�ļ����  
	intptr_t   hFile = 0;
	//�ļ���Ϣ  
	_finddata_t fileinfo;
	std::string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//�����Ŀ¼,����֮  
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					GetFiles(p.assign(path).append("\\").append(fileinfo.name), files, extension);
				}
			}
			else
			{
				std::string fileName = fileinfo.name;
				size_t found = fileName.find_last_of(".");
				std::string ext = fileName.substr(found + 1);
				if (extension != ext)
				{
					continue;
				}
				//�������,�����б�  
				FileInfo fInfo(path, fileinfo.name, fileinfo.size);
				files.emplace_back(fInfo);
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
#endif
}

