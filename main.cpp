#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <sstream>

using namespace std;

// Cấu trúc lưu thông tin của 1 tiến trình
struct Process {
    string id;          // Mã tiến trình (P1, P2...)
    int arrivalTime;    // Thời gian đến (AT)
    int burstTime;      // Thời gian thực thi (BT)
    int remainingTime;  // Thời gian còn lại (Dùng cho SRTF)
    int completionTime; // Thời gian hoàn thành (CT)
    int turnaroundTime; // Thời gian lưu lại hệ thống (TAT = CT - AT)
    int waitingTime;    // Thời gian chờ (WT = TAT - BT)
};

// 1. Hàm đọc danh sách tiến trình từ file input.csv
vector<Process> readProcessesFromFile(string filename) {
    vector<Process> pList;
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Loi: Khong the mo file " << filename << "!\n";
        return pList;
    }

    string line;
    // Bỏ qua dòng tiêu đề đầu tiên: Process_ID,Arrival_Time,Burst_Time
    getline(file, line); 

    while (getline(file, line)) {
        stringstream ss(line);
        string idStr, atStr, btStr;

        getline(ss, idStr, ',');
        getline(ss, atStr, ',');
        getline(ss, btStr, ',');

        if (!idStr.empty() && !atStr.empty() && !btStr.empty()) {
            string pid = idStr;
            int at = stoi(atStr);
            int bt = stoi(btStr);
            // Khởi tạo remainingTime ban đầu bằng burstTime
            pList.push_back({pid, at, bt, bt, 0, 0, 0});
        }
    }

    file.close();
    return pList;
}

// 2. Hàm xuất kết quả ra file output
void writeResultsToFile(string filename, const vector<Process>& p, string algorithmName, float avgWT, float avgTAT, int contextSwitches) {
    ofstream file(filename);

    if (!file.is_open()) {
        cout << "Loi: Khong the tao hoac ghi file " << filename << "!\n";
        return;
    }

    file << "Algorithm," << algorithmName << "\n";
    file << "Process_ID,Arrival_Time,Burst_Time,Completion_Time,Turnaround_Time,Waiting_Time\n";

    for (const auto& proc : p) {
        file << proc.id << ","
             << proc.arrivalTime << ","
             << proc.burstTime << ","
             << proc.completionTime << ","
             << proc.turnaroundTime << ","
             << proc.waitingTime << "\n";
    }

    file << "Context_Switches," << contextSwitches << "\n";
    file << "Average_Waiting_Time," << fixed << setprecision(2) << avgWT << "\n";
    file << "Average_Turnaround_Time," << fixed << setprecision(2) << avgTAT << "\n";

    file.close();
    cout << "-> Da xuat ket qua thanh cong vao file: " << filename << "\n";
}

// =========================================================================
// 3. THUẬT TOÁN SRTF (Preemptive)
// =========================================================================
void runSRTF(vector<Process> p) {
    int n = p.size();
    if (n == 0) return;

    int currentTime = 0;      
    int completed = 0;
string prevProcess = "";     
    int contextSwitches = 0;  

    cout << "\n================ THUAT TOAN SRTF (PREEMPTIVE) ===============\n";

    while (completed < n) {
        int idx = -1;
        int minRemaining = 1e9; 

        for (int i = 0; i < n; i++) {
            if (p[i].arrivalTime <= currentTime && p[i].remainingTime > 0) {
                if (p[i].remainingTime < minRemaining) {
                    minRemaining = p[i].remainingTime;
                    idx = i;
                }
            }
        }

        if (idx != -1) {
            if (prevProcess != p[idx].id) {
                if (!prevProcess.empty()) {
                    contextSwitches++;
                }
                prevProcess = p[idx].id;
            }

            p[idx].remainingTime--;
            currentTime++;

            if (p[idx].remainingTime == 0) {
                completed++;
                p[idx].completionTime = currentTime;
                p[idx].turnaroundTime = p[idx].completionTime - p[idx].arrivalTime;
                p[idx].waitingTime = p[idx].turnaroundTime - p[idx].burstTime;
            }
        } else {
            currentTime++;
        }
    }

    float totalWT = 0, totalTAT = 0;
    for (int i = 0; i < n; i++) {
        totalWT += p[i].waitingTime;
        totalTAT += p[i].turnaroundTime;
    }
    float avgWT = totalWT / n;
    float avgTAT = totalTAT / n;

    // In màn hình
    cout << "Process\tAT\tBT\tCT\tTAT\tWT\n";
    for (int i = 0; i < n; i++) {
        cout << p[i].id << "\t" << p[i].arrivalTime << "\t" << p[i].burstTime << "\t" 
             << p[i].completionTime << "\t" << p[i].turnaroundTime << "\t" << p[i].waitingTime << "\n";
    }
    cout << "-> Tong so lan chuyen CPU: " << contextSwitches << "\n";
    cout << "-> Avg WT: " << fixed << setprecision(2) << avgWT << " | Avg TAT: " << avgTAT << "\n";

    writeResultsToFile("output_srtf.csv", p, "SRTF", avgWT, avgTAT, contextSwitches);
}

