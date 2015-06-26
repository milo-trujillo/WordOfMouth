/*
	LibNettle 3 and up have a "version" header file containing the major and
	minor versions of the library. Unfortunately we strive for compatibility
	with LibNettle 2, which lacks this header or any version marker.

	To solve the problem we attempt to build this header file, and if we
	cannot then we must be on LibNettle 2 (or no nettle is present, in which
	case everything will fail to build anyway).
*/

#include <nettle/version.h>
