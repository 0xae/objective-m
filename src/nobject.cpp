#include <iostream>
#include <string>
#include <sstream>
#include "nobject.hpp"
#include "parser.hpp"
#include "nutils.hpp"
#include "nerror.hpp"
#include "nhandler.hpp"

using std::cout;
using std::string;
using std::stringstream;
using nutils::findSafeSeparator;

using namespace nerror;
using namespace nparser;
using namespace nhandler;

namespace nobject{

/* message def: function, (args), {block} */
static NObject* defMessage(MessageHandler *handler, map<string, NObject*> &locals){
  stringstream blocks;
  stringstream args;
  string fnName = locals["function"]->toString();
  NList *fnArgs = parseList(locals["args"]->toString(), locals, true);
  NList *fnBody = parseList(locals["block"]->toString(), locals, true);

  if(fnBody == 0){
    throw Error("Block expected");
  }

  for(NObject *obj = fnArgs->popFirst(); obj!=0; obj=fnArgs->popFirst()){
    args << obj->toString() << ",";
  }
  args << "\b";

  for(NObject *obj = fnBody->popFirst(); obj!=0; obj=fnBody->popFirst()){
    blocks << obj->toString();
  }

  string block = blocks.str();
  
  NObject *receiver = handler->handlerReceiver();
  SrcMessageHandler *srchandler = new SrcMessageHandler(fnName, receiver, block);
  srchandler->setupArgs(args.str());
  receiver->registerMessage(srchandler);

  return NAtom::get(fnName + "_defined");
}

/* message  self */
static NObject* selfMessage(MessageHandler *handler, map<string, NObject*> &locals){
  return handler->handlerReceiver();
}


/* message  =: */
static NObject* setMessage(MessageHandler *handler, map<string, NObject*> &locals){
  NObject *value = locals["value"];
  NObject *receiver = handler->handlerReceiver();
  string className = receiver->className();

  if(className == kAtomClassName){
    string var = receiver->toString();
    storeVar(var, value);
    locals[var] = value;
    
  }else{
    if(not value->equals(receiver)){
      throw MatchError("match error: '"   + 
                        value->toString()  + 
                        "' and '"          + 
                        receiver->toString() + 
                        "' dont match");
    }
  }

  return value;
}

/* message exit */
static NObject* exitMessage(MessageHandler *handler, map<string, NObject*> &locals){
  exit(0);
}

/* message  ==: value */
static NObject* equalsMessage(MessageHandler *handler, map<string, NObject*> &locals){
  NObject *value = locals["value"];
  NObject *receiver = handler->handlerReceiver();
  bool equals = receiver->equals(value);
  return equals ? NAtom::get("true") : NAtom::get("false");
}

/* message  !=: value */
static NObject* notEqualsMessage(MessageHandler *handler, map<string, NObject*> &locals){
  NObject *value = locals["value"];
  NObject *receiver = handler->handlerReceiver();
  bool eq = receiver->equals(value);

  return eq ? NAtom::get("false") : 
              NAtom::get("true");
}

/* message  ifelse: cond,ifblock,elseblock */
static NObject* ifElseMessage(MessageHandler *handler, map<string, NObject*> &locals){
  string condstr = locals["cond"]->toString();
  NObject *cond = eval(condstr, locals);
  NAtom *True = NAtom::get("true");
  NAtom *False = NAtom::get("false");

  if(cond == True){ 
    string ifstr = locals["ifblock"]->toString();
    std::cout << ifstr << "\n";
    NObject *obj = eval(ifstr, locals);
    return obj;

  }else if(cond == False){
    string elsestr = locals["elseblock"]->toString();
      std::cout << elsestr << "\n";
    NObject *obj = eval(elsestr, locals);
    return obj;

  }else{
    throw Error("Bad condition: " + condstr);
  }
}


/* message  println */
static NObject* printlnMessage(MessageHandler *handler, map<string, NObject*> &locals){
  NObject *value = handler->handlerReceiver();
  std::cout << value->toString() << "\n";
  return value;
}

/* message  print */
static NObject* printMessage(MessageHandler *handler, map<string, NObject*> &locals){
  NObject *value = handler->handlerReceiver();
  std::cout << value->toString();
  return value;
}

/* message  readline */
static NObject* readlineMessage(MessageHandler *handler, map<string, NObject*> &locals){
  string line;
  getline(std::cin, line);
  return new NString(line);
}

/* message  debug */
static NObject* debugMessage(MessageHandler *handler, map<string, NObject*> &locals){
  NObject *value = handler->handlerReceiver();
  std::cout << "value = '" << value->toString() 
	          << "'\naddr  = " << value->memoryAddress() 
      	    << "\nclass = '" << value->className()
	    << "'\n";
  return value;
}

/* message  asAtom */
static NObject* asAtomMessage(MessageHandler *handler, map<string, NObject*> &locals){
  NObject *value = handler->handlerReceiver();
  if(value->className() == kAtomClassName){
    return value;
  }
  return parseAtom(value->toString(), locals);
}

/* message  asString */
static NObject* asStringMessage(MessageHandler *handler, map<string, NObject*> &locals){
  NObject *value = handler->handlerReceiver();
  if(value->className() == kStringClassName){
    return value;
  }
  return parseString(value->toString());
}

/* message  asList */
static NObject* asListMessage(MessageHandler *handler, map<string, NObject*> &locals){
  NObject *value = handler->handlerReceiver();
  if(value->className() == kListClassName){
    return value;
  }
  return parseList(value->toString(), locals);
}

static void initializeObject(NObject *obj){
  BuiltinMessageHandler *ifelse = new BuiltinMessageHandler("ifelse", obj, ifElseMessage);
  BuiltinMessageHandler *self = new BuiltinMessageHandler("self", obj, selfMessage);
  BuiltinMessageHandler *set = new BuiltinMessageHandler("=", obj, setMessage);
  BuiltinMessageHandler *equals = new BuiltinMessageHandler("==", obj, equalsMessage);
  BuiltinMessageHandler *notequals = new BuiltinMessageHandler("!=", obj, notEqualsMessage);
  BuiltinMessageHandler *println = new BuiltinMessageHandler("println", obj, printlnMessage);
  BuiltinMessageHandler *debug = new BuiltinMessageHandler("debug", obj, debugMessage);
  BuiltinMessageHandler *asatom = new BuiltinMessageHandler("asAtom", obj, asAtomMessage);
  BuiltinMessageHandler *asstring = new BuiltinMessageHandler("asString", obj, asStringMessage);
  BuiltinMessageHandler *aslist = new BuiltinMessageHandler("asList", obj, asListMessage);
  BuiltinMessageHandler *exit = new BuiltinMessageHandler("exit", obj, exitMessage);
  BuiltinMessageHandler *print = new BuiltinMessageHandler("print", obj, printMessage);
  BuiltinMessageHandler *readline = new BuiltinMessageHandler("readline", obj, readlineMessage);
  BuiltinMessageHandler *def = new BuiltinMessageHandler("def", obj, defMessage);

  obj->registerMessage(ifelse);
  ifelse->setupArgs("cond,ifblock,elseblock");
  obj->registerMessage(ifelse);


  obj->registerMessage(self);

  def->setupArgs("function,args,block");
  obj->registerMessage(def);

  set->setupArgs("value");
  obj->registerMessage(set);

  equals->setupArgs("value");
  obj->registerMessage(equals);

  notequals->setupArgs("value");
  obj->registerMessage(notequals);

  obj->registerMessage(println);
  obj->registerMessage(debug);
  obj->registerMessage(asatom);
  obj->registerMessage(asstring);
  obj->registerMessage(aslist);
  obj->registerMessage(print);
  obj->registerMessage(exit);
  obj->registerMessage(readline);
}

list<NObject*> gc;
NObject::NObject(){
  char buff[40];
  snprintf(buff, sizeof(buff), "%p", this);
  memLocation = string(buff);
  initializeObject(this);

  #ifdef DEBUG
    //std::cout << "collecting " << toString() << "\n";
  #endif

  gc.push_back(this);
}

NObject* NObject::sendMessage(string message, NList *argobjects){
  MessageHandler *handler = messages[message];
  if(handler == 0){
    throw NoMessageError(className() + " does not respond to '" + message + "'");
  }
  return handler->call(argobjects);
}

void NObject::registerMessage(MessageHandler *handler){
  string messageName = handler->messageName();
  messages[messageName] = handler;
}

/* message  foreach:name,block */
static NObject* foreachMessage(MessageHandler *handler, map<string, NObject*> &locals){
  NList *nlist = dynamic_cast<NList*>(handler->handlerReceiver());
  string varname = locals["name"]->toString();
  string clojure = locals["block"]->toString();
  list<NObject*> &intern = nlist->intern();

  for(NObject *obj : intern){
    locals[varname] = obj;
    eval(clojure, locals);
  }

  return nlist;
}

/* message  head */
static NObject* headMessage(MessageHandler *handler, map<string, NObject*> &locals){
  NList *nlist = dynamic_cast<NList*>(handler->handlerReceiver());
  if(nlist->size() == 0){
    throw Error("Empty list");
  }
  
  return nlist->first();
}

/* message  empty? */
static NObject* emptyMessage(MessageHandler *handler, map<string, NObject*> &locals){
  NList *nlist = dynamic_cast<NList*>(handler->handlerReceiver());
  return (nlist->size() == 0) ? 
              NAtom::get("true") : 
              NAtom::get("false");
}

/* message  tail */
static NObject* tailMessage(MessageHandler *handler, map<string, NObject*> &locals){
  NList *nlist = dynamic_cast<NList*>(handler->handlerReceiver());
  if(nlist->size() == 0){
    throw Error("Empty list");
  }
  nlist->popFirst();
  return nlist;
}

/* message  size */
static NObject* sizeMessage(MessageHandler *handler, map<string, NObject*> &locals){
  NList *nlist = dynamic_cast<NList*>(handler->handlerReceiver());
  stringstream ss;
  ss << nlist->size();
  return NAtom::get(ss.str());
}

NList::NList()  {
  BuiltinMessageHandler *foreach = new BuiltinMessageHandler("foreach", this, foreachMessage);
  BuiltinMessageHandler *head = new BuiltinMessageHandler("head", this, headMessage);
  BuiltinMessageHandler *tail = new BuiltinMessageHandler("tail", this, tailMessage);
  BuiltinMessageHandler *empty = new BuiltinMessageHandler("empty?", this, emptyMessage);
  BuiltinMessageHandler *size = new BuiltinMessageHandler("size", this, sizeMessage);

  foreach->setupArgs("name,block");
  registerMessage(foreach);
  registerMessage(head);
  registerMessage(tail);
  registerMessage(empty);
  registerMessage(size);
}


} /* namespace nobject */
