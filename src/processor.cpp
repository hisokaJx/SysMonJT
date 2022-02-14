#include "processor.h"

// FINISHED: Return the aggregate CPU utilization
float Processor::Utilization() {
    // debug stuff
    /* std::ofstream myfile;//DEBUG */
    /* myfile.open ("debug.txt");//DEBUG */
    /* myfile << "inside Processor::Utilization()\n";//DEBUG */
    // debug stuff

    /* long idle; */
    /* float active, percent; */

    /* idle = LinuxParser::IdleJiffies(); */
    active = LinuxParser::ActiveJiffies();
    total = LinuxParser::Jiffies();
    percent = active / total;

    /* myfile << "active: " << active << "\n"; */
    /* myfile << "total: " << total << "\n"; */
    /* myfile << "percent: " << percent << "\n"; */

    /* myfile.close();//DEBUG */

    return percent;
    /* return 0.5; */
}
