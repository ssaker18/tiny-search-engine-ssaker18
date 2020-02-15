# Grade Lab 4

Grader: Carlos Yepes

## Total Grade: 59
-10% one day late

### Delivery
- -2 you don't have a common directory at all D: that isn't good since you need a shared functions folder to use for the other two assignments ):
- otherwise everything does what it should

18/20 points

### Documentation and Testing
- -2 your README.md barely has anything
- -6 no IMPLEMENTATION.md ):
- -2 no assumptions anywhere

12/22 points

### Code Quality
- -2 a large portion of your crawler is in your main, try to keep things modular and separate into more functions, this is especially more necessary since you didn't have a common directory with a `pagedir` module to store common functions in.
- good job with null checks
- nice job overall

18/20 points

### Functionality
- there was no memory leaks but there was an OBSCENE amount of errors, I think it stems from all of your invalid frees, which happen in your main under all those if statements.
```
==4164107== 
==4164107== HEAP SUMMARY:
==4164107==     in use at exit: 0 bytes in 0 blocks
==4164107==   total heap usage: 1,304 allocs, 1,313 frees, 294,412 bytes allocated
==4164107== 
==4164107== All heap blocks were freed -- no leaks are possible
==4164107== 
==4164107== For lists of detected and suppressed errors, rerun with: -s
==4164107== ERROR SUMMARY: 10765 errors from 52 contexts (suppressed: 0 from 0)
```
5 points
#### Testing
- passed tests 1-7, 11 passed, but with many errors

16 points

- did not pass 8-10, 12-15 :(, it didn't save any of the results for the tests, and many core dumps


21/38 points
