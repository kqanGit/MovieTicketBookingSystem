-- Create Movie table
CREATE TABLE MOVIE (
    MovieID TEXT PRIMARY KEY,
    Title TEXT,
    Genre TEXT,
    Descriptions TEXT,
    Rating REAL
);

-- Create Showtime table
CREATE TABLE SHOWTIME (
    ShowTimeID TEXT PRIMARY KEY,
    MovieID TEXT,
    Date TEXT,
    StartTime TEXT,
    EndTime TEXT,
    FOREIGN KEY (MovieID) REFERENCES MOVIE(MovieID)
);

-- Create SeatType table
CREATE TABLE SEATTYPE (
    SeatType TEXT,
    Price REAL,
    PRIMARY KEY (SeatType, Price)
);

-- Create Seat table
CREATE TABLE SEAT (
    SeatID TEXT PRIMARY KEY,
    SeatType TEXT,
    Price REAL,
    FOREIGN KEY (SeatType) REFERENCES SEATTYPE(SeatType)
);

-- Create Account table
CREATE TABLE ACCOUNT (
    UserID TEXT PRIMARY KEY,
    Password TEXT,
    RoleUser TEXT,
    Gmail TEXT,
    PhoneNumber TEXT,
    UserName TEXT
);

-- Create Booking table
CREATE TABLE BOOKING (
    BookingID TEXT PRIMARY KEY,
    ShowTimeID TEXT,
    UserID TEXT,
    FOREIGN KEY (ShowTimeID) REFERENCES SHOWTIME(ShowTimeID),
    FOREIGN KEY (UserID) REFERENCES ACCOUNT(UserID)
);

-- Create BookSeat table (N-N relationship between Booking and Seat)
CREATE TABLE BOOKSEAT (
    BookingID TEXT,
    SeatID TEXT,
    PRIMARY KEY (BookingID, SeatID),
    FOREIGN KEY (BookingID) REFERENCES BOOKING(BookingID),
    FOREIGN KEY (SeatID) REFERENCES SEAT(SeatID)
);

-- Insert movies
INSERT INTO MOVIE (MovieID,Title, Genre, Descriptions, Rating) VALUES
('001', 'Avengers', 'Action', 'The superhero team saving the world', 8.5),
('002', 'Titanic', 'Romance', 'A tragic love story on the doomed ship', 9.0);

-- Insert showtimes
INSERT INTO SHOWTIME (MovieID, Date, StartTime, EndTime) VALUES
('1', '2025-05-10', '18:00', '20:30'),
('2', '2025-05-11', '20:00', '22:15');

-- Insert seat types
INSERT INTO SEATTYPE (SeatType, Price) VALUES
('Single', 50.0),
('Couple', 90.0);

-- Insert seats
INSERT INTO SEAT (SeatID, SeatType, Price) VALUES
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

-- Insert bookings
INSERT INTO BOOKING (ShowTimeID, UserID) VALUES
('1', '1'),
('2', '2');

-- Insert booked seats
INSERT INTO BOOKSEAT (BookingID, SeatID) VALUES
('1', 'A1'),
('1', 'A2'),
('2', 'B1');
