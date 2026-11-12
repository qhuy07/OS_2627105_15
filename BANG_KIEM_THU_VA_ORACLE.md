# Bảng Kiểm thử Chi tiết & Đối chiếu Oracle Tính tay — Đề tài OS_SCH_02

Tài liệu này trình bày chi tiết phương pháp tính tay (Oracle), kết quả kỳ vọng (Expected Result) và kết quả thực tế (Actual Result) từ bộ kiểm thử tự động `tests/unit_tests.cpp` cùng các file xuất trong thư mục `output/`.

## 1. Đối chiếu Oracle Tính tay cho Kịch bản Chuẩn Giáo trình (`TC01`)

### Dữ liệu đầu vào (`input/tc01_textbook_preemption.csv`)
| Tiến trình (PID) | Thời điểm đến (Arrival Time - AT) | Thời gian thực thi (Burst Time - BT) |
| :--- | :--- | :--- |
| **P1** | 0 | 8 |
| **P2** | 1 | 4 |
| **P3** | 2 | 9 |
| **P4** | 3 | 5 |

### 1.1. Tính tay theo SJF Không Độc Quyền (Non-Preemptive SJF)
- **Tại $t = 0$**: Chỉ có `P1` trong hàng đợi sẵn sàng $\rightarrow$ Cấp CPU cho `P1` chạy hết 8 đơn vị thời gian $[0..8]$. Trong khoảng này, `P2` ($AT=1, BT=4$), `P3` ($AT=2, BT=9$), `P4` ($AT=3, BT=5$) lần lượt đến hàng đợi nhưng **không được chiếm CPU**.
- **Tại $t = 8$**: `P1` hoàn thành ($CT_{P1}=8$). Hàng đợi có $\{P2(4), P3(9), P4(5)\}$ $\rightarrow$ Chọn `P2` có $BT=4$ nhỏ nhất, chạy từ $[8..12]$ ($CT_{P2}=12$).
- **Tại $t = 12$**: `P2` hoàn thành. Hàng đợi còn $\{P3(9), P4(5)\}$ $\rightarrow$ Chọn `P4` ($BT=5$), chạy từ $[12..17]$ ($CT_{P4}=17$).
- **Tại $t = 17$**: `P4` hoàn thành. Chọn `P3` ($BT=9$), chạy từ $[17..26]$ ($CT_{P3}=26$).
- **Chỉ số tính tay (SJF)**:
  - $TAT = CT - AT$: $P1 = 8 - 0 = 8$; $P2 = 12 - 1 = 11$; $P3 = 26 - 2 = 24$; $P4 = 17 - 3 = 14 \Rightarrow \text{Avg TAT} = \frac{8+11+24+14}{4} = \mathbf{14.25}$.
  - $WT = TAT - BT$: $P1 = 0$; $P2 = 7$; $P3 = 15$; $P4 = 9 \Rightarrow \text{Avg WT} = \frac{0+7+15+9}{4} = \mathbf{7.75}$.
  - Số lần chuyển CPU (Context Switches): **3 lần** (tại $t = 8, 12, 17$). Số lần Preemption: **0**.

