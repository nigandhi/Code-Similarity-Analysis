#ifndef ACTIONSANDRULES_H
#define ACTIONSANDRULES_H
///////////////////////////////////////////////////////////////////////
//  ActionsAndRules.h - declares new parsing rules and actions       //
//  Author:		   Nirav Gandhi, Syracuse University			     //
//				   (315) 395-4842, nigandhi@syr.edu					 //			
//	Source:        Prof. Jim Fawcett, Syracuse University            //
//                 (315) 443-3948, jfawcett@twcny.rr.com             //
///////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module defines several action classes.  Its classes provide
specialized services needed for specific applications.  The modules
Parser, SemiExpression, and Tokenizer, are intended to be reusable
without change.  This module provides a place to put extensions of
these facilities and is not expected to be reusable.

Public Interface:
=================
Toker t(someFile);              // create tokenizer instance
SemiExp se(&t);                 // create a SemiExp attached to tokenizer
Parser parser(se);              // now we have a parser
Rule1 r1;                       // create instance of a derived Rule class
Action1 a1;                     // create a derived action
r1.addAction(&a1);              // register action with the rule
parser.addRule(&r1);            // register rule with parser
while(se.getSemiExp())          // get semi-expression
parser.parse();               //   and parse it

Build Process:
==============
Required files
- Parser.h, Parser.cpp, ScopeStack.h, ScopeStack.cpp,
ActionsAndRules.h, ActionsAndRules.cpp, ConfigureParser.cpp,
ItokCollection.h, SemiExpression.h, SemiExpression.cpp, tokenizer.h, tokenizer.cpp
Build commands (either one)
- devenv CodeAnalysis.sln
- cl /EHsc /DTEST_PARSER parser.cpp ActionsAndRules.cpp \
semiexpression.cpp tokenizer.cpp /link setargv.obj

Maintenance History:
====================
ver 1.0 : 11 Feb 14
- first release
- Added functionality for detecting type of scope.
- Added functionality for finding size and complexity of functions.
- Added functionality of passing nodes to tree, finding nodes and similar code region.

*/
#include <queue>
#include <string>
#include <sstream>
#include <stack>
#include "Parser.h"
#include "ITokCollection.h"
#include "ScopeStack.h"
#include "Tokenizer.h"
#include "SemiExpression.h"
#include "FileMgr.h"
#include "ConsoleProcessing.h"
#include <tuple>
#include "Tree.h"
#include "MTree.h"
#include "MNode.h"
#include "FileSystem.h"
#include <algorithm>

///////////////////////////////////////////////////////////////
// ScopeStack element is application specific
struct element
{
	std::string filename;
	std::string type;
	std::string name;
	std::string parenttype;
	std::string parentname;
	int bracelevel;
	int complexity;
	size_t lineCount;
	size_t startline;
	size_t endline;
	int id,parentid,numchildren;
	std::string show()
	{
		std::ostringstream temp;
		temp << "(";
		temp << id;
		temp << ",";
		temp << parentid;
		temp << ",";
		temp << numchildren;
		temp << ",";
		temp << filename;
		temp << ",";
		temp << parenttype;
		temp << ", ";
		temp << parentname;
		temp << ", ";
		temp << type;
		temp << ", ";
		temp << name;
		temp << ", ";
		temp << complexity;
		temp << ", ";
		temp << bracelevel;
		temp << ", ";
		temp << lineCount;
		temp << ",";
		temp << startline;
		temp << ",";
		temp << endline;
		temp << ")";
		return temp.str();
	}
};


///////////////////////////////////////////////////////////////
// Repository instance is used to share resources
// among all actions.
//it also acts as an interface to share resource with Tree.h
// Mtree.h, MNode.h, MTree.cpp, MNode.cpp ,Tree.cpp
using namespace TMTree;
typedef MNode<element> Node;

