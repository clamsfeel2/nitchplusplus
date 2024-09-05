#include "printer.h"
#include "cli_processor.h"
#include <stdexcept>
#include <iostream>

int main(int argc, char* argv[]) {
	try { 
		if(!CliProcessor::ProcessCliArgs(argc, argv)) { 
			return 0;	
		}
		Printer print;
		print.Print();
	} catch(std::invalid_argument& ex) {
		std::cout << ex.what() << std::endl;
		return 1;
	}
	return 0;
} // ends main()
