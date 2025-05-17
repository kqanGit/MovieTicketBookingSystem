#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include <memory>
#include "USER_pending/IUserContext.h"
#include "USER_pending/Guest.h"
#include "repository/IAuthenticationRepository.h"

// Class UserManager quản lý trạng thái người dùng hiện tại
class UserManager {
private:
    std::unique_ptr<IUserContext> currentUser; // Lưu context người dùng hiện tại
    IAuthenticationRepository* authRepo;       // Repository xác thực

public:
    // Constructor
    UserManager(IAuthenticationRepository* repo);

    // Destructor
    ~UserManager() = default;

    // Lấy thông tin người dùng hiện tại
    IUserContext* getCurrentUser() const;

    // Thực hiện đăng nhập
    bool login(const std::string& username, const std::string& password);

    // Thực hiện đăng xuất
    bool logout();

    // Thực hiện đăng ký
    bool registerUser(const AccountInformation& info);
};

#endif // USER_MANAGER_H
