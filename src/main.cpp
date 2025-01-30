//-----------------------------------------------------------------------------
#include <cstdlib>
#include <iostream>
#include <string_view>
//-----------------------------------------------------------------------------
#include "exceptions.h"
#include "freq.h"
//-----------------------------------------------------------------------------
constexpr std::string_view HELP =
    "Run the program:\n"
    "./freq inp.txt out.txt\n"
    "    ./freq  - program name\n"
    "    inp.txt - input file with the data\n"
    "    out.txt - output file with report\n";
//-----------------------------------------------------------------------------

void printHelp()
{
    std::cout << HELP <<std::endl;
}
//-----------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    auto status = EXIT_FAILURE;
    try
    {
        if(argc != 3)
            throw Error::ArgumentCountException();

        Logic::Freq freq(argv[1], argv[2]);
        freq.Run();
        status = EXIT_SUCCESS;
    }
    catch (const Error::ArgumentCountException &ex)
    {
        std::cout<< ex.what() <<std::endl;
        printHelp();
    }
    catch(const std::exception& ex)
    {
        std::cout<< ex.what() <<std::endl;
    }
    return status;
}
