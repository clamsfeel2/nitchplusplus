#include <cstdlib>
#include <iostream>
#include "cli_processor.h"
#include "printer.h"

int main(int argc, char* argv[]) {
    try {
        if(CliProcessor::ProcessCliArgs(argc, argv)) Printer::Print();
    } catch(const std::invalid_argument& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
