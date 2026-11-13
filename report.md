# Báo cáo đề tài OS_SCH_02
## SRTF và SJF không độc quyền

## 1. Giới thiệu
Lập lịch CPU là chức năng của hệ điều hành nhằm lựa chọn tiến trình trong hàng đợi sẵn sàng để cấp CPU. Đề tài xây dựng ứng dụng desktop mô phỏng và so sánh SJF không độc quyền với SRTF có độc quyền.

## 2. Mục tiêu
- Cài đặt hai thuật toán lập lịch CPU.
- Minh họa thứ tự thực thi bằng biểu đồ Gantt.
- Tính CT, TAT, WT và RT.
- Đọc dữ liệu từ CSV, kiểm tra dữ liệu đầu vào và xuất kết quả.
- So sánh kết quả trên cùng một bộ tiến trình.

## 3. Phạm vi và giả định
- Mỗi tiến trình có PID, Arrival Time và CPU Burst Time.
- Arrival Time là số nguyên không âm; Burst Time là số nguyên dương; PID không trùng.
- Thời gian chuyển ngữ cảnh được giả định bằng 0.
- Khi bằng nhau, chương trình dùng Arrival Time rồi PID để phá hòa.
- Đây là mô hình học tập, không mô phỏng đầy đủ mọi chi tiết của bộ lập lịch trong hệ điều hành thực.

## 4. Cơ sở lý thuyết
### 4.1. SJF không độc quyền
Khi CPU rảnh, chọn tiến trình đã đến có Burst Time nhỏ nhất. Khi đã bắt đầu, tiến trình chạy đến khi hoàn thành; tiến trình mới đến không làm ngắt tiến trình đang chạy.

### 4.2. SRTF có độc quyền
Tại mỗi thời điểm, chọn tiến trình đã đến có thời gian còn lại nhỏ nhất. Nếu một tiến trình mới đến có thời gian còn lại ngắn hơn, CPU có thể ngắt tiến trình hiện tại và chuyển sang tiến trình mới.

### 4.3. Công thức
- CT: thời điểm hoàn thành.
- TAT = CT - Arrival Time.
- WT = TAT - Burst Time.
- RT = thời điểm bắt đầu chạy lần đầu - Arrival Time.
- Thời gian chờ trung bình = tổng WT / số tiến trình.
- Turnaround trung bình = tổng TAT / số tiến trình.

## 5. Phân tích và thiết kế
### 5.1. Đầu vào
Danh sách tiến trình gồm PID, Arrival Time, Burst Time.

### 5.2. Đầu ra
Biểu đồ Gantt, bảng các chỉ số theo từng tiến trình và số liệu trung bình để so sánh hai thuật toán.

### 5.3. Quy trình xử lý
1. Nhập hoặc tải dữ liệu CSV.
2. Kiểm tra PID, Arrival Time, Burst Time.
3. Chạy SJF không độc quyền.
4. Chạy SRTF có độc quyền.
5. Tính các chỉ số.
6. Hiển thị biểu đồ Gantt và bảng kết quả.
7. Xuất kết quả CSV nếu cần.

## 6. Pseudocode
### 6.1. SJF không độc quyền
```text
while vẫn còn tiến trình chưa hoàn thành:
    lấy các tiến trình đã đến và chưa chạy
    nếu không có tiến trình sẵn sàng:
        nhảy thời gian đến lần đến tiếp theo
    ngược lại:
        chọn tiến trình có Burst Time nhỏ nhất
        chạy tiến trình đến khi hoàn thành
        ghi thời điểm bắt đầu và kết thúc
tính CT, TAT, WT, RT
```

### 6.2. SRTF có độc quyền
```text
while vẫn còn tiến trình chưa hoàn thành:
    lấy các tiến trình đã đến có thời gian còn lại > 0
    nếu không có tiến trình sẵn sàng:
        nhảy đến thời điểm đến tiếp theo
    ngược lại:
        chọn tiến trình có thời gian còn lại nhỏ nhất
        chạy 1 đơn vị thời gian
        nếu có tiến trình ngắn hơn đến, cho phép chuyển CPU
        ghi thời điểm chạy lần đầu và thời điểm hoàn thành
tính CT, TAT, WT, RT
```

## 7. Kiểm thử
Thực hiện ít nhất các trường hợp:
- Bộ dữ liệu thông thường có nhiều tiến trình đến ở các thời điểm khác nhau.
- Một tiến trình đến muộn, kiểm tra khoảng CPU rảnh.
- Chỉ có một tiến trình.
- Burst Time bằng nhau.
- Dữ liệu sai: PID trùng, Arrival Time âm, Burst Time bằng 0 hoặc nhập chữ.
- Kiểm tra đối chiếu biểu đồ Gantt với bảng kết quả và công thức.

