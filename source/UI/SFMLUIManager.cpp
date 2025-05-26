#include "SFMLUIManager.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include "../model/ISeat.h"
#include "../model/SingleSeat.h"

SFMLUIManager::SFMLUIManager(std::shared_ptr<SessionManager> sessionMgr)
    : sessionManager(sessionMgr), currentState(UIState::LOGIN_SCREEN),
      selectedMovieIndex(0), selectedShowTimeIndex(0),
      isInputtingUsername(true), isInputtingPassword(false),
      isInputtingEmail(false), isInputtingPhone(false) {
}

SFMLUIManager::~SFMLUIManager() {
    shutdown();
}

bool SFMLUIManager::initialize() {
    // Create window - SFML 2.6.2 syntax
    window.create(sf::VideoMode(1200, 800), "Movie Ticket Booking System");
    window.setFramerateLimit(60);
    
    // Initialize admin editing flags
    isEditingTitle = false;
    isEditingDescription = false;
    isEditingGenre = false;
    isEditingDuration = false;
    isEditingPrice = false;
    editingMovieId = -1;
    previousState = UIState::LOGIN_SCREEN;
    
    // Try to load font from various locations
    bool fontLoaded = false;
    std::vector<std::string> fontPaths = {
        "arial.ttf",
        "C:/Windows/Fonts/arial.ttf",
        "C:/Windows/Fonts/calibri.ttf",
        "C:/Windows/Fonts/tahoma.ttf"
    };
    
    for (const auto& path : fontPaths) {
        if (font.loadFromFile(path)) {
            fontLoaded = true;
            std::cout << "Font loaded from: " << path << std::endl;
            break;
        }
    }
    
    if (!fontLoaded) {
        std::cout << "Warning: Could not load any font file. Text may not display correctly." << std::endl;
        // SFML will use default font, but it might not work well
    }
    
    return true;
}

void SFMLUIManager::run() {
    while (window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}

void SFMLUIManager::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        
        if (event.type == sf::Event::TextEntered) {
            handleTextInput(event.text.unicode);
        }
        
        if (event.type == sf::Event::KeyPressed) {
            handleKeyPress(event.key.code);
        }
        
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                handleMouseClick(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
            }
        }
    }
}

void SFMLUIManager::handleTextInput(unsigned int unicode) {
    if (unicode < 128 && unicode > 31) { // Printable characters
        char inputChar = static_cast<char>(unicode);
        
        if (currentState == UIState::LOGIN_SCREEN) {
            if (isInputtingUsername) {
                inputUsername += inputChar;
            } else if (isInputtingPassword) {
                inputPassword += inputChar;
            }
        } else if (currentState == UIState::REGISTER_SCREEN) {
            if (isInputtingUsername) {
                inputUsername += inputChar;
            } else if (isInputtingPassword) {
                inputPassword += inputChar;
            } else if (isInputtingEmail) {
                inputEmail += inputChar;
            } else if (isInputtingPhone) {
                inputPhone += inputChar;
            }
        } else if (currentState == UIState::EDIT_MOVIE) {
            if (isEditingTitle) {
                editMovieTitle += inputChar;
            } else if (isEditingDescription) {
                editMovieDescription += inputChar;
            } else if (isEditingGenre) {
                editMovieGenre += inputChar;
            } else if (isEditingDuration) {
                editMovieDuration += inputChar;
            } else if (isEditingPrice) {
                editMoviePrice += inputChar;
            }
        }
    }
}

void SFMLUIManager::handleKeyPress(sf::Keyboard::Key key) {
    if (key == sf::Keyboard::Backspace) {
        if (currentState == UIState::LOGIN_SCREEN) {
            if (isInputtingUsername && !inputUsername.empty()) {
                inputUsername.pop_back();
            } else if (isInputtingPassword && !inputPassword.empty()) {
                inputPassword.pop_back();
            }
        } else if (currentState == UIState::REGISTER_SCREEN) {
            if (isInputtingUsername && !inputUsername.empty()) {
                inputUsername.pop_back();
            } else if (isInputtingPassword && !inputPassword.empty()) {
                inputPassword.pop_back();
            } else if (isInputtingEmail && !inputEmail.empty()) {
                inputEmail.pop_back();
            } else if (isInputtingPhone && !inputPhone.empty()) {
                inputPhone.pop_back();
            }
        } else if (currentState == UIState::EDIT_MOVIE) {
            if (isEditingTitle && !editMovieTitle.empty()) {
                editMovieTitle.pop_back();
            } else if (isEditingDescription && !editMovieDescription.empty()) {
                editMovieDescription.pop_back();
            } else if (isEditingGenre && !editMovieGenre.empty()) {
                editMovieGenre.pop_back();
            } else if (isEditingDuration && !editMovieDuration.empty()) {
                editMovieDuration.pop_back();
            } else if (isEditingPrice && !editMoviePrice.empty()) {
                editMoviePrice.pop_back();
            }
        }
    } else if (key == sf::Keyboard::Tab) {
        if (currentState == UIState::LOGIN_SCREEN) {
            isInputtingUsername = !isInputtingUsername;
            isInputtingPassword = !isInputtingPassword;
        }
    } else if (key == sf::Keyboard::Enter) {
        if (currentState == UIState::LOGIN_SCREEN) {
            attemptLogin();
        } else if (currentState == UIState::REGISTER_SCREEN) {
            attemptRegister();
        } else if (currentState == UIState::SUCCESS_MESSAGE) {
            currentState = previousState;
        }
    } else if (key == sf::Keyboard::Escape) {
        if (currentState == UIState::SUCCESS_MESSAGE) {
            currentState = previousState;
        } else if (currentState != UIState::LOGIN_SCREEN) {
            logout();
        }
    }
}

