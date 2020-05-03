#include <string>
#include<sstream>

#include "format.h"

using std::string;

// Done: Complete this helper function
string Format::ElapsedTime(long seconds) { 
    std::ostringstream format ;
    int hrs = seconds/3600;
    int mins = (seconds%3600)/60;
    int sec = (seconds%3600)%60;
    format<<hrs<<":"<<mins<<":"<<sec;
    return format.str();
}