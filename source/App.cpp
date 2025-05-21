// App.cpp
#include "App.h"
#include <iostream>
#include "BookingService.h"
#include "ViewBookingHistoryService.h"
#include "MovieViewerService.h"
#include "MovieManagerService.h"
#include "LoginServiceVisitor.h"
#include "LogoutServiceVisitor.h"
#include "BookingServiceVisitor.h"
#include "MovieViewerServiceVisitor.h"
#include "MovieMangerServiceVisitor.h"
#include "ViewBookingHistoryVisitor.h"

App::App() : dbConn(nullptr), authRepo(nullptr) {}

App::App(bool useMock = false) : dbConn(nullptr), authRepo(nullptr) {
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
    if (!dbConn->connect("data.db")) { // Đường dẫn đến file database
        std::cerr << "[App] Failed to connect to database.\n";
        return false;
    }

        // Nếu file chưa tồn tại hoặc mới tạo -> nên luôn chạy schema
    if (!std::filesystem::exists("data.db") || dbConn->executeQuery("SELECT name FROM sqlite_master WHERE type='table';").empty()) {
        std::cout << "[App] Running database schema setup...\n";
        if (!dbConn->executeSQLFile("../database/database.sql")) {
            std::cerr << "[App] Failed to initialize database schema.\n";
            return false;
        }
    }

    authRepo = new AuthenticationRepositorySQL(dbConn);

    ServiceRegistry::addSingleton<ILoginService>(std::make_shared<LoginService>(authRepo));
    ServiceRegistry::addSingleton<IRegisterService>(std::make_shared<RegisterService>(authRepo));
    ServiceRegistry::addSingleton<ILogoutService>(std::make_shared<LogoutService>());
    ServiceRegistry::addSingleton<IBookingService>(std::make_shared<BookingService>());
    ServiceRegistry::addSingleton<IViewBookingHistoryService>(std::make_shared<ViewBookingHistoryService>());
    ServiceRegistry::addSingleton<IMovieViewerService>(std::make_shared<MovieViewerService>());
    ServiceRegistry::addSingleton<IMovieManagerService>(std::make_shared<MovieManagerService>());

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
            if (sessionManager->logout()) {
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
            if (service) service->viewMovieList();
            else std::cout << "[App] No permission to view movies.\n";
        }

        else if (command == "book") {
            auto visitor = std::make_shared<BookingServiceVisitor>();
            sessionManager->getCurrentContext()->accept(visitor);
            auto service = visitor->getBookingService();
            if (service) service->bookTicket();
            else std::cout << "[App] Booking not allowed.\n";
        }

        else if (command == "history") {
            auto visitor = std::make_shared<ViewBookingHistoryVisitor>();
            sessionManager->getCurrentContext()->accept(visitor);
            auto service = visitor->getHistoryService();
            if (service) service->viewAllBookings();
            else std::cout << "[App] You don't have access to view history.\n";
        }

        else if (command == "manage") {
            auto visitor = std::make_shared<MovieManagerServiceVisitor>();
            sessionManager->getCurrentContext()->accept(visitor);
            auto service = visitor->getMovieManagerService();
            if (service) service->addMovie();
            else std::cout << "[App] Only admin can manage movies.\n";
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
    std::cout << "\n[App] Shutting down application...\n";
    if (dbConn) {
        dbConn->disconnect();
        dbConn = nullptr;
    }
    if (authRepo) {
        delete authRepo;
        authRepo = nullptr;
    }
    std::cout << "[App] Shutdown complete.\n";
}
