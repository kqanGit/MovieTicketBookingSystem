#ifndef ILOGOUT_SERVICE_H
#define ILOGOUT_SERVICE_H

#include <memory>
#include "IUserContext.h"

class ILogoutService {
public:
    virtual std::unique_ptr<IUserContext> logout() = 0;
    virtual ~ILogoutService() = default;
};

#endif
