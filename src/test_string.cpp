#include <iostream>
#include <assert.h>
#include "nobject.hpp"

using nobject::NString;

int main(int argc, char *argv[]){
  NString str1("hello world");
  NString str2("ola mundo");

  assert(str1.equals(&str1));
  assert(not str1.equals(&str2));

  assert(str2.equals(&str2));
  assert(not str2.equals(&str1));

  return 0;
}
