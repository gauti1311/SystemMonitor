#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
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
    linestream >> os >>version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
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

// Done: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  string key;
  long memValue;
  long memTotal = 0;
  long memFree = 0;
  long memAvailable = 0;
  long memBuffers;
  string unit;
  if (stream.is_open()) {
    for(int i = 0; i<2 ; i++){
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> key >> memValue >> unit ;
      if(key=="MemTotal:"){
        memTotal=  memValue; 
        continue;
      }
      if(key=="MemFree: "){
        memFree=  memValue; 
        continue;
      }
    }
  }
  return ((memTotal-memFree)/1.0*memTotal); 
  }

// Done: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line;
  long upTime = 0;
  long idleTime;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if(stream.is_open()){
    std::getline(stream,line);
    std::istringstream linestream(line);
    linestream >>upTime>> idleTime;
  }
  return upTime; }

//Done: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
      std::string line, cpu;
      std::getline(filestream, line);
      std::istringstream linestream(line);
      long user, nice, system, idle, iowait, irq, softirq, steal, guess, guessnice; 
      linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guess >> guessnice;
      long totalUserTime = user - guess;
      long totalNiceTime = nice - guessnice;
      long totalIdleTime = idle + iowait;
      long totalSystem = system + irq + softirq;
      long totalVirtualTime = guess + guessnice;
      return totalUserTime + totalNiceTime + totalIdleTime + totalSystem + totalVirtualTime;
  }
  return 0; }

//Done: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { 
  std::stringstream filename;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
      std::string line, cpu;
      std::getline(filestream, line);
      std::istringstream linestream(line);
      std::string ign;
      long utime, stime, cutime, cstime, starttime;
      for(int i = 0; i < 13; i++) linestream >> ign;
      linestream >> utime >> stime >> cutime >> cstime;
      for(int i = 0; i < 4; i++) linestream >> ign;
      linestream >> starttime;

      return utime + stime + cutime + cstime +starttime;
  }
  return 0; 
  }

// Done: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  std::stringstream filename;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
      std::string line, cpu;
      std::getline(filestream, line);
      std::istringstream linestream(line);
      std::string ign;
      long utime, stime, cutime, cstime, starttime;
      for(int i = 0; i < 13; i++) linestream >> ign;
      linestream >> utime >> stime >> cutime >> cstime;
      for(int i = 0; i < 4; i++) linestream >> ign;
      linestream >> starttime;

      return utime + stime + cutime + cstime +starttime;
  }
  return 0;
 }

// Done: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// Done: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string user, nice, system, idle, iowait, irq, softirq, steal;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> user >> nice >> system >> idle >> iowait >> irq >> softirq >>steal;
  }
  vector<string> v{user,nice,system,idle,iowait,irq,softirq,steal};

  return v;}

// Done: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string  processes, line;
  int totProcess= 0;
  int processCount;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> processes >> processCount;
      if (processes == "processes") {
        totProcess = processCount;
        break;
      }
    }
  }
  return totProcess; }

// Done: Read and return the number of running processes
int LinuxParser::RunningProcesses() { string  processes, line;
  int runProcess= 0;
  int processCount;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> processes >> processCount;
      if (processes == "procs_running") {
        runProcess = processCount;
        break;
      }
    }
  }
  return runProcess; }

// Done: Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string line, cmd = string();
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)){
      cmd += line;
    }
  }
  return cmd; }

// Done: Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string line, key;
  long value;
  double val;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize") {
          val = value/1024;
        }
      }
    }
  }
  return to_string(val);}

// Done: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string line, key, value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid") {
          return value;
        }
      }
    }
  }
  return value; }

// Done: Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string line, key, value1, value2;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value1 >> value2) {
        if (value2.compare(Uid(pid)) == 0) {
          return key;
        }
      }
    }
  }
  return key;
  }

// Done: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  long upTime;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < 22; i++) {
      linestream >> upTime;
      return upTime;
    }
  }
  return upTime;
  }
