#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <queue>
#include <limits>

using namespace std;

// Structure to hold process information
struct Process {
    int pid;            // Process ID
    int arrivalTime;    // Arrival Time (AT)
    int burstTime;      // Burst Time (BT)
    int priority;       // Priority (Lower value = Higher priority)
    
    // Output calculation metrics
    int completionTime; // Completion Time (CT)
    int turnaroundTime; // Turnaround Time (TAT = CT - AT)
    int waitingTime;    // Waiting Time (WT = TAT - BT)
    int remainingTime;  // Used for Round Robin
    int startTime;      // First time CPU is allocated

    Process(int id = 0, int at = 0, int bt = 0, int prio = 0) {
        pid = id;
        arrivalTime = at;
        burstTime = bt;
        priority = prio;
        completionTime = 0;
        turnaroundTime = 0;
        waitingTime = 0;
        remainingTime = bt;
        startTime = -1;
    }
};

// Structure for Gantt Chart slice
struct GanttSlot {
    int pid;       // Process ID (-1 for IDLE)
    int startTime;
    int endTime;
};

// Result summary for algorithm comparison
struct AlgoResult {
    string name;
    double avgTurnaroundTime;
    double avgWaitingTime;
};

// Helper function to safely read positive integer
int getIntInput(int minVal = 0, int maxVal = 10000) {
    int val;
    while (!(cin >> val) || val < minVal || val > maxVal) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input! Please enter a value between " << minVal << " and " << maxVal << ": ";
    }
    return val;
}

// Function to print a visual ASCII Gantt Chart
void printGanttChart(const vector<GanttSlot>& chart) {
    if (chart.empty()) return;

    cout << "\n>>> ASCII GANTT CHART <<<\n";
    
    // Top border
    for (size_t i = 0; i < chart.size(); i++) {
        cout << "+-------";
    }
    cout << "+\n";

    // Process IDs
    for (size_t i = 0; i < chart.size(); i++) {
        if (chart[i].pid == -1) {
            cout << "| IDLE  ";
        } else {
            cout << "|  P" << setw(2) << left << chart[i].pid << "  ";
        }
    }
    cout << "|\n";

    // Bottom border
    for (size_t i = 0; i < chart.size(); i++) {
        cout << "+-------";
    }
    cout << "+\n";

    // Time points
    cout << setw(8) << left << chart[0].startTime;
    for (size_t i = 0; i < chart.size(); i++) {
        cout << setw(8) << left << chart[i].endTime;
    }
    cout << "\n\n";
}

// Display Process Table with Metrics
void printMetricsTable(const vector<Process>& procs, double& avgTAT, double& avgWT) {
    cout << "===============================================================================\n";
    cout << left << setw(8) << "PID"
         << setw(14) << "Arrival (AT)"
         << setw(12) << "Burst (BT)"
         << setw(10) << "Priority"
         << setw(16) << "Completion(CT)"
         << setw(16) << "Turnaround(TAT)"
         << "Waiting(WT)\n";
    cout << "-------------------------------------------------------------------------------\n";

    double totalTAT = 0.0;
    double totalWT = 0.0;

    for (const auto& p : procs) {
        cout << left << setw(8) << ("P" + to_string(p.pid))
             << setw(14) << p.arrivalTime
             << setw(12) << p.burstTime
             << setw(10) << p.priority
             << setw(16) << p.completionTime
             << setw(16) << p.turnaroundTime
             << p.waitingTime << "\n";
        totalTAT += p.turnaroundTime;
        totalWT += p.waitingTime;
    }

    avgTAT = totalTAT / procs.size();
    avgWT = totalWT / procs.size();

    cout << "===============================================================================\n";
    cout << fixed << setprecision(2);
    cout << "Average Turnaround Time (TAT): " << avgTAT << " ms\n";
    cout << "Average Waiting Time    (WT) : " << avgWT << " ms\n";
    cout << "===============================================================================\n";
}

// 1. First-Come, First-Served (FCFS) Scheduling
AlgoResult runFCFS(vector<Process> procs) {
    // Sort processes by arrival time
    sort(procs.begin(), procs.end(), [](const Process& a, const Process& b) {
        if (a.arrivalTime == b.arrivalTime) return a.pid < b.pid;
        return a.arrivalTime < b.arrivalTime;
    });

    int currentTime = 0;
    vector<GanttSlot> chart;

    for (auto& p : procs) {
        if (currentTime < p.arrivalTime) {
            chart.push_back({-1, currentTime, p.arrivalTime});
            currentTime = p.arrivalTime;
        }

        int start = currentTime;
        currentTime += p.burstTime;
        p.completionTime = currentTime;
        p.turnaroundTime = p.completionTime - p.arrivalTime;
        p.waitingTime = p.turnaroundTime - p.burstTime;

        chart.push_back({p.pid, start, currentTime});
    }

    cout << "\n-------------------------------------------------------------------------------\n";
    cout << "             FIRST-COME, FIRST-SERVED (FCFS) SCHEDULING RESULTS               \n";
    cout << "-------------------------------------------------------------------------------\n";
    printGanttChart(chart);

    double avgTAT = 0, avgWT = 0;
    printMetricsTable(procs, avgTAT, avgWT);
    return {"FCFS", avgTAT, avgWT};
}

