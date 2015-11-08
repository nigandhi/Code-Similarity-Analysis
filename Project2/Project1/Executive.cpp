///////////////////////////////////////////////////////////////////////
//  Executive.cpp - main function of the project			         //
//  Author:		   Nirav Gandhi, Syracuse University			     //
//				   (315) 395-4842, nigandhi@syr.edu					 //			
///////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
-This module performs as the main function of the project interacting 
 between different packages included for different functionalities.
 Build Process:
 ==============
 Required files
 - Executive, Parser.h, Parser.cpp, ScopeStack.h, ScopeStack.cpp,
 ActionsAndRules.h, ActionsAndRules.cpp,
 ConfigureParser.h, ConfigureParser.cpp,
 ItokCollection.h, SemiExpression.h, SemiExpression.cpp,
 tokenizer.h, tokenizer.cpp ,MNode.h, MNode.cpp,
 Tree.h, Tree.cpp , MTree.h, MTree.cpp
 Build commands (either one)
 - devenv Parser.sln
 - cl /EHsc Executive.cpp parser.cpp ActionsAndRules.cpp \
 ConfigureParser.cpp FoldingRules.cpp ScopeStack.cpp
 semiexpression.cpp tokenizer.cpp /link setargv.obj

 Maintenance History:
 ====================
 ver 2.0 : 26 March 14
 - Displays Similar code region in/between same/different files
 ver 1.0 : 12 Feb 14
 - calls funtions to fetchfiles, recursively search directories, configure parser and display files
 
*/
#include "FileMgr.h"
#include <iostream>
#include <vector>
#include <string>
#include "DirMgr.h"
#include "Display.h"
#include "Tokenizer.h"
#include "SemiExpression.h"
#include "Parser.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"
#include "FoldingRules.h"
#include "ConsoleProcessing.h"
#include "ChronoTimer.h"
#include "FileSystem.h"


int parser(FileMgr::files& myfiles)
{
	ConfigParseToConsole configure;
	Parser* pParser = configure.Build();
	configure.accessRepo()->initializerootvector();
	for (auto file : myfiles) {
		configure.accessRepo()->setfilename(FileSystem::Path::getName(file));
		try	{
			if (pParser)	{
				if (!configure.Attach(file))
				{
					std::cout << "\n  could not open file " << file << std::endl;
					continue;
				}
			}
			else {
				std::cout << "\n\n  Parser not built\n\n";
				return 1;
			}
			// now that parser is built, use it
			while (pParser->next())
				pParser->parse();
		}
		catch (std::exception& ex) {
			std::cout << "\n\n    " << ex.what() << "\n\n";
		}
	}
	//configure.accessRepo()->walkthrough();
	configure.accessRepo()->simplify();
	configure.accessRepo()->sort();
	configure.accessRepo()->detectmatch();
	return 0;
}


int main(int argc, char* argv[]) {
	using  ConProc = ConsoleProcessing;
	std::cout << ConProc::Title("***********Project#2 - Code Similarity Analysis***********") << std::endl;
	ConProc::CommandLine cl = ConProc::parseCommandLine(argc, argv);
	if (argc < 3) {
		std::cout << "\n  please enter path and patterns\n\n";
		return 1;	}
	int count=0,apath=0; std::string path; bool recursivesearch = false, sizencomplx = false;
	path = std::get<0>(cl);
	std::cout << " Path provided= " << path <<"\n";
	//<-------Identifying path position and options from the commandline arguments---->
	std::cout << " " << std::string(43, '=') << std::endl;
	for (int i = 1; i < argc; i++) {
		char* pch = strchr(argv[i], '.');
		if ((int)(pch - argv[i] + 1) == 1) 
			apath = i;
		else if (strcmp(argv[i], "/s") == 0) {
			std::cout << " Recursive mode activated" << "\n";
			recursivesearch = true;		} 	
	}
	std::cout << " " << std::string(43, '=') << std::endl;	
	FileMgr::patterns patts=std::get<1>(cl);
	DirMgr dm; FileMgr fm; Display dis; Timer timer; DirMgr::directories mydirectories, tempdir;
	FileMgr::files myFiles = fm.searchf(path, patts);
	timer.start();
	if (recursivesearch == true) { 				//Method for Recursively searching the current directory path
		mydirectories = dm.searchd(path);
		do {
			tempdir = mydirectories;
			mydirectories.clear();
			for (auto temp : tempdir)
				mydirectories = dm.searchd(temp);
			count++;
		} while (mydirectories.capacity() != tempdir.capacity()); 
	}
	if (recursivesearch == true) {
		std::cout << "\n\n List of Directories Detected\n" << " " << std::string(30, '=') << std::endl;
		dis.displayfiles(mydirectories); 			//Passing Directory names to the Display package  
	}
	for (auto direc : mydirectories) 			//Searching files from the directory list
		myFiles = fm.searchf(direc, patts);
	std::cout << "\n\n List of Files Detected\n" << " " << std::string(30, '=') << std::endl;
	dis.displayfiles(myFiles);					//Passing file names to the Display package
	std::cout <<" "<< std::string(30, '=') << std::endl;
	parser(myFiles);								//Recursively passing files one by one to the Parser
	timer.stop();
	std::cout << "\n\nTime taken for processing in milliseconds = " << timer.elapsedTime<std::chrono::milliseconds>();
	std::cout << "\n";
	return 0;
}
