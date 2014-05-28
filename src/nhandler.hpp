#include <list>
#include <string>
#include <map>
#include "nobject.hpp"

using std::list;
using std::string;
using std::map;
using nobject::NObject;

#ifndef NHANDLER_H_
#define NHANDLER_H_

namespace nhandler{

class MessageHandler{
protected:
  string name;
  list<string> args;
  NObject *receiver;
  int argc;

public:
  MessageHandler(string _name, NObject *_receiver);

  string messageName() const {
    return name;
  }

  NObject* handlerReceiver() const {
    return receiver;
  }

  void setupArgs(string argslist);
  void setupArgValues(NList *argvalues, map<string, NObject*> &locals);
  virtual NObject* call(NList* locals) = 0;
};


typedef NObject* BuiltinCallback(MessageHandler *handler,
                                  map<string, NObject*> &locals);
class BuiltinMessageHandler : public MessageHandler{
  BuiltinCallback *callback;
public:
  BuiltinMessageHandler(string name, NObject *_receiver,
                        BuiltinCallback *_callback) : 
    MessageHandler(name,_receiver),
    callback(_callback)
  {
  }
  virtual NObject* call(NList* objargs);
};


class SrcMessageHandler : public MessageHandler{
  string block;

public:
  SrcMessageHandler(string name, NObject *_receiver,
		                string buffer) :
	  MessageHandler(name,_receiver),
		block(buffer)
  {
  }
  virtual NObject* call(NList* objargs);
};

} /* namespace nhandler */

#endif /* NHANDLER_H_ */






