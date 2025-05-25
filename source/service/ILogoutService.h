#ifndef ILOGOUT_SERVICE_H
#define ILOGOUT_SERVICE_H

#include <memory>
#include "../context/IUserContext.h"

class ILogoutService {
public:
    virtual std::unique_ptr<IUserContext> logout() = 0;
    virtual ~ILogoutService() = default;
};

#endif