void SFMLUIManager::handleMouseClick(sf::Vector2i mousePos) {
    // Handle button clicks based on current state
    switch (currentState) {
        case UIState::LOGIN_SCREEN: {
            sf::RectangleShape loginBtn = createButton(500, 400, 200, 50);
            sf::RectangleShape registerBtn = createButton(500, 470, 200, 50);
            sf::RectangleShape usernameField = createButton(400, 250, 400, 40);
            sf::RectangleShape passwordField = createButton(400, 310, 400, 40);
            
            if (isButtonClicked(loginBtn, mousePos)) {
                attemptLogin();
            } else if (isButtonClicked(registerBtn, mousePos)) {
                currentState = UIState::REGISTER_SCREEN;
                inputUsername.clear();
                inputPassword.clear();
                inputEmail.clear();
                inputPhone.clear();
                statusMessage.clear();
            } else if (isButtonClicked(usernameField, mousePos)) {
                isInputtingUsername = true;
                isInputtingPassword = false;
            } else if (isButtonClicked(passwordField, mousePos)) {
                isInputtingUsername = false;
                isInputtingPassword = true;
            }
            break;
        }
          case UIState::MAIN_MENU: {
            sf::RectangleShape moviesBtn = createButton(500, 300, 200, 50);
            sf::RectangleShape historyBtn = createButton(500, 370, 200, 50);
            sf::RectangleShape logoutBtn = createButton(500, 440, 200, 50);
            sf::RectangleShape adminBtn = createButton(500, 510, 200, 50);
            
            if (isButtonClicked(moviesBtn, mousePos)) {
                loadMovies();
                currentState = UIState::MOVIE_LIST;
            } else if (isButtonClicked(historyBtn, mousePos)) {
                loadBookingHistory();
                currentState = UIState::BOOKING_HISTORY;
            } else if (isButtonClicked(logoutBtn, mousePos)) {
                logout();
            } else if (isButtonClicked(adminBtn, mousePos) && sessionManager->getCurrentRole() == "Admin") {
                currentState = UIState::ADMIN_PANEL;
            }
            break;
        }
        
        case UIState::MOVIE_LIST: {
            sf::RectangleShape backBtn = createButton(50, 50, 100, 40);
            if (isButtonClicked(backBtn, mousePos)) {
                currentState = UIState::MAIN_MENU;
                break;
            }
            
            // Check movie selection
            for (size_t i = 0; i < movies.size(); ++i) {
                sf::RectangleShape movieBtn = createButton(100, 150 + i * 80, 1000, 70);
                if (isButtonClicked(movieBtn, mousePos)) {
                    selectedMovieIndex = i;
                    loadMovieDetails(movies[i].id);
                    loadShowTimes(movies[i].id);
                    currentState = UIState::MOVIE_DETAILS;
                    break;
                }
            }
            break;
        }
        
        case UIState::MOVIE_DETAILS: {
            sf::RectangleShape backBtn = createButton(50, 50, 100, 40);
            sf::RectangleShape bookBtn = createButton(500, 600, 200, 50);
            
            if (isButtonClicked(backBtn, mousePos)) {
                currentState = UIState::MOVIE_LIST;
            } else if (isButtonClicked(bookBtn, mousePos) && !currentShowTimes.empty()) {
                currentState = UIState::BOOKING_SCREEN;
            }
            break;
        }
        
        case UIState::BOOKING_SCREEN: {
            sf::RectangleShape backBtn = createButton(50, 50, 100, 40);
            if (isButtonClicked(backBtn, mousePos)) {
                currentState = UIState::MOVIE_DETAILS;
                break;
            }
            
            // Check showtime selection
            for (size_t i = 0; i < currentShowTimes.size(); ++i) {
                sf::RectangleShape showtimeBtn = createButton(100, 150 + i * 60, 600, 50);
                if (isButtonClicked(showtimeBtn, mousePos)) {
                    selectedShowTimeIndex = i;
                    loadSeats(currentShowTimes[i].showTimeID);
                    currentState = UIState::SEAT_SELECTION;
                    break;
                }
            }
            break;
        }
        
        case UIState::SEAT_SELECTION: {
            sf::RectangleShape backBtn = createButton(50, 50, 100, 40);
            sf::RectangleShape confirmBtn = createButton(500, 700, 200, 50);
            
            if (isButtonClicked(backBtn, mousePos)) {
                currentState = UIState::BOOKING_SCREEN;
                selectedSeats.clear();
            } else if (isButtonClicked(confirmBtn, mousePos) && !selectedSeats.empty()) {
                createBooking();
            } else {
                // Check seat selection
                int seatsPerRow = 10;
                int seatSize = 40;
                int seatSpacing = 45;
                int startX = 200;
                int startY = 200;
                
                for (size_t i = 0; i < currentSeats.size(); ++i) {
                    int row = i / seatsPerRow;
                    int col = i % seatsPerRow;
                    int seatX = startX + col * seatSpacing;
                    int seatY = startY + row * seatSpacing;
                    
                    sf::RectangleShape seat = createButton(seatX, seatY, seatSize, seatSize);
                    if (isButtonClicked(seat, mousePos) && currentSeats[i].status == SeatStatus::AVAILABLE) {
                        std::string seatId = currentSeats[i].seat->id();
                        auto it = std::find(selectedSeats.begin(), selectedSeats.end(), seatId);
                        if (it != selectedSeats.end()) {
                            selectedSeats.erase(it);
                        } else {
                            selectedSeats.push_back(seatId);
                        }
                        break;
                    }
                }
            }
            break;
        }
        
        case UIState::BOOKING_HISTORY: {
            sf::RectangleShape backBtn = createButton(50, 50, 100, 40);
            if (isButtonClicked(backBtn, mousePos)) {
                currentState = UIState::MAIN_MENU;
            }
            break;
        }
        
        case UIState::REGISTER_SCREEN: {
            sf::RectangleShape registerBtn = createButton(500, 500, 200, 50);
            sf::RectangleShape backBtn = createButton(500, 570, 200, 50);
            sf::RectangleShape usernameField = createButton(400, 250, 400, 40);
            sf::RectangleShape passwordField = createButton(400, 310, 400, 40);
            sf::RectangleShape emailField = createButton(400, 370, 400, 40);
            sf::RectangleShape phoneField = createButton(400, 430, 400, 40);
            
            if (isButtonClicked(registerBtn, mousePos)) {
                attemptRegister();
            } else if (isButtonClicked(backBtn, mousePos)) {
                currentState = UIState::LOGIN_SCREEN;
                inputUsername.clear();
                inputPassword.clear();
                inputEmail.clear();
                inputPhone.clear();
                statusMessage.clear();
            } else if (isButtonClicked(usernameField, mousePos)) {
                isInputtingUsername = true;
                isInputtingPassword = false;
                isInputtingEmail = false;
                isInputtingPhone = false;
            } else if (isButtonClicked(passwordField, mousePos)) {
                isInputtingUsername = false;
                isInputtingPassword = true;
                isInputtingEmail = false;
                isInputtingPhone = false;
            } else if (isButtonClicked(emailField, mousePos)) {
                isInputtingUsername = false;
                isInputtingPassword = false;
                isInputtingEmail = true;
                isInputtingPhone = false;
            } else if (isButtonClicked(phoneField, mousePos)) {
                isInputtingUsername = false;
                isInputtingPassword = false;
                isInputtingEmail = false;
                isInputtingPhone = true;            }
            break;
        }
        
        case UIState::ADMIN_PANEL: {
            sf::RectangleShape backBtn = createButton(50, 50, 100, 40);
            sf::RectangleShape manageMoviesBtn = createButton(400, 300, 300, 60);
            sf::RectangleShape viewBookingsBtn = createButton(400, 380, 300, 60);
            
            if (isButtonClicked(backBtn, mousePos)) {
                currentState = UIState::MAIN_MENU;
            } else if (isButtonClicked(manageMoviesBtn, mousePos)) {
                loadMovies();
                currentState = UIState::MOVIE_MANAGEMENT;
            } else if (isButtonClicked(viewBookingsBtn, mousePos)) {
                loadBookingHistory();
                currentState = UIState::BOOKING_HISTORY;
            }
            break;
        }
        
        case UIState::MOVIE_MANAGEMENT: {
            sf::RectangleShape backBtn = createButton(50, 50, 100, 40);
            sf::RectangleShape addMovieBtn = createButton(950, 100, 150, 50);
            
            if (isButtonClicked(backBtn, mousePos)) {
                currentState = UIState::ADMIN_PANEL;
            } else if (isButtonClicked(addMovieBtn, mousePos)) {
                // Reset edit fields for new movie
                editMovieTitle.clear();
                editMovieDescription.clear();
                editMovieGenre.clear();
                editMovieDuration.clear();
                editMoviePrice.clear();
                editingMovieId = -1;
                resetEditingFlags();
                isEditingTitle = true;
                currentState = UIState::EDIT_MOVIE;
            } else {
                // Check for edit/delete buttons on movie cards
                for (size_t i = 0; i < movies.size(); ++i) {
                    int row = i / 3;
                    int col = i % 3;
                    float x = 100 + col * 370;
                    float y = 150 + row * 200;
                    
                    sf::RectangleShape editBtn = createButton(x + 260, y + 90, 80, 30);
                    sf::RectangleShape deleteBtn = createButton(x + 260, y + 125, 80, 30);
                    
                    if (isButtonClicked(editBtn, mousePos)) {
                        editingMovieId = movies[i].id;
                        editMovieTitle = movies[i].title;
                        editMovieGenre = movies[i].genre;
                        editMovieDescription = ""; // MovieDTO doesn't have description
                        editMovieDuration = "120"; // Default
                        editMoviePrice = "10.0"; // Default
                        resetEditingFlags();
                        isEditingTitle = true;
                        currentState = UIState::EDIT_MOVIE;
                        break;
                    } else if (isButtonClicked(deleteBtn, mousePos)) {
                        deleteMovie(movies[i].id);
                        loadMovies(); // Refresh the list
                        break;
                    }
                }
            }
            break;
        }
        
        case UIState::EDIT_MOVIE: {
            sf::RectangleShape backBtn = createButton(50, 50, 100, 40);
            sf::RectangleShape saveBtn = createButton(500, 550, 200, 50);
            sf::RectangleShape titleField = createButton(400, 150, 400, 40);
            sf::RectangleShape descField = createButton(400, 220, 400, 80);
            sf::RectangleShape genreField = createButton(400, 330, 400, 40);
            sf::RectangleShape durationField = createButton(400, 400, 200, 40);
            sf::RectangleShape priceField = createButton(620, 400, 180, 40);
            
            if (isButtonClicked(backBtn, mousePos)) {
                currentState = UIState::MOVIE_MANAGEMENT;
            } else if (isButtonClicked(saveBtn, mousePos)) {
                if (editingMovieId == -1) {
                    // Add new movie logic would go here
                    showSuccessMessage("New movie added successfully!");
                } else {
                    updateMovie(editingMovieId);
                }
                currentState = UIState::MOVIE_MANAGEMENT;
                loadMovies();
            } else if (isButtonClicked(titleField, mousePos)) {
                resetEditingFlags();
                isEditingTitle = true;
            } else if (isButtonClicked(descField, mousePos)) {
                resetEditingFlags();
                isEditingDescription = true;
            } else if (isButtonClicked(genreField, mousePos)) {
                resetEditingFlags();
                isEditingGenre = true;
            } else if (isButtonClicked(durationField, mousePos)) {
                resetEditingFlags();
                isEditingDuration = true;
            } else if (isButtonClicked(priceField, mousePos)) {
                resetEditingFlags();
                isEditingPrice = true;
            }
            break;
        }
        
        case UIState::SUCCESS_MESSAGE: {
            sf::RectangleShape okBtn = createButton(500, 450, 200, 50);
            if (isButtonClicked(okBtn, mousePos)) {
                currentState = previousState;
            }
            break;
        }
    }
}

