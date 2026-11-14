```text
                                           SJF Non-Preemptive (Shortest Job First)
ALGORITHM SJF_Non_Preemptive(Process_List)
Input: Danh sách tiến trình với Arrival_Time và Burst_Time
Output: Mô phỏng chạy không ngắt (Non-preemptive) và thời gian hoàn thành

1.  current_time = 0
2.  completed_count = 0
3.  total_processes = length(Process_List)
4.  running_process = NULL
5.  last_pid = NULL
6.  context_switch = 0
7.  Khởi tạo ready_queue rỗng
8.  Khởi tạo completed_list rỗng
9.  Khởi tạo gantt rỗng

10. WHILE completed_count < total_processes DO
        // 1. Cập nhật tiến trình đã đến
        FOR EACH process P IN Process_List DO
            IF P.Arrival_Time <= current_time AND P NOT IN ready_queue AND P NOT IN completed_list THEN
                Add P to ready_queue
            END IF
        END FOR

        // 2. Chọn tiến trình (KHÔNG có preemption, chỉ chọn khi CPU rảnh)
        IF running_process == NULL AND ready_queue IS NOT EMPTY THEN
            // Hòa Burst -> chọn Arrival_Time nhỏ hơn -> hòa nữa -> chọn ID nhỏ hơn
            P_shortest = Process in ready_queue with MIN(Burst_Time)
            Remove P_shortest from ready_queue
            running_process = P_shortest
            PRINT "CPU bắt đầu chạy tiến trình: " + running_process.id + " tại t = " + current_time

            IF last_pid != NULL AND last_pid != running_process.id THEN
                context_switch = context_switch + 1
            END IF
            last_pid = running_process.id
        END IF

        // Bổ sung: Xử lý trường hợp CPU rảnh rỗi (IDLE)
        IF running_process == NULL AND ready_queue IS EMPTY THEN
            PRINT "CPU IDLE tại t = " + current_time
            last_pid = NULL
            current_time = current_time + 1
            CONTINUE
        END IF

        // 3. Thực thi một mạch đến hết Burst_Time (không bị ngắt)
        start_time = current_time
        current_time = current_time + running_process.Burst_Time
        Add (running_process.id, start_time, current_time) to gantt

        running_process.Completion_Time = current_time
        running_process.Turnaround_Time = running_process.Completion_Time - running_process.Arrival_Time
        running_process.Waiting_Time = running_process.Turnaround_Time - running_process.Burst_Time
        Add running_process to completed_list
        PRINT "Tiến trình " + running_process.id + " hoàn thành tại t = " + current_time

        running_process = NULL
        completed_count = completed_count + 1
    END WHILE

    avg_waiting = SUM(P.Waiting_Time) / total_processes
    avg_turnaround = SUM(P.Turnaround_Time) / total_processes
    PRINT "Gantt chart: " + gantt
    PRINT "Số lần chuyển CPU (Context Switch): " + context_switch
    PRINT "Số lần Preemption: 0"
    PRINT "Avg Waiting: " + avg_waiting + " | Avg Turnaround: " + avg_turnaround
END Algorithm
```
```text
                                                   SRTF (Shortest Remaining Time First)
ALGORITHM SRTF(Process_List) 
Input: Danh sách tiến trình với Arrival_Time và Burst_Time 
Output: Mô phỏng cơ chế Preemption và thời gian hoàn thành 
  
1.  current_time = 0 
2.  completed_count = 0 
3.  total_processes = length(Process_List) 
4.  running_process = NULL 
5.  Khởi tạo ready_queue rỗng 
6.  Khởi tạo completed_list rỗng 
  
7.  WHILE completed_count < total_processes DO 
        // 1. Cập nhật tiến trình mới đến 
        FOR EACH process P IN Process_List DO 
            IF P.Arrival_Time == current_time THEN 
                P.Remaining_Time = P.Burst_Time 
                Add P to ready_queue 
            END IF 
        END FOR 
  
        // 2. Kiểm tra cơ chế PREEMPTION
        IF ready_queue IS NOT EMPTY THEN 
            P_shortest = Process in ready_queue with MIN(Remaining_Time) 
  
            IF running_process == NULL THEN 
                Remove P_shortest from ready_queue 
                running_process = P_shortest 
                PRINT "CPU bắt đầu chạy tiến trình: " + running_process.id + " tại t = " + current_time 
             
            ELSE IF P_shortest.Remaining_Time < running_process.Remaining_Time THEN 
                // --- XẢY RA PREEMPTION --- 
                PRINT ">>> PREEMPTION tại t = " + current_time + ": Tiến trình " + running_process.id + " bị ngắt bởi " + P_shortest.id 
                 
                Add running_process back to ready_queue 
                Remove P_shortest from ready_queue 
                running_process = P_shortest 
                 
                PRINT "CPU Switch: Chuyển sang chạy tiến trình " + running_process.id 
            END IF 
        END IF 

        // Bổ sung: Xử lý trường hợp CPU rảnh rỗi (IDLE) 
        IF running_process == NULL AND ready_queue IS EMPTY THEN 
            PRINT "CPU IDLE tại t = " + current_time 
            current_time = current_time + 1 
            CONTINUE 
        END IF 
  
        // 3. Thực thi tiến trình đang chạy trong 1 đơn vị thời gian 
        IF running_process IS NOT NULL THEN 
            running_process.Remaining_Time = running_process.Remaining_Time - 1 
  
            IF running_process.Remaining_Time == 0 THEN 
                current_time_completed = current_time + 1 
                running_process.Completion_Time = current_time_completed 
                Add running_process to completed_list 
                PRINT "Tiến trình " + running_process.id + " hoàn thành tại t = " + current_time_completed 
                 
                running_process = NULL 
                completed_count = completed_count + 1 
            END IF 
        END IF 
  
        // 4. Tăng thời gian hệ thống 
        current_time = current_time + 1 
    END WHILE 
END Algorithm

I.	Đối chiếu giải thuật 
A. SJF Không độc quyền
1.	Tiến trình điều phối theo thời gian (Trace):
•	t = 0: Chỉ có P1 (AT = 0, BT = 7) trong hàng đợi -> Cấp CPU cho P1. Do tính chất non-preemptive, P1 chạy liên tục từ 0 -> 7. Trong thời gian này, P2(2,4), P3(4,1), P4(5,3) lần lượt đến và xếp hàng chờ.
•	t = 7: P1 hoàn tất (CT = 7). Hàng đợi hiện có: {P2(BT = 4), P3(BT = 1), P4(BT = 3)}. Tiến trình có BT nhỏ nhất là P3(BT = 1) -> Chuyển CPU lần 1 sang P3. P3 chạy từ 7 -> 8.
•	t = 8: P3 hoàn tất (CT = 8). Đúng lúc này P5(AT = 8, BT = 2) vừa đến. Hàng đợi hiện có: {P2(BT = 4), P4(BT = 3), P5(BT = 2)}. Tiến trình có BT nhỏ nhất là P5(BT = 2) -> Chuyển CPU lần 2 sang P5. P5 chạy từ 8 -> 10.
•	t = 10: P5 hoàn tất (CT = 10). Hàng đợi còn: {P2(BT = 4), P4(BT = 3)}. Chọn P4(BT = 3)    -> Chuyển CPU lần 3 sang P4. P4 chạy từ 10 -> 13.
•	t = 13: P4 hoàn tất (CT = 13). Hàng đợi chỉ còn P2(BT = 4) -> Chuyển CPU lần 4 sang P2. P2 chạy từ 13 -> 17 và kết thúc.
2.	Biểu đồ Gantt.
|-------P1(7)-------|-P3(1)-|--P5(2)--|---P4(3)---|----P2(4)----|
0                   7       8         10          13            17
3.	Thống kê chuyển ngữ cảnh.
•	Số lần bị ngắt quãng: 0
•	Số lần chuyển CPU: 4
B. SRTF(Shortest Remaining Time First)
1. Tiến trình điều phối theo thời gian (Trace):
•	t = 0: Chỉ có P1 (AT = 0, BT = 7, Remaining = 7) trong hàng đợi -> Cấp CPU cho P1. P1 chạy từ 0 -> 2.
•	t = 2: P2 đến (AT = 2, BT = 4, Remaining = 4). Lúc này P1 còn lại 5 đơn vị thời gian (7 - 2 = 5). So sánh thấy P2 ngắn hơn P1 (4 < 5) -> Preemption (ngắt quãng lần 1): Tạm dừng P1, chuyển CPU sang P2. P2 chạy từ 2 -> 4.
•	t = 4: P3 đến (AT = 4, BT = 1, Remaining = 1). Lúc này P2 còn lại 2 đơn vị thời gian (4 - 2 = 2). So sánh thấy P3 ngắn hơn P2 (1 < 2) -> Preemption (ngắt quãng lần 2): Tạm dừng P2, chuyển CPU sang P3. P3 chạy từ 4 -> 5.
•	t = 5: P3 chạy xong và hoàn tất (CT = 5). Hàng đợi còn lại P1 (còn 5), P2 (còn 2) và P4 vừa đến lúc t = 5 (BT = 3). Tiến trình có thời gian còn lại ngắn nhất là P2 (còn 2) -> Cấp CPU cho P2 chạy tiếp từ 5 -> 7.
•	t = 7: P2 hoàn tất (CT = 7). Hàng đợi còn P1 (còn 5) và P4 (còn 3). Tiến trình có thời gian còn lại ngắn nhất là P4 (3 < 5) -> Chuyển CPU sang P4. P4 chạy từ 7 -> 10.
•	t = 10: P4 hoàn tất (CT = 10). Lúc này P5 đã đến từ t = 8 (BT = 2). Hàng đợi còn P1 (còn 5) và P5 (còn 2). Tiến trình có thời gian còn lại ngắn nhất là P5 (2 < 5) -> Chuyển CPU sang P5. P5 chạy từ 10 -> 12.
•	t = 12: P5 hoàn tất (CT = 12). Hàng đợi chỉ còn lại duy nhất P1 -> Cấp CPU cho P1 chạy nốt phần còn lại từ 12 -> 17 và kết thúc.
2.  Biểu đồ Gantt.
  (P1=5)    (P2=2)   
|--P1(2)--|--P2(2)--|-P3(1)-|--P2(2)--|---P4(3)---|--P5(2)--|-----P1(5)-----|
0         2         4       5         7          10         12               17
3. Thống kê chuyển ngữ cảnh.
Số lần bị ngắt quãng: 2
•	Số lần chuyển CPU: 6.




