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

----------------------------- DỮ LIỆU MẪU ----------------------

-- Insert movies
INSERT INTO MOVIE (Title, Genre, Descriptions, Rating) VALUES
('Avengers', 'Action', 'The superhero team saving the world', 8.5),
('Titanic', 'Romance', 'A tragic love story on the doomed ship', 9.0);

-- Assuming MovieID: Avengers=1, Titanic=2
INSERT INTO SHOWTIME (MovieID, Date, StartTime, EndTime) VALUES
(1, '2025-05-10', '18:00', '20:30'),
(2, '2025-05-11', '20:00', '22:15');

-- Insert seat types
INSERT INTO SEATTYPE VALUES
('Single', 50.0),
('Couple', 90.0);

-- Insert seats
INSERT INTO SEAT VALUES 
('A1', 'Single', 50.0),
('A2', 'Single', 50.0),
('A3', 'Single', 50.0),
('B1', 'Couple', 90.0),
('B2', 'Couple', 90.0),
('B3', 'Couple', 90.0);

-- Insert accounts
INSERT INTO ACCOUNT (Password, RoleUser, Gmail, PhoneNumber, UserName) VALUES
('pass123', 'User', 'user1@gmail.com', '0912345678', 'Nguyen Van A'),
('admin456', 'Admin', 'admin@gmail.com', '0987654321', 'Tran Thi B');

-- Assuming UserID: Nguyen Van A = 1, Tran Thi B = 2; ShowTimeID = 1 and 2
INSERT INTO BOOKING (ShowTimeID, UserID) VALUES
(1, 1),
(2, 2);

-- Assuming BookingID: 1 and 2
INSERT INTO BOOKSEAT VALUES 
(1, 'A1'),
(1, 'A2'),
(2, 'B1');
