# VIVA VOCE PREPARATION GUIDE: OPERATING SYSTEMS (OS)
## Project: CPU Process Scheduling Simulator
**Student:** Rishi Pandey | **ERP No:** 25ET2041025 | **Institution:** Alard University, Pune

---

### Q1: What is the primary role of a CPU Scheduler?
**Answer:**
The CPU Scheduler (short-term scheduler) selects which ready process from the memory queue should be allocated CPU execution time when the processor becomes free. Its objective is to maximize CPU utilization and throughput while minimizing turnaround time, waiting time, and response time.

---

### Q2: What are the differences between Preemptive and Non-Preemptive Scheduling?
**Answer:**
- **Non-Preemptive:** Once the CPU has been allocated to a process, the process keeps the CPU until it terminates or voluntarily switches to a waiting state (e.g., FCFS, Non-Preemptive SJF, Non-Preemptive Priority).
- **Preemptive:** The operating system can interrupt (preempt) a currently running process before its completion and allocate the CPU to another process (e.g., Round Robin, SRTF, Preemptive Priority).

---

### Q3: Define the core scheduling metrics and their formulas.
**Answer:**
1. **Arrival Time ($AT$):** The time at which a process enters the ready queue.
2. **Burst Time ($BT$):** The duration of CPU time required by the process for execution.
3. **Completion Time ($CT$):** The exact time instant when the process finishes execution.
4. **Turnaround Time ($TAT$):** Total elapsed time from submission to completion:
   $$TAT = CT - AT$$
5. **Waiting Time ($WT$):** Total time spent waiting in the ready queue:
   $$WT = TAT - BT$$

---

### Q4: What is the "Convoy Effect" in FCFS?
**Answer:**
The **Convoy Effect** occurs in FCFS scheduling when a CPU-bound process with a very large burst time executes ahead of several short I/O-bound processes. The short processes are forced to wait in the ready queue for a long duration, drastically degrading CPU and device utilization and increasing average waiting time.

---

### Q5: Why is Shortest Job First (SJF) considered mathematically optimal, and what is its major drawback?
**Answer:**
- **Optimality:** SJF gives the minimum average waiting time for a given set of stationary processes because moving a shorter job ahead of a longer one decreases the waiting time of the shorter job more than it increases the waiting time of the longer job.
- **Drawbacks:**
  1. It is impossible to know the exact length of the next CPU burst in advance in a general OS (must be predicted using exponential averaging).
  2. Can cause **starvation** (indefinite blocking) of longer processes if short processes keep arriving.

---

### Q6: What is Starvation, and how is it solved?
**Answer:**
- **Starvation (Indefinite Blocking):** Occurs in Priority and SJF scheduling when low-priority or long-burst processes remain in the ready queue indefinitely because higher-priority or shorter jobs continuously arrive.
- **Solution:** **Aging**. Aging gradually increases the priority of processes that wait in the system for a long time. Eventually, even the lowest-priority process achieves the highest priority and executes.

---

### Q7: How does Round Robin (RR) work, and how does Time Quantum selection affect performance?
**Answer:**
- **Mechanism:** RR assigns each process a fixed slice of CPU time called a **Time Quantum ($Q$)** using a FIFO ready queue. If a process does not complete within $Q$, it is preempted and pushed back to the rear of the ready queue.
- **Impact of Quantum Size:**
  - **If $Q$ is very large:** Round Robin degrades into FCFS.
  - **If $Q$ is very small:** Context switching overhead becomes significant, consuming CPU cycles on registers/cache swapping rather than useful work.
  - **Rule of Thumb:** About $80\%$ of CPU bursts should be shorter than the time quantum $Q$.

---

### Q8: What does your ASCII Gantt Chart demonstrate?
**Answer:**
The Gantt chart illustrates the chronological timeline of CPU execution blocks:
- Shows the exact process executing at each time step (e.g., `| P1 | P2 |`).
- Accurately captures **CPU IDLE** periods when no process has arrived yet.
- Shows time slice preemption transitions in Round Robin.

---

### Q9: What were the experimental findings from your comparative benchmark?
**Answer:**
On our sample dataset of 5 processes:
- **SJF (Non-Preemptive)** achieved the lowest average waiting time ($6.60\text{ ms}$).
- **Priority Scheduling** matched $6.60\text{ ms}$ because priority values aligned with burst requirements.
- **FCFS** yielded an average waiting time of $8.20\text{ ms}$ due to queuing delays behind longer jobs.
- **Round Robin ($Q=2$)** yielded an average waiting time of $12.60\text{ ms}$ due to frequent context preemptions, but provided the best interactive fairness and response time.
