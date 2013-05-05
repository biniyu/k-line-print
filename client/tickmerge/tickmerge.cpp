// tickmerge.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "direct.h"
#include "io.h"
#include <vector>
#include <string>

using namespace std;

#define DATA_DIR	"C:\\Users\\Oscar\\Documents\\FutureData\\ZZ201212"
#define VARIETY		"TA"

typedef struct _tagstuFileNameAndSize
{
	string name;
	int size;
} stuFileNameAndSize;

vector<stuFileNameAndSize> GetFiles(string dir, string filter, bool bDir)
{
	vector<stuFileNameAndSize> vecFiles;
	
	if(_chdir(dir.c_str())) return vecFiles;
	else
	{
		long hFile;
		_finddata_t fileinfo;
		
		if((hFile = _findfirst(filter.c_str(), &fileinfo))!= -1)
		{
			do
			{
				if(!strcmp(".", fileinfo.name)) continue;
				if(!strcmp("..", fileinfo.name)) continue;
				
				if((bDir && (fileinfo.attrib & _A_SUBDIR))
					|| (!bDir && !(fileinfo.attrib & _A_SUBDIR)))
				{
					stuFileNameAndSize tmp;

					tmp.name = fileinfo.name;
					tmp.size = fileinfo.size;
					
					vecFiles.push_back(tmp);
				}
				
			} while(_findnext(hFile, &fileinfo) == 0);
			
			_findclose(hFile);
		}
	}
	
	return vecFiles;
}

int _tmain(int argc, _TCHAR* argv[])
{
	vector<stuFileNameAndSize> dirlist = GetFiles(DATA_DIR, "*.*", true);

	FILE* pMerged = fopen("merged.csv", "w");

	for(int i = 0; i < dirlist.size(); i++)
	{
		int maxsize = 0, maxsizeidx;

		printf("dir %s\n", dirlist[i].name.c_str());

		vector<stuFileNameAndSize> filelist = GetFiles(string(DATA_DIR) + "\\" + dirlist[i].name, "TA*.csv", false);

		for(int j = 0; j < filelist.size(); j++)
		{

			if(filelist[j].name.find("MI") != string::npos
				|| filelist[j].name.find("PI") != string::npos
				||filelist[j].name.find("VI") != string::npos) continue;

			if(filelist[j].size > maxsize)
			{
				maxsize = filelist[j].size;
				maxsizeidx = j;
			}

//			printf("file %s size %d\n", filelist[j].name.c_str(), filelist[j].size);
		}

		char buf[4096];

		string filename = string(DATA_DIR) + "\\" + dirlist[i].name + "\\" + filelist[maxsizeidx].name;

		printf("major contract %s size %d file %s\n", 
			filelist[maxsizeidx].name.c_str(), 
			filelist[maxsizeidx].size,
			filename.c_str());

//		gets(buf);

		FILE* pf = fopen(filename.c_str(), "r");

		fgets(buf, sizeof(buf), pf);

		int nlines = 0;

		while(fgets(buf, sizeof(buf), pf))
		{
//			printf("%s", buf);

			if(nlines%100 == 0)printf(".");
			fputs(buf, pMerged);
			nlines++;
//			gets(buf);
		}

		printf("\n");

		fclose(pf);
	}

	fclose(pMerged);

	return 0;
}

