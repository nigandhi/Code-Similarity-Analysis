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
#include "Tree.h"
#include <stdio.h>
#include <string.h>
#include <iostream>

void treestack::addcomplex(int __complex)
{
	_complex.push_back(__complex);
}
void treestack::addcount(int __count)
{
	_countstack.push_back(__count);
}
void treestack::incrementcount()
{
	count++;
}
int treestack::getcount()
{
	return count;
}
void treestack::resetcount()
{
	count = 0;
}
void treestack::countstacksize()
{
	if (_countstack.size() != 0)
	{
		count = _countstack.back();
		_countstack.pop_back();
	}
}
int treestack::gettotalcomplex()
{
	int temp=0;
	int j;
	j = count;
	for (int i = 0; i < j ; i++)
	{	
		//if (_complex.size()>0)
		{
		temp += _complex.back();
		_complex.pop_back();
		}
	}
	return temp;
}
void treestack::addbracelevel(int __bracelevel)
{
	_bracelevel.push_back(__bracelevel);
}
int treestack::returnprevbracelevel()
{
	return _bracelevel.back();
}
void treestack::removebracelevel()
{
	_bracelevel.pop_back();
}
int treestack::getsizeofbracelevel()
{
	return _bracelevel.size();
}
int treestack::gettotalcomplexityforall()
{
	int temp = 0;
	int j;

	j = _complex.size();

	for (int i = 0; i < j; i++)
	{
		temp += _complex.back();
		_complex.pop_back();
	}
	//_countstack.pop_back();
	return temp;
}


#ifdef TEST_TREE

int main(int argc, char* argv[])
{	if (getbuffer() == 0) {
		resetcount();
		incrementcount();
		resetcomplexity();
		addtovectorofcount(getcount());
		addtovectorofcomplexity(getcomplexity());
		setbuffer(Bracelevel());
	}
	else if (Bracelevel() == getbuffer()) {
		incrementcount();
		resetcomplexity();
		addtovectorofcomplexity(getcomplexity());
		setbuffer(Bracelevel());
	}
	else if (Bracelevel() < getbuffer() && Bracelevel() != 0) 
	{	if (getsizeofbracelevel() > 0 && returnbracelevel() == Bracelevel()) {
			int temp = gettotalcomplexity();
			setcomplexity(temp + 1);
			addtovectorofcomplexity(getcomplexity());
			setbuffer(Bracelevel());
			removebracelevel();
			temp = 0;
			getcountstacksize(); }
		else {
			int temp = gettotalcomplexity();
			setcomplexity(temp + 1);
			resetcount();
			addtovectorofcomplexity(getcomplexity());
			incrementcount();
			temp = 0;
			setbuffer(Bracelevel()); }
	}
	else if (Bracelevel() > getbuffer()) {
		if (Bracelevel() > getbuffer())
			addbracelevel(getbuffer());
		incrementcount();
		addtovectorofcount(getcount());
		resetcount();
		incrementcount();
		resetcomplexity();
		addtovectorofcomplexity(getcomplexity());
		setbuffer(Bracelevel());
	}
	else if (Bracelevel() == 0)	{
		int temp = gettotalcomplexityforall();
		setcomplexity(temp + 1);
		resetcount(); }	
}
#endif