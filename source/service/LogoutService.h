#ifndef LOGOUT_SERVICE_H
#define LOGOUT_SERVICE_H

#include "ILogoutService.h"
#include "../context/GuestContextCreator.h"
#include <memory>

class LogoutService : public ILogoutService {
public:
    LogoutService() = default;
    std::unique_ptr<IUserContext> logout() override;
};

#endif