#include <iostream>
#include <string>
#include <list>
#include <map>

#ifndef NOBJECT_H_
#define NOBJECT_H_

using std::string;
using std::list;
using std::map;

namespace nhandler {
  class MessageHandler;
  class BuiltinMessageHandler;
  class SrcMessageHandler;
}

namespace nobject {

class NObject;
class NAtom;
class NString;
class NList;

using nhandler::MessageHandler;

/* Class name id */
const string kObjectClassName = "NObject";
const string kAtomClassName   = "NAtom";
const string kStringClassName = "NString";
const string kBlockClassName  = "NBlock";
const string kListClassName   = "NList";
static map<string, NObject*> emptylocals;

class NObject{
  string memLocation;
  map<string, MessageHandler*> messages;

public:
  NObject();

  virtual ~NObject(){
  }

  virtual string className(){
    return kObjectClassName;
  }

  virtual string toString(){
    return memLocation;
  }

  virtual bool equals(NObject *obj){
    return this == obj;
  }

  string memoryAddress() const {
    return memLocation;
  }
  
  NObject *sendMessage(string message, NList *argobjects);
  void registerMessage(MessageHandler *handler);
};

class NList : public NObject{
  list<NObject*> elements;

public:
  NList();
  int size() {
    return elements.size();
  }
  
  NList* operator<<(NObject *obj){
    add(obj);
    return this;
  }
  
  list<NObject*>& intern(){
    return elements;
  }

  NObject* shift(NObject *obj){
    elements.push_front(obj);
    return obj;
  }

  NObject* add(NObject *obj){
    elements.push_back(obj);
    return obj;
  }

  NObject* first(){
    if(elements.size() == 0){
      return 0; /* TODO: replace this */
    }
    NObject *obj = elements.front();
    return obj;
  }

  NObject* last(){
    if(elements.size() == 0){
      return 0; /* TODO: replace this */
    }
    NObject *last = elements.back();
    return last;
  }

  NObject* popLast(){
    NObject *obj = last();
    if(obj != 0){ /* TODO: replace this */
      elements.pop_back();
    }
    return obj;
  }

  NObject* popFirst(){
    NObject *obj = first();
    if(obj != 0){ /* TODO: replace this */
      elements.pop_front(); 
    }
    return obj;
  }

  virtual bool equals(NObject *obj){
    NList *list = dynamic_cast<NList*>(obj);
    if(list == 0){
      return false;
    }

    if(size() == 0 and list->size() == 0){
      return true;
    }

    return this == list;
  }

  virtual string className(){
    return kListClassName;
  }

  virtual string toString(){
    return "<list object at " + memoryAddress() + ">";
  }
};

class NBlock : public NList {
public:
  virtual string toString(){
    return "<block object at " + memoryAddress() + ">";
  }

  virtual string className(){
    return kBlockClassName;
  }
};

void cleanUpAtomTable();
static map<string, NAtom*> atomTable;
class NAtom : public NObject{
  string name;
  NAtom(string _name) : name(_name){
  }

public:
  static NAtom* get(string name){
    NAtom *atom = atomTable[name];
    if(atom == 0){
      atom = new NAtom(name);
      atomTable[name] = atom;
    }
    return atom;
  }

  virtual ~NAtom(){
    #ifdef DEBUG
    std::cout << "[debug::NAtom] free '" << name << "'\n";
    #endif
    atomTable[name] = 0;
  }

  virtual string className(){
    return kAtomClassName;
  }

  virtual string toString(){
    return name;
  }

  virtual bool equals(NObject *obj){
    NAtom *atom = dynamic_cast<NAtom*>(obj);
    return (atom != 0) and (atom->name == name);
  }
};


class NString : public NObject{
  string buffer;
public:
  NString(string _buffer) : 
    buffer(_buffer)
  {
  }

  virtual ~NString(){
    #ifdef DEBUG
    std::cout << "[debug::NString] free '" << buffer << "'\n";
    #endif
  }

  string data() const {
     return buffer;
  }

  virtual string className(){
    return kStringClassName;
  }

  virtual string toString(){
    return buffer;
  }

  virtual bool equals(NObject *obj){
    NString *str = dynamic_cast<NString*>(obj);
    return (str != 0) and (str->buffer == buffer);
  }
};

}  /* namespace nobject */

#endif  /* NOBJECT_H_ */

