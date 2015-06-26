Test Harness
============

This is the Word of Mouth test harness, which tries to make sure each component of the program works on your local system. To run the tests type: `make && make test`

## Adding tests

When developing your own tests try to write them to test as small a segment of Word of Mouth as possible, so different components can be tested independently. To fit the test format your test should output one line, like:

    [+] Foo works correctly
    [-] Something went wrong testing foo

Any other test output should be inside `#ifdef VERBOSE` blocks.
