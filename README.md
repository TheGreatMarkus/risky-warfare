# risky-warfare
A stategy game based on Risk implemented in C++

## Memory leak checking
Checks for memory leaks were done using valgrind;
valgrind --leak-check=full --tool=memcheck ./path/to/executable

## Notes

- I'm using the copy and swap idiom for my classes to avoid duplicating code for the copy constructor and assignment operator of my classes.
  - See: https://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom
- Documentation is given for non-trivial classes, methods, free functions, and operators
  - Documentation for classes is given in header files
  - Documentation for everything else is given in source files.