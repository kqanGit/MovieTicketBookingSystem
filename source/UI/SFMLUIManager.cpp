#include "SFMLUIManager.h"
#include <iostream>
#include <sstream>

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
            
            if (isButtonClicked(moviesBtn, mousePos)) {
                loadMovies();
                currentState = UIState::MOVIE_LIST;
            } else if (isButtonClicked(historyBtn, mousePos)) {
                loadBookingHistory();
                currentState = UIState::BOOKING_HISTORY;
            } else if (isButtonClicked(logoutBtn, mousePos)) {
                logout();
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
            break;
        case UIState::REGISTER_SCREEN:
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

void SFMLUIManager::renderAdminPanel() {
    sf::Text title = createText("Admin Panel", 500, 100, 32);
    window.draw(title);
    
    // Movie Management button
    sf::RectangleShape movieMgmtBtn = createButton(500, 300, 200, 50);
    movieMgmtBtn.setFillColor(sf::Color(0, 150, 0));
    window.draw(movieMgmtBtn);
    
    sf::Text movieMgmtText = createText("Movie Management", 510, 315, 18);
    window.draw(movieMgmtText);
    
    // Back to Main Menu button
    sf::RectangleShape backBtn = createButton(500, 370, 200, 50);
    backBtn.setFillColor(sf::Color(100, 100, 100));
    window.draw(backBtn);
    
    sf::Text backText = createText("Back to Main Menu", 515, 385, 18);
    window.draw(backText);
}

void SFMLUIManager::renderMovieManagement() {
    sf::Text title = createText("Movie Management", 450, 50, 28);
    window.draw(title);
    
    // Back button
    sf::RectangleShape backBtn = createButton(50, 50, 100, 40);
    backBtn.setFillColor(sf::Color(100, 100, 100));
    window.draw(backBtn);
    
    sf::Text backText = createText("Back", 80, 60, 18);
    window.draw(backText);
    
    // Add Movie button
    sf::RectangleShape addBtn = createButton(200, 50, 150, 40);
    addBtn.setFillColor(sf::Color(0, 150, 0));
    window.draw(addBtn);
    
    sf::Text addText = createText("Add Movie", 230, 60, 18);
    window.draw(addText);
    
    // Movie list for management
    for (size_t i = 0; i < movies.size(); ++i) {
        sf::RectangleShape movieRow = createButton(100, 150 + i * 80, 800, 70);
        movieRow.setFillColor(sf::Color(50, 50, 100));
        window.draw(movieRow);
        
        std::string movieInfo = movies[i].title + " - " + movies[i].genre;
        sf::Text movieText = createText(movieInfo, 110, 170 + i * 80, 18);
        window.draw(movieText);
        
        // Edit button
        sf::RectangleShape editBtn = createButton(920, 155 + i * 80, 80, 30);
        editBtn.setFillColor(sf::Color(0, 100, 150));
        window.draw(editBtn);
        
        sf::Text editText = createText("Edit", 940, 165 + i * 80, 16);
        window.draw(editText);
        
        // Delete button
        sf::RectangleShape deleteBtn = createButton(1010, 155 + i * 80, 80, 30);
        deleteBtn.setFillColor(sf::Color(150, 0, 0));
        window.draw(deleteBtn);
        
        sf::Text deleteText = createText("Delete", 1025, 165 + i * 80, 16);
        window.draw(deleteText);
    }
}

void SFMLUIManager::renderEditMovie() {
    sf::Text title = createText("Edit Movie", 500, 50, 28);
    window.draw(title);
    
    // Back button
    sf::RectangleShape backBtn = createButton(50, 50, 100, 40);
    backBtn.setFillColor(sf::Color(100, 100, 100));
    window.draw(backBtn);
    
    sf::Text backText = createText("Back", 80, 60, 18);
    window.draw(backText);
    
    // Save button
    sf::RectangleShape saveBtn = createButton(1050, 50, 100, 40);
    saveBtn.setFillColor(sf::Color(0, 150, 0));
    window.draw(saveBtn);
    
    sf::Text saveText = createText("Save", 1080, 60, 18);
    window.draw(saveText);
    
    // Form fields
    sf::Text titleLabel = createText("Title:", 300, 140, 18);
    window.draw(titleLabel);
    
    sf::RectangleShape titleField = createButton(400, 130, 400, 40);
    titleField.setFillColor(isEditingTitle ? sf::Color(100, 100, 255) : sf::Color(70, 70, 70));
    window.draw(titleField);
    
    sf::Text titleText = createText(editMovieTitle, 410, 140, 18);
    window.draw(titleText);
    
    sf::Text descLabel = createText("Description:", 300, 200, 18);
    window.draw(descLabel);
    
    sf::RectangleShape descField = createButton(400, 220, 400, 80);
    descField.setFillColor(isEditingDescription ? sf::Color(100, 100, 255) : sf::Color(70, 70, 70));
    window.draw(descField);
    
    sf::Text descText = createText(editMovieDescription, 410, 240, 16);
    window.draw(descText);
    
    sf::Text genreLabel = createText("Genre:", 300, 320, 18);
    window.draw(genreLabel);
    
    sf::RectangleShape genreField = createButton(400, 330, 400, 40);
    genreField.setFillColor(isEditingGenre ? sf::Color(100, 100, 255) : sf::Color(70, 70, 70));
    window.draw(genreField);
    
    sf::Text genreText = createText(editMovieGenre, 410, 340, 18);
    window.draw(genreText);
    
    sf::Text durationLabel = createText("Duration:", 300, 390, 18);
    window.draw(durationLabel);
    
    sf::RectangleShape durationField = createButton(400, 400, 200, 40);
    durationField.setFillColor(isEditingDuration ? sf::Color(100, 100, 255) : sf::Color(70, 70, 70));
    window.draw(durationField);
    
    sf::Text durationText = createText(editMovieDuration, 410, 410, 18);
    window.draw(durationText);
    
    sf::Text priceLabel = createText("Price:", 620, 390, 18);
    window.draw(priceLabel);
    
    sf::RectangleShape priceField = createButton(620, 400, 180, 40);
    priceField.setFillColor(isEditingPrice ? sf::Color(100, 100, 255) : sf::Color(70, 70, 70));
    window.draw(priceField);
    
    sf::Text priceText = createText(editMoviePrice, 630, 410, 18);
    window.draw(priceText);
}

void SFMLUIManager::renderSuccessMessage() {
    // Background overlay
    sf::RectangleShape overlay;
    overlay.setSize(sf::Vector2f(1200, 800));
    overlay.setFillColor(sf::Color(0, 0, 0, 128));
    window.draw(overlay);
    
    // Message box
    sf::RectangleShape messageBox = createButton(300, 250, 600, 300);
    messageBox.setFillColor(sf::Color(50, 50, 50));
    messageBox.setOutlineThickness(3);
    messageBox.setOutlineColor(sf::Color::White);
    window.draw(messageBox);
    
    // Success title
    sf::Text title = createText("Success!", 550, 300, 32);
    title.setFillColor(sf::Color::Green);
    window.draw(title);
    
    // Success message
    sf::Text message = createText(successMessage, 320, 380, 20);
    message.setFillColor(sf::Color::White);
    window.draw(message);
    
    // OK button
    sf::RectangleShape okBtn = createButton(500, 450, 200, 50);
    okBtn.setFillColor(sf::Color(0, 150, 0));
    window.draw(okBtn);
    
    sf::Text okText = createText("OK", 580, 465, 20);
    window.draw(okText);
}

// Helper method implementations
sf::Text SFMLUIManager::createText(const std::string& content, float x, float y, int size) {
    sf::Text text;
    text.setFont(font);
    text.setString(content);
    text.setCharacterSize(size);
    text.setFillColor(sf::Color::White);
    text.setPosition(sf::Vector2f(x, y));  // SFML 3.0 requires Vector2f
    return text;
}

sf::RectangleShape SFMLUIManager::createButton(float x, float y, float width, float height) {
    sf::RectangleShape button;
    button.setSize(sf::Vector2f(width, height));
    button.setPosition(sf::Vector2f(x, y));
    button.setFillColor(sf::Color(100, 100, 100));
    button.setOutlineThickness(2);
    button.setOutlineColor(sf::Color::White);
    return button;
}

bool SFMLUIManager::isButtonClicked(const sf::RectangleShape& button, sf::Vector2i mousePos) {
    sf::FloatRect bounds = button.getGlobalBounds();
    return bounds.contains(sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)));  // SFML 3.0 requires Vector2f
}

