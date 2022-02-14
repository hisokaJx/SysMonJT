#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

/* Process::Process() {} */

// constructor
Process::Process(int pid, std::string user, float cpu, std::string ram, long sec, std::string cmd)
    : pid_{pid},
    user_{user},
    cpu_{cpu},
    ram_{ram},
    sec_{sec},
    cmd_{cmd} {};

// FINISHED: Return this process's ID
int Process::Pid() {
    return pid_;
}

// FINISHED: Return this process's CPU utilization
float Process::CpuUtilization() {
    return cpu_;
}

// FINISHED: Return the command that generated this process
string Process::Command() {
    return cmd_;
}

// FINISHED: Return this process's memory utilization
string Process::Ram() {
    return ram_;
}

// FINISHED: Return the user (name) that generated this process
string Process::User() {
    return user_;
}

// FINISHED: Return the age of this process (in seconds)
long int Process::UpTime() {
    return sec_;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const {
    return true;
}
