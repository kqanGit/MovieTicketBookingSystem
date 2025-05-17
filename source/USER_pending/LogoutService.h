// LogoutService.h
#ifndef LOGOUT_SERVICE_H
#define LOGOUT_SERVICE_H

#include "ILogoutService.h"
#include "../repository/IAuthenticationRepository.h"
#include "GuestContextCreator.h"
#include <memory>

class LogoutService : public ILogoutService {
private:
    IAuthenticationRepository* repo;
public:
    LogoutService(IAuthenticationRepository* r) : repo(r) {}
    std::unique_ptr<IUserContext> logout() override;
};

#endif