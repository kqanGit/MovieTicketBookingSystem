// LoginService.h
#ifndef LOGIN_SERVICE_H
#define LOGIN_SERVICE_H

#include "ILoginService.h"
#include "../repository/IAuthenticationRepository.h"
#include "AccountInformation.h"
#include <optional>


// Sửa lại LoginService để lưu loại người dùng vào IUserContext*
class LoginService : public ILoginService {
private:
    IAuthenticationRepository* repo;
public:
    // Constructor mặc định để hỗ trợ Guest.cpp
    LoginService() : repo(nullptr) {}
    // Constructor chính
    LoginService(IAuthenticationRepository* r) : repo(r) {}

    std::optional<AccountInformation> authenticate(const std::string& username, const std::string& password);
};

#endif