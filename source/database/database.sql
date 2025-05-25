
-- Bật khóa ngoại
PRAGMA foreign_keys = ON;

-- Tạo bảng Movie
CREATE TABLE MOVIE (
    MovieID INTEGER PRIMARY KEY AUTOINCREMENT,
    Title TEXT,
    Genre TEXT,
    Descriptions TEXT,
    Rating REAL
);

-- Tạo bảng Showtime
CREATE TABLE SHOWTIME (
    ShowTimeID INTEGER PRIMARY KEY AUTOINCREMENT,
    MovieID INTEGER,
    Date TEXT,
    StartTime TEXT,
    EndTime TEXT,
    FOREIGN KEY (MovieID) REFERENCES MOVIE(MovieID) ON DELETE CASCADE
);

-- Tạo bảng SeatType
CREATE TABLE SEATTYPE (
    SeatType TEXT PRIMARY KEY,
    Price REAL
);

-- Tạo bảng Seat
CREATE TABLE SEAT (
    SeatID TEXT PRIMARY KEY,
    SeatType TEXT,
    Price REAL,
    FOREIGN KEY (SeatType) REFERENCES SEATTYPE(SeatType) ON DELETE CASCADE
);

-- Tạo bảng Account
CREATE TABLE ACCOUNT (
    UserID INTEGER PRIMARY KEY AUTOINCREMENT,
    Password TEXT,
    RoleUser TEXT,
    Gmail TEXT,
    PhoneNumber TEXT,
    UserName TEXT
);

-- Tạo bảng Booking
CREATE TABLE BOOKING (
    BookingID INTEGER PRIMARY KEY AUTOINCREMENT,
    ShowTimeID INTEGER,
    UserID INTEGER,
    FOREIGN KEY (ShowTimeID) REFERENCES SHOWTIME(ShowTimeID) ON DELETE CASCADE,
    FOREIGN KEY (UserID) REFERENCES ACCOUNT(UserID) ON DELETE CASCADE
);

-- Tạo bảng BookSeat
CREATE TABLE BOOKSEAT (
    BookingID INTEGER,
    SeatID TEXT,
    PRIMARY KEY (BookingID, SeatID),
    FOREIGN KEY (BookingID) REFERENCES BOOKING(BookingID) ON DELETE CASCADE,
    FOREIGN KEY (SeatID) REFERENCES SEAT(SeatID) ON DELETE CASCADE
);

-- Dữ liệu mẫu
INSERT INTO MOVIE (Title, Genre, Descriptions, Rating) VALUES
('Avengers', 'Action', 'The superhero team saving the world', 8.5),
('Titanic', 'Romance', 'A tragic love story on the doomed ship', 9.0);

INSERT INTO SHOWTIME (MovieID, Date, StartTime, EndTime) VALUES
(1, '2025-05-10', '18:00', '20:30'),
(2, '2025-05-11', '20:00', '22:15');

INSERT INTO SEATTYPE VALUES
('Single', 50.0),
('Couple', 90.0);

INSERT INTO SEAT VALUES 
('A1', 'Single', 50.0),
('A2', 'Single', 50.0),
('A3', 'Single', 50.0),
('B1', 'Couple', 90.0),
('B2', 'Couple', 90.0),
('B3', 'Couple', 90.0);

INSERT INTO ACCOUNT (Password, RoleUser, Gmail, PhoneNumber, UserName) VALUES
('pass123', 'User', 'user1@gmail.com', '0912345678', 'Nguyen Van A'),
('admin456', 'Admin', 'admin@gmail.com', '0987654321', 'Tran Thi B');

INSERT INTO BOOKING (ShowTimeID, UserID) VALUES
(1, 1),
(2, 2);

INSERT INTO BOOKSEAT VALUES 
(1, 'A1'),
(1, 'A2'),
(2, 'B1');
