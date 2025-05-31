#ifndef SFML_UI_MANAGER_H
#define SFML_UI_MANAGER_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <memory>
#include <string>
#include <vector>
#include "../SessionManager.h"
#include "../core/ServiceRegistry.h"
#include "../service/ILoginService.h"
#include "../service/IMovieViewerService.h"
#include "../service/IBookingService.h"
#include "../service/IRegisterService.h"
#include "../visitor/LoginServiceVisitor.h"
#include "../visitor/MovieViewerServiceVisitor.h"
#include "../visitor/BookingServiceVisitor.h"
#include "../visitor/RegisterServiceVisitor.h"
#include "../visitor/MovieMangerServiceVisitor.h"
#include "../repository/MovieDTO.h"
#include "../repository/BookingView.h"
#include "../repository/SeatView.h"
#include "../model/ShowTime.h"
#include "../model/Movie.h"

enum class UIState {
    GUEST_SCREEN,
    LOGIN_SCREEN,
    MAIN_MENU,
    MOVIE_LIST,
    MOVIE_DETAILS,
    BOOKING_SCREEN,
    SEAT_SELECTION,
    BOOKING_HISTORY,
    REGISTER_SCREEN,
    ADMIN_PANEL,
    MOVIE_MANAGEMENT,
    EDIT_MOVIE,
    SHOWTIME_MANAGEMENT,
    SUCCESS_MESSAGE
};

class SFMLUIManager {
private:
    sf::RenderWindow window;
    sf::Font font;
    UIState currentState;
    std::shared_ptr<SessionManager> sessionManager;
      // UI state variables
    std::string inputUsername;
    std::string inputPassword;
    std::string inputEmail;
    std::string inputPhone;
    std::string statusMessage;
    bool isInputtingUsername;
    bool isInputtingPassword;
    bool isInputtingEmail;
    bool isInputtingPhone;
    
    // Admin management variables
    std::string editMovieTitle;
    std::string editMovieDescription;
    std::string editMovieGenre;
    // Removed: std::string editMovieDuration;
    std::string editMoviePrice;
    int editingMovieId;    bool isEditingTitle;
    bool isEditingDescription;
    bool isEditingGenre;
    // Removed: bool isEditingDuration;
    bool isEditingPrice;
    
    // Showtime management variables
    int managingMovieId;
    std::string newShowtimeDate;
    std::string newShowtimeStartTime;
    std::string newShowtimeEndTime;
    bool isEditingDate;
    bool isEditingStartTime;
    bool isEditingEndTime;
      // Success message variables
    std::string successMessage;
    UIState previousState;
    
    // Movie data
    std::vector<MovieDTO> movies;
    
    // Quản lý nhiều lịch chiếu khi thêm phim
    std::vector<std::string> pendingShowtimes; // Lưu các showtime đang chờ thêm vào phim mới
    bool isAddingShowtime; // Đánh dấu trạng thái đang thêm showtime
    
    // Scrolling variables
    int movieListScrollOffset; // Số lượng phim được "cuộn qua" từ đầu danh sách trong quản lý
    int movieViewScrollOffset; // Số lượng phim được "cuộn qua" từ đầu danh sách trong xem phim
    int selectedMovieIndex;
    std::vector<ShowTime> currentShowTimes;
    int selectedShowTimeIndex;
    std::vector<SeatView> currentSeats;
    std::vector<std::string> selectedSeats;
    std::vector<BookingView> bookingHistory;
    
    // UI helper methods
    void handleEvents();
    void update();
    void render();    // Screen rendering methods
    void renderGuestScreen();
    void renderLoginScreen();
    void renderMainMenu();
    void renderMovieList();
    void renderMovieDetails();
    void renderBookingScreen();
    void renderSeatSelection();
    void renderBookingHistory();
    void renderRegisterScreen();
    void renderAdminPanel();
    void renderMovieManagement();
    void renderEditMovie();
    void renderShowtimeManagement();
    void renderSuccessMessage();// Input handling
    void handleTextInput(unsigned int unicode);
    void handleKeyPress(sf::Keyboard::Key key);
    void handleMouseClick(sf::Vector2i mousePos);
      // UI utility methods
    sf::Text createText(const std::string& content, float x, float y, int size = 24);
    sf::RectangleShape createButton(float x, float y, float width, float height);
    bool isButtonClicked(const sf::RectangleShape& button, sf::Vector2i mousePos);
    
    // Admin utility methods
    sf::RectangleShape createStyledButton(float x, float y, float width, float height, sf::Color color);
    sf::RectangleShape createInputField(float x, float y, float width, float height, bool isActive);
    void drawGradientBackground();
    void drawMovieCard(const MovieDTO& movie, float x, float y, bool isSelected);
    void showSuccessMessage(const std::string& message);
    void deleteMovie(int movieId);
    void updateMovie(int movieId);    void addMovie();
    void resetEditingFlags();
    void clearEditingFields();
    void clearShowtimeFields();
    
    // Showtime management methods
    void addShowtime(int movieId);
    void deleteShowtime(int movieId, int showtimeId);
    void resetShowtimeEditingFlags();
    
    // Multiple showtimes management methods
    void addPendingShowtime();
    void removePendingShowtime(int index);
    void renderPendingShowtimes();
    bool handlePendingShowtimesClick(sf::Vector2i mousePos);
    void clearPendingShowtimes();
    
    // Service interaction methods
    void attemptLogin();
    void attemptRegister();
    void loadMovies();
    void loadMovieDetails(int movieId);
    void loadShowTimes(int movieId);
    void loadSeats(int showTimeId);
    void loadBookingHistory();
    void createBooking();
    void logout();

public:
    SFMLUIManager(std::shared_ptr<SessionManager> sessionMgr);
    ~SFMLUIManager();
    
    bool initialize();
    void run();
    void shutdown();
};

#endif // SFML_UI_MANAGER_H
