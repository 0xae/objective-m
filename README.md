objective-m
===========

objective-m is an primitive implementation
of the message-passing and object-oriented programming
pionered by smalltalk and Io.
In other words, it means that everything (including conditionals and looping) is
done through message-passing.

ex:
  

Syntax
=============


Building up things
===============
objective-m is written in pure ISO C++, so
it shall compile fine on unix/linux.
To build the shell run:

    cd src && make
    bin/objm
  
  

[yourname =: [IO readline]]
[Func ifelse: [yourname ==: @"ayrtoni"],[@"ayrtoni mentioned" println],{[@"Some dude " print];[yourname println]}]