// 2. Shortest Job First (SJF - Non-Preemptive)
AlgoResult runSJF(vector<Process> procs) {
    int n = procs.size();
    vector<bool> completed(n, false);
    vector<GanttSlot> chart;
    int completedCount = 0;
    int currentTime = 0;

    while (completedCount < n) {
        int bestIdx = -1;
        int minBurst = numeric_limits<int>::max();

        for (int i = 0; i < n; i++) {
            if (!completed[i] && procs[i].arrivalTime <= currentTime) {
                if (procs[i].burstTime < minBurst) {
                    minBurst = procs[i].burstTime;
                    bestIdx = i;
                } else if (procs[i].burstTime == minBurst) {
                    // Tie-breaker: earlier arrival or smaller PID
                    if (bestIdx == -1 || procs[i].arrivalTime < procs[bestIdx].arrivalTime) {
                        bestIdx = i;
                    }
                }
            }
        }

        if (bestIdx == -1) {
            // CPU is idle, advance to next arriving process
            int nextArrival = numeric_limits<int>::max();
            for (int i = 0; i < n; i++) {
                if (!completed[i] && procs[i].arrivalTime < nextArrival) {
                    nextArrival = procs[i].arrivalTime;
                }
            }
            chart.push_back({-1, currentTime, nextArrival});
            currentTime = nextArrival;
        } else {
            int start = currentTime;
            currentTime += procs[bestIdx].burstTime;
            procs[bestIdx].completionTime = currentTime;
            procs[bestIdx].turnaroundTime = procs[bestIdx].completionTime - procs[bestIdx].arrivalTime;
            procs[bestIdx].waitingTime = procs[bestIdx].turnaroundTime - procs[bestIdx].burstTime;
            completed[bestIdx] = true;
            completedCount++;

            chart.push_back({procs[bestIdx].pid, start, currentTime});
        }
    }

    cout << "\n-------------------------------------------------------------------------------\n";
    cout << "             SHORTEST JOB FIRST (SJF - NON-PREEMPTIVE) RESULTS                 \n";
    cout << "-------------------------------------------------------------------------------\n";
    printGanttChart(chart);

    double avgTAT = 0, avgWT = 0;
    printMetricsTable(procs, avgTAT, avgWT);
    return {"SJF (Non-Preemptive)", avgTAT, avgWT};
}

// 3. Priority Scheduling (Non-Preemptive, Lower value = Higher Priority)
AlgoResult runPriority(vector<Process> procs) {
    int n = procs.size();
    vector<bool> completed(n, false);
    vector<GanttSlot> chart;
    int completedCount = 0;
    int currentTime = 0;

    while (completedCount < n) {
        int bestIdx = -1;
        int highestPriority = numeric_limits<int>::max();

        for (int i = 0; i < n; i++) {
            if (!completed[i] && procs[i].arrivalTime <= currentTime) {
                if (procs[i].priority < highestPriority) {
                    highestPriority = procs[i].priority;
                    bestIdx = i;
                } else if (procs[i].priority == highestPriority) {
                    if (bestIdx == -1 || procs[i].arrivalTime < procs[bestIdx].arrivalTime) {
                        bestIdx = i;
                    }
                }
            }
        }

        if (bestIdx == -1) {
            int nextArrival = numeric_limits<int>::max();
            for (int i = 0; i < n; i++) {
                if (!completed[i] && procs[i].arrivalTime < nextArrival) {
                    nextArrival = procs[i].arrivalTime;
                }
            }
            chart.push_back({-1, currentTime, nextArrival});
            currentTime = nextArrival;
        } else {
            int start = currentTime;
            currentTime += procs[bestIdx].burstTime;
            procs[bestIdx].completionTime = currentTime;
            procs[bestIdx].turnaroundTime = procs[bestIdx].completionTime - procs[bestIdx].arrivalTime;
            procs[bestIdx].waitingTime = procs[bestIdx].turnaroundTime - procs[bestIdx].burstTime;
            completed[bestIdx] = true;
            completedCount++;

            chart.push_back({procs[bestIdx].pid, start, currentTime});
        }
    }

    cout << "\n-------------------------------------------------------------------------------\n";
    cout << "             PRIORITY SCHEDULING (NON-PREEMPTIVE) RESULTS                      \n";
    cout << "-------------------------------------------------------------------------------\n";
    printGanttChart(chart);

    double avgTAT = 0, avgWT = 0;
    printMetricsTable(procs, avgTAT, avgWT);
    return {"Priority Scheduling", avgTAT, avgWT};
}

