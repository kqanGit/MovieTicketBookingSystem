// App.cpp
#include "App.h"
#include <iostream>
#include <vector>
#include <string>
#include <limits> 
#include <memory> // Required for std::shared_ptr, std::make_shared
#include <sstream> // Required for std::istringstream
#include "BookingService.h"
#include "MovieViewerService.h"
#include "MovieManagerService.h"
#include "LoginServiceVisitor.h"
#include "LogoutServiceVisitor.h"
#include "BookingServiceVisitor.h"
#include "MovieViewerServiceVisitor.h"
#include "MovieMangerServiceVisitor.h"
#include "RegisterServiceVisitor.h"
#include "repository/BookingRepositorySQL.h" // This includes BookingRepository class
#include "repository/BookingView.h"
#include "repository/SeatView.h"
#include "model/Movie.h" 
#include "repository/MovieRepositorySQL.h"
#include "repository/IMovieRepository.h" // Added to ensure IMovieRepository is known for MovieManagerService
#include "repository/AuthenticationRepositorySQL.h" // Added for _authRepository initialization

App::App() : dbConn(nullptr) {} // Removed authRepo initialization

App::App(bool useMock = false) : dbConn(nullptr) { // Removed authRepo initialization
    if (useMock) {
        std::cout << "[App] Using mock repository (not implemented here).\n";
        // TODO: Implement mock repo if needed
    } else {
        std::cout << "[App] Using real repository.\n";
    }
}


App::~App() {
    shutdown();
}

bool App::initialize() {
    std::cout << "\n[App] Initializing application...\n";
    
    std::cout << "[Debug] Working dir: " << std::filesystem::current_path() << "\n";

    dbConn = DatabaseConnection::getInstance();
    if (!dbConn->connect("database.db")) { // Đường dẫn đến file database
        std::cerr << "[App] Failed to connect to database.\n";
        return false;
    }

    // Nếu file chưa tồn tại hoặc mới tạo -> nên luôn chạy schema
    if (!std::filesystem::exists("database.db") || dbConn->executeQuery("SELECT name FROM sqlite_master WHERE type='table';").empty()) {
        std::cout << "[App] Running database schema setup...\n";
        if (!dbConn->executeSQLFile("../database/database.sql")) {
            std::cerr << "[App] Failed to initialize database schema.\n";
            return false;
        }
    }    // Create and store shared repository instances
    _authRepository = std::make_shared<AuthenticationRepositorySQL>(dbConn);
    _movieRepository = std::make_shared<MovieRepositorySQL>("database.db"); 
    _bookingRepository = std::make_shared<BookingRepository>("database.db"); // Changed to pass string filepath

    // Register services with shared repository instances
    ServiceRegistry::addSingleton<ILoginService>(std::make_shared<LoginService>(_authRepository.get())); // Use .get()
    ServiceRegistry::addSingleton<IRegisterService>(std::make_shared<RegisterService>(_authRepository.get())); // Use .get()
    ServiceRegistry::addSingleton<ILogoutService>(std::make_shared<LogoutService>());
    ServiceRegistry::addSingleton<IBookingService>(std::make_shared<BookingService>(_bookingRepository));
    ServiceRegistry::addSingleton<IMovieViewerService>(std::make_shared<MovieViewerService>(_movieRepository));
    ServiceRegistry::addSingleton<IMovieManagerService>(std::make_shared<MovieManagerService>(_movieRepository));

    sessionManager = std::make_unique<SessionManager>();
    std::cout << "[App] Initialization complete.\n";
    return true;
}