// Service interaction methods
void SFMLUIManager::attemptLogin() {
    if (inputUsername.empty() || inputPassword.empty()) {
        statusMessage = "Please enter both username and password";
        return;
    }
    
    auto visitor = std::make_shared<LoginServiceVisitor>();
    sessionManager->getCurrentContext()->accept(visitor);
    auto loginService = visitor->getLoginService();
    
    if (loginService) {
        auto result = loginService->authenticate(inputUsername, inputPassword);
        if (result.has_value()) {
            sessionManager->setUserContext(result.value());
            currentState = UIState::MAIN_MENU;
            statusMessage.clear();
            inputUsername.clear();
            inputPassword.clear();
        } else {
            statusMessage = "Invalid credentials";
        }
    } else {
        statusMessage = "Login service not available";
    }
}

void SFMLUIManager::attemptRegister() {
    if (inputUsername.empty() || inputPassword.empty() || inputEmail.empty() || inputPhone.empty()) {
        statusMessage = "Please fill all fields";
        return;
    }
    
    auto visitor = std::make_shared<RegisterServiceVisitor>();
    sessionManager->getCurrentContext()->accept(visitor);
    auto registerService = visitor->getRegisterService();
    
    if (registerService) {
        try {
            // Create AccountInformation object using default constructor and set fields manually
            AccountInformation accountInfo;
            accountInfo.userName = inputUsername;
            accountInfo.password = inputPassword;
            accountInfo.gmail = inputEmail;
            accountInfo.phoneNumber = inputPhone;
            accountInfo.role = "User";  // Default role for new registrations
            accountInfo.userID = 0;     // Will be set by database
            
            registerService->registerUser(accountInfo);
            statusMessage = "Registration successful! You can now login.";
            // Clear fields after successful registration
            inputUsername.clear();
            inputPassword.clear();
            inputEmail.clear();
            inputPhone.clear();
        } catch (const std::exception& e) {
            statusMessage = "Registration failed: " + std::string(e.what());
        }
    } else {
        statusMessage = "Register service not available";
    }
}

