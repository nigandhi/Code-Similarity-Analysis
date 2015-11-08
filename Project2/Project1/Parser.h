#ifndef PARSER_H
#define PARSER_H
///////////////////////////////////////////////////////////////////////
//  Parser.h   - Analyzes C++ language constructs                    //
//  Author:		   Nirav Gandhi, Syracuse University			     //
//				   (315) 395-4842, nigandhi@syr.edu					 //			
//	Source:        Prof. Jim Fawcett, Syracuse University            //
//                 (315) 443-3948, jfawcett@twcny.rr.com             //
///////////////////////////////////////////////////////////////////////
/*
  Module Operations: 
  ==================
  This module defines a Parser class.  Its instances collect 
  semi-expressions from a file for analysis.  Analysis consists of
  applying a set of rules to the semi-expression, and for each rule
  that matches, a set of actions is invoked.

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
    - Parser.h, Parser.cpp, SemiExpression.h, SemiExpression.cpp,
      tokenizer.h, tokenizer.cpp,
      ActionsAndRules.h, ActionsAndRules.cpp,
      ConfigureParser.h, ConfigureParser.cpp
  Build commands (either one)
    - devenv Parser.sln
    - cl /EHsc /DTEST_PARSER parser.cpp semiexpression.cpp tokenizer.cpp \
         ActionsAndRules.cpp ConfigureParser.cpp /link setargv.obj


 Maintenance History:
 ====================
 ver 1.0 : 12 Jan 06
 - first release
 - Modified some function definitions for passing arguments.

*/

#include <string>
#include <iostream>
#include <vector>
#include "ITokCollection.h"
#include"ConsoleProcessing.h"

///////////////////////////////////////////////////////////////
// abstract base class for parsing actions
//   - when a rule succeeds, it invokes any registered action

class Parser;

class IBuilder
{
public:
  virtual ~IBuilder() {}
  virtual Parser* Build()=0;
};

///////////////////////////////////////////////////////////////
// abstract base class for parsing actions
//   - when a rule succeeds, it invokes any registered action

class IAction
{
public:
  virtual ~IAction() {}
  virtual void doAction(ITokCollection*& pTc)=0;
 
};

///////////////////////////////////////////////////////////////
// abstract base class for parser language construct detections
//   - rules are registered with the parser for use

class IRule
{
public:
  virtual ~IRule() {}
  void addAction(IAction* pAction);
  void doActions(ITokCollection*& pTc);
  virtual bool doTest(ITokCollection*& pTc)=0;
protected:
  std::vector<IAction*> actions;
};

class FoldingRules
{
public:
  virtual void doFold(ITokCollection*& pTc) { return; }
};
///////////////////////////////////////////////////////////////
// class that implements a parser
//   - It collects semi-expressions from a file for analysis.
//   - Each rule is applied to each semi-expression.  if the
//     rule matches, then it invokes any registered actions.

class Parser
{
public:
	Parser();
  Parser(ITokCollection* pTokCollection);
  ~Parser();
  void addRule(IRule* pRule);
  void addFoldingRules(FoldingRules* pRules);
  bool parse();
  bool next();

private:
	
  ITokCollection* pTokColl;
  std::vector<IRule*> rules;
  FoldingRules* pFoldingRules;
};

inline Parser::Parser(){}
inline Parser::Parser(ITokCollection* pTokCollection) : pTokColl(pTokCollection), pFoldingRules(new FoldingRules) {}

inline Parser::~Parser() {}

inline void Parser::addFoldingRules(FoldingRules* pRules)
{
  pFoldingRules = pRules;
}

#endif