void SFMLUIManager::update() {
    // Update logic if needed
}

void SFMLUIManager::render() {
    window.clear(sf::Color::Black);
    
    switch (currentState) {
        case UIState::LOGIN_SCREEN:
            renderLoginScreen();
            break;
        case UIState::MAIN_MENU:
            renderMainMenu();
            break;
        case UIState::MOVIE_LIST:
            renderMovieList();
            break;
        case UIState::MOVIE_DETAILS:
            renderMovieDetails();
            break;
        case UIState::BOOKING_SCREEN:
            renderBookingScreen();
            break;
        case UIState::SEAT_SELECTION:
            renderSeatSelection();
            break;
        case UIState::BOOKING_HISTORY:
            renderBookingHistory();
            break;        case UIState::REGISTER_SCREEN:
            renderRegisterScreen();
            break;
        case UIState::ADMIN_PANEL:
            renderAdminPanel();
            break;
        case UIState::MOVIE_MANAGEMENT:
            renderMovieManagement();
            break;
        case UIState::EDIT_MOVIE:
            renderEditMovie();
            break;
        case UIState::SUCCESS_MESSAGE:
            renderSuccessMessage();
            break;
    }
    
    window.display();
}

void SFMLUIManager::renderLoginScreen() {
    // Title
    sf::Text title = createText("Movie Ticket Booking System", 400, 100, 32);
    window.draw(title);
    
    // Username field
    sf::RectangleShape usernameField = createButton(400, 250, 400, 40);
    usernameField.setFillColor(isInputtingUsername ? sf::Color(100, 100, 255) : sf::Color(70, 70, 70));
    window.draw(usernameField);
    
    sf::Text usernameLabel = createText("Username:", 300, 260, 18);
    window.draw(usernameLabel);
    
    sf::Text usernameText = createText(inputUsername, 410, 260, 18);
    window.draw(usernameText);
    
    // Password field
    sf::RectangleShape passwordField = createButton(400, 310, 400, 40);
    passwordField.setFillColor(isInputtingPassword ? sf::Color(100, 100, 255) : sf::Color(70, 70, 70));
    window.draw(passwordField);
    
    sf::Text passwordLabel = createText("Password:", 300, 320, 18);
    window.draw(passwordLabel);
    
    std::string maskedPassword(inputPassword.length(), '*');
    sf::Text passwordText = createText(maskedPassword, 410, 320, 18);
    window.draw(passwordText);
    
    // Login button
    sf::RectangleShape loginBtn = createButton(500, 400, 200, 50);
    loginBtn.setFillColor(sf::Color(0, 150, 0));
    window.draw(loginBtn);
    
    sf::Text loginText = createText("Login", 570, 415, 20);
    window.draw(loginText);
    
    // Register button
    sf::RectangleShape registerBtn = createButton(500, 470, 200, 50);
    registerBtn.setFillColor(sf::Color(0, 100, 150));
    window.draw(registerBtn);
    
    sf::Text registerText = createText("Register", 560, 485, 20);
    window.draw(registerText);
    
    // Status message
    if (!statusMessage.empty()) {
        sf::Text status = createText(statusMessage, 400, 550, 16);
        status.setFillColor(sf::Color::Red);
        window.draw(status);
    }
}

void SFMLUIManager::renderMainMenu() {
    sf::Text title = createText("Main Menu", 500, 100, 32);
    window.draw(title);
    
    std::string welcomeMsg = "Welcome, " + sessionManager->getCurrentRole();
    sf::Text welcome = createText(welcomeMsg, 500, 150, 20);
    window.draw(welcome);
    
    // Movies button
    sf::RectangleShape moviesBtn = createButton(500, 300, 200, 50);
    moviesBtn.setFillColor(sf::Color(0, 150, 0));
    window.draw(moviesBtn);
    
    sf::Text moviesText = createText("View Movies", 540, 315, 20);
    window.draw(moviesText);
    
    // History button (only for authenticated users)
    if (sessionManager->isUserAuthenticated()) {
        sf::RectangleShape historyBtn = createButton(500, 370, 200, 50);
        historyBtn.setFillColor(sf::Color(150, 100, 0));
        window.draw(historyBtn);
        
        sf::Text historyText = createText("Booking History", 520, 385, 20);
        window.draw(historyText);
    }
    
    // Logout button
    sf::RectangleShape logoutBtn = createButton(500, 440, 200, 50);
    logoutBtn.setFillColor(sf::Color(150, 0, 0));
    window.draw(logoutBtn);
    
    sf::Text logoutText = createText("Logout", 560, 455, 20);
    window.draw(logoutText);
    
    // Admin panel button (only for admin users)
    if (sessionManager->getCurrentRole() == "Admin") {
        sf::RectangleShape adminBtn = createButton(500, 510, 200, 50);
        adminBtn.setFillColor(sf::Color(100, 0, 150));
        window.draw(adminBtn);
        
        sf::Text adminText = createText("Admin Panel", 540, 525, 20);
        window.draw(adminText);
    }
}

void SFMLUIManager::renderMovieList() {
    sf::Text title = createText("Available Movies", 450, 50, 28);
    window.draw(title);
    
    // Back button
    sf::RectangleShape backBtn = createButton(50, 50, 100, 40);
    backBtn.setFillColor(sf::Color(100, 100, 100));
    window.draw(backBtn);
    
    sf::Text backText = createText("Back", 80, 60, 18);
    window.draw(backText);
    
    // Movie list
    for (size_t i = 0; i < movies.size(); ++i) {
        sf::RectangleShape movieBtn = createButton(100, 150 + i * 80, 1000, 70);
        movieBtn.setFillColor(sf::Color(50, 50, 100));
        window.draw(movieBtn);
        
        std::string movieInfo = movies[i].title + " - " + movies[i].genre + " (Rating: " + std::to_string(movies[i].rating) + ")";
        sf::Text movieText = createText(movieInfo, 110, 170 + i * 80, 18);
        window.draw(movieText);
    }
}

void SFMLUIManager::renderMovieDetails() {
    if (selectedMovieIndex < movies.size()) {
        const auto& movie = movies[selectedMovieIndex];
        
        sf::Text title = createText("Movie Details", 450, 50, 28);
        window.draw(title);
        
        // Back button
        sf::RectangleShape backBtn = createButton(50, 50, 100, 40);
        backBtn.setFillColor(sf::Color(100, 100, 100));
        window.draw(backBtn);
        
        sf::Text backText = createText("Back", 80, 60, 18);
        window.draw(backText);
        
        // Movie information
        sf::Text movieTitle = createText("Title: " + movie.title, 100, 150, 24);
        window.draw(movieTitle);
        
        sf::Text movieGenre = createText("Genre: " + movie.genre, 100, 200, 20);
        window.draw(movieGenre);
        
        sf::Text movieRating = createText("Rating: " + std::to_string(movie.rating), 100, 250, 20);
        window.draw(movieRating);
        
        // Show times
        sf::Text showtimesTitle = createText("Available Showtimes:", 100, 320, 22);
        window.draw(showtimesTitle);
        
        for (size_t i = 0; i < currentShowTimes.size(); ++i) {
            const auto& showtime = currentShowTimes[i];
            std::string showtimeInfo = showtime.date + " " + showtime.startTime + " - " + showtime.endTime;
            sf::Text showtimeText = createText(showtimeInfo, 120, 360 + i * 30, 18);
            window.draw(showtimeText);
        }
        
        // Book button
        if (!currentShowTimes.empty() && sessionManager->isUserAuthenticated()) {
            sf::RectangleShape bookBtn = createButton(500, 600, 200, 50);
            bookBtn.setFillColor(sf::Color(0, 150, 0));
            window.draw(bookBtn);
            
            sf::Text bookText = createText("Book Tickets", 540, 615, 20);
            window.draw(bookText);
        }
    }
}

