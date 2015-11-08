///////////////////////////////////////////////////////////////////////
//  Display.cpp -  Display Package for Files/Directories display     //
//  Author:		   Nirav Gandhi, Syracuse University			     //
//				   (315) 395-4842, nigandhi@syr.edu					 //	
///////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module is designed to display the string that contains
file/directory name to be displayed on the console. 
Maintenance History:
====================
ver 1.0 : 12 Feb 14
- first release
*/

#include<iostream>
#include "Display.h"
#include "FileMgr.h"
#include "FileSystem.h"

void Display::displayfiles(FileMgr::files& myfiles)
{
	for (auto file : myfiles)
	{
		std::cout << " " << FileSystem::Path::getName(file);
		std::cout << "\n";
	}
}

#ifdef TEST_DISPLAY

int main(int argc, char* argv[])
{
	for (int i = 0; i < argc; ++i)
		std::cout << "\n  " << argv[i];
	FileMgr fm;
	FileMgr::patterns patts{ "*.h", "*.cpp" };
	FileMgr::files foundFiles = fm.search(".", patts);
	for (auto file : foundFiles)
		displayfiles(file);
	std::cout << "\n\n";
}
#endif