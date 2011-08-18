#ifndef LOGGING_H
#define LOGGING_H

#include <boost/format.hpp>
#include <string>

using boost::format; //HACK everything can use boost::format!

void print(const format &fmt);
void print(const std::string &str);

void warning(const std::string &str);
void warning(const boost::format &fmt);

void fatal(const std::string &str);
void fatal(const boost::format &fmt);


#endif /* LOGGING_H */