class Repository
{
	ScopeStack<element> stack;
	Toker* p_Toker;
	treestack ts;
	std::vector<MTree<Node>*> VectorOfTrees;
	std::vector<Node*> VecOfNodes;
	std::vector<element> VecOfComp;
	std::stack<Node*> NodeStack;
	std::stack<Node*> TempStack;
	MTree<Node>* mtree;
	int nodecount = 0;
	std::string filename;
	bool isfunction;
	int buffercomplex = 0;
	int complexity = 1;
	size_t startline;
	int id=0;
public:
	Repository(){};
	Repository(Toker* pToker)
	{
		p_Toker = pToker;
	}
	int incrementid()
	{
		id++;
		return id;
	}
	void decrementid()
	{
		id--;
	}
	void addbracelevel(int _bracelevel)
	{
		ts.addbracelevel(_bracelevel);
	}
	int returnbracelevel()
	{
		return ts.returnprevbracelevel();
	}
	void removebracelevel()
	{
		ts.removebracelevel();
	}
	int getsizeofbracelevel()
	{
		return ts.getsizeofbracelevel();
	}
	void incrementcount()
	{
		ts.incrementcount();
	}
	void resetcount()
	{
		ts.resetcount();
	}
	int getcount()
	{
		return ts.getcount();
	}
	void getcountstacksize()
	{
		ts.countstacksize();
	}
	void addtovectorofcount(int _count)
	{
		ts.addcount(_count);
	}
	void addtovectorofcomplexity(int _complex)
	{
		ts.addcomplex(_complex);
	}
	int gettotalcomplexity()
	{
		return ts.gettotalcomplex();
	}
	int gettotalcomplexityforall()
	{
		return ts.gettotalcomplexityforall();
	}
	int getcomplexity()
	{
		return complexity;
	}
	void setbuffer(int buf)
	{
		buffercomplex = buf;
	}
	int getbuffer()
	{
		return buffercomplex;
	}
	void setcomplexity(int _complexity)
	{
		complexity = _complexity;
	}
	void incrementcomplexity()
	{
		complexity++;
	}
	void resetcomplexity()
	{
		complexity = 1;
	}
	std::string getfilename()
	{
		return filename;
	}
	void setfilename(std::string file)
	{
		filename = file;
	}
	ScopeStack<element>& scopeStack()
	{
		return stack;
	}
	ScopeStack<element>& dummyscopeStack()
	{
		return stack;
	}
	Toker* Toker()
	{
		return p_Toker;
	}
	size_t lineCount()
	{
		return (size_t)(p_Toker->lines());
	}
	void ShowStack(ScopeStack<element>& stack)
	{
		showStack(stack);
	}
	int Bracelevel()
	{
		return p_Toker->braceLevel();
	}	
	void initializerootvector()				//<-----------------Initializing Root of tree------------------->
	{
		element elem;
		Node* pElem = new Node(elem);
		pElem->value().name = "Namespace";
		pElem->value().type = "Global";
		pElem->value().lineCount = 0;
		pElem->value().startline = 0;
		pElem->value().endline = 0;
		pElem->value().id = 0;
		pElem->value().parentid = 0;
		pElem->value().parentname = "Namespace";
		pElem->value().parenttype = "Global";
		pElem->value().filename = getfilename();
		RootVector().push_back(pElem);
	}
	std::stack<Node*>& Nodes()
	{
		return NodeStack;
	}
	std::vector<element>& ComplexVector()
	{
		return VecOfComp;
	}
	std::vector<Node*>& RootVector() //vector of root nodes
	{
			return VecOfNodes;
	}
	void walkthrough()							//<---------function to display tree---------------------------->
	{		
		class newOper : public Operation<Node>
		{
		public:
			bool operator()(Node* pNode)
			{
				std::cout << "\n" << pNode->ToString();
				return false;
			}
		};
		MTree<Node> tree(new newOper);
		tree.makeRoot(RootVector().back());
		tree.verbose() = true;
		tree.walk();
		std::cout << "\n";		
	}
	void findnode(int tvalue,int i)				//<--------------Function to find nodes from tree and alocating
	{											//  missing values from tree to vector---------------------------->
		class newOper : public Operation<Node>
		{
		public:
			bool operator()(Node* pNode)
			{
				std::cout << "\n" << pNode->ToString();
				return false;
			}
		};
		MTree<Node> tree(new newOper);
		element elem;
		Node* pElem = new Node(elem);
		pElem = RootVector().back();
		tree.makeRoot(pElem);
		int val = tvalue;
		Node* pFound = tree.finder(val);
		ComplexVector().at(i).parentname = pFound->value().parentname;
		ComplexVector().at(i).parenttype = pFound->value().parenttype;
		ComplexVector().at(i).numchildren = pFound->numChildren();
		ComplexVector().at(i).parentid = pFound->value().parentid;
	}
	std::stack<Node*> tempstack()
	{
		return TempStack;
	}
	bool Isfuncflag()
	{
		return isfunction;
	}
	void setfuncflag()
	{
		isfunction = true;
	}
	void resetfuncflag()
	{
		isfunction = false;
	}
	void setline(size_t line)
	{
		startline = line;
	}
	size_t getline()
	{
		return startline;
	}
	void addnodestotree(element& elem)				//<----------Function to add nodes to the tree------------>
	{
		Node* pElem = new Node(elem);
		pElem->value().name = elem.name;
		pElem->value().type = elem.type;
		pElem->value().startline = elem.startline;
		pElem->value().bracelevel = Bracelevel();
		pElem->value().filename = getfilename();
		if (Bracelevel() == 1) {
			pElem->value().parentid = 0;
			pElem->value().parentname = "Namespace";
			pElem->value().parenttype = "Global";
			RootVector().back()->addChild(pElem);
			Nodes().push(pElem);
		}
		else {
			if ((Nodes().top())->value().bracelevel == Bracelevel() - 1)
			{
				pElem->value().parentid = Nodes().top()->value().id;
				pElem->value().parentname = Nodes().top()->value().name;
				pElem->value().parenttype = Nodes().top()->value().type;
				Nodes().top()->addChild(pElem);
				Nodes().push(pElem);
			}
			else
			{
				while (((Nodes().top())->value().bracelevel != Bracelevel() - 1))
					Nodes().pop();
				tempstack().push(Nodes().top());
				pElem->value().parentid = Nodes().top()->value().id;
				pElem->value().parentname = Nodes().top()->value().name;
				pElem->value().parenttype = Nodes().top()->value().type;
				Nodes().top()->addChild(pElem);
				Nodes().push(pElem);
			}
		}
	}
	void findcomplexity()   //<-------------function to find complexity------------------->
	{
		if (getbuffer() == 0) {
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
		else if (Bracelevel() < getbuffer() && Bracelevel() != 0) {
			if (getsizeofbracelevel() > 0 && returnbracelevel() == Bracelevel()) {
				int temp = gettotalcomplexity();
				setcomplexity(temp + 1);
				addtovectorofcomplexity(getcomplexity());
				setbuffer(Bracelevel());
				removebracelevel();
				temp = 0;
				getcountstacksize();	}
			else {
				int temp = gettotalcomplexity();
				setcomplexity(temp + 1);
				resetcount();
				addtovectorofcomplexity(getcomplexity());
				incrementcount();
				temp = 0;
				setbuffer(Bracelevel());	}
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
			setbuffer(Bracelevel());	}
		else if (Bracelevel() == 0)	{
			int temp = gettotalcomplexityforall();
			setcomplexity(temp + 1);
			resetcount();	}
	}
	void simplify()				//<---------------Function to find node corresponding to the id------------>
	{
		int j = ComplexVector().size();
		element elem;
		MNode<element>* pElem = new MNode<element>(elem);
		for (int i = 0; i < j; i++)
		{
			elem = ComplexVector().at(i);
			findnode(elem.id, i);
		}
	}
	void sort()					//<--------------Sorting the vector with complexity-------------------->
	{
		struct myclass {
			bool operator() (element elem1, element elem2) 
			{				
				return (elem1.complexity<elem2.complexity);
			}
		} myobject;
		
		std::sort(VecOfComp.begin(), VecOfComp.end(), myobject);
		element elem;
		int j = VecOfComp.size();
		for (int i = 0; i < j; i++)
		{
			elem = VecOfComp.at(i);
		}
	}
	void detectmatch()					//<--------------Function to check for similar code------------------->
	{
		element elem,elem1,elem3,elem2;
		int k = VecOfComp.size();
		for (int i = 0; i < k; i++)
		{
			elem = VecOfComp.at(i);
			for (int j = 0; j < k && i!=j; j++)
			{
				elem1 = VecOfComp.at(j);
				if (elem.complexity > 2 && elem1.complexity < elem.complexity + 2 && elem1.complexity > elem.complexity - 2/*&& elem.lineCount < elem1.lineCount + 5 && elem.lineCount > elem1.lineCount - 5 */ && elem.type == elem1.type && elem.numchildren == elem1.numchildren && elem.id != elem1.id)
				{
					if (checkchildsimilarity(elem, elem1) == true)
					{
						std::cout << "\n Similar Code";
						std::cout << "\n Filename= "<<elem.filename<<" ,Scopename= " << elem.name << ", ScopeType= " << elem.type << " ,StartLine= " << elem.startline<< " ,Endline= "<<elem.endline<<" ,Complexity"<<elem.complexity;
						std::cout << "\n\n Filename= " << elem1.filename << " ,Scopename= " << elem1.name << ", ScopeType= " << elem1.type << ", StartLine= " << elem1.startline<<" ,Endline= " << elem1.endline << " ,Complexity" << elem1.complexity;
						std::cout << "\n " << std::string(78, '=') << std::endl;
					}
				}
			}
		}
	}

	bool checkchildsimilarity(element& elem, element& elem1)		//<-------checking for similar children-------------->
	{
		int k = VecOfComp.size();
		element elem2, elem3;
		static bool flag = false;
		for (int m = 0; m < k; m++)
		{
			elem2 = VecOfComp.at(m);
			for (int n = 0; n < k; n++)
			{
				elem3 = VecOfComp.at(n);
				if (elem2.parentid == elem.id && elem3.parentid == elem1.id && elem2.type == elem3.type && elem2.complexity == elem3.complexity)
				{
					if (elem2.type == elem3.type && elem2.complexity == elem3.complexity)
						flag = true;
					else
						return false;
				}
			}
		}
		return flag;
	}

};

///////////////////////////////////////////////////////////////
// rule to detect beginning of anonymous scope
class BeginningOfScope : public IRule
{
public:
	//Treestruct tree;
	bool doTest(ITokCollection*& pTc)
	{
		//std::cout << "\n--BeginningOfScope rule";
		if (pTc->find("{") < pTc->length())
		{
			doActions(pTc);
			return false;
		}
		return false;
	}
};
 
///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class HandlePush : public IAction
{
	Repository* p_Repos;
public:
	HandlePush(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)	//<-------------Method to detect Scopetype,Scopename,etc (i.e. element values)----->
	{	ITokCollection& temp = *pTc;
		element elem;
		std::string name = (*pTc)[pTc->find("(") - 1];
		int i = 1;
		elem.type = "unknown";
		if (name == "{" && (*pTc)[pTc->find("(") - 2] != "else" && (*pTc)[pTc->find("(") - 2] != "try" && (*pTc)[pTc->find("(") - 2] != "do")
		{	name = (*pTc)[pTc->find("(") - 2];
			elem.type = (*pTc)[pTc->find("(") - 3];
			if (name == "\n")
			{	name = (*pTc)[pTc->find("(") - 3];
				if (temp.find("struct") != std::string::npos && temp.length() > 3)
					elem.type = (*pTc)[pTc->find("(") - 4];
				if (temp.find(":") < temp.length() && temp.length() > 3)
				{	name = (*pTc)[pTc->find(":") - 1];
					elem.type = (*pTc)[pTc->find(":") - 2];
				}
			}
		}
		else if (name == "{" && (*pTc)[pTc->find("(") - 2] != "else" && (*pTc)[pTc->find("(") - 2] != "try" && (*pTc)[pTc->find("(") - 2] != "do")
			name = (*pTc)[pTc->find("(") - 2];
		if (name == "=")
			name = elem.type = "array";
		elem.name = name;
		if (elem.type == "unknown" && pTc->find("(")>0)
		{	name = (*pTc)[pTc->find("(") - 1];
			elem.type = "function";
			if (name == "{" && pTc->find("(")>1)
			{	name = (*pTc)[pTc->find("(") - 2];
				if (name == "\n" && pTc->find("(")>2)
					name = (*pTc)[pTc->find("(") - 3];
				elem.type = (*pTc)[0];
			}
			elem.name = name;
		}
		elem = exception(elem);
		elem.id = p_Repos->incrementid();
		elem.startline = p_Repos->lineCount()+1;
		elem.bracelevel = p_Repos->Bracelevel();
		elem.filename = p_Repos->getfilename();
		p_Repos->scopeStack().push(elem);
		p_Repos->setline(p_Repos->lineCount() + 1);
		p_Repos->addnodestotree(elem);
	}
	element& exception(element& elem)
	{
		const static std::string nonfunc[] = { "for", "while", "switch", "if","try","else", "do", "catch", "enum", "class", "namespace", "struct" };
		for (int i = 0; i < 11; ++i)
		if (elem.name == nonfunc[i])
			elem.type = elem.name;
		if (elem.name == "do")
			elem.type = elem.name;
		return elem;
	}
};
///////////////////////////////////////////////////////////////
// rule to detect end of scope

class EndOfScope : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		//std::cout << "\n--EndOfScope rule";
		if (pTc->find("}") < pTc->length())
		{
			doActions(pTc);
			return false;
		}
		return false;
	}
};

