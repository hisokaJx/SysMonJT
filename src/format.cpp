#include <string>

#include "format.h"

using std::string;

// FINISHED: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
/* string Format::ElapsedTime(long seconds[[maybe_unused]]) { return string(); } */
string Format::ElapsedTime(long seconds) {
    int sec{0};
    int min{0};
    int hr{0};
    string HH, MM, SS;

    sec = seconds % 60;	
    min = seconds / 60;
    hr  = min / 60;
    min = min % 60;

    HH = std::to_string(hr);
    if ( hr < 10 ) HH = "0" + HH;
    MM = std::to_string(min);
    if ( min < 10 ) MM = "0" + MM;
    SS = std::to_string(sec);
    if ( sec < 10 ) SS = "0" + SS;

    return HH + ":" + MM + ":" + SS;
}