void SFMLUIManager::renderBookingScreen() {
    sf::Text title = createText("Select Showtime", 450, 50, 28);
    window.draw(title);
    
    // Back button
    sf::RectangleShape backBtn = createButton(50, 50, 100, 40);
    backBtn.setFillColor(sf::Color(100, 100, 100));
    window.draw(backBtn);
    
    sf::Text backText = createText("Back", 80, 60, 18);
    window.draw(backText);
    
    // Showtime selection
    for (size_t i = 0; i < currentShowTimes.size(); ++i) {
        sf::RectangleShape showtimeBtn = createButton(100, 150 + i * 60, 600, 50);
        showtimeBtn.setFillColor(sf::Color(50, 100, 50));
        window.draw(showtimeBtn);
        
        const auto& showtime = currentShowTimes[i];
        std::string showtimeInfo = showtime.date + " " + showtime.startTime + " - " + showtime.endTime;
        sf::Text showtimeText = createText(showtimeInfo, 110, 165 + i * 60, 18);
        window.draw(showtimeText);
    }
}

void SFMLUIManager::renderSeatSelection() {
    sf::Text title = createText("Select Seats", 500, 50, 28);
    window.draw(title);
    
    // Back button
    sf::RectangleShape backBtn = createButton(50, 50, 100, 40);
    backBtn.setFillColor(sf::Color(100, 100, 100));
    window.draw(backBtn);
    
    sf::Text backText = createText("Back", 80, 60, 18);
    window.draw(backText);
    
    // Screen indicator
    sf::RectangleShape screen = createButton(300, 120, 400, 20);
    screen.setFillColor(sf::Color::White);
    window.draw(screen);
    
    sf::Text screenText = createText("SCREEN", 470, 125, 16);
    screenText.setFillColor(sf::Color::Black);
    window.draw(screenText);
    
    // Seat grid
    int seatsPerRow = 10;
    int seatSize = 40;
    int seatSpacing = 45;
    int startX = 200;
    int startY = 200;
    
    for (size_t i = 0; i < currentSeats.size(); ++i) {
        int row = i / seatsPerRow;
        int col = i % seatsPerRow;
        int seatX = startX + col * seatSpacing;
        int seatY = startY + row * seatSpacing;
        
        sf::RectangleShape seat = createButton(seatX, seatY, seatSize, seatSize);
        
        std::string seatId = currentSeats[i].seat->id();
        bool isSelected = std::find(selectedSeats.begin(), selectedSeats.end(), seatId) != selectedSeats.end();
        
        if (currentSeats[i].status == SeatStatus::BOOKED) {
            seat.setFillColor(sf::Color::Red);
        } else if (isSelected) {
            seat.setFillColor(sf::Color::Green);
        } else {
            seat.setFillColor(sf::Color::Blue);
        }
        
        window.draw(seat);
        
        sf::Text seatText = createText(seatId, seatX + 5, seatY + 10, 12);
        window.draw(seatText);
    }
    
    // Legend
    sf::Text legend = createText("Blue: Available | Red: Booked | Green: Selected", 200, 650, 16);
    window.draw(legend);
    
    // Selected seats info
    if (!selectedSeats.empty()) {
        std::string selectedInfo = "Selected: ";
        for (const auto& seat : selectedSeats) {
            selectedInfo += seat + " ";
        }
        sf::Text selectedText = createText(selectedInfo, 200, 680, 16);
        window.draw(selectedText);
        
        // Confirm button
        sf::RectangleShape confirmBtn = createButton(500, 700, 200, 50);
        confirmBtn.setFillColor(sf::Color(0, 150, 0));
        window.draw(confirmBtn);
        
        sf::Text confirmText = createText("Confirm Booking", 530, 715, 18);
        window.draw(confirmText);
    }
}

void SFMLUIManager::renderBookingHistory() {
    sf::Text title = createText("Booking History", 450, 50, 28);
    window.draw(title);
    
    // Back button
    sf::RectangleShape backBtn = createButton(50, 50, 100, 40);
    backBtn.setFillColor(sf::Color(100, 100, 100));
    window.draw(backBtn);
    
    sf::Text backText = createText("Back", 80, 60, 18);
    window.draw(backText);
    
    // Booking list
    for (size_t i = 0; i < bookingHistory.size(); ++i) {
        const auto& booking = bookingHistory[i];
        
        sf::RectangleShape bookingBox = createButton(100, 150 + i * 120, 1000, 110);
        bookingBox.setFillColor(sf::Color(30, 30, 50));
        window.draw(bookingBox);
        
        sf::Text movieTitle = createText("Movie: " + booking.movieTitle, 110, 160 + i * 120, 18);
        window.draw(movieTitle);
        
        sf::Text showTime = createText("Date: " + booking.showTime.date + " " + booking.showTime.startTime, 110, 185 + i * 120, 16);
        window.draw(showTime);
        
        std::string seatsInfo = "Seats: ";
        for (const auto& seat : booking.bookedSeats) {
            seatsInfo += seat->id() + " ";
        }
        sf::Text seats = createText(seatsInfo, 110, 210 + i * 120, 16);
        window.draw(seats);
        
        sf::Text price = createText("Total: $" + std::to_string(booking.totalPrice), 110, 235 + i * 120, 16);
        window.draw(price);
    }
}

void SFMLUIManager::renderRegisterScreen() {
    sf::Text title = createText("Register New Account", 450, 100, 28);
    window.draw(title);
    
    // Username field
    sf::RectangleShape usernameField = createButton(400, 250, 400, 40);
    usernameField.setFillColor(isInputtingUsername ? sf::Color(100, 100, 255) : sf::Color(70, 70, 70));
    window.draw(usernameField);
    
    sf::Text usernameLabel = createText("Username:", 300, 260, 18);
    window.draw(usernameLabel);
    
    sf::Text usernameText = createText(inputUsername, 410, 260, 18);
    window.draw(usernameText);
    
    // Password field
    sf::RectangleShape passwordField = createButton(400, 310, 400, 40);
    passwordField.setFillColor(isInputtingPassword ? sf::Color(100, 100, 255) : sf::Color(70, 70, 70));
    window.draw(passwordField);
    
    sf::Text passwordLabel = createText("Password:", 300, 320, 18);
    window.draw(passwordLabel);
    
    std::string maskedPassword(inputPassword.length(), '*');
    sf::Text passwordText = createText(maskedPassword, 410, 320, 18);
    window.draw(passwordText);
    
    // Email field
    sf::RectangleShape emailField = createButton(400, 370, 400, 40);
    emailField.setFillColor(isInputtingEmail ? sf::Color(100, 100, 255) : sf::Color(70, 70, 70));
    window.draw(emailField);
    
    sf::Text emailLabel = createText("Email:", 300, 380, 18);
    window.draw(emailLabel);
    
    sf::Text emailText = createText(inputEmail, 410, 380, 18);
    window.draw(emailText);
    
    // Phone field
    sf::RectangleShape phoneField = createButton(400, 430, 400, 40);
    phoneField.setFillColor(isInputtingPhone ? sf::Color(100, 100, 255) : sf::Color(70, 70, 70));
    window.draw(phoneField);
    
    sf::Text phoneLabel = createText("Phone:", 300, 440, 18);
    window.draw(phoneLabel);
    
    sf::Text phoneText = createText(inputPhone, 410, 440, 18);
    window.draw(phoneText);
    
    // Register button
    sf::RectangleShape registerBtn = createButton(500, 500, 200, 50);
    registerBtn.setFillColor(sf::Color(0, 150, 0));
    window.draw(registerBtn);
    
    sf::Text registerText = createText("Register", 560, 515, 20);
    window.draw(registerText);
    
    // Back button
    sf::RectangleShape backBtn = createButton(500, 570, 200, 50);
    backBtn.setFillColor(sf::Color(100, 100, 100));
    window.draw(backBtn);
    
    sf::Text backText = createText("Back to Login", 540, 585, 20);
    window.draw(backText);
    
    // Status message
    if (!statusMessage.empty()) {
        sf::Text status = createText(statusMessage, 400, 640, 16);
        status.setFillColor(sf::Color::Red);
        window.draw(status);
    }
}

