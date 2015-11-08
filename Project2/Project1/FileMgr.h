#ifndef FILEMGR_H
#define FILEMGR_H
///////////////////////////////////////////////////////////////////////
//  FileMgr.h  -    Search a path for Files                          //
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
#include <vector>
#include <string>

class FileMgr
{
public:
  typedef std::vector<std::string> files;
  typedef std::vector<std::string> patterns;
  files& searchf(const std::string& path, patterns patts);
  
private:
  std::string path;
  files _files;
  patterns _patterns;
};
#endif
