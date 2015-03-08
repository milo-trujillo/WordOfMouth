#include <iostream> // For strings

using namespace std;

// Null out the string before clearing it
void scrub(string& s)
{
	for( int i = s.size() - 1; i >= 0; i-- )
		s[i] = 0;
	s.clear();
}
