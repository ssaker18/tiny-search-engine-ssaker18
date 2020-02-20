# Grading: Lab 5, Sebastian Saker
## Total: 56/100

Raw Score: 76/100

* -20 for 2 days late.

### Overall: 6/10

* -2 for not committing more regularly. You should be committing to the repo about once a day.
* -1 for not having a `.gitignore` in your `indexer` directory.
* -1 for having `.DESIGN.md.swp` editor backup file in your directory, along
with `.gch` files in your `libcs50` directory.
* `indextester` was supposed to be called `indextest`.

### Makefiles: 10/10

* Initially did not compile. Got the following errors:
```
/usr/bin/ld: ../common/common.a(pagedir.o): in function `extract_words':
/net/ifs-users/jaswenberg/cs50grading/lab5/tiny-search-engine-ssaker18/common/pagedir.c:216: undefined reference to `counters_new'
/usr/bin/ld: /net/ifs-users/jaswenberg/cs50grading/lab5/tiny-search-engine-ssaker18/common/pagedir.c:221: undefined reference to `counters_add'
/usr/bin/ld: /net/ifs-users/jaswenberg/cs50grading/lab5/tiny-search-engine-ssaker18/common/pagedir.c:225: undefined reference to `counters_add'
/usr/bin/ld: ../common/common.a(index.o): in function `save_item':
/net/ifs-users/jaswenberg/cs50grading/lab5/tiny-search-engine-ssaker18/common/index.c:41: undefined reference to `counters_iterate'
/usr/bin/ld: ../common/common.a(index.o): in function `ct_delete':
/net/ifs-users/jaswenberg/cs50grading/lab5/tiny-search-engine-ssaker18/common/index.c:52: undefined reference to `counters_delete'
/usr/bin/ld: ../common/common.a(index.o): in function `index_load':
/net/ifs-users/jaswenberg/cs50grading/lab5/tiny-search-engine-ssaker18/common/index.c:84: undefined reference to `counters_new'
/usr/bin/ld: /net/ifs-users/jaswenberg/cs50grading/lab5/tiny-search-engine-ssaker18/common/index.c:87: undefined reference to `counters_set'
collect2: error: ld returned 1 exit status
make: *** [Makefile:17: indexer] Error 1
```
Looks like the linking order was wrong. I fixed the problem by switching
`../common/common.a` and `../libcs50/libcs50.a` in your Makefile.

### Documentation: 7/10

* For headers in markdown to work, you need to put a space after the `#`:
```
# Title
```
* -1 for not using markdown headers or code blocks in most of your `DESIGN.md`
or your `IMPLEMENTATION.md`. It makes it harder to read!
* -2 for not filling in your `IMPLEMENTATION.md` with actual implementation info.

### Testing: 7/10

* -1 for not separately testing unreadable directory and unwritable file tests.
* -2 for not pushing `testing.out` to your repo.

### Coding Style: 18/20

* -2 for having indexer-specific code (`index_build`) in `pagedir.c` instead of `indexer.c`. This is bad form.
* You reuse the code for `webpage_getNextWord`, but you could have just called this method!
You could have made a new `webpage_t` with the proper HTML, then called the function. That way you avoid rewriting an identical function.

### Functionality: 22/30

* -8 for failing all of our test cases with `indexer`. See below.
* Your code segfaults when there's lots of HTML. For instance,
`./indexer ~cs50/data/tse-output/toscrape-depth-0 test.index` leads to a segfault.
This is because you assume in your code that a file doesn't have more than 4000 characters.
This assumption is quickly incorrect. Instead, you should use heap memory
and resize when your buffer is full.
* Even with proper inputs, your `indexer` eventually leads to a seg fault.
It appears the problem is you are trying to do a double free on line 226 of `pagedir.c`
(in your `extract_words` function). This is fixed by wrapping these two lines (225 and 226)
in an `else`.
* When both of these errors are fixed, there still is an issue with
`indextest`. You save your index with a colon after each word, and don't
account for it when you call `index_load`. `indextest` then gives a file
where each word is followed by 2 colons.
* Also, on line 186 of `pagedir.c`, you have
```
snprintf(buffer, sizeof(int) + sizeof(char) * strlen(dir), "%s/%d", dir, id);
```
But `sizeof(int)` is not the string length you are looking for. In fact, in
this case, it is 2. Thus if the id of the current page is over 2 digits (in toscrape, for instance),
the id will not be printed all the way. This causes your code to run
indefinitely (infinite loop) in these cases because instead of reading file 586
(which doesn't exist in toscrape-depth-2), it reads file 58.
* Even when all of these errors are accounted for, `indexer` is building the
wrong index. Just try `~cs50/data/tse-output/letters-depth-2` and compare the
resulting index with `~cs50/data/tse-outtput/letters-index-2`. It seems to be
because you forget to strip the first two lines of each page file, containing
the URL and depth of the current page.

### Memory Leaks: 6/10

* -4: lots of memory errors. All "conditional jump or move depends on uninitialised value(s)" errors.
Just a small snippet from valgrind:
```
==733716== Conditional jump or move depends on uninitialised value(s)
==733716==    at 0x483CA8D: strncat (vg_replace_strmem.c:351)
==733716==    by 0x401861: extract_words (pagedir.c:197)
==733716==    by 0x401A00: index_build (pagedir.c:246)
==733716==    by 0x40130C: main (indexer.c:57)
==733716== 
==733716== Conditional jump or move depends on uninitialised value(s)
==733716==    at 0x483CC58: __strlen_sse2 (vg_replace_strmem.c:463)
==733716==    by 0x4023FC: JenkinsHash (jhash.c:20)
==733716==    by 0x402216: hashtable_find (hashtable.c:104)
==733716==    by 0x4018FD: extract_words (pagedir.c:213)
==733716==    by 0x401A00: index_build (pagedir.c:246)
==733716==    by 0x40130C: main (indexer.c:57)
==733716== 
==733716== Conditional jump or move depends on uninitialised value(s)
==733716==    at 0x483CC58: __strlen_sse2 (vg_replace_strmem.c:463)
==733716==    by 0x4023FC: JenkinsHash (jhash.c:20)
==733716==    by 0x4021A2: hashtable_insert (hashtable.c:85)
==733716==    by 0x401964: extract_words (pagedir.c:222)
==733716==    by 0x401A00: index_build (pagedir.c:246)
==733716==    by 0x40130C: main (indexer.c:57)
```
Not sure what's causing this error, but I think it has to do with `html` never being initialized.
Instead of `strncat`, you should use `strncat` the first time you add something to the string.


