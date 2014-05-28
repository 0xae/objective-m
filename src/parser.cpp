#include <iostream>
#include <list>
#include <map>
#include <string>
#include <stdio.h>

#include "nobject.hpp"
#include "nutils.hpp"
#include "nerror.hpp"
#include "parser.hpp"

using namespace nobject;

using std::list;
using std::map;
using std::string;
using nerror::ExpressionError;
using nerror::NotBoundError;
using nutils::trim;
using nutils::isValidAtom;
using nutils::findSafeSeparator;

namespace nparser{

static map<string, NObject*> symbolTable;

NObject* getVar(string name){
  return symbolTable[name];
}

NObject* storeVar(string name, NObject *value){
  symbolTable[name] = value;
  return value;
}

NString* parseString(string value){
  trim(value);
  int last = value.size() - 1;
  if(value.substr(last,1) != "\""){
    throw ExpressionError("unclosed string at '" + value + "'");
  }

  return new NString(value.substr(2, last-2));
}

static NObject* resolve(string value, map<string,NObject*> &locals, bool lazy){
  if(lazy){
    return new NString(value);
  }else{
    return eval(value, locals);
  }
}

NList*  parseList(string value, map<string, NObject*> &locals, bool lazy){
  trim(value);
  char op =  (value[0] == '(') ? '(' : '{';

  char end = (op == '(') ? ')' : '}';
  char sep = (op == '(') ? ',' : ';';

  int last = value.size() - 1;
  if(value[last] != end){
    throw ExpressionError("unclosed list '" + value + "'");
  }

  string src = value.substr(1, last-1);
  int comma = 0;
  NList *list = (sep == ',') ? (new NList) : (new NBlock);
  NObject *obj = 0;

  for(;;){
    nutils::trim(src);
    comma = findSafeSeparator(src, sep);
    if(comma == -1){
      obj = resolve(src, locals, lazy);
      list->add(obj);
      break;
    }

    string el = src.substr(0, comma);
    obj = resolve(el, locals, lazy);
    list->add(obj);
    src = src.substr(comma+1);
  }

  return list;
}

NObject* parseAtom(string value,  map<string, NObject*> &locals){
  trim(value);
  if(not isValidAtom(value)){
    throw ExpressionError("invalid atom '" + value + "'");
  }

  NObject *obj = locals[value];
  if(obj == 0){
    obj = symbolTable[value];
    if(obj == 0){
      //throw NotBoundError("atom '" + value + "' not bound");
      return NAtom::get(value);
    }
  }

  return obj;
}

NObject *eval(string value, map<string, NObject*> &locals){
  trim(value);
  if(value.substr(0,2) == "@\""){  /* A string object */
    return parseString(value);

  }else if(value[0] == '(' or value[0] == '{'){   /* A list object */
    return parseList(value, locals);

  }else if(value[0] == '['){   /* A message sending */
    return parseExpression(value, locals);

  }else{  /* An atom or a misstyped value */
    return parseAtom(value, locals);
  }
}


NObject *parseExpression(string expr, map<string, NObject*> &locals){
  int len = expr.size();
  trim(expr);
  if(expr[0] != '[' or expr[len-1] != ']'){
    throw ExpressionError("invalid message '" + expr + "'");
  }

  expr = expr.substr(1, len-1);
  int space = findSafeSeparator(expr, ' ');
  if(space == -1){
    throw ExpressionError("no message at " + expr);
  }

  string object = expr.substr(0, space);
  trim(object);
  int pos = expr.size()-1;
  NObject *receiver = eval(object, locals);

  string message = expr.substr(space, pos-space);
  trim(message);

  int tdot = findSafeSeparator(message, ':');

  if(tdot == -1){ /* Message with no args */
    return receiver->sendMessage(message, 0);

  }else{
    string messageName = message.substr(0, tdot);
    if(not isValidAtom(messageName)){
      throw ExpressionError("'" + messageName + "' is not a valid atom");
    }

    string sargs = "(" + message.substr(tdot+1) + ")";
    bool lazy = (messageName == "def") or 
                (messageName == "foreach") or 
                (messageName == "ifelse");

    NObject *list = parseList(sargs, locals, lazy);
    NList *args = dynamic_cast<NList*>(list);
    return receiver->sendMessage(messageName, args);
  }
}

}/* namespace nparser */

