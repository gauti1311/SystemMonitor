#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    std::string line, cpu;
    long  user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  float realTimeUtil = 0;
  std::ifstream filestream(LinuxParser::kProcDirectory +
                           LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> cpu >> user >> nice >> system >> idle >> iowait >>
             irq >> softirq >> steal >> guest >> guest_nice) {
        if (cpu == "cpu") {
          int totalCpuTimeSinceBoot = user + nice + system + idle + iowait + irq + softirq + steal;
          float diff_idle = idle - prev_idle_;
          float diff_total = totalCpuTimeSinceBoot - prev_total_;
          realTimeUtil = std::min((diff_total - diff_idle) / diff_total * 2, 100.f);
          prev_total_ = totalCpuTimeSinceBoot;
          prev_idle_ = idle;

          break;
        }
      }
    }
  }

  return realTimeUtil;
}