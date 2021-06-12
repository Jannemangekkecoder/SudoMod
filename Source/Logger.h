#pragma once
#include "stdafx.h"
/* Logger
* Info = Blue
* Debug = Green
* Error = Red + Close GTA
*/
inline void Log_Info(const char* msg, ...) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	char buffer[4096]{};
	va_list args{};

	va_start(args, msg);
	_vsnprintf(buffer, sizeof(buffer), msg, args) + 1;
	printf(buffer);
	printf("\n");
	va_end(args);
}

inline void Log_Error(const char* msg, ...) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
	char buffer[4096]{};
	va_list args{};

	va_start(args, msg);
	_vsnprintf(buffer, sizeof(buffer), msg, args) + 1;
	printf(buffer);
	printf("\n");
	va_end(args);

	WAIT(250);
	exit(0);
}

inline void Log_Debug(const char* msg, ...) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
	char buffer[4096]{};
	va_list args{};

	va_start(args, msg);
	_vsnprintf(buffer, sizeof(buffer), msg, args) + 1;
	printf(buffer);
	printf("\n");
	va_end(args);
}