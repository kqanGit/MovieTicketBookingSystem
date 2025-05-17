#ifndef DEMO_APP_H
#define DEMO_APP_H

#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include <optional>

// Định nghĩa các interface cần thiết
struct AccountInformation {
    std::string userName;
    std::string password;
    std::string phoneNumber;
    std::string gmail;
    std::string role;
};

// Forward declarations
class IUserInformationService;
class IMovieViewerService;
class IMovieManagerService;
class IBookingService;
class IViewBookingHistoryService;
class ILoginService;
class ILogoutService;
class IRegisterService;

// Interface cho IUserContext
class IUserContext {
public:
    virtual std::string getRole() const = 0;
    virtual IUserInformationService* getUserInformationService() = 0;
    virtual IMovieViewerService* getMovieViewerService() = 0;
    virtual IMovieManagerService* getMovieManagerService() = 0;
    virtual IBookingService* getBookingService() = 0;
    virtual IViewBookingHistoryService* getViewBookingHistoryService() = 0;
    virtual ILoginService* getLoginService() = 0;
    virtual ILogoutService* getLogoutService() = 0;
    virtual IRegisterService* getRegisterService() = 0;
    virtual ~IUserContext() = default;
};

// Interface cho các service
class IUserInformationService { public: virtual ~IUserInformationService() = default; };
class IMovieViewerService { public: virtual ~IMovieViewerService() = default; };
class IMovieManagerService { public: virtual ~IMovieManagerService() = default; };
class IBookingService { public: virtual ~IBookingService() = default; }; 
class IViewBookingHistoryService { public: virtual ~IViewBookingHistoryService() = default; };

class ILoginService {
public:
    virtual std::unique_ptr<IUserContext> login(const std::string& username, const std::string& password) = 0;
    virtual ~ILoginService() = default;
};

class ILogoutService {
public:
    virtual std::unique_ptr<IUserContext> logout() = 0;
    virtual ~ILogoutService() = default;
};

class IRegisterService {
public:
    virtual std::unique_ptr<IUserContext> registerAccount(const AccountInformation& info) = 0;
    virtual ~IRegisterService() = default;
};

// Interface cho UserContextFactory
class UserContextFactory {
public:
    virtual std::unique_ptr<IUserContext> CreateUser(const AccountInformation& info) = 0;
    virtual ~UserContextFactory() = default;
};

// Giả lập IAuthenticationRepository
class IAuthenticationRepository {
public:
    virtual void addUser(const AccountInformation& account) = 0;
    virtual std::optional<AccountInformation> getUserByUserName(const std::string& username, const std::string& password) = 0;
    virtual ~IAuthenticationRepository() = default;
};

// Mock implementation của IAuthenticationRepository
class MockAuthRepository : public IAuthenticationRepository {
private:
    std::vector<AccountInformation> users;
public:
    MockAuthRepository() {
        // Thêm một số tài khoản mẫu
        AccountInformation admin;
        admin.userName = "admin";
        admin.password = "admin123";
        admin.role = "admin";
        admin.gmail = "admin@example.com";
        admin.phoneNumber = "0123456789";
        users.push_back(admin);

        AccountInformation user;
        user.userName = "user";
        user.password = "user123";
        user.role = "user";
        user.gmail = "user@example.com";
        user.phoneNumber = "0987654321";
        users.push_back(user);
    }

    void addUser(const AccountInformation& account) override {
        users.push_back(account);
        std::cout << "Đã thêm tài khoản mới: " << account.userName << std::endl;
    }

    std::optional<AccountInformation> getUserByUserName(const std::string& username, const std::string& password) override {
        for (const auto& user : users) {
            if (user.userName == username && user.password == password) {
                return user;
            }
        }
        return std::nullopt;
    }
};

// Lớp User
// Forward declarations
class BookingService;
class ViewBookingHistoryService;

