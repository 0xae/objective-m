#include <map>
#include <list>
#include <string>

#include "nobject.hpp"
#include "nutils.hpp"
#include "parser.hpp"
#include "nhandler.hpp"

using std::string;
using std::list;
using std::map;

using nobject::NObject;
using nobject::NList;
using nutils::trim;
using nutils::findSafeSeparator;
using nparser::parseExpression;

namespace nhandler{

MessageHandler::MessageHandler(string _name, NObject *_receiver):
  name(_name),
  receiver(_receiver)
{
}

void MessageHandler::setupArgs(string argslist){
  string src = argslist;
  string arg;
  int comma;

  for(;;){
    trim(src);
    comma = findSafeSeparator(src, ',');
    if(comma == -1){
      args.push_back(src);
      break;
    }
    
    arg = src.substr(0, comma);
    args.push_back(arg);
    src = src.substr(comma+1);
  }
}


void MessageHandler::setupArgValues(NList* argvalues, map<string, NObject*> &locals){
  NObject *argvalue = 0;
  if(argvalues == 0){
    return;
  }

  for(string argname : args){
    argvalue = argvalues->popFirst();
    locals[argname] = argvalue;
  }
}


NObject* BuiltinMessageHandler::call(NList *objargs){
  map<string, NObject*> locals;
  setupArgValues(objargs, locals);

  return callback(this, locals);
}


NObject* SrcMessageHandler::call(NList* objargs){
  map<string, NObject*> locals;
  setupArgValues(objargs, locals);

  return parseExpression(block, locals);
}


} /* namespace nhandler */


