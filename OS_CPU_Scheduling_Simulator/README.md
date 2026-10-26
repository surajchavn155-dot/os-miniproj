# CPU Process Scheduling Simulator (OS Mini Project)

**Student Details:**
- **Name:** Rishi Pandey
- **ERP No:** 25ET2041025
- **Branch / Year:** Computer Science & Engineering (CSE) - 2nd Year
- **Institution:** Alard University, Pune
- **Course:** Operating Systems (OS)

---

## 📌 Project Overview
The **CPU Process Scheduling Simulator** is an interactive C++ simulation tool designed to demonstrate and compare core CPU scheduling algorithms taught in undergraduate Operating Systems courses. 

It implements four major scheduling policies:
1. **First-Come, First-Served (FCFS)** (Non-preemptive)
2. **Shortest Job First (SJF)** (Non-preemptive)
3. **Priority Scheduling** (Non-preemptive, lower numeric value = higher priority)
4. **Round Robin (RR)** (Preemptive, configurable Time Quantum)

---

## ⚙️ Key Features
- **Visual ASCII Gantt Chart:** Displays clear process execution blocks and time milestones.
- **Full Metric Computations:**
  - Completion Time (CT)
  - Turnaround Time ($TAT = CT - AT$)
  - Waiting Time ($WT = TAT - BT$)
  - Average TAT and Average WT
- **Comparative Analysis Engine:** Runs all 4 algorithms on the identical process dataset and produces a summary table identifying the most optimal algorithm.
- **Configurable Dataset:** Preloaded sample dataset for instant one-click demonstration, plus custom dataset entry.
- **Configurable Time Quantum:** Easily customize time quantum for Round Robin testing.

---

## 🛠️ Compilation & Execution Instructions

### Prerequisites
- GCC / G++ (v7.0 or higher) with C++17 support.
- Linux, macOS, or Windows (MinGW/WSL).

### Build & Run Using Makefile:
```bash
make
make run
```

### Manual Compilation:
```bash
g++ -std=c++17 -Wall main.cpp -o cpu_scheduler
./cpu_scheduler
```

### Clean Build:
```bash
make clean
```

---

## 📂 File Structure
```
OS_CPU_Scheduling_Simulator/
├── main.cpp               # Complete C++ source code
├── Makefile               # Automated build script
├── README.md              # Quick start documentation
└── OS_PROJECT_REPORT.md   # Complete academic project report
```
