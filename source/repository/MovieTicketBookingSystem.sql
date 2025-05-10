-- Tạo database-----
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

-- Insert Movies
INSERT INTO MOVIE VALUES 
('101', 'Avengers', 'Action', 'A team of superheroes saving the world', 8.5),
('102', 'Titanic', 'Romance', 'A tragic love story on the doomed ship', 9.0);

-- Insert Showtimes
INSERT INTO SHOWTIME VALUES 
('201', '101', '2025-05-10', '18:00', '20:30'),
('202', '102', '2025-05-11', '20:00', '22:15');

-- Insert Seat Types
INSERT INTO SEATTYPE VALUES
('Single', 50.0),
('Double', 90.0);

-- Insert Seats
INSERT INTO SEAT VALUES 
('A1', 'Single', 50.0),
('A2', 'Single', 50.0),
('A3', 'Single', 50.0),
('B1', 'Double', 90.0),
('B2', 'Double', 90.0),
('B3', 'Double', 90.0);

-- Insert Accounts
INSERT INTO ACCOUNT VALUES 
('301', 'pass123', 'Customer', 'user1@gmail.com', '0912345678', 'John Nguyen'),
('302', 'admin456', 'Admin', 'admin@gmail.com', '0987654321', 'Anna Tran');

-- Insert Bookings
INSERT INTO BOOKING VALUES 
('401', '201', '301'),
('402', '202', '302');

-- Insert Booked Seats
INSERT INTO BOOKSEAT VALUES 
('401', 'A1'),
('401', 'A2'),
('402', 'B1');

