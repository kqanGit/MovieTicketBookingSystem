-- Tạo database
CREATE DATABASE MovieTicketBookingSystem;
GO

USE MovieTicketBookingSystem;
GO

-- Bảng Movie: lưu thông tin phim
CREATE TABLE MOVIE (
    MovieID CHAR(3),
    Title NVARCHAR(30),
    Genre NVARCHAR(30),
    Descriptions NVARCHAR(100),
    Rating FLOAT,
    PRIMARY KEY (MovieID)
);

-- Bảng Showtime: lưu thông tin suất chiếu
CREATE TABLE SHOWTIME (
    ShowTimeID CHAR(3),
    MovieID CHAR(3),
    Date DATE,
    StartTime TIME,
    EndTime TIME,
    PRIMARY KEY (ShowTimeID)
);

-- Bảng SeatType: lưu các loại ghế và giá
CREATE TABLE SEATTYPE (
    SeatType NVARCHAR(30),
    Price FLOAT,
    PRIMARY KEY (SeatType, Price)
);

-- Bảng Seat: lưu danh sách ghế và liên kết với SeatType
CREATE TABLE SEAT (
    SeatID CHAR(3),
    SeatType NVARCHAR(30),
    Price FLOAT,
    PRIMARY KEY (SeatID),
    FOREIGN KEY (SeatType) REFERENCES SEATTYPE(SeatType)
);

-- Bảng Account: lưu thông tin người dùng
CREATE TABLE ACCOUNT (
    UserID CHAR(3),
    Password VARCHAR(30),
    RoleUser VARCHAR(30),
    Gmail VARCHAR(50),
    PhoneNumber VARCHAR(15),
    UserName VARCHAR(50),
    PRIMARY KEY (UserID)
);

-- Bảng Booking: mỗi lần đặt chỗ của người dùng cho một suất chiếu
CREATE TABLE BOOKING (
    BookingID CHAR(3),
    ShowTimeID CHAR(3),
    UserID CHAR(3),
    PRIMARY KEY (BookingID)
);

-- Bảng BookSeat: mỗi hàng biểu thị 1 ghế trong 1 booking (quan hệ N-N giữa Booking và Seat)
CREATE TABLE BOOKSEAT (
    BookingID CHAR(3),
    SeatID CHAR(3),
    PRIMARY KEY (BookingID, SeatID)
);

-------------------------- RÀNG BUỘC KHÓA NGOẠI ----------------------

-- SHOWTIME -> MOVIE
ALTER TABLE SHOWTIME
ADD CONSTRAINT FK_SHOWTIME_MOVIE
FOREIGN KEY (MovieID) REFERENCES MOVIE(MovieID);

-- BOOKING -> SHOWTIME
ALTER TABLE BOOKING
ADD CONSTRAINT FK_BOOKING_SHOWTIME
FOREIGN KEY (ShowTimeID) REFERENCES SHOWTIME(ShowTimeID);

-- BOOKING -> ACCOUNT
ALTER TABLE BOOKING
ADD CONSTRAINT FK_BOOKING_ACCOUNT
FOREIGN KEY (UserID) REFERENCES ACCOUNT(UserID);

-- BOOKSEAT -> BOOKING
ALTER TABLE BOOKSEAT
ADD CONSTRAINT FK_BOOKSEAT_BOOKING
FOREIGN KEY (BookingID) REFERENCES BOOKING(BookingID);

-- BOOKSEAT -> SEAT
ALTER TABLE BOOKSEAT
ADD CONSTRAINT FK_BOOKSEAT_SEAT
FOREIGN KEY (SeatID) REFERENCES SEAT(SeatID);

-------------------------- THÊM DỮ LIỆU -----------------------------

-- Thêm phim
INSERT INTO MOVIE VALUES 
('M01', N'Avengers', N'Hành động', N'Biệt đội siêu anh hùng giải cứu thế giới', 8.5),
('M02', N'Titanic', N'Tình cảm', N'Chuyện tình buồn trên con tàu định mệnh', 9.0);

-- Thêm suất chiếu
INSERT INTO SHOWTIME VALUES 
('S01', 'M01', '2025-05-10', '18:00', '20:30'),
('S02', 'M02', '2025-05-11', '20:00', '22:15');

-- Thêm loại ghế và giá
INSERT INTO SEATTYPE VALUES
(N'Đơn', 50.0),
(N'Đôi', 90.0);

-- Thêm ghế
INSERT INTO SEAT VALUES 
('A1', N'Đơn', 50.0),
('A2', N'Đơn', 50.0),
('A3', N'Đơn', 50.0),
('B1', N'Đôi', 90.0),
('B2', N'Đôi', 90.0),
('B3', N'Đôi', 90.0);

-- Thêm người dùng
INSERT INTO ACCOUNT VALUES 
('U01', 'pass123', 'Khách', 'user1@gmail.com', '0912345678', 'Nguyen Van A'),
('U02', 'admin456', 'Admin', 'admin@gmail.com', '0987654321', 'Tran Thi B');

-- Đặt vé: U01 đặt 2 ghế cho suất M01, U02 đặt 1 ghế cho suất M02
INSERT INTO BOOKING VALUES 
('B01', 'S01', 'U01'),
('B02', 'S02', 'U02');

-- Chi tiết ghế được đặt trong từng booking
INSERT INTO BOOKSEAT VALUES 
('B01', 'A1'),
('B01', 'A2'),
('B02', 'B1');
