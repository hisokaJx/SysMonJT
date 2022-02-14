#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() {
    return cpu_;
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
    // debug stuff
    std::ofstream myfile;//DEBUG
    myfile.open ("debug.txt");//DEBUG
    myfile << "inside System::Processes()\n";
    // debug stuff

    // init vars
    vector<int> pids = LinuxParser::Pids();
    string this_uid;
    string this_user;
    float this_cpu;
    string this_ram;
    long this_uptime;
    string this_cmd;

    // loop
    for (auto i : pids) {
	this_uid = LinuxParser::Uid(i);
	this_user = LinuxParser::User(this_uid);
	this_uptime = LinuxParser::UpTime(i);
	this_cmd = LinuxParser::Command(i);
	this_ram = LinuxParser::Ram(i);
	this_cpu = LinuxParser::CpuUtilization(i);
	myfile << "PID: " << i << "; User: " << this_user << "; this_uptime: " << this_uptime << "; ";//DEBUG
	myfile << "CMD: " << this_cmd << "; " << "RAM: " << this_ram << "\n";
	myfile << "CPU: " << this_cpu << "\n";
	// get info about this process and assign to vector
	/* Process::Process(int pid, std::string user, float cpu, std::string ram, long sec, std::string cmd) */
	Process this_proc{i, this_user, this_cpu, this_ram, this_uptime, this_cmd};
	processes_.push_back(this_proc);
    }

    myfile.close();//DEBUG

    return processes_;
}


// DONE
std::string System::Kernel()		{ return LinuxParser::Kernel(); }
std::string System::OperatingSystem()	{ return LinuxParser::OperatingSystem(); }
float System::MemoryUtilization()	{ return LinuxParser::MemoryUtilization(); } // the first % always reutrn 0; same as in the solution video
long int System::UpTime()		{ return LinuxParser::UpTime(); }
int System::TotalProcesses()		{ return LinuxParser::TotalProcesses(); }
int System::RunningProcesses()		{ return LinuxParser::RunningProcesses(); }
