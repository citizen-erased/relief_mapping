#include "precompiled.h"
#include "logging.h"

void print(const std::string &str)
{
    std::cout << str;
}

void print(const format &fmt)
{
    print(boost::str(fmt));
}

void warning(const std::string &str)
{
    print("WARNING: ");
    print(str);
}

void warning(const format &fmt)
{
    warning(boost::str(fmt));
}

void fatal(const std::string &str)
{
    print("FATAL: ");
    print(str);

    if(!boost::ends_with(str, "\n"))
        print("\n");

    exit(-1);
}

void fatal(const format &fmt)
{
    fatal(boost::str(fmt));
}