// Admin UI rendering methods
void SFMLUIManager::renderAdminPanel() {
    drawGradientBackground();
    
    // Title
    sf::Text title = createText("Admin Panel", 520, 80, 32);
    title.setFillColor(sf::Color(255, 215, 0)); // Gold color
    window.draw(title);
    
    // Back button
    sf::RectangleShape backBtn = createStyledButton(50, 50, 100, 40, sf::Color(70, 70, 70));
    window.draw(backBtn);
    sf::Text backText = createText("Back", 80, 60, 18);
    window.draw(backText);
    
    // Admin options
    sf::RectangleShape manageMoviesBtn = createStyledButton(400, 300, 300, 60, sf::Color(0, 120, 180));
    window.draw(manageMoviesBtn);
    sf::Text manageText = createText("Manage Movies", 500, 320, 22);
    window.draw(manageText);
    
    sf::RectangleShape viewBookingsBtn = createStyledButton(400, 380, 300, 60, sf::Color(150, 75, 0));
    window.draw(viewBookingsBtn);
    sf::Text bookingsText = createText("View All Bookings", 480, 400, 22);
    window.draw(bookingsText);
}

void SFMLUIManager::renderMovieManagement() {
    drawGradientBackground();
    
    // Title
    sf::Text title = createText("Movie Management", 480, 50, 28);
    title.setFillColor(sf::Color(255, 215, 0));
    window.draw(title);
    
    // Back button
    sf::RectangleShape backBtn = createStyledButton(50, 50, 100, 40, sf::Color(70, 70, 70));
    window.draw(backBtn);
    sf::Text backText = createText("Back", 80, 60, 18);
    window.draw(backText);
    
    // Add movie button
    sf::RectangleShape addMovieBtn = createStyledButton(950, 100, 150, 50, sf::Color(0, 150, 0));
    window.draw(addMovieBtn);
    sf::Text addText = createText("Add Movie", 980, 115, 18);
    window.draw(addText);
    
    // Movie grid (3 per row)
    for (size_t i = 0; i < movies.size(); ++i) {
        int row = i / 3;
        int col = i % 3;
        float x = 100 + col * 370;
        float y = 150 + row * 200;
        
        drawMovieCard(movies[i], x, y, false);
        
        // Edit button
        sf::RectangleShape editBtn = createStyledButton(x + 260, y + 90, 80, 30, sf::Color(0, 100, 200));
        window.draw(editBtn);
        sf::Text editText = createText("Edit", x + 285, y + 98, 14);
        window.draw(editText);
        
        // Delete button
        sf::RectangleShape deleteBtn = createStyledButton(x + 260, y + 125, 80, 30, sf::Color(200, 0, 0));
        window.draw(deleteBtn);
        sf::Text deleteText = createText("Delete", x + 280, y + 133, 14);
        window.draw(deleteText);
    }
}

void SFMLUIManager::renderEditMovie() {
    drawGradientBackground();
    
    // Title
    std::string titleText = (editingMovieId == -1) ? "Add New Movie" : "Edit Movie";
    sf::Text title = createText(titleText, 500, 50, 28);
    title.setFillColor(sf::Color(255, 215, 0));
    window.draw(title);
    
    // Back button
    sf::RectangleShape backBtn = createStyledButton(50, 50, 100, 40, sf::Color(70, 70, 70));
    window.draw(backBtn);
    sf::Text backText = createText("Back", 80, 60, 18);
    window.draw(backText);
    
    // Form fields
    // Title field
    sf::Text titleLabel = createText("Title:", 300, 150, 18);
    window.draw(titleLabel);
    sf::RectangleShape titleField = createInputField(400, 150, 400, 40, isEditingTitle);
    window.draw(titleField);
    sf::Text titleValue = createText(editMovieTitle, 410, 160, 16);
    window.draw(titleValue);
    
    // Description field
    sf::Text descLabel = createText("Description:", 300, 220, 18);
    window.draw(descLabel);
    sf::RectangleShape descField = createInputField(400, 220, 400, 80, isEditingDescription);
    window.draw(descField);
    sf::Text descValue = createText(editMovieDescription, 410, 230, 16);
    window.draw(descValue);
    
    // Genre field
    sf::Text genreLabel = createText("Genre:", 300, 330, 18);
    window.draw(genreLabel);
    sf::RectangleShape genreField = createInputField(400, 330, 400, 40, isEditingGenre);
    window.draw(genreField);
    sf::Text genreValue = createText(editMovieGenre, 410, 340, 16);
    window.draw(genreValue);
    
    // Duration field
    sf::Text durationLabel = createText("Duration (min):", 300, 400, 18);
    window.draw(durationLabel);
    sf::RectangleShape durationField = createInputField(400, 400, 200, 40, isEditingDuration);
    window.draw(durationField);
    sf::Text durationValue = createText(editMovieDuration, 410, 410, 16);
    window.draw(durationValue);
    
    // Price field
    sf::Text priceLabel = createText("Price ($):", 620, 400, 18);
    window.draw(priceLabel);
    sf::RectangleShape priceField = createInputField(620, 400, 180, 40, isEditingPrice);
    window.draw(priceField);
    sf::Text priceValue = createText(editMoviePrice, 630, 410, 16);
    window.draw(priceValue);
    
    // Save button
    sf::RectangleShape saveBtn = createStyledButton(500, 550, 200, 50, sf::Color(0, 150, 0));
    window.draw(saveBtn);
    sf::Text saveText = createText("Save Movie", 550, 565, 20);
    window.draw(saveText);
}

void SFMLUIManager::renderSuccessMessage() {
    // Semi-transparent overlay
    sf::RectangleShape overlay;
    overlay.setSize(sf::Vector2f(1200, 800));
    overlay.setFillColor(sf::Color(0, 0, 0, 128));
    window.draw(overlay);
    
    // Success message box
    sf::RectangleShape messageBox = createStyledButton(300, 250, 600, 300, sf::Color(40, 60, 80));
    messageBox.setOutlineThickness(3);
    messageBox.setOutlineColor(sf::Color(0, 150, 0));
    window.draw(messageBox);
    
    // Success icon (checkmark)
    sf::Text checkmark = createText("✓", 580, 300, 48);
    checkmark.setFillColor(sf::Color(0, 255, 0));
    window.draw(checkmark);
    
    // Success message text
    sf::Text message = createText(successMessage, 600 - successMessage.length() * 6, 380, 18);
    message.setFillColor(sf::Color::White);
    window.draw(message);
    
    // OK button
    sf::RectangleShape okBtn = createStyledButton(500, 450, 200, 50, sf::Color(0, 150, 0));
    window.draw(okBtn);
    sf::Text okText = createText("OK", 585, 465, 20);
    window.draw(okText);
    
    // Instructions
    sf::Text instruction = createText("Press Enter or click OK to continue", 450, 520, 14);
    instruction.setFillColor(sf::Color(200, 200, 200));
    window.draw(instruction);
}

// Utility methods for admin functionality
sf::RectangleShape SFMLUIManager::createStyledButton(float x, float y, float width, float height, sf::Color color) {
    sf::RectangleShape button;
    button.setSize(sf::Vector2f(width, height));
    button.setPosition(sf::Vector2f(x, y));
    button.setFillColor(color);
    button.setOutlineThickness(2);
    button.setOutlineColor(sf::Color(255, 255, 255, 100));
    return button;
}

sf::RectangleShape SFMLUIManager::createInputField(float x, float y, float width, float height, bool isActive) {
    sf::RectangleShape field;
    field.setSize(sf::Vector2f(width, height));
    field.setPosition(sf::Vector2f(x, y));
    field.setFillColor(isActive ? sf::Color(50, 50, 100) : sf::Color(30, 30, 30));
    field.setOutlineThickness(2);
    field.setOutlineColor(isActive ? sf::Color(0, 150, 255) : sf::Color(100, 100, 100));
    return field;
}

