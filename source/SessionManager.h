#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include <memory>
#include <string>
#include "context/IUserContext.h"
#include "model/AccountInformation.h"
#include "context/UserContextFactory.h"
#include "context/GuestContextCreator.h"
#include "context/UserContextCreator.h"
#include "context/AdminContextCreator.h"

/**
 * @class SessionManager
 * @brief Manages user sessions and context transitions
 * 
 * Quản lý trạng thái người dùng hiện tại (Guest/User/Admin),
 * xử lý đăng nhập/đăng xuất và chuyển đổi giữa các loại người dùng
 */
class SessionManager {
private:
    // Con trỏ quản lý context hiện tại (Guest/User/Admin)
    std::unique_ptr<IUserContext> _currentUserContext;
    
    // Factory duy nhất để tạo các loại context khác nhau
    std::shared_ptr<UserContextFactory> _contextFactory;
    
    // Thông tin về tài khoản hiện tại
    AccountInformation _currentAccount;
    bool _isAuthenticated;
    
public:
    /**
     * @brief Khởi tạo SessionManager với Guest context mặc định
     */
    SessionManager();
    
    /**
     * @brief Lấy context người dùng hiện tại
     * @return Con trỏ đến context người dùng hiện tại
     */
    IUserContext* getCurrentContext() const;
    
    /**
     * @brief Lấy vai trò người dùng hiện tại
     * @return Chuỗi đại diện cho vai trò (guest, user, admin)
     */
    std::string getCurrentRole() const;
    
    /**
     * @brief Kiểm tra người dùng đã đăng nhập chưa
     * @return True nếu đã đăng nhập, false nếu chưa
     */
    bool isUserAuthenticated() const;
    
    /**
     * @brief Lấy thông tin tài khoản hiện tại
     * @return Thông tin tài khoản của người dùng hiện tại
     */
    const AccountInformation& getCurrentAccount() const;
    
    /**
     * @brief Đặt context người dùng sau khi xác thực thành công
     * @param authInfo Thông tin tài khoản từ xác thực thành công
     * @return True nếu context được đặt, false nếu không (ví dụ: đã đăng nhập rồi)
     */
    bool setUserContext(const AccountInformation& authInfo);
    
    /**
     * @brief Đăng xuất người dùng hiện tại và đặt lại về Guest context
     * @return True nếu đăng xuất thành công, false nếu không
     */
    bool logout();
};

#endif // SESSION_MANAGER_H