#include <iostream>
#include <iomanip>
#include <cstring>
#include "md5.h"

using std::cout;		using std::endl;
using std::hex;			using std::setw;
using std::setfill;

int main(int argc, char* argv[])
{
	if (argc != 2) {
		cout << "format: " << argv[0] <<
			" <filename>" << endl;
		return -1;
	}
	// const char *message = argv[1];
	const char *filename = argv[1];
	uint8_t digest[16];
	// md5(message, strlen(message), (char*)digest);
	if (!fmd5(filename, (char*)digest))
		return -1;
	for (int i = 0; i != 16; ++i)
		cout << hex << setw(2) << setfill('0') << (uint32_t)digest[i];
	cout << endl;
	return 0;
}