// 4. Round Robin (RR - Preemptive)
AlgoResult runRoundRobin(vector<Process> procs, int timeQuantum) {
    int n = procs.size();
    for (int i = 0; i < n; i++) {
        procs[i].remainingTime = procs[i].burstTime;
    }

    // Sort initially by arrival time
    sort(procs.begin(), procs.end(), [](const Process& a, const Process& b) {
        if (a.arrivalTime == b.arrivalTime) return a.pid < b.pid;
        return a.arrivalTime < b.arrivalTime;
    });

    vector<GanttSlot> chart;
    queue<int> readyQueue;
    vector<bool> inQueue(n, false);

    int currentTime = 0;
    int completedCount = 0;

    // Check if initial processes arrive at 0
    for (int i = 0; i < n; i++) {
        if (procs[i].arrivalTime <= currentTime) {
            readyQueue.push(i);
            inQueue[i] = true;
        }
    }

    while (completedCount < n) {
        if (readyQueue.empty()) {
            // Find next arriving process
            int nextArrival = numeric_limits<int>::max();
            for (int i = 0; i < n; i++) {
                if (procs[i].remainingTime > 0 && procs[i].arrivalTime < nextArrival) {
                    nextArrival = procs[i].arrivalTime;
                }
            }
            if (nextArrival != numeric_limits<int>::max()) {
                chart.push_back({-1, currentTime, nextArrival});
                currentTime = nextArrival;
                for (int i = 0; i < n; i++) {
                    if (procs[i].remainingTime > 0 && procs[i].arrivalTime <= currentTime && !inQueue[i]) {
                        readyQueue.push(i);
                        inQueue[i] = true;
                    }
                }
            }
            continue;
        }

        int idx = readyQueue.front();
        readyQueue.pop();

        int execTime = min(timeQuantum, procs[idx].remainingTime);
        int start = currentTime;
        currentTime += execTime;
        procs[idx].remainingTime -= execTime;

        // Consolidate consecutive execution in Gantt Chart if any
        if (!chart.empty() && chart.back().pid == procs[idx].pid) {
            chart.back().endTime = currentTime;
        } else {
            chart.push_back({procs[idx].pid, start, currentTime});
        }

        // Add newly arrived processes while idx was executing
        for (int i = 0; i < n; i++) {
            if (i != idx && procs[i].remainingTime > 0 && procs[i].arrivalTime <= currentTime && !inQueue[i]) {
                readyQueue.push(i);
                inQueue[i] = true;
            }
        }

        // If current process still has remaining work, put it back into ready queue
        if (procs[idx].remainingTime > 0) {
            readyQueue.push(idx);
        } else {
            procs[idx].completionTime = currentTime;
            procs[idx].turnaroundTime = procs[idx].completionTime - procs[idx].arrivalTime;
            procs[idx].waitingTime = procs[idx].turnaroundTime - procs[idx].burstTime;
            completedCount++;
        }
    }

    // Sort back by PID for clean display
    sort(procs.begin(), procs.end(), [](const Process& a, const Process& b) {
        return a.pid < b.pid;
    });

    cout << "\n-------------------------------------------------------------------------------\n";
    cout << "          ROUND ROBIN (RR) SCHEDULING RESULTS (Time Quantum = " << timeQuantum << " ms)         \n";
    cout << "-------------------------------------------------------------------------------\n";
    printGanttChart(chart);

    double avgTAT = 0, avgWT = 0;
    printMetricsTable(procs, avgTAT, avgWT);
    return {"Round Robin (Q=" + to_string(timeQuantum) + ")", avgTAT, avgWT};
}

// Preloaded standard sample process set for fast demonstration
vector<Process> getSampleProcesses() {
    return {
        Process(1, 0, 5, 2),
        Process(2, 1, 3, 1),
        Process(3, 2, 8, 4),
        Process(4, 3, 6, 3),
        Process(5, 4, 4, 2)
    };
}

// Function to print current process list
void displayCurrentProcesses(const vector<Process>& procs) {
    cout << "\n=======================================================\n";
    cout << "                 CURRENT PROCESS SET                   \n";
    cout << "=======================================================\n";
    cout << left << setw(8) << "PID"
         << setw(16) << "Arrival (AT)"
         << setw(14) << "Burst (BT)"
         << "Priority\n";
    cout << "-------------------------------------------------------\n";
    for (const auto& p : procs) {
        cout << left << setw(8) << ("P" + to_string(p.pid))
             << setw(16) << p.arrivalTime
             << setw(14) << p.burstTime
             << p.priority << "\n";
    }
    cout << "=======================================================\n";
}