///////////////////////////////////////////////////////////////
//
class EndOfFile :public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		if (!pTc->find("#"))
		{
			//std::cout << "\nEnd of file";
			return false;
		}
		return false;
	}
};
///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class HandlePop : public IAction, public Parser
{
	Repository* p_Repos;
public:
	HandlePop(){}
	HandlePop(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	ConsoleProcessing::Options _options;
	void doAction(ITokCollection*& pTc)
	{
		
		element elem;
		if (p_Repos->scopeStack().size() == 0)
		{
			return;
		}
		static bool flag = false;
		elem = p_Repos->scopeStack().pop();
		p_Repos->findcomplexity();
		elem.complexity = p_Repos->getcomplexity();
		elem.endline = p_Repos->lineCount() + 1;
		elem.lineCount = elem.endline - elem.startline + 1;
		elem.filename = p_Repos->getfilename();
		p_Repos->ComplexVector().push_back(elem);
		//std::cout <<elem.filename<<" "<<elem.type<<" "<<elem.name<<" "<<elem.startline<<" "<< elem.complexity << "\n";
	}	
};

///////////////////////////////////////////////////////////////
// rule to detect preprocessor statements

class PreprocStatement : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		//std::cout << "\n--PreprocStatement rule";
		if (pTc->find("#") < pTc->length())
		{
			doActions(pTc);
			return true;
		}
		return false;
	}
};

