#ifndef DIRMGR_H
#define DIRMGR_H
///////////////////////////////////////////////////////////////////////
//  DirMgr.h    -  Search a path for Directories                     //
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
#include <vector>
#include <string>

class DirMgr
{
public:
	typedef std::vector<std::string> directories;
	typedef std::vector<std::string> directoriesalias;
	typedef std::vector<std::string> patterns;
	directories& searchd(const std::string& path);
private:
	std::string path;
	directories _directories;
	patterns _patterns;
};
#endif
