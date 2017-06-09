objective-m
===========

objective-m is an unstable and buggy and primitive implementation
of message-passing and object-oriented programming
pionered by smalltalk and Io.
In other words, it means that everything
(including conditionals and looping) is
done through message-passing.


Building up things
===============
objective-m is written in pure ISO C++0x, so
it shall compile fine on unix/linux.
To build the shell run:

    cd src && make
    bin/objm


Example
================
Read something from the user,
convert it to an atom,
and doing some comparisons
with it.

    [yourname =: [IO readline]]
    [Func ifelse: [yourname ==: @"ayrtoni"],
                  [@"ayrtoni mentioned" println],
                  {
                    [@"Some dude " print];
                    [yourname println]
                  }]



