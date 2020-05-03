#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) {this->pid_ = pid;}

// Done: Return this process's ID
int Process::Pid() { return pid_;} 

// Done: Return this process's CPU utilization
float Process::CpuUtilization() { 
  string line, value;
  float result;
  std::ifstream stream("/proc/" + to_string(pid_) + "/stat");
  getline(stream, line);
  string str = line;
  std::istringstream buf(str);
  std::istream_iterator<string> beg(buf), end;
  vector<string> values(beg, end);  
  float utime = UpTime();
  float stime = stof(values[14]);
  float starttime = stof(values[21]);
  float uptime = LinuxParser::UpTime(); 
  float freq = sysconf(_SC_CLK_TCK);
  float total_time = utime + stime;
  float seconds = uptime - (starttime / freq);
  result = 100.0 * ((total_time / freq) / seconds);
  return result;
}

// Done: Return the command that generated this process
string Process::Command() { 
    return LinuxParser::Command(pid_); 
    }

// Done: Return this process's memory utilization
string Process::Ram() { 
    return LinuxParser::Ram(pid_) ;
     }

// Done: Return the user (name) that generated this process
string Process::User() { 
    return LinuxParser::User(pid_);
    }

// Done: Return the age of this process (in seconds)
long int Process::UpTime() { 
    return LinuxParser::UpTime(pid_); 
    }

// Done: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
    return true; }