| Trường hợp | Kết quả mong đợi |
|---|---|
| Dữ liệu hợp lệ | Chạy được cả hai thuật toán và hiện bảng kết quả |
| CPU không có tiến trình sẵn sàng | Hiện đoạn IDLE |
| PID trùng | Báo lỗi, không chạy mô phỏng |
| Burst Time <= 0 | Báo lỗi, không chạy mô phỏng |
| CSV sai cột | Báo định dạng CSV không hợp lệ |

## 8. Đánh giá hiệu năng
Chạy nhiều bộ dữ liệu có kích thước tăng dần, ví dụ 10, 50, 100, 500 tiến trình. Lặp mỗi lần đo 5 lần và lấy trung bình thời gian chạy thuật toán, không tính thời gian người dùng tương tác GUI. Ghi lại cấu hình máy, phiên bản Python, kích thước dữ liệu và phương pháp đo. Không điền số liệu giả; hãy chạy thực tế rồi bổ sung bảng:

| Số tiến trình | SJF trung bình (ms) | SRTF trung bình (ms) | Ghi chú |
|---:|---:|---:|---|
| 10 | ... | ... | ... |
| 50 | ... | ... | ... |
| 100 | ... | ... | ... |
| 500 | ... | ... | ... |

## 9. Kết quả và nhận xét
Chèn ảnh giao diện và kết quả chạy thật. Nhận xét dựa trên WT/TAT trung bình và số lần chuyển giữa tiến trình. SRTF thường có thể giảm thời gian chờ trung bình trong một số bộ dữ liệu nhưng có thể tăng số lần ngắt/chuyển CPU. Không khẳng định một thuật toán luôn tốt hơn trong mọi trường hợp.

## 10. Hạn chế và hướng phát triển
- Giả định thời gian chuyển ngữ cảnh bằng 0.
- Chưa mô phỏng ưu tiên, Round Robin hoặc I/O blocking.
- Có thể bổ sung xuất biểu đồ, tạo dữ liệu ngẫu nhiên và đo hiệu năng tự động.

## 11. Phân công và đóng góp cá nhân
Ghi rõ thành viên, nhiệm vụ, tệp phụ trách, commit Git và bằng chứng đóng góp. Mỗi thành viên cần hiểu thuật toán và có thể giải thích phần mình làm.
 STT | Họ và tên | MSSV | Nhiệm vụ / Output chịu trách nhiệm | Đánh giá |
|:---:|:---|:---:|:---|:---|:---:|
| 1 | **Ngụy Thái Dương** | `[072207010104]` | Quản lý tích hợp hệ thống, tài liệu tổng thể, mô hình toán học & giả định, flowchart kiến trúc, điều phối và tổng hợp báo cáo | **100%** |
| 2 | **Vũ Thanh Hùng** | `[075207011121]` | Module Core (`memory_manager.h/.cpp`): Cài đặt thuật toán First-Fit, Best-Fit, Worst-Fit; logic tách/gộp khối (`coalesce`), làm tròn đơn vị cấp phát, kiểm tra bất biến `validateState` | **100%** |
| 3 | **Trịnh Quang Huy** | `[075207004369]` | Xây dựng bộ test cases hồi quy (`test_memory_manager.cpp`), kiểm thử bất biến trạng thái, kiểm thử áp lực (stress test) và đo lường benchmark hiệu năng | **100%** |
| 4 | **Nguyễn Hoàng Long** | `[Điền MSSV]` | Thiết kế giao diện Qt6 Widgets (`main_window.ui/.cpp`), vẽ sơ đồ thanh bộ nhớ động (`memory_map_widget.cpp`), bộ điều phối mô phỏng (`simulation_controller.cpp`), cấu hình CI/CD GitHub Actions | **100%** |
| 5 | **Nguyễn Văn Trường** | `[075207008588]` | Module CSV Reader/Writer (`csv_handler.h/.cpp`); xây dựng bộ dữ liệu kiểm thử input/output (`config_sample.csv`, `events_*.csv`); soạn thảo phụ lục kỹ thuật và đặc tả dữ liệu | **100%** |
---


## 12. Tài liệu tham khảo
1. Giáo trình Hệ điều hành của môn học (bổ sung tên tác giả, năm và thông tin xuất bản theo tài liệu thực tế).
2. Silberschatz, A., Galvin, P. B., & Gagne, G., *Operating System Concepts* (bổ sung đúng ấn bản đã tham khảo).
3. Python Software Foundation, tài liệu Python: https://docs.python.org/3/
4. Python Software Foundation, tài liệu Tkinter: https://docs.python.org/3/library/tkinter.html

Chỉ giữ các tài liệu thực sự đã đọc và trích dẫn trong báo cáo.
