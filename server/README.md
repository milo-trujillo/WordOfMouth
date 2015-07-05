# Server

The Word of Mouth "server" is written in C++. It both relays messages between other relays and listens on localhost for new instructions.

## Design

We're trying to keep the code logic modular. Each type of encryption is in its own file (RSA key encryption, AES cyphers, and SHA hashing), and only knows how to operate on strings.

Messages are bundled up into a class so that only messages have to know how they're implemented and what type of crypto to call where.

Networking is probably the highest level of logic, as it has to parse config information, read and decrypt messages sent over the wire, and create messages and encrypt them to send over the wire.

All other modules (logging, config files, and daemonization) are confined to their own bubbles with little knowledge of the rest of the code base.

## Installation

You should be able to build with a simple `make && make test` or `gmake && gmake test`. If that is proving insufficient then see the *INSTALL* file for troubleshooting suggestions.

## Code style

Variables, functions, and methods are all in camelCase. Class names are in UpperCamelCase.

Constants are in UPPER_UNDERSCORE_CASE

Indentation is done with tabs, comment alignment with spaces.

Braces are on the next line, like:

    void foo()
    {
        for( something )
        {
            switch( condition )
            {
                case 'a':
                    break;
            }
        }
    }

All other formatting is up to the programmer's discretion, but try to fit the style of the code you're working in.