### 1.2. Tính tay theo SRTF Có Chiếm Quyền (Preemptive Shortest Remaining Time First)
- **Tại $t = 0$**: `P1` bắt đầu chạy với $RT_{P1} = 8$.
- **Tại $t = 1$**: `P2` đến với $BT_{P2} = 4$. Lúc này `P1` đã chạy được 1 đơn vị nên còn $RT_{P1} = 7$. Vì $RT_{P2} = 4 < RT_{P1} = 7 \Rightarrow$ **Xảy ra Preemption #1**: `P2` chiếm CPU của `P1`.
- **Tại $t = 2$**: `P3` đến với $BT_{P3} = 9 > RT_{P2} = 3 \Rightarrow$ `P2` tiếp tục chạy.
- **Tại $t = 3$**: `P4` đến với $BT_{P4} = 5 > RT_{P2} = 2 \Rightarrow$ `P2` tiếp tục chạy đến khi hoàn tất tại $t = 5$ ($CT_{P2}=5$).
- **Tại $t = 5$**: Hàng đợi còn $\{P1(RT=7), P3(RT=9), P4(RT=5)\}$ $\rightarrow$ Chọn `P4` ($RT=5$ nhỏ nhất), chạy một mạch từ $[5..10]$ ($CT_{P4}=10$).
- **Tại $t = 10$**: Hàng đợi còn $\{P1(RT=7), P3(RT=9)\}$ $\rightarrow$ Khôi phục `P1` chạy tiếp từ $[10..17]$ ($CT_{P1}=17$).
- **Tại $t = 17$**: Chỉ còn `P3` ($RT=9$), chạy từ $[17..26]$ ($CT_{P3}=26$).
- **Chỉ số tính tay (SRTF)**:
  - $TAT = CT - AT$: $P1 = 17 - 0 = 17$; $P2 = 5 - 1 = 4$; $P3 = 26 - 2 = 24$; $P4 = 10 - 3 = 7 \Rightarrow \text{Avg TAT} = \frac{17+4+24+7}{4} = \mathbf{13.00}$.
  - $WT = TAT - BT$: $P1 = 9$; $P2 = 0$; $P3 = 15$; $P4 = 2 \Rightarrow \text{Avg WT} = \frac{9+0+15+2}{4} = \mathbf{6.50}$.
  - $RT = ST - AT$: $P1 = 0 - 0 = 0$; $P2 = 1 - 1 = 0$; $P3 = 17 - 2 = 15$; $P4 = 5 - 3 = 2 \Rightarrow \text{Avg RT} = \frac{0+0+15+2}{4} = \mathbf{4.25}$.
  - Số lần chuyển CPU (Context Switches): **4 lần** (tại $t = 1, 5, 10, 17$). Số lần Preemption: **1 lần** (tại $t = 1$).

## 2. Bảng Tổng hợp Toàn bộ 7 Kịch bản Kiểm thử (Expected vs. Actual)

