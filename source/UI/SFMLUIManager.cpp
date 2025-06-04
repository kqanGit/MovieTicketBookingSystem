#include "SFMLUIManager.h"
#include <iostream>
#include <sstream>

SFMLUIManager::SFMLUIManager(std::shared_ptr<SessionManager> sessionMgr)
    : sessionManager(sessionMgr), currentState(UIState::GUEST_SCREEN), previousState(UIState::GUEST_SCREEN),
      selectedMovieIndex(0), selectedShowTimeIndex(0), editingMovieId(-1), managingMovieId(-1),
      isInputtingUsername(true), isInputtingPassword(false), isInputtingEmail(false), isInputtingPhone(false),
      isEditingTitle(false), isEditingDescription(false), isEditingGenre(false), isEditingPrice(false),
      isEditingDate(false), isEditingStartTime(false), isEditingEndTime(false),
      movieListScrollOffset(0), movieViewScrollOffset(0), isAddingShowtime(false) {
}

SFMLUIManager::~SFMLUIManager() {
    shutdown();
}

bool SFMLUIManager::initialize() {
    // Create window - SFML 2.6.2 syntax
    window.create(sf::VideoMode(windowWidth, windowHeight), "Movie Ticket Booking System");
    window.setFramerateLimit(60);
    
    // Initialize admin editing flags
    isEditingTitle = false;
    isEditingDescription = false;
    isEditingGenre = false;
    isEditingPrice = false;    editingMovieId = -1;
    previousState = UIState::GUEST_SCREEN;
    
    // Try to load font from various locations
    bool fontLoaded = false;
    std::vector<std::string> fontPaths = {
        "./fonts/arial.ttf",
        "./fonts/calibri.ttf",
        "./fonts/tahoma.ttf"
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
      // Load background image - try multiple paths
    bool backgroundLoaded = false;
    std::vector<std::string> backgroundPaths = {
        "./image/background@.png"};

    for (const auto& path : backgroundPaths) {
        if (backgroundTexture.loadFromFile(path)) {
            std::cout << "Successfully loaded background image from: " << path << std::endl;
            backgroundLoaded = true;
            break;
        } else {
            std::cout << "Failed to load background image from: " << path << std::endl;
        }
    }
    
    if (!backgroundLoaded) {
        std::cout << "ERROR: Could not load background image from any path!" << std::endl;
        // Create a default colored texture if image loading fails
        sf::Image defaultBg;
        defaultBg.create(windowWidth, windowHeight, sf::Color(30, 30, 50)); // Dark blue background
        backgroundTexture.loadFromImage(defaultBg);
        std::cout << "Created a default background color instead" << std::endl;
    }
    backgroundSprite.setTexture(backgroundTexture);    // Scale background to window
    sf::Vector2u texSize = backgroundTexture.getSize();
    sf::Vector2u winSize = window.getSize();
    
    // Print texture and window sizes for debugging
    std::cout << "Background texture size: " << texSize.x << "x" << texSize.y << std::endl;
    std::cout << "Window size: " << winSize.x << "x" << winSize.y << std::endl;
    
    // Set the scaling to fit the window
    float scaleX = static_cast<float>(winSize.x) / texSize.x;
    float scaleY = static_cast<float>(winSize.y) / texSize.y;
    
    // Apply the scaling to the sprite
    backgroundSprite.setScale(scaleX, scaleY);
    std::cout << "Applied scale: " << scaleX << "x" << scaleY << std::endl;

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
          // Handle mouse wheel scrolling for movie management and movie list screens
        if (event.type == sf::Event::MouseWheelScrolled && 
            event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
            
            const int maxVisibleMovies = 7;
            
            if (currentState == UIState::MOVIE_MANAGEMENT) {
                if (event.mouseWheelScroll.delta > 0 && movieListScrollOffset > 0) {
                    // Scroll up
                    movieListScrollOffset--;
                }
                else if (event.mouseWheelScroll.delta < 0 && 
                        (movieListScrollOffset + maxVisibleMovies < movies.size())) {
                    // Scroll down
                    movieListScrollOffset++;
                }
            }
            else if (currentState == UIState::MOVIE_LIST) {
                if (event.mouseWheelScroll.delta > 0 && movieViewScrollOffset > 0) {
                    // Scroll up
                    movieViewScrollOffset--;
                }
                else if (event.mouseWheelScroll.delta < 0 && 
                        (movieViewScrollOffset + maxVisibleMovies < movies.size())) {
                    // Scroll down
                    movieViewScrollOffset++;
                }
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
                editMovieGenre += inputChar;            // Removed Movie Duration
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
                editMovieGenre.pop_back();            // Removed Movie Duration
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
    switch (currentState) {
        case UIState::GUEST_SCREEN: {
            // Calculate button positions to match renderGuestScreen()
            float w = buttonWidth;
            float h = buttonHeight;
            float gap = buttonGap;
            float totalWidth = (w * 3) + (gap * 2);
            float startX = (windowWidth - totalWidth) / 2;
            float y = buttonY;
            sf::RectangleShape loginBtn = createStyledButton(startX, y, w, h, sf::Color(0, 150, 0));
            sf::RectangleShape registerBtn = createStyledButton(startX + w + gap, y, w, h, sf::Color(0, 100, 200));
            sf::RectangleShape browseMoviesBtn = createStyledButton(startX + (w + gap) * 2, y, w, h, sf::Color(150, 100, 0));
            if (isButtonClicked(loginBtn, mousePos)) {
                currentState = UIState::LOGIN_SCREEN;
                inputUsername.clear();
                inputPassword.clear();
                statusMessage.clear();
            } else if (isButtonClicked(registerBtn, mousePos)) {
                currentState = UIState::REGISTER_SCREEN;
                inputUsername.clear();
                inputPassword.clear();
                inputEmail.clear();
                inputPhone.clear();
                statusMessage.clear();
            } else if (isButtonClicked(browseMoviesBtn, mousePos)) {
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
        }        case UIState::MAIN_MENU: {
            // Use member variables for button layout (matching renderMainMenu)
            float w = buttonWidth * 0.72f; // Main menu buttons are smaller
            float h = buttonHeight;
            float gap = buttonGap * 0.75f;
            float totalWidth = (w * 4) + (gap * 3);
            float startX = (windowWidth - totalWidth) / 2;
            float y = buttonY;
            
            // Movies button
            sf::RectangleShape moviesBtn = createButton(startX, y, w, h);
            if (isButtonClicked(moviesBtn, mousePos)) {
                loadMovies();
                currentState = UIState::MOVIE_LIST;
            }
            
            // History button (only for authenticated users)
            if (sessionManager->isUserAuthenticated()) {
                sf::RectangleShape historyBtn = createButton(startX + (w + gap), y, w, h);
                if (isButtonClicked(historyBtn, mousePos)) {
                    loadBookingHistory();
                    currentState = UIState::BOOKING_HISTORY;
                }
            }
            
            // Admin panel button (only for admin users)
            if (sessionManager->getCurrentRole() == "Admin") {
                sf::RectangleShape adminBtn = createButton(startX + 2 * (w + gap), y, w, h);
                if (isButtonClicked(adminBtn, mousePos)) {
                    currentState = UIState::ADMIN_PANEL;
                }
            }
            
            // Logout button
            sf::RectangleShape logoutBtn = createButton(startX + 3 * (w + gap), y, w, h);
            if (isButtonClicked(logoutBtn, mousePos)) {
                logout();
            }
            break;
        }case UIState::MOVIE_LIST: {
            sf::RectangleShape backBtn = createButton(50, 50, 100, 40);
            // Scroll buttons
            sf::RectangleShape scrollUpBtn = createStyledButton(1150, 150, 50, 40, sf::Color(80, 40, 120)); // Matched renderMovieList
            sf::RectangleShape scrollDownBtn = createStyledButton(1150, 600, 50, 40, sf::Color(80, 40, 120)); // Matched renderMovieList
            
            if (isButtonClicked(backBtn, mousePos)) {
                // Return guests to guest screen, authenticated users to main menu
                if (sessionManager->isUserAuthenticated()) {
                    currentState = UIState::MAIN_MENU;
                } else {
                    currentState = UIState::GUEST_SCREEN;
                }
                break;
            } else if (isButtonClicked(scrollUpBtn, mousePos) && movieViewScrollOffset > 0) {
                movieViewScrollOffset--;
            } else if (isButtonClicked(scrollDownBtn, mousePos) &&
                       (movieViewScrollOffset + 6 < movies.size())) { // 6 is maxVisibleMovies from renderMovieList
                movieViewScrollOffset++;
            } else {
                // Check movie selection - Adjusted to match renderMovieList
                const int maxVisibleMovies = 6; // Matches renderMovieList
                int endIndex = std::min(movieViewScrollOffset + maxVisibleMovies, static_cast<int>(movies.size()));
                
                for (int i = movieViewScrollOffset; i < endIndex; ++i) {
                    int displayIndex = i - movieListScrollOffset; // Display position
                    // Use coordinates and dimensions from renderMovieList for movieBtn
                    sf::RectangleShape movieBtn = createButton(100, 150 + displayIndex * 90, 800, 70);
                    if (isButtonClicked(movieBtn, mousePos)) {
                        selectedMovieIndex = i;
                        loadMovieDetails(movies[i].id); // Load details for the selected movie
                        loadShowTimes(movies[i].id);    // Load showtimes for the selected movie
                        currentState = UIState::MOVIE_DETAILS;
                        break; 
                    }
                }
            }
            break;
        }
          case UIState::MOVIE_DETAILS: {
            sf::RectangleShape backBtn = createButton(50, 50, 100, 40);
            
            if (isButtonClicked(backBtn, mousePos)) {
                currentState = UIState::MOVIE_LIST; // Go back to movie list
            } else if (!currentShowTimes.empty() && sessionManager->isUserAuthenticated()) {
                // Match coordinates from renderMovieDetails
                sf::RectangleShape bookBtn = createButton(400, 555, 200, 50);
                if (isButtonClicked(bookBtn, mousePos)) {
                    currentState = UIState::BOOKING_SCREEN;
                    // selectedMovieIndex should already be set from MOVIE_LIST state
                    // loadShowTimes(movies[selectedMovieIndex].id); // Already loaded when going to details
                }
            } else if (!currentShowTimes.empty() && !sessionManager->isUserAuthenticated()) {
                // Match coordinates from renderMovieDetails for the "GO TO LOGIN" button
                sf::RectangleShape loginPromptBtn = createButton(400, 595, 200, 40);
                if (isButtonClicked(loginPromptBtn, mousePos)) {
                    currentState = UIState::LOGIN_SCREEN;
                    statusMessage.clear(); // Clear any previous login messages
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
            sf::RectangleShape confirmBtn = createButton(900, 650, 200, 50);
            
            if (isButtonClicked(backBtn, mousePos)) {
                currentState = UIState::BOOKING_SCREEN;
                selectedSeats.clear();
            } else if (isButtonClicked(confirmBtn, mousePos) && !selectedSeats.empty()) {
                createBooking();            
            } else {
                // Check seat selection
                int seatSize = 40;
                int seatSpacing = 45;
                int startX = 200;
                int startY = 200;
                
                // Tạo map để nhóm ghế theo hàng
                std::map<char, std::vector<std::pair<std::string, SeatStatus>>> seatsByRow;
                
                // Nhóm các ghế theo hàng
                for (size_t i = 0; i < currentSeats.size(); ++i) {
                    std::string seatId = currentSeats[i].seat->id();
                    char row = seatId[0]; // Lấy chữ cái đầu tiên (hàng)
                    seatsByRow[row].push_back({seatId, currentSeats[i].status});
                }
                
                bool seatClicked = false;
                int rowIndex = 0;
                
                // Kiểm tra các ghế theo hàng
                for (const auto& rowPair : seatsByRow) {
                    if (seatClicked) break;
                    
                    const auto& seats = rowPair.second;
                    for (size_t i = 0; i < seats.size(); ++i) {
                        const auto& [seatId, status] = seats[i];
                        
                        // Chỉ lấy số từ ID ghế
                        int seatNumber = 0;
                        if (seatId.length() > 1) {
                            try {
                                seatNumber = std::stoi(seatId.substr(1)) - 1;
                            } catch (const std::exception&) {
                                seatNumber = i;
                            }
                        }
                          int seatX = startX + seatNumber * seatSpacing;
                        int seatY = startY + rowIndex * seatSpacing;
                        
                        // Determine seat type for proper sizing
                        SeatType type = SeatType::SINGLE; // Default
                        for (const auto& currentSeat : currentSeats) {
                            if (currentSeat.seat->id() == seatId) {
                                type = currentSeat.seat->type();
                                break;
                            }
                        }                          // Create hit detection area with uniform size for all seat types
                        sf::RectangleShape seat;
                        seat = createButton(seatX, seatY, seatSize, seatSize);
                        
                        if (isButtonClicked(seat, mousePos) && status == SeatStatus::AVAILABLE) {
                            auto it = std::find(selectedSeats.begin(), selectedSeats.end(), seatId);
                            if (it != selectedSeats.end()) {
                                selectedSeats.erase(it);
                            } else {
                                selectedSeats.push_back(seatId);
                            }
                            seatClicked = true;
                            break;
                        }                    }
                    rowIndex++;
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
            } else if (isButtonClicked(emailField, mousePos)) {                isInputtingUsername = false;
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
        }
        
        case UIState::ADMIN_PANEL: {
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
              // Scroll up button - position updated to match renderMovieManagement
            sf::RectangleShape scrollUpBtn = createStyledButton(950, 50, 50, 30, sf::Color(100, 100, 150));
            // Scroll down button - position updated to match renderMovieManagement
            sf::RectangleShape scrollDownBtn = createStyledButton(950, 680, 50, 30, sf::Color(100, 100, 150));
              if (isButtonClicked(backBtn, mousePos)) {
                currentState = UIState::ADMIN_PANEL;
            } else if (isButtonClicked(addMovieBtn, mousePos)) {
                // Reset edit fields for new movie
                clearEditingFields();
                isEditingTitle = true;
                currentState = UIState::EDIT_MOVIE;
            } else if (isButtonClicked(scrollUpBtn, mousePos) && movieListScrollOffset > 0) {
                // Scroll up when there are movies to show above
                movieListScrollOffset--;
            } else if (isButtonClicked(scrollDownBtn, mousePos) && 
                      (movieListScrollOffset + 7 < movies.size())) { // 7 is maxVisibleMovies
                // Scroll down when there are more movies below
                movieListScrollOffset++;
            } else {
                // Check for edit/delete buttons on movie cards (coordinates match renderMovieManagement)
                const int maxVisibleMovies = 7;
                int endIndex = std::min(movieListScrollOffset + maxVisibleMovies, static_cast<int>(movies.size()));
                
                for (int i = movieListScrollOffset; i < endIndex; ++i) {
                    int displayIndex = i - movieListScrollOffset; // Display position
                    
                    sf::RectangleShape editBtn = createButton(920, 155 + displayIndex * 80, 80, 30);
                    sf::RectangleShape deleteBtn = createButton(1010, 155 + displayIndex * 80, 80, 30);
                    sf::RectangleShape showtimeBtn = createButton(1100, 155 + displayIndex * 80, 80, 25);
                    
                    if (isButtonClicked(editBtn, mousePos)) {
                        editingMovieId = movies[i].id;
                        editMovieTitle = movies[i].title;
                        editMovieGenre = movies[i].genre;
                        // Lấy description từ service (nếu có)
                        auto visitor = std::make_shared<MovieViewerServiceVisitor>();
                        sessionManager->getCurrentContext()->accept(visitor);
                        auto movieService = visitor->getMovieViewerService();
                        if (movieService) {
                            auto detail = movieService->showMovieDetail(movies[i].id);
                            if (detail) {
                                editMovieDescription = detail->getDescription();
                            } else {
                                editMovieDescription = "";
                            }
                        } else {
                            editMovieDescription = "";
                        }                        // Removed Movie Duration
                        editMoviePrice = std::to_string(movies[i].rating); // Use actual movie rating
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
            // Check for clicks on pending showtimes buttons first
            if (handlePendingShowtimesClick(mousePos)) {
                // If we handled a click on the pending showtimes section
                break;
            }
            
            sf::RectangleShape backBtn = createButton(50, 50, 100, 40);
            sf::RectangleShape saveBtn = createButton(180, 492, 80, 50);            
            sf::RectangleShape titleField = createButton(400, 150, 400, 40);
            sf::RectangleShape descField = createButton(400, 220, 400, 80);
            sf::RectangleShape genreField = createButton(400, 330, 400, 40);
            sf::RectangleShape ratingField = createButton(620, 400, 180, 40);
            
            if (isButtonClicked(backBtn, mousePos)) {
                currentState = UIState::MOVIE_MANAGEMENT;
                clearPendingShowtimes();
            } else if (isButtonClicked(saveBtn, mousePos)) {
                if (editingMovieId == -1) {
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
            } else if (isButtonClicked(ratingField, mousePos)) {
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
                    resetEditingFlags();                } else if (isButtonClicked(endField, mousePos)) {
                    resetShowtimeEditingFlags();
                    isEditingEndTime = true;
                    resetEditingFlags();
                }
            }
            break;
        }
        
        case UIState::SHOWTIME_MANAGEMENT: {
            // Only handle back button and delete buttons
            sf::RectangleShape backButton = createStyledButton(50, 500, 100, 40, sf::Color(100, 100, 100));
            
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
    window.clear(sf::Color::Black);    switch (currentState) {
        case UIState::GUEST_SCREEN:
            renderGuestScreen();
            break;
        case UIState::LOGIN_SCREEN:
            renderLoginScreen();
            break;
        case UIState::REGISTER_SCREEN:
            renderRegisterScreen();
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
            break;        case UIState::BOOKING_HISTORY:
            renderBookingHistory();
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
    backgroundSprite.setPosition(0, 0);
    window.draw(backgroundSprite);

    // Calculate button dimensions and positions
    float w = buttonWidth;
    float h = buttonHeight;
    float gap = buttonGap;
    float totalWidth = (w * 3) + (gap * 2);
    float startX = (windowWidth - totalWidth) / 2;
    float y = buttonY;

    // Define button properties
    std::vector<std::string> labels = { "Login", "Register", "Browse Movies" };
    std::vector<sf::Color> colors = {
        sf::Color(0, 150, 0),      // Green for Login
        sf::Color(0, 100, 200),    // Blue for Register
        sf::Color(150, 100, 0)     // Brown for Browse Movies
    };    // Draw buttons in a centered horizontal row
    for (int i = 0; i < labels.size(); ++i) {
        float x = startX + i * (w + gap);

        float h = 60;   // Keep same height
        // Create button with hover effect
        sf::RectangleShape button = createStyledButton(x, y, w, h, colors[i]);
        window.draw(button);

        // Center text in button
        sf::Text text = createText(labels[i], 0, 0, 24);
        float textX = x + (w - text.getLocalBounds().width) / 2;
        float textY = y + (h - text.getLocalBounds().height) / 2;
        text.setPosition(textX, textY);
        text.setFillColor(sf::Color::White);
        window.draw(text);
    }
    
    // Footer message
    sf::Text footer = createText("No account needed to browse movies", 500, 580, 18);
    footer.setFillColor(sf::Color(150, 150, 150));
    window.draw(footer);
}

void SFMLUIManager::renderLoginScreen() {
    // Only draw gradient background for non-guest screens
    drawGradientBackground();

    sf::Text title = createText("Login Account", 470, 100, 40);
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
    // Only draw gradient background for non-guest screens
    drawGradientBackground();
    
    std::string welcomeMsg = "Logged in as: " + sessionManager->getCurrentRole();
    sf::Text userText = createText(welcomeMsg, 500, 200, 40);
    userText.setFillColor(sf::Color(200, 200, 255));
    window.draw(userText);
    // Use member variables for button layout
    float w = buttonWidth * 0.72f; // Main menu buttons are smaller
    float h = buttonHeight;
    float gap = buttonGap * 0.75f;
    float totalWidth = (w * 4) + (gap * 3);
    float startX = (windowWidth - totalWidth) / 2;
    float y = buttonY;
    std::vector<std::string> labels = { "Movies", "History", "Admin", "Logout" };
    std::vector<sf::Color> colors = {
        sf::Color(0, 150, 0),
        sf::Color(150, 100, 0),
        sf::Color(100, 0, 150),
        sf::Color(150, 0, 0)
    };
    // Draw buttons in a centered horizontal row

    for (int i = 0; i < labels.size(); ++i) {
        float x = startX + i * (w + gap);
        
        // Create button with hover effect
        sf::RectangleShape button = createButton(x, y, w, h);
        button.setFillColor(colors[i]);
        button.setOutlineThickness(2);
        button.setOutlineColor(sf::Color(200, 200, 200, 100));
        window.draw(button);

        // Center text in button
        sf::Text label = createText(labels[i], 0, 0, 20);
        float textX = x + (w - label.getLocalBounds().width) / 2;
        float textY = y + (h - label.getLocalBounds().height) / 2;
        label.setPosition(textX, textY);
        label.setFillColor(sf::Color::White);
        label.setStyle(sf::Text::Bold);
        window.draw(label);
    }
}


void SFMLUIManager::renderMovieList() {
    // Only draw gradient background
    drawGradientBackground();
    
    // Create a stylish title with background
    sf::RectangleShape titleBackground = createButton(400, 40, 500, 50);//
    titleBackground.setFillColor(sf::Color(80, 40, 120, 230));
    window.draw(titleBackground);
    
    sf::Text title = createText("AVAILABLE MOVIES", 500, 50, 30);
    title.setStyle(sf::Text::Bold);
    title.setFillColor(sf::Color(255, 240, 150)); // Goldish color for title
    title.setOutlineColor(sf::Color(100, 50, 150));
    title.setOutlineThickness(1.5);
    window.draw(title);
    
    // Back button with improved styling
    sf::RectangleShape backBtn = createButton(50, 50, 100, 40);
    backBtn.setFillColor(sf::Color(60, 60, 100));
    backBtn.setOutlineThickness(2);
    backBtn.setOutlineColor(sf::Color(100, 100, 150));
    window.draw(backBtn);
    
    sf::Text backText = createText("Back", 80, 60, 18);
    backText.setFillColor(sf::Color(220, 220, 255));
    window.draw(backText);
    
    // Movie count with improved styling
    std::string movieCountText = "Total: " + std::to_string(movies.size()) + " movies";
    sf::Text movieCount = createText(movieCountText, 550, 100, 18);
    movieCount.setFillColor(sf::Color(180, 180, 255));
    movieCount.setStyle(sf::Text::Italic);
    window.draw(movieCount);
      // Enhanced scroll buttons
    const int maxVisibleMovies = 6; // Adjusted for larger movie cards
    
    // Scroll up button with improved styling
    sf::RectangleShape scrollUpBtn = createStyledButton(1150, 150, 50, 40, sf::Color(80, 40, 120));
    scrollUpBtn.setOutlineThickness(2);
    scrollUpBtn.setOutlineColor(sf::Color(120, 80, 160));
    sf::Text scrollUpText = createText("^", 1168, 160, 20);
    if (movieViewScrollOffset > 0) {
        scrollUpBtn.setFillColor(sf::Color(80, 40, 120));
        scrollUpText.setFillColor(sf::Color(220, 220, 255));
    } else {
        scrollUpBtn.setFillColor(sf::Color(50, 30, 70));
        scrollUpText.setFillColor(sf::Color(150, 150, 180));
    }
    window.draw(scrollUpBtn);
    window.draw(scrollUpText);
    
    // Scroll down button with improved styling
    sf::RectangleShape scrollDownBtn = createStyledButton(1150, 600, 50, 40, sf::Color(80, 40, 120));
    scrollDownBtn.setOutlineThickness(2);
    scrollDownBtn.setOutlineColor(sf::Color(120, 80, 160));
    sf::Text scrollDownText = createText("v", 1168, 605, 20);
    if (movieViewScrollOffset + maxVisibleMovies < movies.size()) {
        scrollDownBtn.setFillColor(sf::Color(80, 40, 120));
        scrollDownText.setFillColor(sf::Color(220, 220, 255));
    } else {
        scrollDownBtn.setFillColor(sf::Color(50, 30, 70));
        scrollDownText.setFillColor(sf::Color(150, 150, 180));
    }
    window.draw(scrollDownBtn);
    window.draw(scrollDownText);
      // Movie list with scrolling and enhanced visual effect
    int endIndex = std::min(movieViewScrollOffset + maxVisibleMovies, static_cast<int>(movies.size()));
    for (int i = movieViewScrollOffset; i < endIndex; ++i) {
        int displayIndex = i - movieListScrollOffset; // Vị trí hiển thị trên màn hình
        
       // Dịch sang phải +100 pixels
int offsetX = 100;

// Create outer card
sf::RectangleShape movieCard = createButton(90 + offsetX, 145 + displayIndex * 90, 820, 80);
sf::Color cardColor(40, 40, 90, 255);
sf::Color highlightColor(70, 50, 120, 255);
movieCard.setFillColor(highlightColor);
window.draw(movieCard);

// Inner button
sf::RectangleShape movieBtn = createButton(100 + offsetX, 150 + displayIndex * 90, 800, 70);
movieBtn.setFillColor(cardColor);
movieBtn.setOutlineThickness(2.0f);
movieBtn.setOutlineColor(sf::Color(100, 100, 150, 200));
window.draw(movieBtn);

// Movie title
sf::Text movieTitle = createText(movies[i].title, 120 + offsetX, 160 + displayIndex * 90, 22);
movieTitle.setStyle(sf::Text::Bold);
movieTitle.setFillColor(sf::Color(220, 220, 255));
window.draw(movieTitle);

// Movie details
std::string movieDetails = "Genre: " + movies[i].genre + " | Rating: " + std::to_string(movies[i].rating);
sf::Text detailsText = createText(movieDetails, 120 + offsetX, 190 + displayIndex * 90, 16);
detailsText.setFillColor(sf::Color(180, 180, 220));
detailsText.setStyle(sf::Text::Italic);
window.draw(detailsText);

// View Details hint
sf::Text viewText = createText("Click to view details", 700 + offsetX, 190 + displayIndex * 90, 14);
viewText.setFillColor(sf::Color(150, 200, 255));
window.draw(viewText);

    }
}

void SFMLUIManager::renderMovieDetails() {
    // Only draw gradient background for non-guest screens
    drawGradientBackground();
    
    if (selectedMovieIndex < movies.size()) {
        const auto& movie = movies[selectedMovieIndex];
        
        // Create a stylish header with background
        sf::RectangleShape headerBg = createButton(300, 40, 600, 50);
        headerBg.setFillColor(sf::Color(80, 40, 120, 230));
        window.draw(headerBg);
        
        sf::Text title = createText("MOVIE DETAILS", 450, 50, 30);
        title.setStyle(sf::Text::Bold);
        title.setFillColor(sf::Color(255, 240, 150)); // Goldish color for title
        title.setOutlineColor(sf::Color(100, 50, 150));
        title.setOutlineThickness(1.5);
        window.draw(title);
        
        // Back button with improved styling
        sf::RectangleShape backBtn = createButton(50, 50, 100, 40);
        backBtn.setFillColor(sf::Color(60, 60, 100));
        backBtn.setOutlineThickness(2);
        backBtn.setOutlineColor(sf::Color(100, 100, 150));
        window.draw(backBtn);
        
        sf::Text backText = createText("Back", 80, 60, 18);
        backText.setFillColor(sf::Color(220, 220, 255));
        window.draw(backText);
        
        // Movie card background
        sf::RectangleShape movieCardBg = createButton(90, 120, 820, 400);
        movieCardBg.setFillColor(sf::Color(30, 30, 60, 200));
        movieCardBg.setOutlineThickness(2);
        movieCardBg.setOutlineColor(sf::Color(100, 80, 140));
        window.draw(movieCardBg);
        
        // Movie information with better styling
        sf::Text movieTitle = createText(movie.title, 100, 140, 32);
        movieTitle.setStyle(sf::Text::Bold);
        movieTitle.setFillColor(sf::Color(220, 220, 255));
        movieTitle.setOutlineThickness(1);
        movieTitle.setOutlineColor(sf::Color(100, 80, 140));
        window.draw(movieTitle);
                
        // Create genre label with tag-like design
        sf::RectangleShape genreTag = createButton(100, 190, 120 + movie.genre.length() * 8, 30);
        genreTag.setFillColor(sf::Color(60, 80, 120));
        genreTag.setOutlineThickness(1);
        genreTag.setOutlineColor(sf::Color(100, 120, 180));
        window.draw(genreTag);
        
        sf::Text genreLabel = createText("GENRE: " + movie.genre, 110, 195, 18);
        genreLabel.setFillColor(sf::Color(180, 200, 255));
        window.draw(genreLabel);
        
        // Rating with star visualization
        sf::CircleShape star(12, 5);
        star.setPosition(100, 230);
        star.setFillColor(sf::Color(255, 215, 0)); // Gold color
        window.draw(star);
        
        sf::Text ratingLabel = createText("Rating: " + std::to_string(movie.rating) + "/10", 130, 230, 20);
        ratingLabel.setFillColor(sf::Color(255, 220, 100));
        window.draw(ratingLabel);
          // Get description from MovieViewerService
        std::string description;
        auto visitor = std::make_shared<MovieViewerServiceVisitor>();
        sessionManager->getCurrentContext()->accept(visitor);
        auto movieService = visitor->getMovieViewerService();
        if (movieService) {
            auto detail = movieService->showMovieDetail(movie.id);
            if (detail) {
                description = detail->getDescription();
            }
        }
        
        // Description section with styled background
        sf::RectangleShape descriptionBg = createButton(100, 265, 800, 110);
        descriptionBg.setFillColor(sf::Color(40, 40, 70, 200));
        descriptionBg.setOutlineThickness(1);
        descriptionBg.setOutlineColor(sf::Color(100, 100, 150));
        window.draw(descriptionBg);
        
        sf::Text descTitle = createText("DESCRIPTION", 110, 270, 16);
        descTitle.setStyle(sf::Text::Bold);
        descTitle.setFillColor(sf::Color(180, 180, 255));
        window.draw(descTitle);
        
        if (!description.empty()) {
            // Wrap text if it's too long
            std::string wrappedDesc = description;
            if (wrappedDesc.length() > 100) {
                // Simple text wrapping - insert line breaks
                int pos = 0;
                int lineLength = 85; // Characters per line
                while (pos + lineLength < wrappedDesc.length()) {
                    // Find a good place to break (space)
                    int breakPos = wrappedDesc.rfind(' ', pos + lineLength);
                    if (breakPos != std::string::npos && breakPos > pos) {
                        wrappedDesc.replace(breakPos, 1, "\n");
                        pos = breakPos + 1;
                    } else {
                        pos += lineLength;
                    }
                }
            }
            
            sf::Text descText = createText(wrappedDesc, 110, 295, 16);
            descText.setFillColor(sf::Color(220, 220, 220));
            window.draw(descText);
        } else {
            sf::Text noDesc = createText("No description available", 110, 295, 16);
            noDesc.setFillColor(sf::Color(180, 180, 180));
            noDesc.setStyle(sf::Text::Italic);
            window.draw(noDesc);
        }
        
        // Showtimes section with styled header
        sf::RectangleShape showtimesBg = createButton(100, 390, 800, 130);
        showtimesBg.setFillColor(sf::Color(40, 40, 70, 200));
        showtimesBg.setOutlineThickness(1);
        showtimesBg.setOutlineColor(sf::Color(100, 100, 150));
        window.draw(showtimesBg);
        
        sf::Text showtimesTitle = createText("AVAILABLE SHOWTIMES", 110, 395, 18);
        showtimesTitle.setStyle(sf::Text::Bold);
        showtimesTitle.setFillColor(sf::Color(180, 180, 255));
        window.draw(showtimesTitle);
        
        if (currentShowTimes.empty()) {
            sf::Text noShowtimes = createText("No showtimes available for this movie", 110, 425, 16);
            noShowtimes.setFillColor(sf::Color(180, 180, 180));
            noShowtimes.setStyle(sf::Text::Italic);
            window.draw(noShowtimes);
        } else {
            for (size_t i = 0; i < currentShowTimes.size() && i < 3; ++i) {
                const auto& showtime = currentShowTimes[i];
                
                // Create a styled card for each showtime
                sf::RectangleShape showtimeCard = createButton(120, 425 + i * 30, 400, 25);
                showtimeCard.setFillColor(sf::Color(60, 60, 90));
                showtimeCard.setOutlineThickness(1);
                showtimeCard.setOutlineColor(sf::Color(100, 100, 150));
                window.draw(showtimeCard);
                
                std::string showtimeInfo = showtime.date + " | " + showtime.startTime + " - " + showtime.endTime;
                sf::Text showtimeText = createText(showtimeInfo, 130, 427 + i * 30, 16);
                showtimeText.setFillColor(sf::Color(220, 220, 250));
                window.draw(showtimeText);
                
                // Add a small clock icon
                sf::CircleShape clockIcon(8);
                clockIcon.setPosition(535, 427 + i * 30);
                clockIcon.setFillColor(sf::Color(180, 180, 220));
                window.draw(clockIcon);
            }
            
            // If there are more showtimes than we can display
            if (currentShowTimes.size() > 3) {
                sf::Text moreShowtimes = createText("+" + std::to_string(currentShowTimes.size() - 3) + " more showtimes...", 120, 485, 14);
                moreShowtimes.setFillColor(sf::Color(160, 160, 200));
                moreShowtimes.setStyle(sf::Text::Italic);
                window.draw(moreShowtimes);
            }
        }        // Call-to-action section with styled background
        sf::RectangleShape ctaBg = createButton(100, 535, 800, 100);
        ctaBg.setFillColor(sf::Color(40, 40, 70, 200));
        ctaBg.setOutlineThickness(1);
        ctaBg.setOutlineColor(sf::Color(100, 100, 150));
        window.draw(ctaBg);
        
        // Book button (only for authenticated users)
        if (!currentShowTimes.empty() && sessionManager->isUserAuthenticated()) {
            // Create a gradient-like button with two rectangles
            sf::RectangleShape bookBtnShadow = createButton(400, 560, 210, 55);
            bookBtnShadow.setFillColor(sf::Color(0, 80, 0));
            bookBtnShadow.setOutlineThickness(1);
            bookBtnShadow.setOutlineColor(sf::Color(0, 200, 0));
            window.draw(bookBtnShadow);
            
            sf::RectangleShape bookBtn = createButton(400, 555, 200, 50);
            bookBtn.setFillColor(sf::Color(0, 150, 0));
            bookBtn.setOutlineThickness(1);
            bookBtn.setOutlineColor(sf::Color(0, 200, 0));
            window.draw(bookBtn);
            
            sf::Text bookText = createText("BOOK TICKETS", 435, 570, 22);
            bookText.setStyle(sf::Text::Bold);
            bookText.setFillColor(sf::Color::White);
            window.draw(bookText);
            
            // Add a ticket icon
            sf::RectangleShape ticketIcon(sf::Vector2f(20, 15));
            ticketIcon.setPosition(405, 575);
            ticketIcon.setFillColor(sf::Color::White);
            window.draw(ticketIcon);
        } else if (!currentShowTimes.empty() && !sessionManager->isUserAuthenticated()) {
            // Message for guests with stylish design
            sf::RectangleShape messageBox = createButton(350, 555, 300, 30);
            messageBox.setFillColor(sf::Color(60, 60, 0, 180));
            messageBox.setOutlineThickness(1);
            messageBox.setOutlineColor(sf::Color(200, 200, 0));
            window.draw(messageBox);
            
            sf::Text guestMessage = createText("Please login to book tickets", 400, 560, 18);
            guestMessage.setFillColor(sf::Color(255, 255, 150));
            window.draw(guestMessage);
            
            // Attractive login button
            sf::RectangleShape loginShadow = createButton(400, 600, 210, 45);
            loginShadow.setFillColor(sf::Color(0, 60, 100));
            loginShadow.setOutlineThickness(1);
            loginShadow.setOutlineColor(sf::Color(100, 180, 255));
            window.draw(loginShadow);
            
            sf::RectangleShape loginPromptBtn = createButton(400, 595, 200, 40);
            loginPromptBtn.setFillColor(sf::Color(0, 100, 150));
            loginPromptBtn.setOutlineThickness(1);
            loginPromptBtn.setOutlineColor(sf::Color(100, 180, 255));
            window.draw(loginPromptBtn);
            
            sf::Text loginPromptText = createText("GO TO LOGIN", 440, 605, 18);
            loginPromptText.setStyle(sf::Text::Bold);
            loginPromptText.setFillColor(sf::Color(220,240,255));
            window.draw(loginPromptText);
            
            // Add a login icon
            sf::CircleShape userIcon(10);
            userIcon.setPosition(415, 605);
            userIcon.setFillColor(sf::Color(220, 240, 255));
            window.draw(userIcon);
        } else {
            // Message when no showtimes are available
            sf::Text noBookingText = createText("No showtimes available for booking", 350, 570, 20);
            noBookingText.setFillColor(sf::Color(180, 180, 180));
            noBookingText.setStyle(sf::Text::Italic);
            window.draw(noBookingText);
        }
    }
}

void SFMLUIManager::renderBookingScreen() {
    // Only draw gradient background for non-guest screens
    drawGradientBackground();
    
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
    // Only draw gradient background for non-guest screens
    drawGradientBackground();
    
    sf::Text title = createText("Select Seats", 550, 50, 35);
    window.draw(title);
    
    // Back button
    sf::RectangleShape backBtn = createButton(50, 50, 100, 40);
    backBtn.setFillColor(sf::Color(100, 100, 100));
    window.draw(backBtn);
    
    sf::Text backText = createText("Back", 80, 60, 18);
    window.draw(backText);
      // Screen indicator - improved to look more like a theater screen
    sf::RectangleShape screenBorder = createButton(290, 120, 420, 30);
    screenBorder.setFillColor(sf::Color(50, 50, 50));
    screenBorder.setOutlineThickness(2);
    screenBorder.setOutlineColor(sf::Color(100, 100, 100));
    window.draw(screenBorder);
    
    sf::RectangleShape screen = createButton(300, 125, 400, 20);
    screen.setFillColor(sf::Color(220, 220, 220));
    window.draw(screen);
    
    sf::Text screenText = createText("SCREEN", 470, 127, 16);
    screenText.setFillColor(sf::Color::Black);
    window.draw(screenText);
    
    // Seat grid
    int seatSize = 40;
    int seatSpacing = 45;
    int startX = 200;
    int startY = 200;
      // Arrange seats by row (first letter)
    std::map<char, std::vector<std::pair<std::string, SeatStatus>>> seatsByRow;
    
    // Group seats by row (first character of seat ID)
    for (size_t i = 0; i < currentSeats.size(); ++i) {
        std::string seatId = currentSeats[i].seat->id();
        char row = seatId[0]; // Get first character (e.g., 'A' from 'A1')
        seatsByRow[row].push_back({seatId, currentSeats[i].status});
    }
      // Display column numbers at the top with highlighting
    sf::RectangleShape columnHeaderBg(sf::Vector2f(520, 30));
    columnHeaderBg.setPosition(float(startX - 10), float(startY - 40));
    columnHeaderBg.setFillColor(sf::Color(40, 40, 60));
    columnHeaderBg.setOutlineThickness(1);
    columnHeaderBg.setOutlineColor(sf::Color(100, 100, 100));
    window.draw(columnHeaderBg);
    
    for (int col = 1; col <= 10; ++col) {
        // Circle background for column numbers
        sf::CircleShape colBg(10);
        colBg.setPosition(float(startX + (col-1) * seatSpacing + 10), float(startY - 35));
        colBg.setFillColor(sf::Color(60, 60, 80));
        colBg.setOutlineThickness(1);
        colBg.setOutlineColor(sf::Color(120, 120, 120));
        window.draw(colBg);
        
        sf::Text colText = createText(std::to_string(col), startX + (col-1) * seatSpacing + 15, startY - 30, 14);
        colText.setFillColor(sf::Color(220, 220, 220));
        window.draw(colText);
    }
      // Display row names
    int rowIndex = 0;
    for (const auto& rowPair : seatsByRow) {
        char rowName = rowPair.first;
        
        // Display row name on the left with a background for better visibility
        sf::CircleShape rowBg(12);
        rowBg.setPosition(float(startX - 35), float(startY + rowIndex * seatSpacing + 7));
        rowBg.setFillColor(sf::Color(60, 60, 60));
        rowBg.setOutlineThickness(1);
        rowBg.setOutlineColor(sf::Color(150, 150, 150));
        window.draw(rowBg);
        
        sf::Text rowText = createText(std::string(1, rowName), startX - 30, startY + rowIndex * seatSpacing + 10, 16);
        rowText.setFillColor(sf::Color(220, 220, 0)); // Yellow
        window.draw(rowText);
        
        // Hiển thị ghế của hàng đó
        const auto& seats = rowPair.second;
        for (size_t i = 0; i < seats.size(); ++i) {
            const auto& [seatId, status] = seats[i];
            
            // Chỉ lấy số từ ID ghế (ví dụ: '1' từ 'A1')
            int seatNumber = 0;
            if (seatId.length() > 1) {
                try {
                    seatNumber = std::stoi(seatId.substr(1)) - 1; // -1 vì số ghế bắt đầu từ 1, nhưng index bắt đầu từ 0
                } catch (const std::exception&) {
                    seatNumber = i; // Fallback nếu không thể chuyển đổi
                }
            }
            
            int seatX = startX + seatNumber * seatSpacing;
            int seatY = startY + rowIndex * seatSpacing;
            
            sf::RectangleShape seat = createButton(seatX, seatY, seatSize, seatSize);
            
            bool isSelected = std::find(selectedSeats.begin(), selectedSeats.end(), seatId) != selectedSeats.end();
            
            if (status == SeatStatus::BOOKED) {
                seat.setFillColor(sf::Color::Red);
            } else if (isSelected) {
                seat.setFillColor(sf::Color::Green);
            } else {
                // Màu khác cho ghế Single và Couple để dễ phân biệt
                SeatType type = SeatType::SINGLE; // Mặc định là Single
                
                // Tìm kiếm thông tin loại ghế từ danh sách currentSeats
                for (const auto& currentSeat : currentSeats) {
                    if (currentSeat.seat->id() == seatId) {
                        type = currentSeat.seat->type();
                        break;
                    }
                }                
                // Use different colors for different seat types, but keep the size the same
                if (type == SeatType::COUPLE) {
                    seat.setFillColor(sf::Color(100, 100, 255)); // Blue-purple for Couple
                    
                    // Add a small love heart icon or symbol
                    sf::CircleShape heart(5);
                    heart.setPosition(float(seatX + seatSize - 12), float(seatY + 5));
                    heart.setFillColor(sf::Color(255, 150, 150));
                    window.draw(heart);
                } else {
                    seat.setFillColor(sf::Color(0, 150, 255));    // Sky blue for Single
                }
            }
            
            window.draw(seat);
            
            sf::Text seatText = createText(seatId, seatX + 5, seatY + 10, 12);
            window.draw(seatText);        }
        
        rowIndex++;
    }
    // Legend with visual indicators
    // Background for the legend
    sf::RectangleShape legendBg(sf::Vector2f(700, 60));
    legendBg.setPosition(200, 640);
    legendBg.setFillColor(sf::Color(40, 40, 50, 150));
    legendBg.setOutlineThickness(1);
    legendBg.setOutlineColor(sf::Color(100, 100, 100));
    window.draw(legendBg);
    
    // Legend title
    sf::Text legendTitle = createText("Seat Legend:", 220, 650, 16);
    legendTitle.setStyle(sf::Text::Bold);
    window.draw(legendTitle);
      // Single seat indicator
    sf::RectangleShape singleSeatIcon(sf::Vector2f(20, 20));
    singleSeatIcon.setPosition(330, 650);
    singleSeatIcon.setFillColor(sf::Color(0, 150, 255));
    window.draw(singleSeatIcon);
    sf::Text singleSeatText = createText("Single", 355, 650, 16);
    window.draw(singleSeatText);
    
    // Couple seat indicator
    sf::RectangleShape coupleSeatIcon(sf::Vector2f(30, 20));
    coupleSeatIcon.setPosition(420, 650);
    coupleSeatIcon.setFillColor(sf::Color(100, 100, 255));
    window.draw(coupleSeatIcon);
    
    sf::Text coupleSeatText = createText("Couple", 455, 650, 16);
    window.draw(coupleSeatText);
    
    // Booked seat indicator
    sf::RectangleShape bookedSeatIcon(sf::Vector2f(20, 20));
    bookedSeatIcon.setPosition(520, 650);
    bookedSeatIcon.setFillColor(sf::Color::Red);
    window.draw(bookedSeatIcon);
    sf::Text bookedSeatText = createText("Booked", 545, 650, 16);
    window.draw(bookedSeatText);
    
    // Selected seat indicator
    sf::RectangleShape selectedSeatIcon(sf::Vector2f(20, 20));
    selectedSeatIcon.setPosition(620, 650);
    selectedSeatIcon.setFillColor(sf::Color::Green);
    window.draw(selectedSeatIcon);
    sf::Text selectedSeatText = createText("Selected", 645, 650, 16);
    window.draw(selectedSeatText);
    
    // Instructions text
    sf::Text instructions = createText("Click on available seats to select them for booking", 220, 675, 14);
    instructions.setFillColor(sf::Color(200, 200, 200));
    window.draw(instructions);      // Selected seats info
    // if (!selectedSeats.empty()) {
    //     // Background for selected seats info
    //     sf::RectangleShape selectedBg(sf::Vector2f(700, 40));
    //     selectedBg.setPosition(200, 710);
    //     selectedBg.setFillColor(sf::Color(60, 80, 60, 200));
    //     selectedBg.setOutlineThickness(1);
    //     selectedBg.setOutlineColor(sf::Color(0, 150, 0));
    //     window.draw(selectedBg);
        
        // // Title for selected seats
        // sf::Text selectedTitle = createText("Selected Seats:", 220, 720, 16);
        // selectedTitle.setStyle(sf::Text::Bold);
        // selectedTitle.setFillColor(sf::Color(220, 255, 220));
        // window.draw(selectedTitle);
        
        // // Display each seat as a mini icon with text
        // float xPos = 340;
        // for (const auto& seat : selectedSeats) {
        //     // Small seat icon
        //     sf::RectangleShape seatIcon(sf::Vector2f(16, 16));
        //     seatIcon.setPosition(xPos, 722);
        //     seatIcon.setFillColor(sf::Color::Green);
        //     window.draw(seatIcon);
        //       // Seat ID text
        //     sf::Text seatText = createText(seat, xPos + 20, 720, 16);
        //     seatText.setFillColor(sf::Color(220, 255, 220));
        //     window.draw(seatText);
            
        //     xPos += 60; // Space between seats
        // }

    //}

        // Display each seat as a mini icon with text in a square/grid layout (right side)
        float gridStartX = 950;  // Gần cạnh phải của cửa sổ (1280px)
        float gridStartY = 100;  // Bắt đầu phía dưới
        int seatsPerRow = 3;     // Số ghế trên mỗi hàng
        int seatGap = 60;        // Khoảng cách giữa các ghế
        int index = 0;

        for (const auto& seat : selectedSeats) {
        int row = index / seatsPerRow;
        int col = index % seatsPerRow;

        float xPos = gridStartX + col * seatGap;
        float yPos = gridStartY + row * seatGap;

        // Small seat icon
        sf::RectangleShape seatIcon(sf::Vector2f(16, 16));
        seatIcon.setPosition(xPos, yPos);
        seatIcon.setFillColor(sf::Color::Green);
        window.draw(seatIcon);

        // Seat ID text (hiển thị ngay bên phải biểu tượng ghế)
        sf::Text seatText = createText(seat, xPos + 20, yPos - 2, 16);
        seatText.setFillColor(sf::Color(220, 255, 220));
        window.draw(seatText);

        ++index;
        
        // Confirm button with enhanced styling (moved to the right side)
        sf::RectangleShape confirmBtn = createButton(1050, 650, 200, 50);
        confirmBtn.setFillColor(sf::Color(0, 150, 0));
        confirmBtn.setOutlineThickness(2);
        confirmBtn.setOutlineColor(sf::Color(100, 255, 100));
        window.draw(confirmBtn);
        
        sf::Text confirmText = createText("Confirm Booking", 1085, 665, 18);
        confirmText.setStyle(sf::Text::Bold);
        window.draw(confirmText);
    }
}

void SFMLUIManager::renderBookingHistory() {
    // Only draw gradient background for non-guest screens
    drawGradientBackground();
    
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
    // Only draw gradient background for non-guest screens
    drawGradientBackground();
    
    // Title
    sf::Text title = createText("Register New Account", 400, 100, 40);
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
    
    sf::Text title = createText("Admin Panel", 500, 100, 45);
    title.setFillColor(sf::Color::White);
    window.draw(title);
    
    // Movie Management button
    sf::RectangleShape movieMgmtBtn = createButton(530, 300, 200, 50);
    movieMgmtBtn.setFillColor(sf::Color(0, 150, 0));
    window.draw(movieMgmtBtn);
    
    sf::Text movieMgmtText = createText("Movie Management", 545, 315, 18);
    movieMgmtText.setFillColor(sf::Color::White);
    window.draw(movieMgmtText);
    
    // Back to Main Menu button
    sf::RectangleShape backBtn = createButton(530, 380, 200, 50);
    backBtn.setFillColor(sf::Color(100, 100, 100));
    window.draw(backBtn);
    
    sf::Text backText = createText("Back to Main Menu", 545, 395, 18);
    backText.setFillColor(sf::Color::White);
    window.draw(backText);
}

void SFMLUIManager::renderMovieManagement() {
    // Only draw gradient background for non-guest screens
    drawGradientBackground();
    
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
    
    // Hiển thị thông tin về tổng số phim
    std::string movieCountText = "Total: " + std::to_string(movies.size()) + " movies";
    sf::Text movieCount = createText(movieCountText, 450, 90, 16);
    movieCount.setFillColor(sf::Color(200, 200, 200));
    window.draw(movieCount);
    
    // Scroll buttons
    const int maxVisibleMovies = 7; // Số phim tối đa hiển thị đồng thời
      // Scroll up button - moved higher
    sf::RectangleShape scrollUpBtn = createStyledButton(950, 50, 50, 30, sf::Color(100, 100, 150));
    sf::Text scrollUpText = createText("^", 968, 55, 18);
    if (movieListScrollOffset > 0) {
        scrollUpBtn.setFillColor(sf::Color(100, 100, 150));
        scrollUpText.setFillColor(sf::Color::White);
    } else {
        scrollUpBtn.setFillColor(sf::Color(70, 70, 70));
        scrollUpText.setFillColor(sf::Color(150, 150, 150));
    }
    window.draw(scrollUpBtn);
    window.draw(scrollUpText);
      // Scroll down button - positioned at the bottom of the screen
    sf::RectangleShape scrollDownBtn = createStyledButton(950, 680, 50, 30, sf::Color(100, 100, 150));
    sf::Text scrollDownText = createText("v", 968, 685, 18);
    if (movieListScrollOffset + maxVisibleMovies < movies.size()) {
        scrollDownBtn.setFillColor(sf::Color(100, 100, 150));
        scrollDownText.setFillColor(sf::Color::White);
    } else {
        scrollDownBtn.setFillColor(sf::Color(70, 70, 70));
        scrollDownText.setFillColor(sf::Color(150, 150, 150));
    }
    window.draw(scrollDownBtn);
    window.draw(scrollDownText);
    
    // Movie list for management - với cơ chế cuộn
    int endIndex = std::min(movieListScrollOffset + maxVisibleMovies, static_cast<int>(movies.size()));
    for (int i = movieListScrollOffset; i < endIndex; ++i) {
        int displayIndex = i - movieListScrollOffset; // Display position
        sf::RectangleShape movieRow = createButton(100, 150 + displayIndex * 80, 800, 70);
        movieRow.setFillColor(sf::Color(50, 50, 100));
        window.draw(movieRow);
        
        std::string movieInfo = movies[i].title + " - " + movies[i].genre;
        sf::Text movieText = createText(movieInfo, 110, 170 + displayIndex * 80, 18);
        window.draw(movieText);
          // Edit button
        sf::RectangleShape editBtn = createButton(920, 155 + displayIndex * 80, 80, 30);
        editBtn.setFillColor(sf::Color(0, 100, 150));
        window.draw(editBtn);
        
        sf::Text editText = createText("Edit", 940, 165 + displayIndex * 80, 16);
        window.draw(editText);
          // Delete button
        sf::RectangleShape deleteBtn = createButton(1010, 155 + displayIndex * 80, 80, 30);
        deleteBtn.setFillColor(sf::Color(150, 0, 0));
        window.draw(deleteBtn);
        
        sf::Text deleteText = createText("Delete", 1025, 165 + displayIndex * 80, 16);
        window.draw(deleteText);
        
        // Showtimes button
        sf::RectangleShape showtimeBtn = createButton(1100, 155 + displayIndex * 80, 80, 30);
        showtimeBtn.setFillColor(sf::Color(150, 100, 0));
        window.draw(showtimeBtn);
        
        sf::Text showtimeText = createText("Times", 1118, 163 + displayIndex * 80, 16);
        window.draw(showtimeText);
    }
}

void SFMLUIManager::renderEditMovie() {
    // Only draw gradient background for non-guest screens
    drawGradientBackground();
    
    sf::Text title = createText("Edit Movie", 500, 50, 28);
    window.draw(title);
    
    // Back button
    sf::RectangleShape backBtn = createButton(50, 50, 100, 40);
    backBtn.setFillColor(sf::Color(100, 100, 100));
    window.draw(backBtn);
    
    sf::Text backText = createText("Back", 80, 60, 18);
    window.draw(backText);
      // Save button
    sf::RectangleShape saveBtn = createButton(180, 492, 80, 50);
    saveBtn.setFillColor(sf::Color(0, 150, 0));
    window.draw(saveBtn);
    
    sf::Text saveText = createText("Save", 200, 505, 18);
    window.draw(saveText);
      // Form fields
    sf::Text titleLabel = createText("Title:", 300, 160, 18);
    window.draw(titleLabel);
      sf::RectangleShape titleField = createButton(400, 150, 400, 40);
    titleField.setFillColor(isEditingTitle ? sf::Color(100, 100, 255) : sf::Color(70, 70, 70));
    window.draw(titleField);
    
    sf::Text titleText = createText(editMovieTitle, 410, 160, 18);
    window.draw(titleText);
    
    sf::Text descLabel = createText("Description:", 300, 220, 18);
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
    
    sf::Text ratingLabel = createText("Rating:", 550, 400, 18);
    window.draw(ratingLabel);
    
    sf::RectangleShape ratingField = createButton(620, 400, 180, 40);
    ratingField.setFillColor(isEditingPrice ? sf::Color(100, 100, 255) : sf::Color(70, 70, 70));
    window.draw(ratingField);
    
    sf::Text ratingText = createText(editMoviePrice, 630, 410, 18);
    window.draw(ratingText);
      // Showtime section (only show when adding new movie)
      if (editingMovieId == -1) {
        sf::Text showtimeLabel = createText("Initial Showtimes:", 300, 460, 20);
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
        
        // Add showtime button
        sf::RectangleShape addShowtimeBtn = createStyledButton(850, 505, 120, 30, sf::Color(0, 100, 200));
        window.draw(addShowtimeBtn);
        
        sf::Text addShowtimeText = createText("Add Showtime", 860, 515, 14);
        window.draw(addShowtimeText);
        
        // Render the list of pending showtimes
        renderPendingShowtimes();
    }
}

void SFMLUIManager::renderSuccessMessage() {
    // Background overlay
    sf::RectangleShape overlay;
    overlay.setSize(sf::Vector2f(windowWidth, windowHeight));
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
        // Reset scroll offsets whenever movies are loaded
        movieViewScrollOffset = 0;
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
            
            // Create detailed success message
            std::string movieTitle = (selectedMovieIndex < movies.size()) ? movies[selectedMovieIndex].title : "Movie";
            std::string showTimeInfo = currentShowTimes[selectedShowTimeIndex].date + " " + 
                                     currentShowTimes[selectedShowTimeIndex].startTime;
            std::string seatsInfo = "";
            for (size_t i = 0; i < selectedSeats.size(); i++) {
                if (i > 0) seatsInfo += ", ";
                seatsInfo += selectedSeats[i];
            }
            successMessage = std::string("Booking Successful!\n\n") +
                           "Movie: " + movieTitle + "\n" +
                           "Show Time: " + showTimeInfo + "\n" +
                           "Seats: " + seatsInfo + "\n\n" +
                           "Thank you for your booking!";
            
            selectedSeats.clear(); // Moved here
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
              // Collect showtime data from input fields and pending showtimes list
            std::vector<std::string> movieShowTimes;

            // Thêm các showtime từ danh sách chờ
            movieShowTimes = pendingShowtimes; // Đã chuẩn bị sẵn danh sách

            // Thêm showtime từ các trường nhập hiện tại (nếu có)
            if (!newShowtimeDate.empty() && !newShowtimeStartTime.empty() && !newShowtimeEndTime.empty()) {
                // Corrected format: comma-separated
                std::string showtime = newShowtimeDate + "," + newShowtimeStartTime + "," + newShowtimeEndTime;
                movieShowTimes.push_back(showtime);
            }
            
            movieManagerService->addMovie(newMovie, movieShowTimes);
            
            // Create success message showing the total number of showtimes
            std::string showtimeMessage;
            if (movieShowTimes.empty()) {
                showtimeMessage = "\nNo showtimes added. You can add them later.";
            } else {
                showtimeMessage = "\n" + std::to_string(movieShowTimes.size()) + " showtime(s) added to the movie.";
            }
            
            showSuccessMessage("New movie \'" + editMovieTitle + "\' added successfully!" + showtimeMessage); // Keep user-facing message format
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
    // Removed: isEditingDuration = false;
    isEditingPrice = false;
    // Don't clear input fields here - only reset boolean flags
    // editingMovieId = -1;  // Keep this for new movies only
    // editMovieTitle.clear();  // Don't clear text
    // editMovieDescription.clear();  // Don't clear text
    // editMovieGenre.clear();  // Don't clear text
    // Removed Movie Duration
    // editMoviePrice.clear();  // Don't clear text
}

void SFMLUIManager::clearEditingFields() {
    // New function to completely clear all editing data
    isEditingTitle = false;
    isEditingDescription = false;
    isEditingGenre = false;
    // Removed: isEditingDuration = false;
    isEditingPrice = false;
    editingMovieId = -1;
    editMovieTitle.clear();
    editMovieDescription.clear();
    editMovieGenre.clear();
    // Removed Movie Duration
    editMoviePrice.clear();
    
    // Clear showtime fields as well
    clearShowtimeFields();
    
    // Clear pending showtimes
    clearPendingShowtimes();
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
    
    sf::Text infoMessage1 = createText("* Showtimes can only be added when creating a new movie", 50, 180, 16);
    infoMessage1.setFillColor(sf::Color(200, 200, 0));
    window.draw(infoMessage1);
    
    sf::Text infoMessage2 = createText("* Use 'Add Movie' in Movie Management to create movies with showtimes", 50, 200, 16);
    infoMessage2.setFillColor(sf::Color(200, 200, 0));
    window.draw(infoMessage2);
    
    sf::Text infoMessage3 = createText("* This screen allows you to view and delete existing showtimes only", 50, 220, 16);
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
        window.draw(deleteText);    }
      // Back button - moved up from bottom of screen to be more visible
    sf::RectangleShape backButton = createStyledButton(50, 500, 100, 40, sf::Color(100, 100, 100));
    window.draw(backButton);
    
    sf::Text backText = createText("Back", 85, 515, 16);
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
    for (int i = 0; i < windowHeight; i += 10) {
        sf::RectangleShape strip;
        strip.setSize(sf::Vector2f(windowWidth, 10));
        strip.setPosition(sf::Vector2f(0, i));
        
        // Calculate gradient color
        float ratio = static_cast<float>(i) / static_cast<float>(windowHeight);
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
      // Removed duration display
    
    // Price (placeholder)
    sf::Text price = createText("Price: $10.00", x + 10, y + 140, 16);
    price.setFillColor(sf::Color(0, 255, 0)); // Green for price
    window.draw(price);
}
