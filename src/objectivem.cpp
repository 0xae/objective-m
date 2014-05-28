#include <iostream>
#include <sstream>
#include <string>
#include <assert.h>

#include "nobject.hpp"
#include "parser.hpp"
#include "nutils.hpp"
#include "nerror.hpp"

using std::string;
using std::stringstream;
using nerror::Error;

using namespace nobject;
using namespace nutils;
using namespace nparser;

static inline char lastc(string s){
  return s[s.size() - 1];
}

int main(int argc, char *argv[]){
  string line;
  map<string, NObject*> locals;
  NObject *obj;
  locals["Runtime"] = new NObject();
  std::cout << "> ";

  while(getline(std::cin, line)){
    if(line == ""){
      std::cout << "> ";
      continue;
    }

    try{
      obj = parseExpression(line, locals);
      std::cout << "=> "
                << obj->toString()
                << std::endl;

    }catch(Error &e){
      std::cout << "Error: " 
	              << e.errorMsg()
	              << "\n";
    }
    std::cout << "> ";

  }
  
  return 0;
}