void SFMLUIManager::drawGradientBackground() {
    // Simple gradient effect using rectangles
    for (int i = 0; i < 800; i += 10) {
        sf::RectangleShape stripe;
        stripe.setSize(sf::Vector2f(1200, 10));
        stripe.setPosition(0, i);
        
        int intensity = 20 + (i * 20 / 800);
        stripe.setFillColor(sf::Color(intensity, intensity * 1.2, intensity * 1.5));
        window.draw(stripe);
    }
}

void SFMLUIManager::drawMovieCard(const MovieDTO& movie, float x, float y, bool isSelected) {
    // Movie card background
    sf::RectangleShape card = createStyledButton(x, y, 350, 160, 
        isSelected ? sf::Color(70, 90, 130) : sf::Color(45, 55, 75));
    window.draw(card);
    
    // Movie title
    sf::Text title = createText(movie.title, x + 15, y + 15, 20);
    title.setFillColor(sf::Color(255, 255, 255));
    window.draw(title);
    
    // Movie details
    sf::Text genre = createText("Genre: " + movie.genre, x + 15, y + 45, 14);
    genre.setFillColor(sf::Color(200, 200, 200));
    window.draw(genre);
    
    sf::Text rating = createText("Rating: " + std::to_string(movie.rating), x + 15, y + 65, 14);
    rating.setFillColor(sf::Color(255, 215, 0));
    window.draw(rating);
    
    sf::Text movieId = createText("ID: " + std::to_string(movie.id), x + 250, y + 15, 12);
    movieId.setFillColor(sf::Color(150, 150, 150));
    window.draw(movieId);
}

void SFMLUIManager::showSuccessMessage(const std::string& message) {
    successMessage = message;
    previousState = currentState;
    currentState = UIState::SUCCESS_MESSAGE;
}

void SFMLUIManager::deleteMovie(int movieId) {
    // TODO: Implement actual movie deletion using MovieManagerService
    showSuccessMessage("Movie deleted successfully!");
}

void SFMLUIManager::updateMovie(int movieId) {
    // TODO: Implement actual movie update using MovieManagerService
    showSuccessMessage("Movie updated successfully!");
}

void SFMLUIManager::resetEditingFlags() {
    isEditingTitle = false;
    isEditingDescription = false;
    isEditingGenre = false;
    isEditingDuration = false;
    isEditingPrice = false;
}

// UI utility methods implementation
sf::Text SFMLUIManager::createText(const std::string& content, float x, float y, int size) {
    sf::Text text;
    text.setFont(font); // SFML 2.6.2 syntax
    text.setString(content);
    text.setCharacterSize(size);
    text.setFillColor(sf::Color::White);
    text.setPosition(x, y);
    return text;
}

sf::RectangleShape SFMLUIManager::createButton(float x, float y, float width, float height) {
    sf::RectangleShape button;
    button.setSize(sf::Vector2f(width, height));
    button.setPosition(sf::Vector2f(x, y));
    button.setFillColor(sf::Color(50, 50, 100));
    button.setOutlineThickness(2);
    button.setOutlineColor(sf::Color::White);
    return button;
}

bool SFMLUIManager::isButtonClicked(const sf::RectangleShape& button, sf::Vector2i mousePos) {
    sf::FloatRect bounds = button.getGlobalBounds();
    return bounds.contains(static_cast<sf::Vector2f>(mousePos));
}

// Service interaction methods implementation
void SFMLUIManager::attemptLogin() {
    // TODO: Implement proper authentication service integration
    // For now, simulate login with sample user account
    AccountInformation sampleUser;
    sampleUser.userName = "testuser";
    sampleUser.password = "password";
    sampleUser.phoneNumber = "123456789";
    sampleUser.gmail = "test@example.com";
    sampleUser.role = "User";
    
    // Use SessionManager's setUserContext method instead of login
    if (sessionManager->setUserContext(sampleUser)) {
        statusMessage = "Login successful!";
        currentState = UIState::MAIN_MENU;
    } else {
        statusMessage = "Login failed - already logged in or invalid account";
    }
}

void SFMLUIManager::attemptRegister() {
    // TODO: Implement registration logic
    statusMessage = "Registration functionality not implemented yet";
}

void SFMLUIManager::loadMovies() {
    // TODO: Implement movie loading from service
    movies.clear();
    // Add some sample movies for testing
    MovieDTO movie1{1, "Sample Movie 1", "Action", 4.5f};
    MovieDTO movie2{2, "Sample Movie 2", "Comedy", 4.0f};
    MovieDTO movie3{3, "Sample Movie 3", "Drama", 4.8f};
    movies.push_back(movie1);
    movies.push_back(movie2);
    movies.push_back(movie3);
}

void SFMLUIManager::loadMovieDetails(int movieId) {
    // TODO: Implement movie details loading
}

void SFMLUIManager::loadShowTimes(int movieId) {
    // TODO: Implement showtime loading from service
    currentShowTimes.clear();
    // Add sample showtimes - ShowTime constructor: (showTimeID, date, startTime, endTime)
    ShowTime showtime1(1, "2024-01-15", "14:00", "16:30");
    ShowTime showtime2(2, "2024-01-15", "19:00", "21:30");
    ShowTime showtime3(3, "2024-01-16", "14:00", "16:30");
    currentShowTimes.push_back(showtime1);
    currentShowTimes.push_back(showtime2);
    currentShowTimes.push_back(showtime3);
}

// Rendering methods
void SFMLUIManager::renderMainMenu() {
    sf::Text title = createText("Main Menu", 500, 100, 32);
    window.draw(title);
    
    std::string welcomeMsg = "Welcome, " + sessionManager->getCurrentRole();
    sf::Text welcome = createText(welcomeMsg, 500, 150, 20);
    window.draw(welcome);
    
    // Movies button
    sf::RectangleShape moviesBtn = createButton(500, 300, 200, 50);
    moviesBtn.setFillColor(sf::Color(0, 150, 0));
    window.draw(moviesBtn);
    
    sf::Text moviesText = createText("View Movies", 540, 315, 20);
    window.draw(moviesText);
    
    // History button (only for authenticated users)
    if (sessionManager->isUserAuthenticated()) {
        sf::RectangleShape historyBtn = createButton(500, 370, 200, 50);
        historyBtn.setFillColor(sf::Color(150, 100, 0));
        window.draw(historyBtn);
        
        sf::Text historyText = createText("Booking History", 520, 385, 20);
        window.draw(historyText);
    }
    
    // Logout button
    sf::RectangleShape logoutBtn = createButton(500, 440, 200, 50);
    logoutBtn.setFillColor(sf::Color(150, 0, 0));
    window.draw(logoutBtn);
    
    sf::Text logoutText = createText("Logout", 560, 455, 20);
    window.draw(logoutText);
    
    // Admin panel button (only for admin users)
    if (sessionManager->getCurrentRole() == "Admin") {
        sf::RectangleShape adminBtn = createButton(500, 510, 200, 50);
        adminBtn.setFillColor(sf::Color(100, 0, 150));
        window.draw(adminBtn);
        
        sf::Text adminText = createText("Admin Panel", 540, 525, 20);
        window.draw(adminText);
    }
}

void SFMLUIManager::renderMovieList() {
    sf::Text title = createText("Available Movies", 450, 50, 28);
    window.draw(title);
    
    // Back button
    sf::RectangleShape backBtn = createButton(50, 50, 100, 40);
    backBtn.setFillColor(sf::Color(100, 100, 100));
    window.draw(backBtn);
    
    sf::Text backText = createText("Back", 80, 60, 18);
    window.draw(backText);
    
    // Movie list
    for (size_t i = 0; i < movies.size(); ++i) {
        sf::RectangleShape movieBtn = createButton(100, 150 + i * 80, 1000, 70);
        movieBtn.setFillColor(sf::Color(50, 50, 100));
        window.draw(movieBtn);
        
        std::string movieInfo = movies[i].title + " - " + movies[i].genre + " (Rating: " + std::to_string(movies[i].rating) + ")";
        sf::Text movieText = createText(movieInfo, 110, 170 + i * 80, 18);
        window.draw(movieText);
    }
}

