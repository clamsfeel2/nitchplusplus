#pragma once

namespace C {
	// Reset Color
	constexpr const char* NC  = "\033[0m";

	// DIM
	constexpr const char* DIM="\e[2m";

	// White
	constexpr const char* B_WHITE  = "\033[1;38m";
	constexpr const char* CO_WHITE  = "\033[9;38m";
	constexpr const char* B_CO_WHITE  = "\033[1;9;38m";
	constexpr const char* U_WHITE = "\033[4;38m";
	constexpr const char* B_U_WHITE = "\033[1;4;38m";

	// Purple 
	constexpr const char* PURPLE   = "\033[0;95m";
	constexpr const char* U_PURPLE  = "\033[0;4;95m";
	constexpr const char* B_PURPLE  = "\033[1;95m";
	constexpr const char* B_U_PURPLE = "\033[1;4;95m";

	// Green
	constexpr const char* G = "\033[0;32m";
	constexpr const char* GREEN = "\033[0;38;5;114m";
	constexpr const char* BRIGHT_GREEN = "\033[1;38;5;114m";
	constexpr const char* CO_BRIGHT_GREEN = "\033[1;9;38;5;114m";
	constexpr const char* CO_GREEN   = "\033[9;32m";
	constexpr const char* U_GREEN  = "\033[0;4;32m";
	constexpr const char* B_GREEN  = "\033[1;38;5;114m";
	constexpr const char* B_U_GREEN = "\033[1;4;32m";

	// Yellow
	constexpr const char* YELLOW     = "\033[0;33m";
	constexpr const char* BRIGHT_YELLOW     = "\033[0;38;5;229m";
	constexpr const char* U_YELLOW    = "\033[0;4;33m";
	constexpr const char* B_YELLOW    = "\033[1;33m";
	constexpr const char* B_U_YELLOW   = "\033[1;4;33m";
	constexpr const char* CO_YELLOW = "\033[9;33m";

	// Red
	constexpr const char* RED   = "\033[0;31m";
	constexpr const char* U_RED  = "\033[0;4;31m";
	constexpr const char* B_RED  = "\033[1;31m";
	constexpr const char* B_U_RED = "\033[1;4;31m";

	// Blue
	constexpr const char* BLUE   = "\033[0;94m";
	constexpr const char* BRIGHT_BLUE = "\033[0;38;5;117m";
	constexpr const char* B_BRIGHT_BLUE = "\033[1;38;5;117m";
	constexpr const char* U_BLUE  = "\033[4;34m";
	constexpr const char* B_BLUE  = "\033[1;34m";
	constexpr const char* B_U_BLUE = "\033[1;4;34m";
};