///////////////////////////////////////////////////////////////
// action to print preprocessor statement to console

class PrintPreproc : public IAction
{
public:
	void doAction(ITokCollection*& pTc)
	{
		std::cout << "\n\n  Preproc Stmt: " << pTc->show().c_str();
	}
};

///////////////////////////////////////////////////////////////
// rule to detect function definitions

class FunctionDefinition : public IRule
{
public:

	bool isSpecialKeyWord(const std::string& tok)
	{
		const static std::string keys[]	= { "for" , "do" , "while" , "switch" , "if" , "else" , "catch" };
		for (int i = 0; i<7; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	bool isNamespace(const std::string& tok)
	{
		const static std::string keys1[]= { "enum", "class", "namespace", "struct" };
		for (int i = 0; i<4; ++i)
			if (tok == keys1[i])
				return true;
		return false;
	}
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (size_t len = tc[tc.length() - 1] == "{")
		{
			if (len < tc.length() && isNamespace(tc[len - 1]))
			{
				//std::cout << "\n--NamespaceDefinition rule";
				doActions(pTc);
				return true;
			}
			len = tc.find("(");
			if (len < tc.length() && !isSpecialKeyWord(tc[len - 1]))
			{
				//std::cout << "\n--FunctionDefinition rule";
				doActions(pTc);
				return true;
			}
		}
		return false;
	}
};


///////////////////////////////////////////////////////////////
// action to push function name onto ScopeStack

class PushFunction : public IAction
{
	Repository* p_Repos;
public:
	PushFunction(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		p_Repos->scopeStack().pop();
		std::string name;
		element elem;
		name = (*pTc)[pTc->find("(") - 1];
		elem.type = "function";
		if (name == "{")
		{
			name = (*pTc)[pTc->find("(") - 2];
			if (name == "\n")
			{
				name = (*pTc)[pTc->find("(") - 3];
			}
			elem.type = (*pTc)[0];
		}
		p_Repos->decrementid();
		elem.id = p_Repos->incrementid();
		elem.name = name;
		elem.startline = p_Repos->lineCount()+1;
		p_Repos->scopeStack().push(elem);

	}
};
///////////////////////////////////////////////////////////////
// action to send signature of a function def to console

class PrettyPrintFunction : public IAction
{
public:
	void doAction(ITokCollection*& pTc)
	{
		pTc->remove("public");
		pTc->remove(":");
		pTc->trimFront();
		int len = pTc->find(")");
		std::cout << "\n\n  Pretty Stmt:    ";
		for (int i = 0; i<len + 1; ++i)
			std::cout << (*pTc)[i] << " ";
	}
};

#endif		