void App::run() {
    std::string command;
    std::cout << "\n[App] Application is running...\n";

    while (true) {
        std::cout << "\n>> Command (login, register, logout, whoami, view, book, history, manage, quit): ";
        std::cin >> command;

        if (command == "login") {
            if (sessionManager->isUserAuthenticated()) {
                std::cout << "[App] Already logged in as: " << sessionManager->getCurrentRole() << "\n";
                continue;
            }

            std::string username, password;
            std::cout << "Username: ";
            std::getline(std::cin >> std::ws, username);

            std::cout << "Password: ";
            std::getline(std::cin >> std::ws, password);


            auto visitor = std::make_shared<LoginServiceVisitor>();
            sessionManager->getCurrentContext()->accept(visitor);
            auto loginService = visitor->getLoginService();

            if (!loginService) {
                std::cout << "[App] Login service not available.\n";
                continue;
            }

            auto result = loginService->authenticate(username, password);
            if (result.has_value()) {
                sessionManager->setUserContext(result.value());
                std::cout << "[App] Logged in as: " << result->role << "\n";
            } else {
                std::cout << "[App] Invalid credentials.\n";
            }
        }

        else if (command == "logout") {
            if (!sessionManager->isUserAuthenticated()) {
                std::cout << "[App] Already in Guest mode.\n";
                continue;
            }
            // Pass the ILogoutService instance from the service registry
            // auto logoutService = ServiceRegistry::getSingleton<ILogoutService>(); // Not needed, SessionManager handles it
            if (sessionManager->logout()) { // Call logout without parameters
                std::cout << "[App] Logged out to Guest.\n";
            } else {
                std::cout << "[App] Logout failed.\n";
            }
        }

        else if (command == "whoami") {
            auto info = sessionManager->getCurrentContext()->getUserInformationService();
            if (info) info->printAccountInfo();
            else std::cout << "[App] Guest - no account info.\n";
        }

        else if (command == "view") {
            auto visitor = std::make_shared<MovieViewerServiceVisitor>();
            sessionManager->getCurrentContext()->accept(visitor);
            auto service = visitor->getMovieViewerService();
            if (service) {
                std::string view_choice;
                std::cout << "View options: all, detail, showtimes (or 'back'): ";
                std::cin >> view_choice;

                if (view_choice == "all") {
                    auto movies = service->showAllMovies();
                    if (movies.empty()) {
                        std::cout << "[App] No movies available.\n";
                    } else {
                        std::cout << "[App] Available Movies:\n";
                        for (const auto& movie : movies) {
                            std::cout << "  ID: " << movie.id 
                                      << ", Title: " << movie.title 
                                      << ", Genre: " << movie.genre 
                                      << ", Rating: " << movie.rating << "\n";
                        }
                    }
                } else if (view_choice == "detail") {
                    int movie_id;
                    std::cout << "Enter movie ID to view details: ";
                    std::cin >> movie_id;
                    if (std::cin.fail()) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "[App] Invalid movie ID input.\n";
                    } else {
                        auto movieDetail = service->showMovieDetail(movie_id);
                        if (movieDetail) {
                            std::cout << "[App] Movie Details:\n";
                            std::cout << "  ID: " << movieDetail->getId() << "\n";
                            std::cout << "  Title: " << movieDetail->getTitle() << "\n";
                            std::cout << "  Genre: " << movieDetail->getGenre() << "\n";
                            std::cout << "  Description: " << movieDetail->getDescription() << "\n";
                            std::cout << "  Rating: " << movieDetail->getRating() << "\n";
                            // Assuming IMovie has getDuration(), if not, this line should be removed or adapted
                            // std::cout << "  Duration: " << movieDetail->getDuration() << " mins\n"; 
                        } else {
                            std::cout << "[App] Movie with ID " << movie_id << " not found.\n";
                        }
                    }
                } else if (view_choice == "showtimes") {
                    int movie_id_for_showtimes;
                    std::cout << "Enter movie ID to view showtimes: ";
                    std::cin >> movie_id_for_showtimes;
                    if (std::cin.fail()) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "[App] Invalid movie ID input.\n";
                    } else {
                        auto showtimes = service->showMovieShowTimes(movie_id_for_showtimes);
                        if (showtimes.empty()) {
                            std::cout << "[App] No showtimes available for movie ID " << movie_id_for_showtimes << ".\n";
                        } else {
                            std::cout << "[App] Available Showtimes for Movie ID " << movie_id_for_showtimes << ":\n";
                                // Assuming showtimes are returned as formatted strings
                                // If they are ShowTime objects, you'll need to format them here
                                // Nếu st_str là một chuỗi chứa thông tin ShowTime (ví dụ: "ShowTimeID: 123 - 2025-05-25,19:30,21:45")
                                // thì bạn cần chuyển đổi nó thành struct ShowTime để in đẹp hơn.
                                // Tuy nhiên, nếu service->showMovieShowTimes trả về std::vector<ShowTime> thay vì std::vector<std::string> thì sẽ tốt hơn.
                                // Ở đây tôi sẽ giả sử bạn chỉ có chuỗi, nên sẽ in ra như sau:

                                // Nếu bạn có struct ShowTime như sau:
                                /*
                                struct ShowTime {
                                    int id;
                                    std::string date;
                                    std::string startTime;
                                    std::string endTime;
                                };
                                */
                                // Và nếu showtimes là std::vector<ShowTime> thì nên in như sau:
                            for (const auto& st : showtimes) {
                                std::cout << "  ShowTimeID: " << st.showTimeID
                                            << " | Date: " << st.date
                                            << " | Start: " << st.startTime
                                            << " | End: " << st.endTime << "\n";
                            }
                        }
                    }
                } else if (view_choice == "back") {
                    // Do nothing, just go back to the main command prompt
                } else {
                    std::cout << "[App] Invalid view option.\n";
                }
            } else {
                std::cout << "[App] No permission to view movies.\n";
            }
        }

        else if (command == "book") {
            auto bookingVisitor = std::make_shared<BookingServiceVisitor>();
            sessionManager->getCurrentContext()->accept(bookingVisitor);
            auto bookingService = bookingVisitor->getBookingService();

            auto movieVisitor = std::make_shared<MovieViewerServiceVisitor>();
            sessionManager->getCurrentContext()->accept(movieVisitor);
            auto movieService = movieVisitor->getMovieViewerService();

            if (bookingService && movieService) {
                if (!sessionManager->isUserAuthenticated()) {
                    std::cout << "[App] You need to be logged in to book tickets. \n";
                    continue;
                }
                int userID = sessionManager->getCurrentAccount().userID;
                
                // Step 1: Get Movie ID
                int movieID;
                std::cout << "Enter Movie ID to book: ";
                std::cin >> movieID;
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "[App] Invalid Movie ID input.\n";
                    continue;
                }

                // Step 2: Display Showtimes for the movie
                auto movieDetail = movieService->showMovieDetail(movieID);
                if (!movieDetail) {
                    std::cout << "[App] Movie with ID " << movieID << " not found.\n";
                    continue;
                }
                std::cout << "\n--- Movie: " << movieDetail->getTitle() << " ---\n";
                auto showtimes = movieService->showMovieShowTimes(movieID);
                if (showtimes.empty()) {
                    std::cout << "[App] No showtimes available for this movie.\n";
                    continue;
                }
                std::cout << "Available Showtimes (ShowTimeID - Details):\n";
                // Assuming showtimes strings are already formatted. If they are ShowTime objects, adapt this.
                // For now, let's assume the string itself might contain an ID or be parsable.
                // This part might need adjustment based on how showMovieShowTimes formats its output
                // and how ShowTime IDs are actually structured/retrieved.
                // For this example, we'll just list them and ask for a ShowTimeID.
                // A better approach would be if showMovieShowTimes returned objects with IDs.
                for (const auto& st : showtimes) {
                    // If showtimes is a vector of ShowTime struct/object, print its fields
                    std::cout << "  ShowTimeID: " << st.showTimeID
                              << " - Date: " << st.date
                              << ", Start: " << st.startTime
                              << ", End: " << st.endTime << "\n";
                }


                // Step 3: Get ShowTime ID
                int showTimeID;
                std::cout << "Enter ShowTime ID: ";
                std::cin >> showTimeID;
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "[App] Invalid ShowTime ID input.\n";
                    continue;
                }

                // Step 4: Display Seat Status
                std::cout << "\n--- Seat Status for ShowTime ID: " << showTimeID << " ---\n";
                try {
                    std::vector<SeatView> seatsStatus = bookingService->viewSeatsStatus(showTimeID);
                    if (seatsStatus.empty()) {
                        std::cout << "[App] No seat information available for this showtime. It might be an invalid ShowTime ID or no seats configured.\n";
                        continue;
                    }
                    for (const auto& seatView : seatsStatus) {
                        std::cout << "  Seat ID: " << seatView.seat->id() 
                                  << " - Price: " << seatView.seat->price()
                                  << " - Status: " << (seatView.status == SeatStatus::AVAILABLE ? "Available" : "Booked") 
                                  << "\n";
                    }
                } catch (const std::exception& e) {
                    std::cerr << "[App] Error fetching seat status: " << e.what() << '\n';
                    continue;
                }
                

                // Step 5: Get seat IDs to book
                std::vector<std::string> seatsToBook;
                std::string seatID_str;
                std::cout << "\nEnter seat IDs to book (type 'done' when finished):\n";
                while (true) {
                    std::cout << "Seat ID: ";
                    std::cin >> seatID_str;
                    if (seatID_str == "done") {
                        break;
                    }
                    // Basic validation: check if the entered seat ID was listed as available
                    bool isValidSeat = false;
                    bool isAvailable = false;
                     try {
                        std::vector<SeatView> currentSeatsStatus = bookingService->viewSeatsStatus(showTimeID);
                        for (const auto& sv : currentSeatsStatus) {
                            if (sv.seat->id() == seatID_str) {
                                isValidSeat = true;
                                if (sv.status == SeatStatus::AVAILABLE) {
                                    isAvailable = true;
                                }
                                break;
                            }
                        }
                    } catch (const std::exception& e) { /* ignore */ }

                    if (!isValidSeat) {
                        std::cout << "[App] Seat ID '" << seatID_str << "' is not a valid seat for this showtime. Please try again.\n";
                    } else if (!isAvailable) {
                        std::cout << "[App] Seat ID '" << seatID_str << "' is already booked or not available. Please choose an available seat.\n";
                    }
                    else {
                        bool alreadySelected = false;
                        for(const auto& selected_seat : seatsToBook){
                            if(selected_seat == seatID_str){
                                alreadySelected = true;
                                std::cout << "[App] Seat ID '" << seatID_str << "' already selected.\n";
                                break;
                            }
                        }
                        if(!alreadySelected){
                             seatsToBook.push_back(seatID_str);
                        }
                    }
                }

                if (seatsToBook.empty()) {
                    std::cout << "[App] No seats selected for booking.\n";
                    continue;
                }

                // Step 6: Create Booking
                try {
                    bookingService->createBooking(userID, showTimeID, seatsToBook);
                    std::cout << "[App] Booking successful!\n";
                } catch (const std::exception& e) {
                    std::cerr << "[App] Booking failed: " << e.what() << '\n';
                }
            } else {
                std::cout << "[App] Booking or Movie service not available for your current role.\n";
            }
        }

        else if (command == "history") {
            auto visitor = std::make_shared<BookingServiceVisitor>();
            sessionManager->getCurrentContext()->accept(visitor);
            auto service = visitor->getBookingService();
            if (service) {
                if (!sessionManager->isUserAuthenticated()) {
                    std::cout << "[App] You need to be logged in to view booking history.\n";
                    continue;
                }
                int userID = sessionManager->getCurrentAccount().userID;
                try {
                    std::vector<BookingView> bookings = service->viewBookingHistory(userID);
                    if (bookings.empty()) {
                        std::cout << "[App] No booking history found.\n";
                    } else {
                        std::cout << "[App] Your Booking History:\n";
                        std::cout << "Booking history for userID " << userID << ": " << std::endl;
                        for (const auto& booking : bookings) {
                            std::cout << "Booking ID: " << booking.bookingID << ", Movie Title: " << booking.movieTitle
                                    << ", Date: " << booking.showTime.date << ", Start Time: " << booking.showTime.startTime
                                    << ", End Time: " << booking.showTime.endTime << ", Seats: ";
                            for (const auto& seat : booking.bookedSeats) {
                                std::cout << seat->id() << " ";
                            }
                            std::cout << ", Total Price: " << booking.totalPrice << std::endl;
                        }
                    }
                } catch (const std::exception& e) {
                    std::cerr << "[App] Failed to retrieve booking history: " << e.what() << '\n';
                }
            } else {
                std::cout << "[App] Viewing booking history is not allowed for your current role.\n";
            }
        }

        else if (command == "manage") {
            auto visitor = std::make_shared<MovieManagerServiceVisitor>();
            sessionManager->getCurrentContext()->accept(visitor);
            auto service = visitor->getMovieManagerService();
            if (service) {
                std::string manage_choice;
                std::cout << "Manage options: add_movie, delete_movie, delete_showtime (or 'back'): ";
                std::cin >> manage_choice;

                if (manage_choice == "add_movie") {
                    std::string title, genre, description;
                    float rating;
                    std::cout << "Enter movie title: ";
                    std::getline(std::cin >> std::ws, title);
                    std::cout << "Enter movie genre: ";
                    std::getline(std::cin >> std::ws, genre);
                    std::cout << "Enter movie description: ";
                    std::getline(std::cin >> std::ws, description);
                    std::cout << "Enter movie rating (0.0 - 10.0): ";
                    std::cin >> rating;
                    if (std::cin.fail()) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "[App] Invalid rating input.\n";
                    } else {
                        auto newMovie = std::make_shared<Movie>(title, genre, description, rating);
                        std::vector<std::string> showTimes;
                        std::string showTimeInput;
                        std::cout << "Enter showtimes using the format: YYYY-MM-DD,HH:MM:SS,HH:MM:SS\n";
                        std::cout << "  Where: YYYY-MM-DD = date (e.g., 2025-05-25)\n";
                        std::cout << "         First HH:MM = start time (e.g., 19:30:00)\n";
                        std::cout << "         Second HH:MM = end time (e.g., 21:45:00)\n";
                        std::cout << "Example: 2025-05-25,19:30:00,21:45:00\n";
                        std::cout << "Type 'done' when finished adding showtimes.\n\n";
                        
                        while (true) {
                            std::cout << "Showtime: ";
                            std::getline(std::cin >> std::ws, showTimeInput);
                            if (showTimeInput == "done") {
                                break;
                            }
                            
                            // Validate format
                            bool validFormat = false;
                            try {
                                // Simple format check using regex or string search
                                if (showTimeInput.find_first_of(',') != std::string::npos && 
                                    showTimeInput.find_last_of(',') != showTimeInput.find_first_of(',')) {
                                    validFormat = true;
                                }
                            } catch (...) {
                                validFormat = false;
                            }
                            
                            if (validFormat) {
                                showTimes.push_back(showTimeInput);
                            } else {
                                std::cout << "[App] Invalid format. Please use YYYY-MM-DD,HH:MM,HH:MM\n";
                            }
                        }
                        try {
                            service->addMovie(newMovie, showTimes);
                            std::cout << "[App] Movie added successfully!\n";
                        } catch (const std::exception& e) {
                            std::cerr << "[App] Failed to add movie: " << e.what() << '\n';
                        }
                    }
                } else if (manage_choice == "delete_movie") {
                    int movie_id_to_delete;
                    std::cout << "Enter ID of the movie to delete: ";
                    std::cin >> movie_id_to_delete;
                    if (std::cin.fail()) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "[App] Invalid movie ID input.\n";
                    } else {
                        try {
                            service->deleteMovie(movie_id_to_delete);
                            std::cout << "[App] Movie deleted successfully!\n";
                        } catch (const std::exception& e) {
                            std::cerr << "[App] Failed to delete movie: " << e.what() << '\n';
                        }
                    }
                } else if (manage_choice == "delete_showtime") {
                    int movie_id_for_showtime, showtime_id_to_delete;
                    std::cout << "Enter Movie ID for the showtime: ";
                    std::cin >> movie_id_for_showtime;
                    if (std::cin.fail()) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "[App] Invalid Movie ID input.\n";
                    } else {
                        std::cout << "Enter ID of the showtime to delete: ";
                        std::cin >> showtime_id_to_delete;
                        if (std::cin.fail()) {
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::cout << "[App] Invalid Showtime ID input.\n";
                        } else {
                            try {
                                service->deleteShowTime(movie_id_for_showtime, showtime_id_to_delete);
                                std::cout << "[App] Showtime deleted successfully!\n";
                            } catch (const std::exception& e) {
                                std::cerr << "[App] Failed to delete showtime: " << e.what() << '\n';
                            }
                        }
                    }
                } else if (manage_choice != "back") {
                    std::cout << "[App] Invalid manage option.\n";
                }
            } else {
                std::cout << "[App] Only admin can manage movies.\n";
            }
        }

        else if (command == "quit") {
            break;
        }

        else if (command == "register") {
            if (sessionManager->isUserAuthenticated()) {
                std::cout << "[App] Please logout before registering a new account.\n";
                continue;
            }

            std::string username, password, phone, email;

            std::cout << "Username: ";
            std::getline(std::cin >> std::ws, username);

            std::cout << "Password: ";
            std::getline(std::cin >> std::ws, password);

            std::cout << "Phone number: ";
            std::getline(std::cin >> std::ws, phone);

            std::cout << "Email: ";
            std::getline(std::cin >> std::ws, email);

            AccountInformation acc;
            acc.userName = username;
            acc.password = password;
            acc.phoneNumber = phone;
            acc.gmail = email;
            acc.role = "User"; // mặc định đăng ký là user

            auto visitor = std::make_shared<RegisterServiceVisitor>();
            sessionManager->getCurrentContext()->accept(visitor);
            auto registerService = visitor->getRegisterService();

            if (!registerService) {
                std::cout << "[App] Register service not available.\n";
                continue;
            }

            if (registerService->registerUser(acc)) {
                std::cout << "[App] Registration successful. You can now login.\n";
            } else {
                std::cout << "[App] Registration failed.\n";
            }
        }


        else {
            std::cout << "[App] Unknown command.\n";
        }
    }
}

void App::shutdown() {
    std::cout << "[App] Shutting down application...\n";
    // ServiceRegistry::clear(); // Optional: Clear service registry if needed
    if (dbConn) {
        dbConn->disconnect();
        // delete dbConn; // DatabaseConnection is a singleton, managed by itself or a smart pointer if applicable
        // dbConn = nullptr; // No need if DatabaseConnection::getInstance() handles lifetime
    }
    // delete authRepo; // Removed, _authRepository is a shared_ptr and manages its own lifetime
    // authRepo = nullptr; // Removed

    // _authRepository, _movieRepository, _bookingRepository are shared_ptrs, 
    // they will automatically release resources when they go out of scope 
    // or when their reference count drops to zero.
    // No explicit deletion is needed here for them.

    std::cout << "[App] Shutdown complete.\n";
}