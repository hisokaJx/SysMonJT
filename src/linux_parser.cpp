#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
// -------------------- debug stuff --------------------
#include <iostream> //DEBUG
#include <fstream> //DEBUG
// -------------------- end debug stuff --------------------

#include "linux_parser.h"

using std::stof;
using std::stol; // for uptime calc
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// TODO/BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
    vector<int> pids;
    DIR* directory = opendir(kProcDirectory.c_str());
    struct dirent* file;
    while ((file = readdir(directory)) != nullptr) {
	// Is this a directory?
	if (file->d_type == DT_DIR) {
	    // Is every character of the name a digit?
	    string filename(file->d_name);
	    if (std::all_of(filename.begin(), filename.end(), isdigit)) {
		int pid = stoi(filename);
		pids.push_back(pid);
	    }
	}
    }
    closedir(directory);
    return pids;
}

// FINISHED: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
    // According to the following post, use MemTotal - MemFree
    //https://stackoverflow.com/questions/41224738/how-to-calculate-system-memory-usage-from-proc-meminfo-like-htop/41251290#41251290

    // debug stuff
    /* std::ofstream myfile;//DEBUG */
    /* myfile.open ("debug.txt");//DEBUG */
    /* myfile << "inside LinuxParser::MemoryUtilization()\n";//DEBUG */

    // memory vars
    bool bTotal{false};
    bool bFree{false};
    float mTotal{0};
    float mFree{0};
    float usage{0};

    // copied logic from LinuxParser::OperatingSystem()
    string line;
    string key;
    string value;
    string unit;
    /* string this_file{kProcDirectory + kMeminfoFilename}; */
    std::ifstream filestream(kProcDirectory + kMeminfoFilename);
    /* myfile << "reading this file: " << this_file << "\n";//DEBUG */
    if (filestream.is_open()) {
	while (std::getline(filestream, line)) {
	    std::istringstream linestream(line);
	    while (linestream >> key >> value >> unit) {
		/* myfile << "inside while linestream >> key >> value >> unit\n";//DEBUG */
		if (key == "MemTotal:") {
		    mTotal = std::stof(value);
		    /* myfile << "mTotal: " << mTotal << "\n";//DEBUG */
		    bTotal = true;
		}
		if (key == "MemFree:") {
		    mFree = std::stof(value);
		    bFree = true;
		}
		if (bTotal && bFree) {
		    /* myfile << "printing memories\n";//DEBUG */
		    usage =  (mTotal - mFree)/mTotal;
		    /* return (mTotal - mFree)/mTotal; */
		}
	    }
	}
    }
    /* myfile.close();//DEBUG */
    return usage;
}

// FINISHED: Read and return the system uptime
long LinuxParser::UpTime() {
    // debug stuff
    /* std::ofstream myfile;//DEBUG */
    /* myfile.open ("debug.txt");//DEBUG */
    /* myfile << "inside LinuxParser::UpTime()\n";//DEBUG */
    /* myfile << "string utime: " << utime << "\n"; */
    /* myfile.close();//DEBUG */
    // debug stuff

    // open file
    /* This file contains two numbers (values in seconds): */
    /* the uptime of the system (including time spent in suspend) and the amount of time spent in the idle process. */
    std::ifstream filestream(kProcDirectory + kUptimeFilename);

    // vars
    string line;
    string utime;
    string itime;
    long sec;
    
    if (filestream.is_open()) {
	std::getline(filestream, line);
	std::istringstream linestream(line);
	linestream >> utime >> itime;
	/* myfile << "string utime: " << utime << "\n"; */
	sec = std::stoi(utime);
	/* myfile << "stoi sec: " << sec << "\n"; */
    }

    /* myfile.close();//DEBUG */
    return sec;
}

