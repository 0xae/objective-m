#include <map>
#include <string>
#include "nobject.hpp"
#include "nutils.hpp"
#include "nerror.hpp"

#ifndef NPARSER_H_
#define NPARSER_H_

using nobject::NObject;
using nobject::NString;
using nobject::NList;
using std::string;
using std::map;

namespace nparser{

NObject* getVar(string name);
NObject* storeVar(string name, NObject *value);

NString* parseString(string value);
NList*   parseList(string value, map<string, NObject*> &locals, bool lazy=false);
NObject* parseAtom(string value, map<string, NObject*> &locals);
NObject *parseExpression(string expr, map<string, NObject*> &locals);
NObject *eval(string value, map<string, NObject*> &locals);

} /* namespace nparser */

#endif /* NPARSER_H_ */