| Mã Test | Mục tiêu Kiểm thử & Đặc trưng | Dữ liệu Đầu vào (`(PID, AT, BT)`) | Kết quả Kỳ vọng (Expected Oracle) | Kết quả Thực tế (Actual Output) | Trạng thái |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **TC01** | Trường hợp chuẩn giáo trình (1 lần Preemption) | `P1(0,8), P2(1,4), P3(2,9), P4(3,5)` | SJF: Avg WT=`7.75`, Avg TAT=`14.25`, Preempt=`0`<br>SRTF: Avg WT=`6.50`, Avg TAT=`13.00`, Avg RT=`4.25`, Preempt=`1` (t=1) | SJF: WT=`7.75`, TAT=`14.25`, Preempt=`0`<br>SRTF: WT=`6.50`, TAT=`13.00`, RT=`4.25`, Preempt=`1` | **PASSED** |
| **TC02** | Preemption dây chuyền liên tiếp (Cascade Preemption) | `P1(0,12), P2(2,7), P3(4,3), P4(5,1)` | SJF: Gantt `P1->P4->P3->P2`, Avg WT=`8.50`<br>SRTF: 3 lần Preemption tại `t=2, 4, 5`; CT: `P4=6, P3=8, P2=13, P1=23`; Avg WT=`4.25`, Avg RT=`0.00` | SJF: Avg WT=`8.50`, Preempt=`0`<br>SRTF: 3 Preemptions (`t=2,4,5`), CT=`(23,13,8,6)`, Avg WT=`4.25`, Avg RT=`0.00` | **PASSED** |
| **TC03** | Phá hòa (Tie-breaking) & Ngưỡng bằng Remaining Time | `P1(0,2), P2(1,4), P3(1,4), P4(3,2), P5(2,6)` | Tại `t=2`, `P2` và `P3` cùng AT=1, BT=4 $\rightarrow$ Chọn `P2` theo thứ tự đầu vào. Tại `t=3`, `P4(BT=2)` < `P2(RT=3)` $\rightarrow$ Chiếm quyền. Khi 2 tiến trình có RT bằng nhau $\rightarrow$ Không chiếm quyền. | SJF: Avg WT=`4.40`, Preempt=`0`<br>SRTF: Avg WT=`3.80`, Avg RT=`2.20`, Preempt=`1` (`P4` chiếm `P2` lúc `t=3`) | **PASSED** |
| **TC04** | Khoảng trống CPU rỗi (CPU IDLE Gaps) | `P1(2,3), P2(8,6), P3(10,2), P4(18,4)` | CPU IDLE trong 3 khoảng `[0..2]`, `[5..8]`, `[16..18]` (Tổng IDLE = `7`). Tại `t=10`, `P3(BT=2)` chiếm CPU của `P2(RT=4)`. | SJF: Total IDLE=`7`, Avg WT=`0.75`<br>SRTF: Total IDLE=`7`, Avg WT=`0.25`, Preempt=`1` | **PASSED** |
| **TC05** | Trường hợp biên: Tất cả tiến trình đến tại $t = 0$ | `P1(0,6), P2(0,2), P3(0,8), P4(0,3), P5(0,4)` | Khi mọi $AT = 0$, không có tiến trình mới nào đến giữa chừng $\rightarrow$ SJF và SRTF cho thứ tự thực thi `P2->P4->P5->P1->P3` và chỉ số giống hệt nhau. | SJF & SRTF trùng khớp tuyệt đối: Avg WT=`7.00`, Avg TAT=`11.60`, Preempt=`0` | **PASSED** |
| **TC06** | Hiện tượng đói CPU (Starvation) của tiến trình dài | `P1(0,20), P2(1,2), P3(2,3), P4(4,2), P5(6,1), P6(7,2)` | Trong SRTF, `P1(BT=20)` bị chiếm quyền tại `t=1` bởi `P2` và phải chờ toàn bộ chuỗi tiến trình ngắn `P2..P6` chạy xong mới được chạy tiếp từ `[11..30]`. | SJF: Avg WT=`12.83` (`P1` giữ CPU làm `P2..P6` chờ lâu)<br>SRTF: Avg WT=`3.67`, `P1` chịu WT=`10` | **PASSED** |
| **TC07** | Kiểm thử bắt lỗi dữ liệu rỗng, âm, trùng PID hoặc sai cú pháp CSV | `P1(0,5), P2(-2,4), P3(1,0)` & chuỗi CSV lỗi | Bộ kiểm tra `validate_inputs` và `CsvIO` từ chối dữ liệu lỗi, trả về thông báo lỗi rõ ràng, **không crash** ứng dụng. | Trả về mã lỗi và thông báo chi tiết (`Arrival Time (-2) phai >= 0`), không crash | **PASSED** |

## 3. Bảng Kết quả Đo Hiệu năng & Stress Test (`output/benchmark_performance.csv`)

Thử nghiệm được thực hiện bằng công cụ `tests/stress_benchmark.cpp` sử dụng đồng hồ độ phân giải cao `std::chrono::high_resolution_clock` với bộ sinh số ngẫu nhiên có hạt giống cố định (`std::mt19937`) để đảm bảo tính tái lập:

| Kích thước dữ liệu ($N$ tiến trình) | Thời gian chạy SJF ($\mu s$) | Thời gian chạy SRTF ($\mu s$) | SJF Avg Waiting Time | SRTF Avg Waiting Time | Tổng Context Switches (SJF) | Tổng Context Switches (SRTF) | Số lần Preemption (SRTF) |
| ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| **10** | 139.95 | 36.86 | 109.50 | 108.90 | 10 | 11 | 1 |
| **50** | 136.77 | 446.29 | 322.56 | 322.20 | 49 | 51 | 2 |
| **100** | 319.88 | 408.81 | 526.98 | 525.31 | 99 | 111 | 12 |
| **500** | 3,123.27 | 3,420.39 | 3,420.07 | 3,419.72 | 499 | 540 | 41 |
| **1,000** | 7,117.25 | 11,761.69 | 7,220.88 | 7,220.30 | 999 | 1,098 | 99 |
| **2,500** | 45,229.55 | 82,576.92 | 18,238.05 | 18,224.93 | 2,500 | 2,704 | 204 |
| **5,000** | 204,577.26 | 295,239.25 | 36,245.78 | 36,245.30 | 5,000 | 5,400 | 400 |
| **10,000** | 938,736.60 | 1,423,142.08 | 72,877.55 | 72,873.93 | 10,000 | 10,899 | 899 |
