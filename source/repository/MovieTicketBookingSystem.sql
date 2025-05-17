CREATE TABLE MOVIE (
    MovieID INTEGER PRIMARY KEY AUTOINCREMENT, -- id
    Title TEXT,
    Genre TEXT,
    Duration INTEGER,
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
    FOREIGN KEY (MovieID) REFERENCES MOVIE(MovieID)
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
    FOREIGN KEY (SeatType) REFERENCES SEATTYPE(SeatType)
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
    FOREIGN KEY (ShowTimeID) REFERENCES SHOWTIME(ShowTimeID),
    FOREIGN KEY (UserID) REFERENCES ACCOUNT(UserID)
);

-- Tạo bảng BookSeat
CREATE TABLE BOOKSEAT (
    BookingID INTEGER,
    SeatID TEXT,
    PRIMARY KEY (BookingID, SeatID),
    FOREIGN KEY (BookingID) REFERENCES BOOKING(BookingID),
    FOREIGN KEY (SeatID) REFERENCES SEAT(SeatID)
);

-- Dữ liệu mẫu phim (7 phim)
INSERT INTO MOVIE (Title, Genre, Duration, Descriptions, Rating) VALUES
('Avengers', 'Action', 150, 'The superhero team saving the world', 8.5),
('Titanic', 'Romance', 195, 'A tragic love story on the doomed ship', 9.0),
('Inception', 'Sci-Fi', 148, 'A mind-bending journey through dreams', 8.8),
('The Godfather', 'Crime', 175, 'A mafia family drama across generations', 9.2),
('Interstellar', 'Sci-Fi', 169, 'A space exploration to save humanity', 8.6),
('Frozen', 'Animation', 102, 'A magical tale of two royal sisters', 7.5),
('Joker', 'Drama', 122, 'The origin story of the infamous villain', 8.4);

-- Dữ liệu suất chiếu (cho 2 phim đầu tiên)
INSERT INTO SHOWTIME (MovieID, Date, StartTime, EndTime) VALUES
(1, '2025-05-10', '18:00', '20:30'),
(2, '2025-05-11', '20:00', '22:15');

-- Dữ liệu loại ghế
INSERT INTO SEATTYPE VALUES
('Single', 50.0),
('Couple', 90.0);

-- Dữ liệu ghế ngồi
INSERT INTO SEAT VALUES 
('A1', 'Single', 50.0),
('A2', 'Single', 50.0),
('A3', 'Single', 50.0),
('B1', 'Couple', 90.0),
('B2', 'Couple', 90.0),
('B3', 'Couple', 90.0);

-- Dữ liệu tài khoản
INSERT INTO ACCOUNT (Password, RoleUser, Gmail, PhoneNumber, UserName) VALUES
('pass123', 'User', 'user1@gmail.com', '0912345678', 'Nguyen Van A'),
('admin456', 'Admin', 'admin@gmail.com', '0987654321', 'Tran Thi B');

-- Dữ liệu đặt vé
INSERT INTO BOOKING (ShowTimeID, UserID) VALUES
(1, 1),
(2, 2);

-- Dữ liệu đặt chỗ
INSERT INTO BOOKSEAT VALUES 
(1, 'A1'),
(1, 'A2'),
(2, 'B1');