
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

-- Ghế Single: A1–A10
INSERT INTO SEAT VALUES 
('A1', 'Single', 50.0), ('A2', 'Single', 50.0), ('A3', 'Single', 50.0),
('A4', 'Single', 50.0), ('A5', 'Single', 50.0), ('A6', 'Single', 50.0),
('A7', 'Single', 50.0), ('A8', 'Single', 50.0), ('A9', 'Single', 50.0), ('A10', 'Single', 50.0);

-- Ghế Couple: B1–B5
INSERT INTO SEAT VALUES 
('B1', 'Couple', 90.0), ('B2', 'Couple', 90.0), ('B3', 'Couple', 90.0), 
('B4', 'Couple', 90.0), ('B5', 'Couple', 90.0);

-- Ghế Single: C1–C10
INSERT INTO SEAT VALUES 
('C1', 'Single', 50.0), ('C2', 'Single', 50.0), ('C3', 'Single', 50.0), 
('C4', 'Single', 50.0), ('C5', 'Single', 50.0), ('C6', 'Single', 50.0), 
('C7', 'Single', 50.0), ('C8', 'Single', 50.0), ('C9', 'Single', 50.0), ('C10', 'Single', 50.0);

-- Ghế Single: D1–D10
INSERT INTO SEAT VALUES 
('D1', 'Single', 50.0), ('D2', 'Single', 50.0), ('D3', 'Single', 50.0), 
('D4', 'Single', 50.0), ('D5', 'Single', 50.0), ('D6', 'Single', 50.0), 
('D7', 'Single', 50.0), ('D8', 'Single', 50.0), ('D9', 'Single', 50.0), ('D10', 'Single', 50.0);

-- Ghế Single: F1–F10
INSERT INTO SEAT VALUES 
('F1', 'Single', 50.0), ('F2', 'Single', 50.0), ('F3', 'Single', 50.0), 
('F4', 'Single', 50.0), ('F5', 'Single', 50.0), ('F6', 'Single', 50.0), 
('F7', 'Single', 50.0), ('F8', 'Single', 50.0), ('F9', 'Single', 50.0), ('F10', 'Single', 50.0);

-- Ghế Couple: G1–G5
INSERT INTO SEAT VALUES 
('G1', 'Couple', 90.0), ('G2', 'Couple', 90.0), ('G3', 'Couple', 90.0), 
('G4', 'Couple', 90.0), ('G5', 'Couple', 90.0);

-- Ghế Single: H1–H10
INSERT INTO SEAT VALUES 
('H1', 'Single', 50.0), ('H2', 'Single', 50.0), ('H3', 'Single', 50.0), 
('H4', 'Single', 50.0), ('H5', 'Single', 50.0), ('H6', 'Single', 50.0), 
('H7', 'Single', 50.0), ('H8', 'Single', 50.0), ('H9', 'Single', 50.0), ('H10', 'Single', 50.0);

-- Ghế Couple: I1–I5
INSERT INTO SEAT VALUES 
('I1', 'Couple', 90.0), ('I2', 'Couple', 90.0), ('I3', 'Couple', 90.0), 
('I4', 'Couple', 90.0), ('I5', 'Couple', 90.0);

-- Ghế Single: J1–J10
INSERT INTO SEAT VALUES 
('J1', 'Single', 50.0), ('J2', 'Single', 50.0), ('J3', 'Single', 50.0), 
('J4', 'Single', 50.0), ('J5', 'Single', 50.0), ('J6', 'Single', 50.0), 
('J7', 'Single', 50.0), ('J8', 'Single', 50.0), ('J9', 'Single', 50.0), ('J10', 'Single', 50.0);




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
