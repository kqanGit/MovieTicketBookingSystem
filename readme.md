# ĐỒ ÁN OOP: HỆ THỐNG ĐẶT VÉ XEM PHIM 
- Dự án gốc: https://www.codewithc.com/movie-ticket-booking-system-in-c-with-mysql/

### Mô tả đồ án 
- Hệ thống đặt vé xem phim được xây dựng theo hướng đối tượng (OOP), cho phép người dùng (guest, customer) dễ dàng:
+ Xem thông tin phim
+ Tìm kiếm phim theo tên, thể loại hoặc đánh giá
+ Đặt vé với lựa chọn suất chiếu, ghế ngồi và combo bắp nước
+ Thực hiện thanh toán và áp dụng mã giảm giá
+ Đăng nhập, đánh giá phim, và quản lý lịch sử đặt vé
+ Quản trị viên (Admin) có thể quản lý phim, rạp chiếu, và suất chiếu thông qua các chức năng riêng biệt.


## CÁC ĐỀ XUẤT VÀ DỰ ĐỊNH THỰC HIỆN

### Các chức năng cơ bản
#### **Người dùng (USER)**
- Đăng ký / Đăng nhập / Đăng xuất
- Xem danh sách phim đang chiếu
- Lọc phim theo: thể loại, ngày chiếu, độ tuổi
- Xem thông tin chi tiết phim
- Đặt vé:
  - Chọn phim → rạp → suất chiếu → ghế → combo bắp nước
  - Nhập mã giảm giá nếu có
  - Thanh toán
- Xem lịch sử đặt vé 
- Đánh giá và bình luận phim (rating & review)

#### **Quản lý (ADMIN)**
- Thêm / Xóa / Sửa phim
  - Tên, mô tả, thể loại, độ dài, độ tuổi, hình ảnh, trailer
- Quản lý cụm rạp và phòng chiếu
- Quản lý lịch chiếu cho từng phim / phòng / rạp
- Thêm / sửa / xóa combo bắp nước

### Môi trường thực hiện
- Ngôn ngữ: C++
- IDE: Visual Studio Code
- Hệ điều hành: Window
- Build: g++ compiler

### Kiến trúc phần mềm
- 3 Layer architecture
    - UI: Qt (nếu không kết hợp được github codespaces thì chuyển qua làm console)
    - Business logic: Xử lý đặt vé, kiểm tra ghế trống, áp dụng mã giảm giá...
    - Data access: SQL (nếu không tích hợp được thì dùng file .txt thay thế)
- SOLID architecture

- Design pattern: Factory 

### Unit test:
- Framework: gTest

### Thành viên
- Trương Thành Phát - 23120319
- Bửu Huỳnh Vĩnh Phúc - 23120328
- Bùi Minh Quân - 23120337 (Trưởng nhóm)
