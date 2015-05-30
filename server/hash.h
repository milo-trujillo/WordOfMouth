#ifndef HASH_H
#define HASH_H

// This function reads the data in src, generates a hash for it, and saves
// that hash in a string we aim *dst at. Returns success or failure.
// On failure we guarantee dst will be unallocated.

bool hash(const char* src, char** dst);

#endif
