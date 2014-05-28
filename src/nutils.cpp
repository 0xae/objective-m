#include <string>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include "nutils.hpp"

using std::string;
using std::find_if;

namespace nutils{

/*
  Find more information about trimming here:
  http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
*/

/* trim from start */
std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

/* trim from end */
std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

/* trim from both ends */
std::string &trim(std::string &s) {
  return ltrim(rtrim(s));
}

bool isUpper(char c){
  return (c >= 'A' and  c <= 'Z');
}

bool isLower(char c){
  return (c >= 'a' and c <= 'z');
}

bool isAlpha(char c){
  return (isLower(c) or isUpper(c));
}

bool isNum(char c){
  return (c >= '0' and c <= '9');
}

bool isAlphaNum(char c){
  return (isAlpha(c) or isNum(c));
}

bool isQuote(char c){
  return (c == '\'' or
	       c == '"');
}

bool isSpace(char c){
  return (c == ' '  or
	  c == '\t' or
	  c == '\f' or
	  c == '\n' or
	  c == '\r');
}

bool isContainer(char c){
  return (c == '[' or c == ']') or
          (c == '(' or c == ')') or
          (c == '{' or c == '}');
}

/* 
  A valid atom cannot contain a container character,
  a quote character and a space character.
  SEE functions above.
*/
bool isValidAtom(string s){
  return s != "" and 
          find_if(s.begin(), s.end(), isContainer) == s.end()  and
          find_if(s.begin(), s.end(), isSpace) == s.end() and
          find_if(s.begin(), s.end(), isQuote) == s.end();
}

/*
  Find a safe sep(outside a string
                  or a container(list, block) )
*/
int findSafeSeparator(string src, char sep){
  char c;
  int len = src.size();
  int count = 0;

  for(int i=0; i<len; i++){
    c = src[i];
    if(c == sep and count == 0){
      return i;
    }
    if(c == '(' or c == '{' or c == '@' or c == '['){
      if(c == '@'){
	      if(i == len-1){
	        break;
	      }
	      if(src[i+1] != '"'){
	        continue;
	      }
	      i += 1;
      }

      count += 1;
    }else if(c == ')' or c == '}' or c == '"' or c == ']'){
      count -= 1;
    }
  }

  return -1;
}

} /* namespace nutils */


