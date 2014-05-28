#include <string>

#ifndef NUTILS_H_
#define NUTILS_H_

using std::string;

namespace nutils{

string &ltrim(string &s);
string &rtrim(string &s);
string &trim(std::string &s);
bool isUpper(char c);
bool isLower(char c);
bool isAlpha(char c);
bool isNum(char c);
bool isAlphaNum(char c);
bool isQuote(char c);
bool isSpace(char c);
bool isContainer(char c);
bool isValidAtom(string s);
int findSafeSeparator(string src, char sep);


} /* namespace nutils */

#endif /* NUTILS_H_ */


