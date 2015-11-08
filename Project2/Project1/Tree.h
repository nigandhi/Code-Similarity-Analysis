#ifndef TREE_H
#define TREE_H
///////////////////////////////////////////////////////////////////////
//  Tree.h    -    Stores vectors and variables			             //
//                 for determining complexity.	         			 //
//  Author:		   Nirav Gandhi, Syracuse University			     //
//				   (315) 395-4842, nigandhi@syr.edu					 //			
///////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module defines varibles and vectors that are useful for
computation of complexity.

Build Process:
==============
Required files
- library files

Maintenance History:
====================
ver 1.0 : 26 March 14
- first release

*/
#include<stdio.h>
#include<vector>


class treestack
{
public:
	void addcomplex( int);
	void addcount(int);
	void incrementcount();
	int getcount();
	void resetcount();
	int gettotalcomplex();
	void countstacksize();
	void addbracelevel(int);
	int returnprevbracelevel();
	void removebracelevel();
	int getsizeofbracelevel();
	int gettotalcomplexityforall();
private:
	int count=0;
	using Complex = std::vector<int>;
	using Countstack = std::vector<int>;
	using Bracelevel = std::vector<int>;
	Complex _complex;
	Countstack _countstack;
	Bracelevel _bracelevel;
};


#endif