/* Jiffies notes:
https://www.linkedin.com/pulse/linux-kernel-system-timer-jiffies-mohamed-yasser/
- a global variable that holds num of ticks that have occurred since system booted
    - init to 0, increments during each timer interrupt

https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux

		       usertime = usertime - guest;                             # As you see here, it subtracts guest from user time
		       nicetime = nicetime - guestnice;                         # and guest_nice from nice time
unsigned long long int idlealltime = idletime + ioWait;  # ioWait is added in the idleTime
unsigned long long int systemalltime = systemtime + irq + softIrq;
unsigned long long int virtalltime = guest + guestnice;
unsigned long long int totaltime = usertime + nicetime + systemalltime + idlealltime + steal + virtalltime;

     user    nice   system  idle      iowait irq   softirq  steal  guest  guest_nice
cpu  74608   2520   24433   1117073   6176   4054  0        0      0      0

// CPU usage calc
PrevIdle = previdle + previowait
Idle = idle + iowait

PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal
NonIdle = user + nice + system + irq + softirq + steal

PrevTotal = PrevIdle + PrevNonIdle
Total = Idle + NonIdle

# differentiate: actual value minus the previous one
totald = Total - PrevTotal
idled = Idle - PrevIdle

CPU_Percentage = (totald - idled)/totald
*/

// FINISHED: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
    // debug stuff
    /* std::ofstream myfile;//DEBUG */
    /* myfile.open ("debug.txt");//DEBUG */
    /* myfile << "inside LinuxParser::Jiffies()\n";//DEBUG */
    /* myfile.close();//DEBUG */
    // debug stuff

    // open file
    /* std::ifstream filestream(kProcDirectory + kStatFilename); */

    // vars
    /* string line; */
    /* string cpu; */
    /* long user{0}, nice{0}, system{0}, idle{0}, iowait{0}, irq{0}, softirq{0}, steal{0}, guest{0}, guest_nice{0}; */

    /*
     user    nice   system  idle      iowait irq   softirq  steal  guest  guest_nice
cpu  74608   2520   24433   1117073   6176   4054  0        0      0      0
    */

    /* if (filestream.is_open()) { */
	/* std::getline(filestream, line); */
	/* std::istringstream linestream(line); */
	/* linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice; */
	/* myfile << "user: " << user << "\n"; */
	/* myfile << "nice: " << nice << "\n"; */
	/* myfile << "system: " << system << "\n"; */
	/* myfile << "irq: " << irq << "\n"; */
	/* myfile << "softirq: " << softirq << "\n"; */
	/* myfile << "steal: " << steal << "\n"; */
    /* } */

    /* myfile << "ActiveJiffies(): " << ActiveJiffies() << "\n"; */
    /* myfile << "IdleJiffies(); " << IdleJiffies() << "\n"; */
    /* myfile.close();//DEBUG */

    // Total = Idle + NonIdle
    return ActiveJiffies() + IdleJiffies();
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) {
    return 0;
}

// FINISHED: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
    // debug stuff
    /* std::ofstream myfile;//DEBUG */
    /* myfile.open ("debug.txt");//DEBUG */
    /* myfile << "inside LinuxParser::ActiveJiffies()\n";//DEBUG */
    /* myfile.close();//DEBUG */
    // debug stuff

    // open file
    std::ifstream filestream(kProcDirectory + kStatFilename);

    // vars
    string line;
    string cpu;
    long user{0}, nice{0}, system{0}, idle{0}, iowait{0}, irq{0}, softirq{0}, steal{0}, guest{0}, guest_nice{0};

    /*
     user    nice   system  idle      iowait irq   softirq  steal  guest  guest_nice
cpu  74608   2520   24433   1117073   6176   4054  0        0      0      0
    */

    if (filestream.is_open()) {
	std::getline(filestream, line);
	std::istringstream linestream(line);
	linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
	/* myfile << "user: " << user << "\n"; */
	/* myfile << "nice: " << nice << "\n"; */
	/* myfile << "system: " << system << "\n"; */
	/* myfile << "irq: " << irq << "\n"; */
	/* myfile << "softirq: " << softirq << "\n"; */
	/* myfile << "steal: " << steal << "\n"; */
    }

    /* myfile.close();//DEBUG */

    //NonIdle = user + nice + system + irq + softirq + steal
    return user+nice+system+irq+softirq+steal;
}