class User : public IUserContext {
private:
    std::unique_ptr<ILogoutService> logoutService;
    std::unique_ptr<IBookingService> bookingService;
    std::unique_ptr<IViewBookingHistoryService> viewHistoryService;
    std::string role;
    AccountInformation account;

public:
    User(const AccountInformation& acc, IAuthenticationRepository* repo);

    std::string getRole() const override { return role; }

    // Implementation của các phương thức interface
    IUserInformationService* getUserInformationService() override { return nullptr; }
    IMovieViewerService* getMovieViewerService() override { return nullptr; }
    IMovieManagerService* getMovieManagerService() override { return nullptr; }
    IBookingService* getBookingService() override { return bookingService.get(); }
    IViewBookingHistoryService* getViewBookingHistoryService() override { return viewHistoryService.get(); }
    ILoginService* getLoginService() override { return nullptr; }
    ILogoutService* getLogoutService() override;
    IRegisterService* getRegisterService() override { return nullptr; }
};

// Lớp Admin
// Forward declaration
class MovieManagerService;

class Admin : public IUserContext {
private:
    std::unique_ptr<ILogoutService> logoutService;
    std::unique_ptr<IMovieManagerService> movieManager;
    std::string role;
    AccountInformation account;

public:
    Admin(const AccountInformation& acc, IAuthenticationRepository* repo);

    std::string getRole() const override { return role; }

    // Implementation của các phương thức interface
    IUserInformationService* getUserInformationService() override { return nullptr; }
    IMovieViewerService* getMovieViewerService() override { return nullptr; }
    IMovieManagerService* getMovieManagerService() override { return movieManager.get(); }
    IBookingService* getBookingService() override { return nullptr; }
    IViewBookingHistoryService* getViewBookingHistoryService() override { return nullptr; }
    ILoginService* getLoginService() override { return nullptr; }
    ILogoutService* getLogoutService() override;
    IRegisterService* getRegisterService() override { return nullptr; }
};

// Lớp UserContextCreator
class UserContextCreator : public UserContextFactory {
public:
    std::unique_ptr<IUserContext> CreateUser(const AccountInformation& acc) override {
        return std::make_unique<User>(acc, nullptr);
    }
};

// Lớp AdminContextCreator
class AdminContextCreator : public UserContextFactory {
public:
    std::unique_ptr<IUserContext> CreateUser(const AccountInformation& acc) override {
        return std::make_unique<Admin>(acc, nullptr);
    }
};

// Lớp Guest
class Guest : public IUserContext {
private:
    std::unique_ptr<ILoginService> loginService;
    std::unique_ptr<IRegisterService> registerService;
    std::string role;

public:
    Guest(IAuthenticationRepository* repo);

    std::string getRole() const override { return role; }

    // Implementation của các phương thức interface
    IUserInformationService* getUserInformationService() override { return nullptr; }
    IMovieViewerService* getMovieViewerService() override { return nullptr; }
    IMovieManagerService* getMovieManagerService() override { return nullptr; }
    IBookingService* getBookingService() override { return nullptr; }
    IViewBookingHistoryService* getViewBookingHistoryService() override { return nullptr; }
    ILoginService* getLoginService() override;
    ILogoutService* getLogoutService() override { return nullptr; }
    IRegisterService* getRegisterService() override;
};

// Lớp LoginService
class LoginService : public ILoginService {
private:
    IAuthenticationRepository* repo;
    std::unique_ptr<UserContextFactory> userFactory;
    std::unique_ptr<UserContextFactory> adminFactory;

public:    LoginService(IAuthenticationRepository* r, UserContextFactory* uf, UserContextFactory* af)
        : repo(r), userFactory(nullptr), adminFactory(nullptr) {
        if (uf) userFactory.reset(uf);
        if (af) adminFactory.reset(af);
    }

    std::unique_ptr<IUserContext> login(const std::string& username, const std::string& password) override {
        auto infoOpt = repo->getUserByUserName(username, password);
        if (!infoOpt.has_value()) return nullptr;
        AccountInformation info = infoOpt.value();
        if (info.role == "admin") return adminFactory->CreateUser(info);
        else return userFactory->CreateUser(info);
    }
};