void SFMLUIManager::loadMovies() {
    auto visitor = std::make_shared<MovieViewerServiceVisitor>();
    sessionManager->getCurrentContext()->accept(visitor);
    auto movieService = visitor->getMovieViewerService();
    
    if (movieService) {
        movies = movieService->showAllMovies();
    }
}

void SFMLUIManager::loadMovieDetails(int movieId) {
    auto visitor = std::make_shared<MovieViewerServiceVisitor>();
    sessionManager->getCurrentContext()->accept(visitor);
    auto movieService = visitor->getMovieViewerService();
    
    if (movieService) {
        auto movieDetail = movieService->showMovieDetail(movieId);
        // Movie detail is already loaded when showing movie list
    }
}

void SFMLUIManager::loadShowTimes(int movieId) {
    auto visitor = std::make_shared<MovieViewerServiceVisitor>();
    sessionManager->getCurrentContext()->accept(visitor);
    auto movieService = visitor->getMovieViewerService();
    
    if (movieService) {
        currentShowTimes = movieService->showMovieShowTimes(movieId);
    }
}

void SFMLUIManager::loadSeats(int showTimeId) {
    auto visitor = std::make_shared<BookingServiceVisitor>();
    sessionManager->getCurrentContext()->accept(visitor);
    auto bookingService = visitor->getBookingService();
    
    if (bookingService) {
        currentSeats = bookingService->viewSeatsStatus(showTimeId);
        selectedSeats.clear();
    }
}

void SFMLUIManager::loadBookingHistory() {
    auto visitor = std::make_shared<BookingServiceVisitor>();
    sessionManager->getCurrentContext()->accept(visitor);
    auto bookingService = visitor->getBookingService();
    
    if (bookingService && sessionManager->isUserAuthenticated()) {
        int userID = sessionManager->getCurrentAccount().userID;
        bookingHistory = bookingService->viewBookingHistory(userID);
    }
}

void SFMLUIManager::createBooking() {
    if (selectedSeats.empty() || !sessionManager->isUserAuthenticated()) {
        return;
    }
    
    auto visitor = std::make_shared<BookingServiceVisitor>();
    sessionManager->getCurrentContext()->accept(visitor);
    auto bookingService = visitor->getBookingService();
    
    if (bookingService && selectedShowTimeIndex < currentShowTimes.size()) {
        try {
            int userID = sessionManager->getCurrentAccount().userID;
            int showTimeID = currentShowTimes[selectedShowTimeIndex].showTimeID;
            
            bookingService->createBooking(userID, showTimeID, selectedSeats);
            
            // Booking successful - go back to main menu
            currentState = UIState::MAIN_MENU;
            selectedSeats.clear();
            statusMessage = "Booking successful!";
        } catch (const std::exception& e) {
            statusMessage = "Booking failed: " + std::string(e.what());
        }
    }
}

void SFMLUIManager::logout() {
    sessionManager->logout();
    currentState = UIState::LOGIN_SCREEN;
    inputUsername.clear();
    inputPassword.clear();
    inputEmail.clear();
    inputPhone.clear();
    statusMessage.clear();
    movies.clear();
    currentShowTimes.clear();
    currentSeats.clear();
    selectedSeats.clear();
    bookingHistory.clear();
}

