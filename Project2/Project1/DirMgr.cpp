///////////////////////////////////////////////////////////////////////
//  DirMgr.cpp  -  Search a path for Directories                     //
//  Author:		   Nirav Gandhi, Syracuse University			     //
//				   (315) 395-4842, nigandhi@syr.edu					 //			
///////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module takes path as arguments to its function
and returns the directories present on that path.
Build Process:
==============
Required files
FileSystem.h
Maintenance History:
====================
ver 1.0 : 12 Feb 14
- first release
*/
#include "DirMgr.h"
#include <iostream>
#include "FileSystem.h"

DirMgr::directories& DirMgr::searchd(const std::string& path)
	{
		directories temp1 = FileSystem::Directory::getDirectories(path);
		
		for (auto dir : temp1)
		{
			int flag = 1;
			
			//std::cout << dir << "\n";
			if (dir.at(dir.length() - 1) != '.' && flag==1)
			{
				std::string temp = path;
				if (temp.at(temp.length() - 1) != '\\')
				{
					temp.append("\\");
				}
				temp.append(dir);
				dir = temp;	
				for (auto dir1 : _directories)
				{
					if (dir == dir1)
						flag++;
				}
				if (flag==1)
				_directories.push_back(temp);
				
			}
		}
			return _directories;
	}

#ifdef TEST_DIRMGR

int main(int argc, char* argv[])
{
	_directories = searchd(".");
	// std::cout << "Recursive";
	do
	{
		directories tempdir = _directories;
		_directories.clear();
		for (auto temp : tempdir)
		{
			mydirectories = searchd(temp);
		}
		count++;
	} while (_directories.capacity() != tempdir.capacity());
}
#endif