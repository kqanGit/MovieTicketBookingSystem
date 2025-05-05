--CREATE DATABASE MovieTicketBookingSystem
--GO 

--USE MovieTicketBookingSystem
--GO

--CREATE TABLE MOVIE (
--    MovieID char(3), 
--    Title nvarchar(30),
--    Genre nvarchar(30),
--    Descriptions nvarchar(30),
--    Rating FLOAT,
--    PRIMARY KEY(MovieID)
--);

--CREATE TABLE SHOWTIME(
--	ShowTimeID char(3),
--	MovieID char(3),
--	SeatID char(3),
--	StatusSeat nvarchar(30),
--	Dates date,
--	StartTime Time,
--	EndTime Time,

--	PRIMARY KEY(ShowTimeID, MovieID, SeatID)
--);

--CREATE TABLE SEAT(
--	SeatID char(3),
--	BookingID char(3),

--	PRIMARY KEY(SeatID)
--);

--CREATE TABLE BOOKING(
--	BookingID char(3),
--	ShowTimeID char(3),
--	MovieID char(3),
--	SeatID char(3),
--	UserID char(3),

--	PRIMARY KEY(BookingID, ShowTimeID, MovieID, SeatID, UserID)
--);

--CREATE TABLE ACCOUNT(
--	UserID char(3),
--	Password varchar(30),
--	RoleUser varchar(30),
--	Gmail varchar(30),
--	PhoneNumber varchar(30),

--	PRIMARY KEY(UserID)
--)


--ALTER TABLE SHOWTIME ADD CONSTRAINT FK_SHOWTIME_MOVIE FOREIGN KEY MovieID REFERENCES MOVIE(MovieID)

--ALTER TABLE SHOWTIME ADD CONSTRAINT FK_SHOWTIME_SEAT FOREIGN KEY SeatID REFERENCES SEAT(SeatID)

--ALTER TABLE SEAT ADD CONSTRAINT FK_SEAT_BOOKING FOREIGN KEY BookingID REFERENCES BOOKING(BookingID)

--ALTER TABLE BOOKING ADD CONSTRAINT FK_BOOKING_SHOWTIME FOREIGN KEY (ShowTimeID, MovieID, SeatID) REFERENCES SHOWTIME(ShowTimeID, MovieID, SeatID)

--ALTER TABLE BOOKING ADD CONSTRAINT FK_BOOKING_ACCOUNT FOREIGN KEY UserID REFERENCES ACCOUNT(UserID)


CREATE DATABASE MovieTicketBookingSystem;
GO

USE MovieTicketBookingSystem;
GO

-- Bảng Movie: lưu thông tin phim
CREATE TABLE MOVIE (
    MovieID CHAR(3),
    Title NVARCHAR(30),
    Genre NVARCHAR(30),
    Descriptions NVARCHAR(30),
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

-- Bảng Seat: lưu danh sách các ghế trong rạp
CREATE TABLE SEAT (
    SeatID CHAR(3),
	SeatType NVARCHAR(30),
    PRIMARY KEY (SeatID)
);

-- Bảng Account: lưu thông tin người dùng
CREATE TABLE ACCOUNT (
    UserID CHAR(3),
    Password VARCHAR(30),
    RoleUser VARCHAR(30),
    Gmail VARCHAR(30),
    PhoneNumber VARCHAR(30),
    PRIMARY KEY (UserID)
);

-- Bảng Booking: người dùng đặt ghế nào cho suất chiếu nào
CREATE TABLE BOOKING (
    BookingID CHAR(3),
    ShowTimeID CHAR(3),
    SeatID CHAR(3),
    UserID CHAR(3),
    PRIMARY KEY (BookingID),
    -- Ràng buộc thêm bên dưới
);


-- Ràng buộc SHOWTIME -> MOVIE
ALTER TABLE SHOWTIME
ADD CONSTRAINT FK_SHOWTIME_MOVIE
FOREIGN KEY (MovieID) REFERENCES MOVIE(MovieID);

-- Ràng buộc BOOKING -> SHOWTIME
ALTER TABLE BOOKING
ADD CONSTRAINT FK_BOOKING_SHOWTIME
FOREIGN KEY (ShowTimeID) REFERENCES SHOWTIME(ShowTimeID);

-- Ràng buộc BOOKING -> SEAT
ALTER TABLE BOOKING
ADD CONSTRAINT FK_BOOKING_SEAT
FOREIGN KEY (SeatID) REFERENCES SEAT(SeatID);

-- Ràng buộc BOOKING -> ACCOUNT
ALTER TABLE BOOKING
ADD CONSTRAINT FK_BOOKING_ACCOUNT
FOREIGN KEY (UserID) REFERENCES ACCOUNT(UserID);


INSERT INTO MOVIE VALUES 
('M01', N'Avengers', N'Hành động', N'Biệt đội siêu anh hùng', 8.5),
('M02', N'Titanic', N'Tình cảm', N'Chuyện tình trên biển', 9.0);

INSERT INTO SHOWTIME VALUES 
('S01', 'M01', '2025-05-10', '18:00', '20:30'),
('S02', 'M02', '2025-05-11', '20:00', '22:15');

INSERT INTO SEAT VALUES 
('A1', N'Đơn'),
('A2', N'Đơn'),
('A3', N'Đơn'),
('B1', N'Đôi'),
('B2', N'Đôi'),
('B3', N'Đôi');


INSERT INTO ACCOUNT VALUES 
('U01', 'pass123', 'Khách', 'user1@gmail.com', '0912345678'),
('U02', 'admin456', 'Admin', 'admin@gmail.com', '0987654321');

INSERT INTO BOOKING VALUES 
('B01', 'S01', 'A1', 'U01'),  -- U01 đặt ghế đơn cho suất M01
('B02', 'S01', 'B1', 'U02'),  -- U02 đặt ghế đôi cho suất M01
('B03', 'S02', 'A2', 'U01');  -- U01 đặt ghế đơn cho suất M02