// FINISHED: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
    // debug stuff
    /* std::ofstream myfile;//DEBUG */
    /* myfile.open ("debug.txt");//DEBUG */
    /* myfile << "inside LinuxParser::IdleJiffies()\n";//DEBUG */
    /* myfile.close();//DEBUG */
    // debug stuff

    // open file
    std::ifstream filestream(kProcDirectory + kStatFilename);

    // vars
    string line;
    string cpu;
    long user{0}, nice{0}, system{0}, idle{0}, iowait{0};
    
    /*
     user    nice   system  idle      iowait irq   softirq  steal  guest  guest_nice
cpu  74608   2520   24433   1117073   6176   4054  0        0      0      0
    */
    if (filestream.is_open()) {
	std::getline(filestream, line);
	std::istringstream linestream(line);
	linestream >> cpu >> user >> nice >> system >> idle >> iowait;
	/* myfile << "idle: " << idle << "\n"; */
	/* myfile << "iowait: " << iowait << "\n"; */
    }

    /* myfile.close();//DEBUG */

    // Idle = idle + iowait
    return idle+iowait;
}

// TODO: Read and return CPU utilization
/* vector<string> LinuxParser::CpuUtilization() { */
/*     return {}; */
/* } */

// This is the CPU utilization of indivisual process
// calc logic according to this:
// https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
float LinuxParser::CpuUtilization(int pid) {
    // debug stuff
    std::ofstream myfile;//DEBUG
    myfile.open ("debug-cpuUtil.txt");//DEBUG
    myfile << "inside LinuxParser::CpuUtilization(int pid)\n";//DEBUG
    // debug stuff
    string line;
    string _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22;

    long sysUptime = LinuxParser::UpTime();
    long total_time{0};
    long seconds{0};
    long cpu_usage{0};
    long HERTZ{sysconf(_SC_CLK_TCK)};

    std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
    if (filestream.is_open()) {
	std::getline(filestream, line);
	std::istringstream linestream(line);
	linestream  >>  _1 >>  _2 >>  _3 >>  _4 >>  _5 >>  _6 >>  _7
		    >>  _8 >>  _9 >> _10 >> _11 >> _12 >> _13 >> _14
		    >> _15 >> _16 >> _17 >> _18 >> _19 >> _20 >> _21 >> _22;
	myfile << " " <<_1 << " " <<_2 << " " <<_3 << " " <<_4 << " " <<_5 << " " <<_6 << " " <<_7 << " " <<_8 << " " <<_9 << " " <<_10 << " " <<_11
	    << " " <<_12 << " " <<_13 << " " <<_14 << " " <<_15 << " " <<_16 << " " <<_17 << " " <<_18 << " " <<_19 << " " <<_20 << " " <<_21 << " "
	    <<_22 << "\n";
	// in ticks
	total_time = stol(_14) + stol(_15) + stol(_16) + stol(_17);
	// in sec
	// this is == to "this_uptime" in LinuxParser::UpTime(int pid)
	seconds = sysUptime - (stol(_22) / HERTZ ); // (stol(_22) / HERTZ) is in seconds, same as sysUptime; 
	if (seconds > 1) {
	    cpu_usage = 100 * ((total_time / HERTZ) / seconds);
	}
	myfile << "PID: " << pid << "; total_time: " << total_time << "; seconds: " << seconds << "; cpu: " << cpu_usage << "\n";
    }
    myfile.close();
    return cpu_usage;
}

// FINISHED: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
    string line;
    string key;
    string value;
    int tProcess;

    std::ifstream filestream(kProcDirectory + kStatFilename);
    if (filestream.is_open()) {
	while (std::getline(filestream, line)) {
	    std::istringstream linestream(line);
	    while (linestream >> key >> value) {
		if (key == "processes") {
		    tProcess = std::stoi(value);
		}
	    }
	}
    }
    return tProcess;
}

