#ifndef SCRUB_H
#define SCRUB_H

#include <iostream> // For strings

/*
	These functions are for scrubbing memory or disk files after we're done with
	them to limit the chance of leaking sensitive data.
*/

void scrub(std::string& s);

#endif
