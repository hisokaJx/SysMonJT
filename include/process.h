#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
public:
    /* Process(); // default constructor disabled, no ghost process */
    /* Process(int p, std::string u, float c, float r, long s, std::string cmd); */
    Process(int, std::string, float, std::string, long, std::string);

    int Pid();                               // TODO: See src/process.cpp
    std::string User();                      // TODO: See src/process.cpp
    std::string Command();                   // TODO: See src/process.cpp
    float CpuUtilization();                  // TODO: See src/process.cpp
    std::string Ram();                       // TODO: See src/process.cpp
    long int UpTime();                       // TODO: See src/process.cpp
    bool operator<(Process const& a) const;  // TODO: See src/process.cpp

// FINISHED: Declare any necessary private members
private:
    int pid_;
    std::string user_;
    float cpu_;
    std::string ram_;
    long sec_;
    std::string cmd_;
};

#endif