void SFMLUIManager::shutdown() {
    if (window.isOpen()) {
        window.close();
    }
}

void SFMLUIManager::showSuccessMessage(const std::string& message) {
    successMessage = message;
    previousState = currentState;
    currentState = UIState::SUCCESS_MESSAGE;
}

void SFMLUIManager::deleteMovie(int movieId) {
    // This would connect to a movie management service to delete the movie
    // For now, just show a success message
    showSuccessMessage("Movie deleted successfully!");
    loadMovies(); // Reload the movie list
}

void SFMLUIManager::updateMovie(int movieId) {
    // This would connect to a movie management service to update the movie
    // For now, just show a success message
    showSuccessMessage("Movie updated successfully!");
    resetEditingFlags();
    loadMovies(); // Reload the movie list
}

void SFMLUIManager::resetEditingFlags() {
    isEditingTitle = false;
    isEditingDescription = false;
    isEditingGenre = false;
    isEditingDuration = false;
    isEditingPrice = false;
    editingMovieId = -1;
    editMovieTitle.clear();
    editMovieDescription.clear();
    editMovieGenre.clear();
    editMovieDuration.clear();
    editMoviePrice.clear();
}

// Additional UI utility method implementations
sf::RectangleShape SFMLUIManager::createStyledButton(float x, float y, float width, float height, sf::Color color) {
    sf::RectangleShape button;
    button.setSize(sf::Vector2f(width, height));
    button.setPosition(sf::Vector2f(x, y));
    button.setFillColor(color);
    button.setOutlineThickness(2);
    button.setOutlineColor(sf::Color::White);
    return button;
}

sf::RectangleShape SFMLUIManager::createInputField(float x, float y, float width, float height, bool isActive) {
    sf::RectangleShape field;
    field.setSize(sf::Vector2f(width, height));
    field.setPosition(sf::Vector2f(x, y));
    field.setFillColor(isActive ? sf::Color(100, 100, 255) : sf::Color(70, 70, 70));
    field.setOutlineThickness(2);
    field.setOutlineColor(isActive ? sf::Color::Cyan : sf::Color::White);
    return field;
}

void SFMLUIManager::drawGradientBackground() {
    // Create a simple gradient effect using multiple rectangles
    for (int i = 0; i < 800; i += 10) {
        sf::RectangleShape strip;
        strip.setSize(sf::Vector2f(1200, 10));
        strip.setPosition(0, i);
        
        // Calculate gradient color from dark blue to black
        int colorValue = 50 - (i / 16); // Gradually darken
        if (colorValue < 0) colorValue = 0;
        
        strip.setFillColor(sf::Color(colorValue / 3, colorValue / 2, colorValue));
        window.draw(strip);
    }
}

void SFMLUIManager::drawMovieCard(const MovieDTO& movie, float x, float y, bool isSelected) {
    // Card background
    sf::RectangleShape card = createButton(x, y, 350, 180);
    card.setFillColor(isSelected ? sf::Color(80, 80, 120) : sf::Color(50, 50, 80));
    card.setOutlineThickness(isSelected ? 3 : 2);
    card.setOutlineColor(isSelected ? sf::Color::Yellow : sf::Color::White);
    window.draw(card);
    
    // Movie title
    sf::Text title = createText(movie.title, x + 10, y + 10, 20);
    title.setFillColor(sf::Color::White);
    window.draw(title);
    
    // Genre
    sf::Text genre = createText("Genre: " + movie.genre, x + 10, y + 40, 16);
    genre.setFillColor(sf::Color(200, 200, 200));
    window.draw(genre);
    
    // Rating
    sf::Text rating = createText("Rating: " + std::to_string(movie.rating), x + 10, y + 65, 16);
    rating.setFillColor(sf::Color(255, 215, 0)); // Gold color for rating
    window.draw(rating);
      // Movie ID
    sf::Text movieId = createText("ID: " + std::to_string(movie.id), x + 10, y + 90, 14);
    movieId.setFillColor(sf::Color(180, 180, 180));
    window.draw(movieId);
    
    // Duration (placeholder)
    sf::Text duration = createText("Duration: 120 min", x + 10, y + 115, 14);
    duration.setFillColor(sf::Color(180, 180, 180));
    window.draw(duration);
    
    // Price (placeholder)
    sf::Text price = createText("Price: $10.00", x + 10, y + 140, 16);
    price.setFillColor(sf::Color(0, 255, 0)); // Green for price
    window.draw(price);
}