void SFMLUIManager::renderMovieDetails() {
    if (selectedMovieIndex < movies.size()) {
        const auto& movie = movies[selectedMovieIndex];
        
        sf::Text title = createText("Movie Details", 450, 50, 28);
        window.draw(title);
        
        // Back button
        sf::RectangleShape backBtn = createButton(50, 50, 100, 40);
        backBtn.setFillColor(sf::Color(100, 100, 100));
        window.draw(backBtn);
        
        sf::Text backText = createText("Back", 80, 60, 18);
        window.draw(backText);
        
        // Movie information
        sf::Text movieTitle = createText("Title: " + movie.title, 100, 150, 24);
        window.draw(movieTitle);
        
        sf::Text movieGenre = createText("Genre: " + movie.genre, 100, 200, 20);
        window.draw(movieGenre);
        
        sf::Text movieRating = createText("Rating: " + std::to_string(movie.rating), 100, 250, 20);
        window.draw(movieRating);
        
        // Show times
        sf::Text showtimesTitle = createText("Available Showtimes:", 100, 320, 22);
        window.draw(showtimesTitle);
        
        for (size_t i = 0; i < currentShowTimes.size(); ++i) {
            const auto& showtime = currentShowTimes[i];
            std::string showtimeInfo = showtime.date + " " + showtime.startTime + " - " + showtime.endTime;
            sf::Text showtimeText = createText(showtimeInfo, 120, 360 + i * 30, 18);
            window.draw(showtimeText);
        }
        
        // Book button
        if (!currentShowTimes.empty() && sessionManager->isUserAuthenticated()) {
            sf::RectangleShape bookBtn = createButton(500, 600, 200, 50);
            bookBtn.setFillColor(sf::Color(0, 150, 0));
            window.draw(bookBtn);
            
            sf::Text bookText = createText("Book Tickets", 540, 615, 20);
            window.draw(bookText);
        }
    }
}

void SFMLUIManager::renderBookingScreen() {
    sf::Text title = createText("Select Showtime", 450, 50, 28);
    window.draw(title);
    
    // Back button
    sf::RectangleShape backBtn = createButton(50, 50, 100, 40);
    backBtn.setFillColor(sf::Color(100, 100, 100));
    window.draw(backBtn);
    
    sf::Text backText = createText("Back", 80, 60, 18);
    window.draw(backText);
    
    // Showtime selection
    for (size_t i = 0; i < currentShowTimes.size(); ++i) {
        sf::RectangleShape showtimeBtn = createButton(100, 150 + i * 60, 600, 50);
        showtimeBtn.setFillColor(sf::Color(50, 100, 50));
        window.draw(showtimeBtn);
        
        const auto& showtime = currentShowTimes[i];
        std::string showtimeInfo = showtime.date + " " + showtime.startTime + " - " + showtime.endTime;
        sf::Text showtimeText = createText(showtimeInfo, 110, 165 + i * 60, 18);
        window.draw(showtimeText);
    }
}

void SFMLUIManager::renderSeatSelection() {
    sf::Text title = createText("Select Seats", 500, 50, 28);
    window.draw(title);
    
    // Back button
    sf::RectangleShape backBtn = createButton(50, 50, 100, 40);
    backBtn.setFillColor(sf::Color(100, 100, 100));
    window.draw(backBtn);
    
    sf::Text backText = createText("Back", 80, 60, 18);
    window.draw(backText);
    
    // Screen indicator
    sf::RectangleShape screen = createButton(300, 120, 400, 20);
    screen.setFillColor(sf::Color::White);
    window.draw(screen);
    
    sf::Text screenText = createText("SCREEN", 470, 125, 16);
    screenText.setFillColor(sf::Color::Black);
    window.draw(screenText);
    
    // Seat grid
    int seatsPerRow = 10;
    int seatSize = 40;
    int seatSpacing = 45;
    int startX = 200;
    int startY = 200;
    
    for (size_t i = 0; i < currentSeats.size(); ++i) {
        int row = i / seatsPerRow;
        int col = i % seatsPerRow;
        int seatX = startX + col * seatSpacing;
        int seatY = startY + row * seatSpacing;
        
        sf::RectangleShape seat = createButton(seatX, seatY, seatSize, seatSize);
        
        std::string seatId = currentSeats[i].seat->id();
        bool isSelected = std::find(selectedSeats.begin(), selectedSeats.end(), seatId) != selectedSeats.end();
        
        if (currentSeats[i].status == SeatStatus::BOOKED) {
            seat.setFillColor(sf::Color::Red);
        } else if (isSelected) {
            seat.setFillColor(sf::Color::Green);
        } else {
            seat.setFillColor(sf::Color::Blue);
        }
        
        window.draw(seat);
        
        sf::Text seatText = createText(seatId, seatX + 5, seatY + 10, 12);
        window.draw(seatText);
    }
    
    // Legend
    sf::Text legend = createText("Blue: Available | Red: Booked | Green: Selected", 200, 650, 16);
    window.draw(legend);
    
    // Selected seats info
    if (!selectedSeats.empty()) {
        std::string selectedInfo = "Selected: ";
        for (const auto& seat : selectedSeats) {
            selectedInfo += seat + " ";
        }
        sf::Text selectedText = createText(selectedInfo, 200, 680, 16);
        window.draw(selectedText);
        
        // Confirm button
        sf::RectangleShape confirmBtn = createButton(500, 700, 200, 50);
        confirmBtn.setFillColor(sf::Color(0, 150, 0));
        window.draw(confirmBtn);
        
        sf::Text confirmText = createText("Confirm Booking", 530, 715, 18);
        window.draw(confirmText);
    }
}

void SFMLUIManager::renderBookingHistory() {
    sf::Text title = createText("Booking History", 450, 50, 28);
    window.draw(title);
    
    // Back button
    sf::RectangleShape backBtn = createButton(50, 50, 100, 40);
    backBtn.setFillColor(sf::Color(100, 100, 100));
    window.draw(backBtn);
    
    sf::Text backText = createText("Back", 80, 60, 18);
    window.draw(backText);
    
    // Booking list
    for (size_t i = 0; i < bookingHistory.size(); ++i) {
        const auto& booking = bookingHistory[i];
        
        sf::RectangleShape bookingBox = createButton(100, 150 + i * 120, 1000, 110);
        bookingBox.setFillColor(sf::Color(30, 30, 50));
        window.draw(bookingBox);
        
        sf::Text movieTitle = createText("Movie: " + booking.movieTitle, 110, 160 + i * 120, 18);
        window.draw(movieTitle);
        
        sf::Text showTime = createText("Date: " + booking.showTime.date + " " + booking.showTime.startTime, 110, 185 + i * 120, 16);
        window.draw(showTime);
        
        std::string seatsInfo = "Seats: ";
        for (const auto& seat : booking.bookedSeats) {
            seatsInfo += seat->id() + " ";
        }
        sf::Text seats = createText(seatsInfo, 110, 210 + i * 120, 16);
        window.draw(seats);
        
        sf::Text price = createText("Total: $" + std::to_string(booking.totalPrice), 110, 235 + i * 120, 16);
        window.draw(price);
    }
}

void SFMLUIManager::renderRegisterScreen() {
    sf::Text title = createText("Register New Account", 450, 100, 28);
    window.draw(title);
    
    // Username field
    sf::RectangleShape usernameField = createButton(400, 250, 400, 40);
    usernameField.setFillColor(isInputtingUsername ? sf::Color(100, 100, 255) : sf::Color(70, 70, 70));
    window.draw(usernameField);
    
    sf::Text usernameLabel = createText("Username:", 300, 260, 18);
    window.draw(usernameLabel);
    
    sf::Text usernameText = createText(inputUsername, 410, 260, 18);
    window.draw(usernameText);
    
    // Password field
    sf::RectangleShape passwordField = createButton(400, 310, 400, 40);
    passwordField.setFillColor(isInputtingPassword ? sf::Color(100, 100, 255) : sf::Color(70, 70, 70));
    window.draw(passwordField);
    
    sf::Text passwordLabel = createText("Password:", 300, 320, 18);
    window.draw(passwordLabel);
    
    std::string maskedPassword(inputPassword.length(), '*');
    sf::Text passwordText = createText(maskedPassword, 410, 320, 18);
    window.draw(passwordText);
    
    // Email field
    sf::RectangleShape emailField = createButton(400, 370, 400, 40);
    emailField.setFillColor(isInputtingEmail ? sf::Color(100, 100, 255) : sf::Color(70, 70, 70));
    window.draw(emailField);
    
    sf::Text emailLabel = createText("Email:", 300, 380, 18);
    window.draw(emailLabel);
    
    sf::Text emailText = createText(inputEmail, 410, 380, 18);
    window.draw(emailText);
    
    // Phone field
    sf::RectangleShape phoneField = createButton(400, 430, 400, 40);
    phoneField.setFillColor(isInputtingPhone ? sf::Color(100, 100, 255) : sf::Color(70, 70, 70));
    window.draw(phoneField);
    
    sf::Text phoneLabel = createText("Phone:", 300, 440, 18);
    window.draw(phoneLabel);
    
    sf::Text phoneText = createText(inputPhone, 410, 440, 18);
    window.draw(phoneText);
    
    // Register button
    sf::RectangleShape registerBtn = createButton(500, 500, 200, 50);
    registerBtn.setFillColor(sf::Color(0, 150, 0));
    window.draw(registerBtn);
    
    sf::Text registerText = createText("Register", 560, 515, 20);
    window.draw(registerText);
    
    // Back button
    sf::RectangleShape backBtn = createButton(500, 570, 200, 50);
    backBtn.setFillColor(sf::Color(100, 100, 100));
    window.draw(backBtn);
    
    sf::Text backText = createText("Back to Login", 540, 585, 20);
    window.draw(backText);
    
    // Status message
    if (!statusMessage.empty()) {
        sf::Text status = createText(statusMessage, 400, 640, 16);
        status.setFillColor(sf::Color::Red);
        window.draw(status);
    }
}