// Lớp LogoutService
class LogoutService : public ILogoutService {
private:
    IAuthenticationRepository* repo;

public:
    LogoutService(IAuthenticationRepository* r) : repo(r) {}

    std::unique_ptr<IUserContext> logout() override {
        return std::make_unique<Guest>(repo);
    }
};

// Lớp RegisterService
class RegisterService : public IRegisterService {
private:
    IAuthenticationRepository* repo;
    std::unique_ptr<UserContextFactory> factory;

public:
    RegisterService(IAuthenticationRepository* r) : repo(r) {
        factory = std::unique_ptr<UserContextFactory>(new UserContextCreator());
    }

    std::unique_ptr<IUserContext> registerAccount(const AccountInformation& info) override {
        repo->addUser(info);
        return factory->CreateUser(info);
    }
};

// Implementation của các constructors

User::User(const AccountInformation& acc, IAuthenticationRepository* repo) : account(acc) {
    role = "user";
    if (repo) {
        logoutService = std::make_unique<LogoutService>(repo);
    }
    
    // Initialize the booking and history services
    bookingService = std::make_unique<BookingService>();
    viewHistoryService = std::make_unique<ViewBookingHistoryService>();
    
    std::cout << "User context created with role: " << role << std::endl;
}

ILogoutService* User::getLogoutService() {
    return logoutService.get();
}

Admin::Admin(const AccountInformation& acc, IAuthenticationRepository* repo) : account(acc) {
    role = "admin";
    if (repo) {
        logoutService = std::make_unique<LogoutService>(repo);
    }
    
    // Initialize movie manager service
    movieManager = std::make_unique<MovieManagerService>();
    
    std::cout << "Admin context created with role: " << role << std::endl;
}

ILogoutService* Admin::getLogoutService() {
    return logoutService.get();
}

Guest::Guest(IAuthenticationRepository* repo) {
    role = "guest";
    loginService.reset(new LoginService(repo, new UserContextCreator(), new AdminContextCreator()));
    registerService.reset(new RegisterService(repo));
}

ILoginService* Guest::getLoginService() {
    return loginService.get();
}

IRegisterService* Guest::getRegisterService() {
    return registerService.get();
}

// Class quản lý người dùng
class UserManager {
private:
    std::unique_ptr<IUserContext> currentUser;
    IAuthenticationRepository* authRepo;

public:
    UserManager(IAuthenticationRepository* repo) : authRepo(repo) {
        // Khởi tạo với người dùng mặc định là Guest
        currentUser = std::make_unique<Guest>(authRepo);
        std::cout << "Khởi tạo người dùng mặc định: " << currentUser->getRole() << std::endl;
    }

    IUserContext* getCurrentUser() const {
        return currentUser.get();
    }

    bool login(const std::string& username, const std::string& password) {
        // Kiểm tra xem có phải là Guest không
        if (currentUser->getRole() != "guest") {
            std::cout << "Bạn đã đăng nhập với vai trò: " << currentUser->getRole() << std::endl;
            return false;
        }

        // Lấy service đăng nhập từ context hiện tại
        auto loginService = currentUser->getLoginService();
        if (!loginService) {
            std::cout << "Không thể thực hiện đăng nhập!" << std::endl;
            return false;
        }

        try {
            // Thực hiện đăng nhập và lấy context mới
            auto newContext = loginService->login(username, password);
            if (newContext) {
                // Chuyển sang context mới (User hoặc Admin)
                currentUser = std::move(newContext);
                std::cout << "Đăng nhập thành công với vai trò: " << currentUser->getRole() << std::endl;
                return true;
            } else {
                std::cout << "Đăng nhập thất bại: Thông tin không hợp lệ" << std::endl;
                return false;
            }
        } catch (const std::exception& e) {
            std::cout << "Lỗi đăng nhập: " << e.what() << std::endl;
            return false;
        }
    }

