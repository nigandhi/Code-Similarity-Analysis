#ifndef DISPLAY_H
#define DISPLAY_H
///////////////////////////////////////////////////////////////////////
//  Display.h   -  Display Package for Files/Directories display     //
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
#include<string>
#include<vector>
#include"FileMgr.h"
class Display
{
public:
	void displayfiles(FileMgr::files&  filelist);
private:
	std::string file;

};

#endif