// FINISHED: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
    string line;
    string key;
    string value;
    int rProcess;

    std::ifstream filestream(kProcDirectory + kStatFilename);
    if (filestream.is_open()) {
	while (std::getline(filestream, line)) {
	    std::istringstream linestream(line);
	    while (linestream >> key >> value) {
		if (key == "procs_running") {
		    rProcess = std::stoi(value);
		}
	    }
	}
    }
    return rProcess;
}

// FINISHED: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
    string line;
    string this_cmd;

    std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
    if (filestream.is_open()) {
	std::getline(filestream, line);
	std::istringstream linestream(line);
	linestream >> this_cmd;
    }
    return this_cmd;
}

// FINISHED: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
    string line;
    string key;
    string value;
    long memSize;

    std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
    if (filestream.is_open()) {
	while (std::getline(filestream, line)) {
	    std::istringstream linestream(line);
	    while (linestream >> key >> value) {
		if (key == "VmSize:") {
		    memSize = std::stoi(value);
		}
	    }
	}
    }
    return std::to_string(memSize / 1024);
}

// FINISHED: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
    string line;
    string key;
    string value;
    string uid;

    std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
    if (filestream.is_open()) {
	while (std::getline(filestream, line)) {
	    std::istringstream linestream(line);
	    while (linestream >> key >> value) {
		if (key == "Uid:") {
		    uid = value;
		}
	    }
	}
    }

    return uid;
}

// FINISHED: Read and return the user associated with a process
string LinuxParser::User(string in_uid) {
    string line;
    string user;
    string x;
    string uid;
    string out_user;

    std::ifstream filestream(kPasswordPath);
    if (filestream.is_open()) {
	while (std::getline(filestream, line)) {
	    std::replace(line.begin(), line.end(), ':', ' ');
	    std::istringstream linestream(line);
	    while (linestream >> user >> x >> uid) {
		if (uid == in_uid) {
		    out_user = user;
		}
	    }
	}
    }

    return out_user;
}

// FINISHED: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
    // debug stuff
    /* std::ofstream myfile;//DEBUG */
    /* myfile.open ("debug_uptime.txt");//DEBUG */
    /* myfile << "inside LinuxParser::Uptime(int pid)\n";//DEBUG */
    // debug stuff

// -------------------------old code for CPU usage---------------------------------------
   /* 2/11/2022:	the following should be for processor utilization, */
		/* not uptime. update */
		/* Check "13. Process Data" */

    string line;
    string _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22;

    long sysUptime = LinuxParser::UpTime();
    /* long total_time{0}; */
    /* long seconds{0}; */
    /* long cpu_usage{0}; */
    long this_uptime{0};
    long HERTZ{sysconf(_SC_CLK_TCK)};

    std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
    if (filestream.is_open()) {
	std::getline(filestream, line);
	std::istringstream linestream(line);
	linestream  >>  _1 >>  _2 >>  _3 >>  _4 >>  _5 >>  _6 >>  _7
		    >>  _8 >>  _9 >> _10 >> _11 >> _12 >> _13 >> _14
		    >> _15 >> _16 >> _17 >> _18 >> _19 >> _20 >> _21 >> _22;
	/* myfile << " " <<_1 << " " <<_2 << " " <<_3 << " " <<_4 << " " <<_5 << " " <<_6 << " " <<_7 << " " <<_8 << " " <<_9 << " " <<_10 << " " <<_11 */
	/*     << " " <<_12 << " " <<_13 << " " <<_14 << " " <<_15 << " " <<_16 << " " <<_17 << " " <<_18 << " " <<_19 << " " <<_20 << " " <<_21 << " " */
	/*     <<_22 << "\n"; */
    }
    this_uptime = stol(_22);
    /* myfile << "sysUptime: " << sysUptime << "\n"; */
    /* myfile << "this_uptime/HERTZ: " << this_uptime/HERTZ << "\n"; */
    /* myfile << "diff: " << sysUptime - (this_uptime / HERTZ) << "\n"; */

    /* myfile.close();//DEBUG */
    return sysUptime - (this_uptime / HERTZ);
}
