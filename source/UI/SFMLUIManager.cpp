#include "SFMLUIManager.h"
#include <iostream>
#include <sstream>

SFMLUIManager::SFMLUIManager(std::shared_ptr<SessionManager> sessionMgr)
    : sessionManager(sessionMgr), currentState(UIState::GUEST_SCREEN),
      selectedMovieIndex(0), selectedShowTimeIndex(0),
      isInputtingUsername(true), isInputtingPassword(false),
      isInputtingEmail(false), isInputtingPhone(false),
      managingMovieId(-1), isEditingDate(false), isEditingStartTime(false), isEditingEndTime(false) {
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
    isEditingPrice = false;    editingMovieId = -1;
    previousState = UIState::GUEST_SCREEN;
    
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
            }        } else if (currentState == UIState::EDIT_MOVIE) {
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
            } else if (isEditingDate) {
                newShowtimeDate += inputChar;
            } else if (isEditingStartTime) {
                newShowtimeStartTime += inputChar;
            } else if (isEditingEndTime) {
                newShowtimeEndTime += inputChar;
            }
        } else if (currentState == UIState::SHOWTIME_MANAGEMENT) {
            if (isEditingDate) {
                newShowtimeDate += inputChar;
            } else if (isEditingStartTime) {
                newShowtimeStartTime += inputChar;
            } else if (isEditingEndTime) {
                newShowtimeEndTime += inputChar;
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
            }        } else if (currentState == UIState::EDIT_MOVIE) {
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
            } else if (isEditingDate && !newShowtimeDate.empty()) {
                newShowtimeDate.pop_back();
            } else if (isEditingStartTime && !newShowtimeStartTime.empty()) {
                newShowtimeStartTime.pop_back();
            } else if (isEditingEndTime && !newShowtimeEndTime.empty()) {
                newShowtimeEndTime.pop_back();
            }
        } else if (currentState == UIState::SHOWTIME_MANAGEMENT) {
            if (isEditingDate && !newShowtimeDate.empty()) {
                newShowtimeDate.pop_back();
            } else if (isEditingStartTime && !newShowtimeStartTime.empty()) {
                newShowtimeStartTime.pop_back();
            } else if (isEditingEndTime && !newShowtimeEndTime.empty()) {
                newShowtimeEndTime.pop_back();
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
    switch (currentState) {        case UIState::GUEST_SCREEN: {
            // Match the coordinates from renderGuestScreen()
            sf::RectangleShape loginBtn = createStyledButton(450, 320, 300, 60, sf::Color(0, 150, 0));
            sf::RectangleShape registerBtn = createStyledButton(450, 400, 300, 60, sf::Color(0, 100, 200));
            sf::RectangleShape browseMoviesBtn = createStyledButton(450, 480, 300, 60, sf::Color(150, 100, 0));
            
            if (isButtonClicked(loginBtn, mousePos)) {
                currentState = UIState::LOGIN_SCREEN;
                // Clear input fields when switching to login
                inputUsername.clear();
                inputPassword.clear();
                statusMessage.clear();
            } else if (isButtonClicked(registerBtn, mousePos)) {
                currentState = UIState::REGISTER_SCREEN;
                // Clear input fields when switching to register
                inputUsername.clear();
                inputPassword.clear();
                inputEmail.clear();
                inputPhone.clear();
                statusMessage.clear();
            } else if (isButtonClicked(browseMoviesBtn, mousePos)) {
                // Guest can browse movies without authentication
                loadMovies();
                currentState = UIState::MOVIE_LIST;
            }
            break;
        }
        
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
            int buttonY = 370;
            
            if (isButtonClicked(moviesBtn, mousePos)) {
                loadMovies();
                currentState = UIState::MOVIE_LIST;
            } else if (sessionManager->isUserAuthenticated()) {
                sf::RectangleShape historyBtn = createButton(500, buttonY, 200, 50);
                if (isButtonClicked(historyBtn, mousePos)) {
                    loadBookingHistory();
                    currentState = UIState::BOOKING_HISTORY;
                }
                buttonY += 70;
                
                // Admin panel access for admin users
                if (sessionManager->getCurrentRole() == "Admin") {
                    sf::RectangleShape adminBtn = createButton(500, buttonY, 200, 50);
                    if (isButtonClicked(adminBtn, mousePos)) {
                        currentState = UIState::ADMIN_PANEL;
                    }
                    buttonY += 70;
                }
            }
            
            // Logout button (position depends on previous buttons)
            sf::RectangleShape logoutBtn = createButton(500, buttonY, 200, 50);
            if (isButtonClicked(logoutBtn, mousePos)) {
                logout();
            }
            break;
        }
          case UIState::MOVIE_LIST: {
            sf::RectangleShape backBtn = createButton(50, 50, 100, 40);
            if (isButtonClicked(backBtn, mousePos)) {
                // Return guests to guest screen, authenticated users to main menu
                if (sessionManager->isUserAuthenticated()) {
                    currentState = UIState::MAIN_MENU;
                } else {
                    currentState = UIState::GUEST_SCREEN;
                }
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
            
            if (isButtonClicked(backBtn, mousePos)) {
                currentState = UIState::MOVIE_LIST;
            } else if (!currentShowTimes.empty()) {
                if (sessionManager->isUserAuthenticated()) {
                    // Authenticated users can book tickets
                    sf::RectangleShape bookBtn = createButton(500, 600, 200, 50);
                    if (isButtonClicked(bookBtn, mousePos)) {
                        currentState = UIState::BOOKING_SCREEN;
                    }
                } else {
                    // Guests get a login prompt button
                    sf::RectangleShape loginPromptBtn = createButton(500, 630, 200, 40);
                    if (isButtonClicked(loginPromptBtn, mousePos)) {
                        currentState = UIState::LOGIN_SCREEN;
                        // Clear any previous input
                        inputUsername.clear();
                        inputPassword.clear();
                        statusMessage.clear();
                    }
                }
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
                // Return to guest screen for guest users, login screen for authenticated users
                currentState = UIState::GUEST_SCREEN;
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
                isInputtingPhone = true;
            }
            break;
        }        case UIState::ADMIN_PANEL: {
            sf::RectangleShape backBtn = createButton(500, 380, 200, 50);
            sf::RectangleShape manageMoviesBtn = createButton(500, 300, 200, 50);
            
            if (isButtonClicked(backBtn, mousePos)) {
                currentState = UIState::MAIN_MENU;
            } else if (isButtonClicked(manageMoviesBtn, mousePos)) {
                loadMovies();
                currentState = UIState::MOVIE_MANAGEMENT;
            }
            break;
        }
          case UIState::MOVIE_MANAGEMENT: {
            sf::RectangleShape backBtn = createButton(50, 50, 100, 40);
            sf::RectangleShape addMovieBtn = createButton(200, 50, 150, 40);  // Match coordinates from render function
            
            if (isButtonClicked(backBtn, mousePos)) {
                currentState = UIState::ADMIN_PANEL;            } else if (isButtonClicked(addMovieBtn, mousePos)) {
                // Reset edit fields for new movie
                clearEditingFields();
                isEditingTitle = true;
                currentState = UIState::EDIT_MOVIE;} else {
                // Check for edit/delete buttons on movie cards (coordinates match renderMovieManagement)
                for (size_t i = 0; i < movies.size(); ++i) {
                    sf::RectangleShape editBtn = createButton(920, 155 + i * 80, 80, 30);
                    sf::RectangleShape deleteBtn = createButton(1010, 155 + i * 80, 80, 30);
                    sf::RectangleShape showtimeBtn = createButton(1100, 155 + i * 80, 80, 25);
                    
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
                    } else if (isButtonClicked(showtimeBtn, mousePos)) {
                        managingMovieId = movies[i].id;
                        loadShowTimes(movies[i].id);
                        resetShowtimeEditingFlags();
                        currentState = UIState::SHOWTIME_MANAGEMENT;
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
                    // Add new movie
                    addMovie();
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
            
            // Showtime fields (only for new movies)
            if (editingMovieId == -1) {
                sf::RectangleShape dateField = createInputField(360, 505, 150, 30, isEditingDate);
                sf::RectangleShape startField = createInputField(580, 505, 100, 30, isEditingStartTime);
                sf::RectangleShape endField = createInputField(740, 505, 100, 30, isEditingEndTime);
                
                if (isButtonClicked(dateField, mousePos)) {
                    resetShowtimeEditingFlags(); // Reset all showtime flags first
                    isEditingDate = true;        // Then activate the clicked one
                    // Also reset main editing flags if a showtime field is clicked
                    resetEditingFlags(); 
                } else if (isButtonClicked(startField, mousePos)) {
                    resetShowtimeEditingFlags();
                    isEditingStartTime = true;
                    resetEditingFlags();
                } else if (isButtonClicked(endField, mousePos)) {
                    resetShowtimeEditingFlags();
                    isEditingEndTime = true;
                    resetEditingFlags();
                }
            }
            break;
        }case UIState::SHOWTIME_MANAGEMENT: {
            // Only handle back button and delete buttons
            sf::RectangleShape backButton = createStyledButton(50, 720, 100, 40, sf::Color(100, 100, 100));
            
            if (isButtonClicked(backButton, mousePos)) {
                currentState = UIState::MOVIE_MANAGEMENT;
                resetShowtimeEditingFlags();
            } else {
                // Check for delete buttons on showtime entries (updated coordinates)
                for (size_t i = 0; i < currentShowTimes.size() && i < 8; ++i) {
                    sf::RectangleShape deleteBtn = createStyledButton(700, 305 + i * 30, 80, 25, sf::Color(150, 0, 0));
                    if (isButtonClicked(deleteBtn, mousePos)) {
                        deleteShowtime(managingMovieId, currentShowTimes[i].showTimeID);
                        break;
                    }
                }
            }
            break;
        }
          case UIState::SUCCESS_MESSAGE: {
            sf::RectangleShape okBtn = createButton(500, 520, 200, 50);
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
        case UIState::GUEST_SCREEN:
            renderGuestScreen();
            break;
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
            break;        case UIState::EDIT_MOVIE:
            renderEditMovie();
            break;
        case UIState::SHOWTIME_MANAGEMENT:
            renderShowtimeManagement();
            break;
        case UIState::SUCCESS_MESSAGE:
            renderSuccessMessage();
            break;
    }
    
    window.display();
}

void SFMLUIManager::renderGuestScreen() {
    // Draw gradient background
    drawGradientBackground();
    
    // Main title
    sf::Text title = createText("Movie Ticket Booking System", 300, 150, 40);
    title.setFillColor(sf::Color::White);
    window.draw(title);
    
    // Welcome message
    sf::Text welcome = createText("Welcome! Please choose an option to continue", 350, 220, 24);
    welcome.setFillColor(sf::Color(200, 200, 200));
    window.draw(welcome);
    
    // Login button
    sf::RectangleShape loginBtn = createStyledButton(450, 320, 300, 60, sf::Color(0, 150, 0));
    window.draw(loginBtn);
    
    sf::Text loginText = createText("Login", 570, 340, 24);
    loginText.setFillColor(sf::Color::White);
    window.draw(loginText);
    
    // Register button
    sf::RectangleShape registerBtn = createStyledButton(450, 400, 300, 60, sf::Color(0, 100, 200));
    window.draw(registerBtn);
    
    sf::Text registerText = createText("Register", 555, 420, 24);
    registerText.setFillColor(sf::Color::White);
    window.draw(registerText);
    
    // Browse Movies button (Guest can view movies without logging in)
    sf::RectangleShape browseBtn = createStyledButton(450, 480, 300, 60, sf::Color(150, 100, 0));
    window.draw(browseBtn);
    
    sf::Text browseText = createText("Browse Movies", 530, 500, 24);
    browseText.setFillColor(sf::Color::White);
    window.draw(browseText);
    
    // Footer message
    sf::Text footer = createText("No account needed to browse movies", 420, 580, 18);
    footer.setFillColor(sf::Color(150, 150, 150));
    window.draw(footer);
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
    
    int buttonY = 370;
    
    // History button (only for authenticated users)
    if (sessionManager->isUserAuthenticated()) {
        sf::RectangleShape historyBtn = createButton(500, buttonY, 200, 50);
        historyBtn.setFillColor(sf::Color(150, 100, 0));
        window.draw(historyBtn);
        
        sf::Text historyText = createText("Booking History", 520, buttonY + 15, 20);
        window.draw(historyText);
        
        buttonY += 70;
    }
    
    // Admin Panel button (only for admin users)
    if (sessionManager->getCurrentRole() == "Admin") {
        sf::RectangleShape adminBtn = createButton(500, buttonY, 200, 50);
        adminBtn.setFillColor(sf::Color(100, 0, 150));
        window.draw(adminBtn);
        
        sf::Text adminText = createText("Admin Panel", 540, buttonY + 15, 20);
        window.draw(adminText);
        
        buttonY += 70;
    }
    
    // Logout button
    sf::RectangleShape logoutBtn = createButton(500, buttonY, 200, 50);
    logoutBtn.setFillColor(sf::Color(150, 0, 0));
    window.draw(logoutBtn);
    
    sf::Text logoutText = createText("Logout", 560, buttonY + 15, 20);
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
          // Book button (only for authenticated users)
        if (!currentShowTimes.empty() && sessionManager->isUserAuthenticated()) {
            sf::RectangleShape bookBtn = createButton(500, 600, 200, 50);
            bookBtn.setFillColor(sf::Color(0, 150, 0));
            window.draw(bookBtn);
            
            sf::Text bookText = createText("Book Tickets", 540, 615, 20);
            window.draw(bookText);
        } else if (!currentShowTimes.empty() && !sessionManager->isUserAuthenticated()) {
            // Message for guests
            sf::Text guestMessage = createText("Please login to book tickets", 450, 600, 18);
            guestMessage.setFillColor(sf::Color(200, 200, 0));
            window.draw(guestMessage);
            
            sf::RectangleShape loginPromptBtn = createButton(500, 630, 200, 40);
            loginPromptBtn.setFillColor(sf::Color(0, 100, 150));
            window.draw(loginPromptBtn);
            
            sf::Text loginPromptText = createText("Go to Login", 550, 640, 18);
            window.draw(loginPromptText);
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
    drawGradientBackground();
    
    sf::Text title = createText("Admin Panel", 500, 100, 32);
    title.setFillColor(sf::Color::White);
    window.draw(title);
    
    // Movie Management button
    sf::RectangleShape movieMgmtBtn = createButton(500, 300, 200, 50);
    movieMgmtBtn.setFillColor(sf::Color(0, 150, 0));
    window.draw(movieMgmtBtn);
    
    sf::Text movieMgmtText = createText("Movie Management", 510, 315, 18);
    movieMgmtText.setFillColor(sf::Color::White);
    window.draw(movieMgmtText);
    
    // Back to Main Menu button
    sf::RectangleShape backBtn = createButton(500, 380, 200, 50);
    backBtn.setFillColor(sf::Color(100, 100, 100));
    window.draw(backBtn);
    
    sf::Text backText = createText("Back to Main Menu", 515, 395, 18);
    backText.setFillColor(sf::Color::White);
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
        
        // Showtimes button
        sf::RectangleShape showtimeBtn = createButton(1100, 155 + i * 80, 80, 25);
        showtimeBtn.setFillColor(sf::Color(150, 100, 0));
        window.draw(showtimeBtn);
        
        sf::Text showtimeText = createText("Times", 1118, 163 + i * 80, 14);
        window.draw(showtimeText);
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
    sf::RectangleShape saveBtn = createButton(500, 550, 200, 50);
    saveBtn.setFillColor(sf::Color(0, 150, 0));
    window.draw(saveBtn);
    
    sf::Text saveText = createText("Save", 580, 565, 18);
    window.draw(saveText);
      // Form fields
    sf::Text titleLabel = createText("Title:", 300, 160, 18);
    window.draw(titleLabel);
      sf::RectangleShape titleField = createButton(400, 150, 400, 40);
    titleField.setFillColor(isEditingTitle ? sf::Color(100, 100, 255) : sf::Color(70, 70, 70));
    window.draw(titleField);
    
    sf::Text titleText = createText(editMovieTitle, 410, 160, 18);
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
    window.draw(durationText);    sf::Text priceLabel = createText("Rating:", 620, 390, 18);
    window.draw(priceLabel);
    
    sf::RectangleShape priceField = createButton(620, 400, 180, 40);
    priceField.setFillColor(isEditingPrice ? sf::Color(100, 100, 255) : sf::Color(70, 70, 70));
    window.draw(priceField);
    
    sf::Text priceText = createText(editMoviePrice, 630, 410, 18);
    window.draw(priceText);
    
    // Showtime section (only show when adding new movie)
    if (editingMovieId == -1) {
        sf::Text showtimeLabel = createText("Initial Showtimes (Optional):", 300, 460, 20);
        showtimeLabel.setFillColor(sf::Color(200, 200, 255));
        window.draw(showtimeLabel);
        
        sf::Text showtimeInfo = createText("Format: Date (YYYY-MM-DD), Start Time (HH:MM), End Time (HH:MM)", 300, 485, 14);
        showtimeInfo.setFillColor(sf::Color(150, 150, 150));
        window.draw(showtimeInfo);
        
        // Date input
        sf::Text dateLabel = createText("Date:", 300, 510, 16);
        window.draw(dateLabel);
        
        sf::RectangleShape dateField = createInputField(360, 505, 150, 30, isEditingDate);
        window.draw(dateField);
        
        sf::Text dateText = createText(newShowtimeDate.empty() ? "YYYY-MM-DD" : newShowtimeDate, 370, 515, 14);
        dateText.setFillColor(isEditingDate ? sf::Color::Black : sf::Color(100, 100, 100));
        window.draw(dateText);
        
        // Start time input
        sf::Text startLabel = createText("Start:", 530, 510, 16);
        window.draw(startLabel);
        
        sf::RectangleShape startField = createInputField(580, 505, 100, 30, isEditingStartTime);
        window.draw(startField);
        
        sf::Text startText = createText(newShowtimeStartTime.empty() ? "HH:MM" : newShowtimeStartTime, 590, 515, 14);
        startText.setFillColor(isEditingStartTime ? sf::Color::Black : sf::Color(100, 100, 100));
        window.draw(startText);
        
        // End time input
        sf::Text endLabel = createText("End:", 700, 510, 16);
        window.draw(endLabel);
        
        sf::RectangleShape endField = createInputField(740, 505, 100, 30, isEditingEndTime);
        window.draw(endField);
        
        sf::Text endText = createText(newShowtimeEndTime.empty() ? "HH:MM" : newShowtimeEndTime, 750, 515, 14);
        endText.setFillColor(isEditingEndTime ? sf::Color::Black : sf::Color(100, 100, 100));
        window.draw(endText);
    }
}

void SFMLUIManager::renderSuccessMessage() {
    // Background overlay
    sf::RectangleShape overlay;
    overlay.setSize(sf::Vector2f(1200, 800));
    overlay.setFillColor(sf::Color(0, 0, 0, 128));
    window.draw(overlay);
    
    // Message box
    sf::RectangleShape messageBox = createButton(250, 200, 700, 400);
    messageBox.setFillColor(sf::Color(50, 50, 50));
    messageBox.setOutlineThickness(3);
    messageBox.setOutlineColor(sf::Color::Green);
    window.draw(messageBox);
    
    // Success title
    sf::Text title = createText("SUCCESS!", 550, 250, 36);
    title.setFillColor(sf::Color::Green);
    window.draw(title);
    
    // Parse and display multi-line success message
    std::string line;
    std::istringstream stream(successMessage);
    float yPos = 320;
    
    while (std::getline(stream, line)) {
        if (!line.empty()) {
            sf::Text messageLine = createText(line, 280, yPos, 18);
            messageLine.setFillColor(sf::Color::White);
            window.draw(messageLine);
        }
        yPos += 25; // Line spacing
    }
    
    // OK button
    sf::RectangleShape okBtn = createButton(500, 520, 200, 50);
    okBtn.setFillColor(sf::Color(0, 150, 0));
    window.draw(okBtn);
    
    sf::Text okText = createText("OK", 580, 535, 20);
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
            
            // Booking successful - show success message
            previousState = UIState::MAIN_MENU;
            selectedSeats.clear();
            
            // Create detailed success message
            std::string movieTitle = (selectedMovieIndex < movies.size()) ? movies[selectedMovieIndex].title : "Movie";
            std::string showTimeInfo = currentShowTimes[selectedShowTimeIndex].date + " " + 
                                     currentShowTimes[selectedShowTimeIndex].startTime;
            std::string seatsInfo = "";
            for (size_t i = 0; i < selectedSeats.size(); ++i) {
                if (i > 0) seatsInfo += ", ";
                seatsInfo += selectedSeats[i];
            }
              successMessage = std::string("Booking Successful!\n\n") +
                           "Movie: " + movieTitle + "\n" +
                           "Show Time: " + showTimeInfo + "\n" +
                           "Seats: " + seatsInfo + "\n\n" +
                           "Thank you for your booking!";
            
            currentState = UIState::SUCCESS_MESSAGE;
        } catch (const std::exception& e) {
            statusMessage = "Booking failed: " + std::string(e.what());
            // Stay in current state to show error
        }
    }
}

void SFMLUIManager::logout() {
    sessionManager->logout();
    currentState = UIState::GUEST_SCREEN;
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
    auto visitor = std::make_shared<MovieManagerServiceVisitor>();
    sessionManager->getCurrentContext()->accept(visitor);
    auto movieManagerService = visitor->getMovieManagerService();
    
    if (movieManagerService) {
        try {
            movieManagerService->deleteMovie(movieId);
            showSuccessMessage("Movie deleted successfully!");
            loadMovies(); // Reload the movie list
        } catch (const std::exception& e) {
            showSuccessMessage("Error deleting movie: " + std::string(e.what()));
        }
    } else {
        showSuccessMessage("Access denied: Admin privileges required!");
    }
}

void SFMLUIManager::updateMovie(int movieId) {
    auto visitor = std::make_shared<MovieManagerServiceVisitor>();
    sessionManager->getCurrentContext()->accept(visitor);
    auto movieManagerService = visitor->getMovieManagerService();    if (movieManagerService) {
        try {
            // Create updated movie object with ID - use try-catch for price conversion
            float rating = 5.0f; // Default rating
            try {
                if (!editMoviePrice.empty()) {
                    rating = std::stof(editMoviePrice);
                    if (rating < 0.0f || rating > 10.0f) {
                        rating = 5.0f; // Clamp to valid range
                    }
                }
            } catch (...) {
                rating = 5.0f; // Default if conversion fails
            }
            
            auto updatedMovie = std::make_shared<Movie>(
                movieId,  // Include ID in constructor
                editMovieTitle,
                editMovieGenre, 
                editMovieDescription.empty() ? "No description available" : editMovieDescription,
                rating
            );
              // For now, we'll simulate update by deleting and re-adding
            // In a real implementation, you'd have an updateMovie method
            movieManagerService->deleteMovie(movieId);
            
            // Add the updated movie back without default showtimes
            std::vector<std::string> emptyShowTimes;
            movieManagerService->addMovie(updatedMovie, emptyShowTimes);
            
            showSuccessMessage("Movie updated successfully!");
            clearEditingFields();
            loadMovies(); // Reload the movie list
        } catch (const std::exception& e) {
            showSuccessMessage("Error updating movie: " + std::string(e.what()));
        }} else {
        showSuccessMessage("Access denied: Admin privileges required!");
    }
}

void SFMLUIManager::addMovie() {
    auto visitor = std::make_shared<MovieManagerServiceVisitor>();
    sessionManager->getCurrentContext()->accept(visitor);
    auto movieManagerService = visitor->getMovieManagerService();
    
    if (movieManagerService) {
        try {
            // Validate input fields
            if (editMovieTitle.empty() || editMovieGenre.empty()) {
                showSuccessMessage("Error: Title and Genre are required!");
                return;
            }
            
            float rating = 5.0f; // Default rating
            try {
                if (!editMoviePrice.empty()) {
                    rating = std::stof(editMoviePrice);
                    if (rating < 0.0f || rating > 10.0f) {
                        rating = 5.0f; // Clamp to valid range
                    }
                }
            } catch (...) {
                rating = 5.0f; // Default if conversion fails
            }            // Create new movie object
            auto newMovie = std::make_shared<Movie>(
                editMovieTitle,
                editMovieGenre,
                editMovieDescription.empty() ? "No description available" : editMovieDescription,
                rating
            );
            
            // Collect showtime data from input fields
            std::vector<std::string> movieShowTimes;
            if (!newShowtimeDate.empty() && !newShowtimeStartTime.empty() && !newShowtimeEndTime.empty()) {
                // Corrected format: comma-separated
                std::string showtime = newShowtimeDate + "," + newShowtimeStartTime + "," + newShowtimeEndTime;
                movieShowTimes.push_back(showtime);
            }
            
            movieManagerService->addMovie(newMovie, movieShowTimes);
            
            showSuccessMessage("New movie \'" + editMovieTitle + "\' added successfully!" + 
                (movieShowTimes.empty() ? "\\nTo add showtimes, edit the movie and add them during creation." : 
                "\\nShowtime added: " + newShowtimeDate + " " + newShowtimeStartTime + "-" + newShowtimeEndTime)); // Keep user-facing message format
            clearEditingFields();
            loadMovies(); // Reload the movie list
        } catch (const std::exception& e) {
            showSuccessMessage("Error adding movie: " + std::string(e.what()));
        }
    } else {
        showSuccessMessage("Access denied: Admin privileges required!");
    }
}

void SFMLUIManager::resetEditingFlags() {
    isEditingTitle = false;
    isEditingDescription = false;
    isEditingGenre = false;
    isEditingDuration = false;
    isEditingPrice = false;
    // Don't clear input fields here - only reset boolean flags
    // editingMovieId = -1;  // Keep this for new movies only
    // editMovieTitle.clear();  // Don't clear text
    // editMovieDescription.clear();  // Don't clear text
    // editMovieGenre.clear();  // Don't clear text
    // editMovieDuration.clear();  // Don't clear text
    // editMoviePrice.clear();  // Don't clear text
}

void SFMLUIManager::clearEditingFields() {
    // New function to completely clear all editing data
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
    
    // Clear showtime fields as well
    clearShowtimeFields();
}

void SFMLUIManager::renderShowtimeManagement() {
    drawGradientBackground();
    
    // Title
    sf::Text title = createText("Showtime Management", 50, 50, 36);
    title.setFillColor(sf::Color::White);
    window.draw(title);
    
    // Movie info if managing specific movie
    if (managingMovieId >= 0) {
        auto it = std::find_if(movies.begin(), movies.end(), 
            [this](const MovieDTO& movie) { return movie.id == managingMovieId; });
        
        if (it != movies.end()) {
            sf::Text movieInfo = createText("Managing showtimes for: " + it->title, 50, 100, 24);
            movieInfo.setFillColor(sf::Color(200, 200, 255));
            window.draw(movieInfo);
        }
    }
    
    // Information message about adding showtimes
    sf::Text infoLabel = createText("Showtime Information:", 50, 150, 20);
    infoLabel.setFillColor(sf::Color::White);
    window.draw(infoLabel);
    
    sf::Text infoMessage1 = createText("• Showtimes can only be added when creating a new movie", 50, 180, 16);
    infoMessage1.setFillColor(sf::Color(200, 200, 0));
    window.draw(infoMessage1);
    
    sf::Text infoMessage2 = createText("• Use 'Add Movie' in Movie Management to create movies with showtimes", 50, 200, 16);
    infoMessage2.setFillColor(sf::Color(200, 200, 0));
    window.draw(infoMessage2);
    
    sf::Text infoMessage3 = createText("• This screen allows you to view and delete existing showtimes only", 50, 220, 16);
    infoMessage3.setFillColor(sf::Color(200, 200, 0));
    window.draw(infoMessage3);
    
    // Current Showtimes Section
    sf::Text currentLabel = createText("Current Showtimes:", 50, 270, 20);
    currentLabel.setFillColor(sf::Color::White);
    window.draw(currentLabel);
    
    // Display current showtimes
    for (size_t i = 0; i < currentShowTimes.size() && i < 8; ++i) {
        std::string showtimeInfo = "Date: " + currentShowTimes[i].date + 
                                  " | Time: " + currentShowTimes[i].startTime + 
                                  " - " + currentShowTimes[i].endTime;
        
        sf::Text showtimeText = createText(showtimeInfo, 50, 310 + i * 30, 14);
        showtimeText.setFillColor(sf::Color(200, 200, 200));
        window.draw(showtimeText);
        
        // Delete button for each showtime
        sf::RectangleShape deleteBtn = createStyledButton(700, 305 + i * 30, 80, 25, sf::Color(150, 0, 0));
        window.draw(deleteBtn);
        
        sf::Text deleteText = createText("Delete",  720, 312 + i * 30, 12);
        deleteText.setFillColor(sf::Color::White);
        window.draw(deleteText);
    }
      // Back button
    sf::RectangleShape backButton = createStyledButton(50, 720, 100, 40, sf::Color(100, 100, 100));
    window.draw(backButton);
    
    sf::Text backText = createText("Back", 85, 735, 16);
    backText.setFillColor(sf::Color::White);
    window.draw(backText);
}

void SFMLUIManager::addShowtime(int movieId) {
    // Display message that showtimes can only be added when creating a movie
    showSuccessMessage("Showtimes can only be added when creating a new movie.\n\nTo add showtimes:\n1. Go to Movie Management\n2. Click 'Add Movie'\n3. Enter movie details and showtimes together\n\nThis screen is for viewing and deleting existing showtimes only.");
}

void SFMLUIManager::deleteShowtime(int movieId, int showtimeId) {
    auto visitor = std::make_shared<MovieManagerServiceVisitor>();
    sessionManager->getCurrentContext()->accept(visitor);
    auto movieManagerService = visitor->getMovieManagerService();
    
    if (movieManagerService) {
        try {
            // Call the deleteShowTime method
            movieManagerService->deleteShowTime(movieId, showtimeId);
            showSuccessMessage("Showtime deleted successfully!");
            
            // Reload showtimes for the current movie
            if (managingMovieId >= 0) {
                loadShowTimes(managingMovieId);
            }
            
        } catch (const std::exception& e) {
            showSuccessMessage("Error deleting showtime: " + std::string(e.what()));
        }
    } else {
        showSuccessMessage("Access denied: Admin privileges required!");
    }
}

void SFMLUIManager::resetShowtimeEditingFlags() {
    isEditingDate = false;
    isEditingStartTime = false;
    isEditingEndTime = false;
    // Don't clear the input data here - only reset boolean flags
    // newShowtimeDate.clear();
    // newShowtimeStartTime.clear();
    // newShowtimeEndTime.clear();
}

void SFMLUIManager::clearShowtimeFields() {
    // Function to completely clear showtime input data
    newShowtimeDate.clear();
    newShowtimeStartTime.clear();
    newShowtimeEndTime.clear();
    isEditingDate = false;
    isEditingStartTime = false;
    isEditingEndTime = false;
}

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
    field.setFillColor(isActive ? sf::Color(255, 255, 255, 220) : sf::Color(200, 200, 200, 180));
    field.setOutlineThickness(2);
    field.setOutlineColor(isActive ? sf::Color::Blue : sf::Color(100, 100, 100));
    return field;
}

void SFMLUIManager::drawGradientBackground() {
    // Create a simple gradient background using multiple rectangles
    for (int i = 0; i < 800; i += 10) {
        sf::RectangleShape strip;
        strip.setSize(sf::Vector2f(1200, 10));
        strip.setPosition(sf::Vector2f(0, i));
        
        // Calculate gradient color
        float ratio = static_cast<float>(i) / 800.0f;
        int colorValue = static_cast<int>(20 + ratio * 40); // From dark to slightly lighter
        strip.setFillColor(sf::Color(colorValue, colorValue, colorValue + 10));
        window.draw(strip);
    }
}

void SFMLUIManager::drawMovieCard(const MovieDTO& movie, float x, float y, bool isSelected) {
    // Card background
    sf::RectangleShape card;
    card.setSize(sf::Vector2f(300, 180));
    card.setPosition(sf::Vector2f(x, y));
    card.setFillColor(isSelected ? sf::Color(80, 80, 120) : sf::Color(50, 50, 80));
    card.setOutlineThickness(2);
    card.setOutlineColor(isSelected ? sf::Color::Yellow : sf::Color::White);
    window.draw(card);
    
    // Movie title
    sf::Text title = createText(movie.title, x + 10, y + 10, 18);
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