// =========================================================================
// 4. THUẬT TOÁN SJF NON-PREEMPTIVE
// =========================================================================
void runSJF_NonPreemptive(vector<Process> p) {
    int n = p.size();
    if (n == 0) return;

    int currentTime = 0;
    int completed = 0;
    int contextSwitches = 0;
    string prevProcess = "";

    cout << "\n============= THUAT TOAN SJF (NON-PREEMPTIVE) =============\n";

    while (completed < n) {
        int idx = -1;
        int minBurst = 1e9;

        for (int i = 0; i < n; i++) {
            if (p[i].arrivalTime <= currentTime && p[i].remainingTime > 0) {
                if (p[i].burstTime < minBurst) {
                    minBurst = p[i].burstTime;
                    idx = i;
                }
            }
        }

        if (idx != -1) {
            if (!prevProcess.empty() && prevProcess != p[idx].id) contextSwitches++;
            prevProcess = p[idx].id;
currentTime += p[idx].burstTime;
            p[idx].remainingTime = 0;
            completed++;

            p[idx].completionTime = currentTime;
            p[idx].turnaroundTime = p[idx].completionTime - p[idx].arrivalTime;
            p[idx].waitingTime = p[idx].turnaroundTime - p[idx].burstTime;
        } else {
            currentTime++;
        }
    }

    float totalWT = 0, totalTAT = 0;
    for (int i = 0; i < n; i++) {
        totalWT += p[i].waitingTime;
        totalTAT += p[i].turnaroundTime;
    }
    float avgWT = totalWT / n;
    float avgTAT = totalTAT / n;

    // In màn hình
    cout << "Process\tAT\tBT\tCT\tTAT\tWT\n";
    for (int i = 0; i < n; i++) {
        cout << p[i].id << "\t" << p[i].arrivalTime << "\t" << p[i].burstTime << "\t" 
             << p[i].completionTime << "\t" << p[i].turnaroundTime << "\t" << p[i].waitingTime << "\n";
    }
    cout << "-> Tong so lan chuyen CPU: " << contextSwitches << "\n";
    cout << "-> Avg WT: " << fixed << setprecision(2) << avgWT << " | Avg TAT: " << avgTAT << "\n";

    writeResultsToFile("output_sjf_np.csv", p, "SJF_NonPreemptive", avgWT, avgTAT, contextSwitches);
}

int main() {
    string filename = "input.csv";
    
    // Đọc danh sách tiến trình từ file input.csv đã tạo trên OnlineGDB
    vector<Process> processes = readProcessesFromFile(filename);

    if (processes.empty()) {
        cout << "Khong co du lieu hoac chua tao file input.csv tren OnlineGDB!\n";
        return 1;
    }

    // Chạy thuật toán SRTF và SJF Non-Preemptive
    runSRTF(processes);
    runSJF_NonPreemptive(processes);

    return 0;
}