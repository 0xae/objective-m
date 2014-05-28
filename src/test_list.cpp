#include <iostream>
#include <assert.h>
#include "nobject.hpp"

using nobject::NList;
using nobject::NObject;
using nobject::NAtom;

int main(int argc, char *argv[]){
  NAtom *value1 = NAtom::get("value1");
  NAtom *value2 = NAtom::get("value2");
  NList list;
  NObject *ptr = &list;

  assert(ptr->equals(ptr));
  assert(ptr->equals(&list));
  assert(list.equals(&list));
  assert(list.equals(ptr));

  list << value1;
  list << value2;
  assert(list.size() == 2);

  assert(list.popLast() == value2);
  assert(list.popLast() == value1);
  assert(list.size() == 0);

  list.shift(value2);
  list.shift(value1);
  assert(list.size() == 2);

  assert(list.popFirst() == value1);
  assert(list.popFirst() == value2);
  assert(list.size() == 0);

  return 0;
}
