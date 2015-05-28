WordOfMouth
===========

Worth of Mouth is an anonymous, distributed, encrypted communication system for use by secret societies and other organizations.

Notice
------

This is the original version of the project, following a design we came up with after one night. It is poorly written in several areas and we've since revised our design. The Master branch contains the up to date version of the project.

Overview
--------

Anyone can contact anyone else in the society if you know their name, but there’s no way to get a list of the people in the society or intercept communications between any other people. Furthermore, if any node in the network disconnects then all message passing ceases, making a trivial "kill switch" for any users involved.

The program is designed to require a minimal understanding of crypto to operate.

How it works
------------

Each computer in the network is a "node" with a key set of information:

* An IP address and port number to contact *one* other node

* The public key of the next node in the network

* A public / private keypair for receiving messages

* An alias associated with the current node, solicited at the user's discretion

Messages are relayed through the network by encrypting each leg with the public key for the next node.

There are two types of messages that can be sent through the network:

* A chat solicitation request

* A conversation message

Chat solicitation requests are temporary public keys generated for a single conversation, which are then cyphered using the alias of the individual you mean to contact. To respond, that individual also generates a temporary keypair, and sends their public key, encrypted using the public key they received in the solicitation request. 

At this stage the two users have completed a key exchange, and can communicate securely through the network without knowing one another's IP addresses.

At no time does any message have under two layers of encryption: The relay-level encryption keys to secure messages so they are unreadable from outside the network, and either a cypher (only during first stage of key exchange) or another encryption key so that only the intended recipient may read the final message.

Crypto
------

All public / private keypairs are made using GPG, while cyphers are made using OpenSSL's AES. 

Cyphers are only used during one side of an initial key exchange, and to decrypt the configuration file for the program upon startup, so during normal communication all messages are protected by two layers of PGP keys.

Requirements
------------

Aside from standard C and C++ libraries, the following will be needed to build:

* pthread

* gcrypt

* gpg-error

* openssl
