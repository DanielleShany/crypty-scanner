#include "cryptyScanner.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <directory> <signature_file>\n";
        return 1;
    }

    return run(argv[1], argv[2]);
}
