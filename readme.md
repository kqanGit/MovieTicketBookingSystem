# ĐỒ ÁN OOP: HỆ THỐNG ĐẶT VÉ XEM PHIM 
- Dự án gốc: https://www.codewithc.com/movie-ticket-booking-system-in-c-with-mysql/

### Mô tả đồ án 
- Hệ thống đặt vé xem phim được xây dựng theo hướng đối tượng (OOP) quản lý 1 rạp phim có 1 phòng duy nhất, cho phép người dùng (guest, user) dễ dàng:
+ Xem thông tin phim
+ Đặt vé với lựa chọn suất chiếu, ghế ngồi 
+ Đăng nhập, đăng ký, quản lý lịch sử đặt vé
+ Quản trị viên (Admin) có thể quản lý phim, thêm phim, xóa phim


## CÁC ĐỀ XUẤT VÀ DỰ ĐỊNH THỰC HIỆN (Chỉnh sửa)

### Các chức năng cơ bản
#### **Người dùng (GUEST)**
- Đăng nhập, đăng kí 
- Có thể xem danh sách phim đang chiếu, và chi tiết phim

#### **Người dùng (USER)**
- Đăng xuất
- Xem danh sách phim đang chiếu
- Lọc phim theo: thể loại, ngày chiếu, độ tuổi
- Xem thông tin chi tiết phim
- Đặt vé:
  - Chọn phim → suất chiếu → ghế 
- Xem lịch sử đặt vé 

#### **Quản lý (ADMIN)**
- Chức năng tương tự User
- Thêm / Xóa / Sửa phim

### Kiến trúc phần mềm
#### Mô hình kiến trúc
- 3 layers architecture
    - UI: Giao diện người dùng sử dụng Qt
    - Business logic: Xử lý các logic nghiệp vụ như đặt vé, kiểm tra ghế trống, xử lý đăng nhập,...
    - Data access: 	Quản lý truy vấn, cập nhật dữ liệu với SQLite thông qua DatabaseConnection
#### Nguyên tắc thiết kế
- SOLID
- Giao tiếp giữa các lớp thông qua DTO


#### Các mẫu thiết kế áp dụng
- FactorFactory Method: Tạo đối tượng UserContext phù hợp với vai trò (Guest/User/Admin)
- Singleton: Quản lý kết nối đến SQLite qua lớp DatabaseConnection
- Repository Pattern: Quản lý dữ liệu phim, đặt vé, người dùng,... độc lập với logic xử lý
- Dependency Injection: Giảm phụ thuộc giữa các lớp, dễ test và mở rộng

### Kiểm đảm bảo chất lượng phần mềm
#### Đảm bảo chất lượng code
- Sử dụng coding convention C++, đặt tên rõ ràng, phân tách logic theo SOLID.
- Kiểm tra thủ công và code review trong nhóm để phát hiện lỗi logic sớm.

### Quản lý mã nguồn
- Sử dụng **GitHub** để quản lý code, phân chia nhánh theo chức năng.

#### Kiểm thử 
- Unit test các lớp service (LoginService, BookingService, ...) bằng gTest
- Manual Test toàn bộ use case thông qua GUI


- Framework: gTest

### Môi trường thực hiện
- Ngôn ngữ: C++
- IDE: Visual Studio Code
- Hệ điều hành: Window
- Build: g++ compiler
- Giao diện: Qt
- Database: SQLite3

### Thành viên
- Trương Thành Phát - 23120319
- Bửu Huỳnh Vĩnh Phúc - 23120328
- Bùi Minh Quân - 23120337 (Trưởng nhóm)


## 👨‍👩‍👦 Thành viên và phân công

| STT | Họ tên                  | MSSV      | Vai trò        | Công việc | Mức độ hoàn thành công việc được giao tới thời điểm hiện tại |
|-----|-------------------------|-----------|----------------|--------------------------------------|----------|
| 1   | **Trương Thành Phát**    | 23120319  | Thành viên       | - Module Movie + Vẽ ER và thiết kế Database                                 | 30%
| 2   | **Bửu Huỳnh Vĩnh Phúc**  | 23120328  | Thành viên       | - Module Authentication (login/register)<br> - Tạo kết nối, truy vấn database bằng SQLite               | 30%
| 3   | **Bùi Minh Quân**        | 23120337  | Nhóm trưởng      | - Module Booking & Seat<br> & ShowTime - Thiết kế kiến trúc, vẽ class diagram, usecase diagram, viết báo cáo, quản lý tiến độ                | 40%

---

## 📊 Tỉ lệ đóng góp

| Thành viên                | Tỉ lệ đóng góp |
|---------------------------|----------------|
| Trương Thành Phát         | 33%            | 
| Bửu Huỳnh Vĩnh Phúc       | 33%            | 
| Bùi Minh Quân             | 34%            | 


## Tỉ lệ điểm
- Chia đều


---

## 🧪 Cách đánh giá đồ án

| Tiêu chí                            | Mô tả thực hiện của nhóm                                  |
|-------------------------------------|------------------------------------------------------------|
| Tính đúng đắn                       | Hệ thống hoàn chỉnh với đăng nhập, đặt vé, xem lịch sử... |
| Áp dụng hướng đối tượng             | Xây dựng module hóa theo domain (Movie, Booking, Auth...) |
| Thiết kế hệ thống tốt               | Sơ đồ lớp rõ ràng, mô-đun độc lập, giao tiếp chuẩn hóa     |
| Design Pattern                      | Singleton, Factory, Repository                             |
| Có kiểm thử                         | gTest cho Booking, Login, Movie filtering                  |
| Giao diện                           | Qt đơn giản, chức năng đầy đủ                             |
| Quản lý nhóm                        | Có phân công, theo dõi tiến độ qua GitHub                 |
| Tài liệu đầy đủ                     | UML, báo cáo markdown, mô tả chức năng                     |

---


## Video
Chưa có