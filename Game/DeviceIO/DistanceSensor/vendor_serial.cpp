#include "../../vendor/serial/src/serial.cc"

#if defined(_WIN32)
	#pragma comment(lib, "setupapi.lib")
	#include "../vendor/serial/src/impl/win.cc"
	#include "../vendor/serial/src/impl/list_ports/list_ports_win.cc"
#elif defined(__APPLE__)
	#include "../vendor/serial/src/impl/unix.cc"
	#include "../vendor/serial/src/impl/list_ports/list_ports_osx.cc"
#else
	#include "../vendor/serial/src/impl/unix.cc"
	#include "../vendor/serial/src/impl/list_ports/list_ports_linux.cc"
#endif
