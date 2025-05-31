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
    // Window dimensions
    unsigned int windowWidth = 1280;
    unsigned int windowHeight = 720;
    // Button dimensions
    float buttonWidth = 250;  // Button width
    float buttonHeight = 60;  // Button height
    float buttonGap = 40;     // Gap between buttons
    float buttonY = 500;      // Default vertical position for buttons

    // GUI components
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::RenderWindow window;
    sf::Font font;
    UIState currentState;
    UIState previousState;
    std::shared_ptr<SessionManager> sessionManager;
    
    // UI state variables
    std::string inputUsername;
    std::string inputPassword;
    std::string inputEmail;
    std::string inputPhone;
    std::string statusMessage;
    std::string successMessage;
    bool isInputtingUsername;
    bool isInputtingPassword;
    bool isInputtingEmail;
    bool isInputtingPhone;
    
    // Navigation state
    int selectedMovieIndex;
    int selectedShowTimeIndex;
    
    // Data storage
    std::vector<MovieDTO> movies;
    std::vector<ShowTime> currentShowTimes;
    std::vector<SeatView> currentSeats;
    std::vector<std::string> selectedSeats;
    std::vector<BookingView> bookingHistory;
    
    // Admin management variables
    std::string editMovieTitle;
    std::string editMovieDescription;
    std::string editMovieGenre;
    std::string editMoviePrice;
    int editingMovieId;
    bool isEditingTitle;
    bool isEditingDescription;
    bool isEditingGenre;
    bool isEditingPrice;
    
    // Showtime management variables
    int managingMovieId;
    std::string newShowtimeDate;
    std::string newShowtimeStartTime;
    std::string newShowtimeEndTime;
    bool isEditingDate;
    bool isEditingStartTime;
    bool isEditingEndTime;
    std::vector<std::string> pendingShowtimes;
    int movieListScrollOffset;
    int movieViewScrollOffset;
    bool isAddingShowtime;

    // Screen render methods
    void renderLoginScreen();
    void renderGuestScreen();
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
    void renderSuccessMessage();
    void renderPendingShowtimes();

    // Event handlers
    void handleTextInput(unsigned int unicode);
    void handleKeyPress(sf::Keyboard::Key key);
    void handleMouseClick(sf::Vector2i mousePos);
    void handleEvents();
    void update();
    void render();
    bool isButtonClicked(const sf::RectangleShape& button, sf::Vector2i mousePos);

    // UI utility methods
    sf::Text createText(const std::string& content, float x, float y, int size = 24);
    sf::RectangleShape createButton(float x, float y, float width, float height);
    sf::RectangleShape createStyledButton(float x, float y, float width, float height, sf::Color color);
    sf::RectangleShape createInputField(float x, float y, float width, float height, bool isActive);
    void drawGradientBackground();
    void drawMovieCard(const MovieDTO& movie, float x, float y, bool isSelected);
    void showSuccessMessage(const std::string& message);

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

    // Admin management methods
    void deleteMovie(int movieId);
    void updateMovie(int movieId);
    void addMovie();
    void resetEditingFlags();
    void clearEditingFields();
    void clearShowtimeFields();
    
    // Showtime management methods
    void addShowtime(int movieId);
    void deleteShowtime(int movieId, int showtimeId);
    void resetShowtimeEditingFlags();
    void addPendingShowtime();
    void removePendingShowtime(int index);
    bool handlePendingShowtimesClick(sf::Vector2i mousePos);
    void clearPendingShowtimes();

public:
    SFMLUIManager(std::shared_ptr<SessionManager> sessionMgr);
    ~SFMLUIManager();
    
    bool initialize();
    void run();
    void shutdown();
};

#endif // SFML_UI_MANAGER_H