// Admin UI rendering methods
void SFMLUIManager::renderAdminPanel() {
    drawGradientBackground();
    
    // Title
    sf::Text title = createText("Admin Panel", 520, 80, 32);
    title.setFillColor(sf::Color(255, 215, 0)); // Gold color
    window.draw(title);
    
    // Back button
    sf::RectangleShape backBtn = createStyledButton(50, 50, 100, 40, sf::Color(70, 70, 70));
    window.draw(backBtn);
    sf::Text backText = createText("Back", 80, 60, 18);
    window.draw(backText);
    
    // Admin options
    sf::RectangleShape manageMoviesBtn = createStyledButton(400, 300, 300, 60, sf::Color(0, 120, 180));
    window.draw(manageMoviesBtn);
    sf::Text manageText = createText("Manage Movies", 500, 320, 22);
    window.draw(manageText);
    
    sf::RectangleShape viewBookingsBtn = createStyledButton(400, 380, 300, 60, sf::Color(150, 75, 0));
    window.draw(viewBookingsBtn);
    sf::Text bookingsText = createText("View All Bookings", 480, 400, 22);
    window.draw(bookingsText);
}

void SFMLUIManager::renderMovieManagement() {
    drawGradientBackground();
    
    // Title
    sf::Text title = createText("Movie Management", 480, 50, 28);
    title.setFillColor(sf::Color(255, 215, 0));
    window.draw(title);
    
    // Back button
    sf::RectangleShape backBtn = createStyledButton(50, 50, 100, 40, sf::Color(70, 70, 70));
    window.draw(backBtn);
    sf::Text backText = createText("Back", 80, 60, 18);
    window.draw(backText);
    
    // Add movie button
    sf::RectangleShape addMovieBtn = createStyledButton(950, 100, 150, 50, sf::Color(0, 150, 0));
    window.draw(addMovieBtn);
    sf::Text addText = createText("Add Movie", 980, 115, 18);
    window.draw(addText);
    
    // Movie grid (3 per row)
    for (size_t i = 0; i < movies.size(); ++i) {
        int row = i / 3;
        int col = i % 3;
        float x = 100 + col * 370;
        float y = 150 + row * 200;
        
        drawMovieCard(movies[i], x, y, false);
        
        // Edit button
        sf::RectangleShape editBtn = createStyledButton(x + 260, y + 90, 80, 30, sf::Color(0, 100, 200));
        window.draw(editBtn);
        sf::Text editText = createText("Edit", x + 285, y + 98, 14);
        window.draw(editText);
        
        // Delete button
        sf::RectangleShape deleteBtn = createStyledButton(x + 260, y + 125, 80, 30, sf::Color(200, 0, 0));
        window.draw(deleteBtn);
        sf::Text deleteText = createText("Delete", x + 280, y + 133, 14);
        window.draw(deleteText);
    }
}

void SFMLUIManager::renderEditMovie() {
    drawGradientBackground();
    
    // Title
    std::string titleText = (editingMovieId == -1) ? "Add New Movie" : "Edit Movie";
    sf::Text title = createText(titleText, 500, 50, 28);
    title.setFillColor(sf::Color(255, 215, 0));
    window.draw(title);
    
    // Back button
    sf::RectangleShape backBtn = createStyledButton(50, 50, 100, 40, sf::Color(70, 70, 70));
    window.draw(backBtn);
    sf::Text backText = createText("Back", 80, 60, 18);
    window.draw(backText);
    
    // Form fields
    // Title field
    sf::Text titleLabel = createText("Title:", 300, 150, 18);
    window.draw(titleLabel);
    sf::RectangleShape titleField = createInputField(400, 150, 400, 40, isEditingTitle);
    window.draw(titleField);
    sf::Text titleValue = createText(editMovieTitle, 410, 160, 16);
    window.draw(titleValue);
    
    // Description field
    sf::Text descLabel = createText("Description:", 300, 220, 18);
    window.draw(descLabel);
    sf::RectangleShape descField = createInputField(400, 220, 400, 80, isEditingDescription);
    window.draw(descField);
    sf::Text descValue = createText(editMovieDescription, 410, 230, 16);
    window.draw(descValue);
    
    // Genre field
    sf::Text genreLabel = createText("Genre:", 300, 330, 18);
    window.draw(genreLabel);
    sf::RectangleShape genreField = createInputField(400, 330, 400, 40, isEditingGenre);
    window.draw(genreField);
    sf::Text genreValue = createText(editMovieGenre, 410, 340, 16);
    window.draw(genreValue);
    
    // Duration field
    sf::Text durationLabel = createText("Duration (min):", 300, 400, 18);
    window.draw(durationLabel);
    sf::RectangleShape durationField = createInputField(400, 400, 200, 40, isEditingDuration);
    window.draw(durationField);
    sf::Text durationValue = createText(editMovieDuration, 410, 410, 16);
    window.draw(durationValue);
    
    // Price field
    sf::Text priceLabel = createText("Price ($):", 620, 400, 18);
    window.draw(priceLabel);
    sf::RectangleShape priceField = createInputField(620, 400, 180, 40, isEditingPrice);
    window.draw(priceField);
    sf::Text priceValue = createText(editMoviePrice, 630, 410, 16);
    window.draw(priceValue);
    
    // Save button
    sf::RectangleShape saveBtn = createStyledButton(500, 550, 200, 50, sf::Color(0, 150, 0));
    window.draw(saveBtn);
    sf::Text saveText = createText("Save Movie", 550, 565, 20);
    window.draw(saveText);
}

void SFMLUIManager::renderSuccessMessage() {
    // Semi-transparent overlay
    sf::RectangleShape overlay;
    overlay.setSize(sf::Vector2f(1200, 800));
    overlay.setFillColor(sf::Color(0, 0, 0, 128));
    window.draw(overlay);
    
    // Success message box
    sf::RectangleShape messageBox = createStyledButton(300, 250, 600, 300, sf::Color(40, 60, 80));
    messageBox.setOutlineThickness(3);
    messageBox.setOutlineColor(sf::Color(0, 150, 0));
    window.draw(messageBox);
    
    // Success icon (checkmark)
    sf::Text checkmark = createText("✓", 580, 300, 48);
    checkmark.setFillColor(sf::Color(0, 255, 0));
    window.draw(checkmark);
    
    // Success message text
    sf::Text message = createText(successMessage, 600 - successMessage.length() * 6, 380, 18);
    message.setFillColor(sf::Color::White);
    window.draw(message);
    
    // OK button
    sf::RectangleShape okBtn = createStyledButton(500, 450, 200, 50, sf::Color(0, 150, 0));
    window.draw(okBtn);
    sf::Text okText = createText("OK", 585, 465, 20);
    window.draw(okText);
    
    // Instructions
    sf::Text instruction = createText("Press Enter or click OK to continue", 450, 520, 14);
    instruction.setFillColor(sf::Color(200, 200, 200));
    window.draw(instruction);
}