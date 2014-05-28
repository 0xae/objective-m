#ifndef NERROR_H_
#define NERROR_H_

namespace nerror{

class Error {
  string msg;
public:
  Error(string err): msg(err){ }
  string errorMsg() const {
    return msg;
  }
};

class MatchError : public Error {
public:
  MatchError(string err) : Error(err){}
};

class ExpressionError : public Error{
public:
  ExpressionError(string err) : Error(err){}
};

class NoMessageError : public Error{
public:
  NoMessageError(string err) : Error(err){}
};

class NotBoundError : public Error{
public:
  NotBoundError(string err) : Error(err){}
};

}/* namespace nerror */

#endif /* NERROR_H_ */

