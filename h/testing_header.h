#pragma once
#include <sstream>
#include <iostream>

int main(int argc, char* argv[]);

class RedirectOutput {
public:
    RedirectOutput() {
        originalCout = std::cout.rdbuf();
        originalCerr = std::cerr.rdbuf();
        std::cout.rdbuf(coutStream.rdbuf());
        std::cerr.rdbuf(cerrStream.rdbuf());
    }

    ~RedirectOutput() {
        std::cout.rdbuf(originalCout);
        std::cerr.rdbuf(originalCerr);
    }

    std::string getCoutOutput() const {
        return coutStream.str();
    }

    std::string getCerrOutput() const {
        return cerrStream.str();
    }

private:
    std::streambuf* originalCout;
    std::streambuf* originalCerr;
    std::ostringstream coutStream;
    std::ostringstream cerrStream;
};
