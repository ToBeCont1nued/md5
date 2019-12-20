#include <iostream>
#include <iomanip>
#include <cstring>
#include "md5.h"

using std::cout;		using std::endl;
using std::hex;			using std::setw;
using std::setfill;

int main(int argc, char* argv[])
{
	const char* message = argv[1];
	uint8_t digest[32];
	md5(message, strlen(message), digest);
	for (int i = 0; i != 16; ++i)
		cout << hex << setw(2) << setfill('0') << (uint32_t)digest[i];
	cout << endl;
	return 0;
}