int main() {
    vector<Process> processList = getSampleProcesses();
    int timeQuantum = 2;
    int choice = 0;

    do {
        cout << "\n=======================================================\n";
        cout << "         CPU PROCESS SCHEDULING SIMULATOR (OS)         \n";
        cout << "   Student: Rishi Pandey | ERP No: 25ET2041025         \n";
        cout << "             Alard University, Pune                    \n";
        cout << "=======================================================\n";
        cout << " 1. View Current Processes Table\n";
        cout << " 2. Run First-Come, First-Served (FCFS)\n";
        cout << " 3. Run Shortest Job First (SJF - Non-Preemptive)\n";
        cout << " 4. Run Priority Scheduling (Non-Preemptive)\n";
        cout << " 5. Run Round Robin (RR)\n";
        cout << " 6. Run ALL & Show Comparative Analysis Table\n";
        cout << " 7. Change Round Robin Time Quantum (Current: " << timeQuantum << " ms)\n";
        cout << " 8. Enter Custom Process Dataset\n";
        cout << " 9. Reset to Default Sample Dataset\n";
        cout << " 10. Exit\n";
        cout << "=======================================================\n";
        cout << "Enter your choice (1-10): ";

        choice = getIntInput(1, 10);

        switch (choice) {
            case 1:
                displayCurrentProcesses(processList);
                break;
            case 2:
                runFCFS(processList);
                break;
            case 3:
                runSJF(processList);
                break;
            case 4:
                runPriority(processList);
                break;
            case 5:
                runRoundRobin(processList, timeQuantum);
                break;
            case 6: {
                cout << "\n=======================================================\n";
                cout << "       RUNNING COMPREHENSIVE ALGORITHM BENCHMARK       \n";
                cout << "=======================================================\n";
                vector<AlgoResult> results;
                results.push_back(runFCFS(processList));
                results.push_back(runSJF(processList));
                results.push_back(runPriority(processList));
                results.push_back(runRoundRobin(processList, timeQuantum));

                cout << "\n====================================================================\n";
                cout << "                     OVERALL COMPARISON SUMMARY                     \n";
                cout << "====================================================================\n";
                cout << left << setw(28) << "Algorithm"
                     << setw(22) << "Avg Turnaround (ms)"
                     << "Avg Waiting (ms)\n";
                cout << "--------------------------------------------------------------------\n";
                for (const auto& res : results) {
                    cout << left << setw(28) << res.name
                         << fixed << setprecision(2) << setw(22) << res.avgTurnaroundTime
                         << res.avgWaitingTime << "\n";
                }
                cout << "====================================================================\n";

                // Identify best algorithm for waiting time
                auto bestAlgo = min_element(results.begin(), results.end(), 
                    [](const AlgoResult& a, const AlgoResult& b) {
                        return a.avgWaitingTime < b.avgWaitingTime;
                    });
                cout << "[*] Recommendation: \"" << bestAlgo->name 
                     << "\" yields the lowest average waiting time (" << bestAlgo->avgWaitingTime << " ms) for this dataset!\n";
                break;
            }
            case 7:
                cout << "Enter new Time Quantum for Round Robin (ms): ";
                timeQuantum = getIntInput(1, 100);
                cout << "[+] Time Quantum set to " << timeQuantum << " ms.\n";
                break;
            case 8: {
                cout << "How many processes would you like to enter? (1-20): ";
                int count = getIntInput(1, 20);
                processList.clear();
                for (int i = 1; i <= count; i++) {
                    cout << "\n--- Process P" << i << " Details ---\n";
                    cout << "Arrival Time (AT >= 0): ";
                    int at = getIntInput(0, 1000);
                    cout << "Burst Time (BT >= 1)  : ";
                    int bt = getIntInput(1, 1000);
                    cout << "Priority (1 = Highest): ";
                    int prio = getIntInput(1, 100);
                    processList.push_back(Process(i, at, bt, prio));
                }
                cout << "\n[+] Custom processes loaded successfully!\n";
                displayCurrentProcesses(processList);
                break;
            }
            case 9:
                processList = getSampleProcesses();
                cout << "\n[+] Reset to default sample processes successfully!\n";
                displayCurrentProcesses(processList);
                break;
            case 10:
                cout << "\nExiting CPU Process Scheduling Simulator.\n";
                cout << "Developed by: Rishi Pandey (ERP: 25ET2041025)\n";
                cout << "Department of Computer Science & Engineering, Alard University Pune\n\n";
                break;
            default:
                break;
        }

    } while (choice != 10);

    return 0;
}
