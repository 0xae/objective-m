#include <iostream>
#include <assert.h>
#include "nobject.hpp"

using nobject::NAtom;
using nobject::NObject;
using nobject::atomTable;

int main(int argc, char *argv[]){
  NObject *atom1Object = NAtom::get("atom1");
  NAtom *atom2 =  NAtom::get("atom2");

  assert(atom1Object->equals(atom1Object));
  assert(atom1Object->equals(NAtom::get("atom1")));

  assert(not atom2->equals(atom1Object));
  assert(not atom1Object->equals(atom2));
  
  delete atom1Object;
  delete atom2;

  assert(atomTable["atom1"] == 0);
  assert(atomTable["atom2"] == 0);

  return 0;
}