    bool logout() {
        // Kiểm tra xem có phải là Guest không
        if (currentUser->getRole() == "guest") {
            std::cout << "Bạn chưa đăng nhập!" << std::endl;
            return false;
        }

        // Lấy service đăng xuất từ context hiện tại
        auto logoutService = currentUser->getLogoutService();
        if (!logoutService) {
            std::cout << "Không thể thực hiện đăng xuất!" << std::endl;
            return false;
        }

        // Thực hiện đăng xuất và lấy context mới (Guest)
        currentUser = logoutService->logout();
        std::cout << "Đăng xuất thành công" << std::endl;
        return true;
    }

    bool registerUser(const AccountInformation& info) {
        // Chỉ Guest mới có thể đăng ký
        if (currentUser->getRole() != "guest") {
            std::cout << "Vui lòng đăng xuất trước khi đăng ký tài khoản mới" << std::endl;
            return false;
        }

        // Lấy service đăng ký từ context hiện tại
        auto registerService = currentUser->getRegisterService();
        if (!registerService) {
            std::cout << "Không thể thực hiện đăng ký!" << std::endl;
            return false;
        }

        try {
            // Thực hiện đăng ký
            auto newContext = registerService->registerAccount(info);
            if (newContext) {
                // Chuyển sang context mới (User)
                currentUser = std::move(newContext);
                std::cout << "Đăng ký và đăng nhập thành công với tài khoản: " << info.userName << std::endl;
                return true;
            } else {
                std::cout << "Đăng ký thất bại" << std::endl;
                return false;
            }
        } catch (const std::exception& e) {
            std::cout << "Lỗi đăng ký: " << e.what() << std::endl;
            return false;
        }
    }
    
    // Demo functions for admin functionality
    void manageMovies() {
        if (currentUser->getRole() != "admin") {
            std::cout << "Chỉ admin mới có thể quản lý phim!" << std::endl;
            return;
        }
        
        auto movieManager = currentUser->getMovieManagerService();
        if (!movieManager) {
            std::cout << "Dịch vụ quản lý phim không khả dụng!" << std::endl;
            return;
        }
        
        std::cout << "--- Demo chức năng quản lý phim cho Admin ---" << std::endl;
        std::cout << "(Đây là chức năng dành cho admin, khi được gọi, các hàm sẽ in ra thông báo)" << std::endl;
        
        // Using dynamic_cast to access the specific functions of the service
        auto* managerService = dynamic_cast<IMovieManagerService*>(movieManager);
        if (managerService) {
            std::cout << "Đang hiển thị các tính năng quản lý phim" << std::endl;
        }
    }
    
    // Demo functions for user booking functionality
    void bookTicket() {
        if (currentUser->getRole() != "user" && currentUser->getRole() != "admin") {
            std::cout << "Vui lòng đăng nhập để đặt vé!" << std::endl;
            return;
        }
        
        auto bookingService = currentUser->getBookingService();
        if (!bookingService) {
            std::cout << "Dịch vụ đặt vé không khả dụng!" << std::endl;
            return;
        }
        
        std::cout << "--- Demo chức năng đặt vé cho User ---" << std::endl;
        std::cout << "(Đây là chức năng đặt vé, khi được gọi, các hàm sẽ in ra thông báo)" << std::endl;
    }
    
    // Demo functions for user history functionality
    void viewBookingHistory() {
        if (currentUser->getRole() != "user" && currentUser->getRole() != "admin") {
            std::cout << "Vui lòng đăng nhập để xem lịch sử đặt vé!" << std::endl;
            return;
        }
        
        auto historyService = currentUser->getViewBookingHistoryService();
        if (!historyService) {
            std::cout << "Dịch vụ xem lịch sử không khả dụng!" << std::endl;
            return;
        }
        
        std::cout << "--- Demo chức năng xem lịch sử đặt vé cho User ---" << std::endl;
        std::cout << "(Đây là chức năng xem lịch sử, khi được gọi, các hàm sẽ in ra thông báo)" << std::endl;
    }
};

#endif // DEMO_APP_H
