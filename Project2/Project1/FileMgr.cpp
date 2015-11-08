///////////////////////////////////////////////////////////////////////
//  FileMgr.cpp -  Search a path for Files                           //
//  Author:		   Nirav Gandhi, Syracuse University			     //
//				   (315) 395-4842, nigandhi@syr.edu					 //	
///////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module takes path and patterns as arguments to its function
and returns the name of the files present on that path having the
required pattern.
Build Process:
==============
Required files
FileSystem.h
Maintenance History:
====================
ver 1.0 : 12 Feb 14
- first release
*/
#include "FileMgr.h"
#include <iostream>
#include "FileSystem.h"

FileMgr::files& FileMgr::searchf(const std::string& path, FileMgr::patterns patts)
{
	for (auto patt : patts)
	{

		files temp = FileSystem::Directory::getFiles(path, patt);
		for (auto file : temp)
		{
			std::string temp = path;
			if (temp.at(temp.length() - 1) != '\\')
			{
				temp.append("\\");
			}
			temp.append(file);
			//file = temp;
			_files.push_back(temp);
		}
	}		  
  return _files;
}
	
#ifdef TEST_FILEMGR

int main(int argc, char* argv[])
{
  for (int i = 0; i < argc; ++i)
    std::cout << "\n  " << argv[i];
  patterns patts{ "*.h", "*.cpp" };
  files foundFiles = fm.searchf(".", patts);
  for (auto file: foundFiles)
    std::cout << "\n  " << file;
  std::cout << "\n\n";
}
#endif
