#ifndef IREGISTER_SERVICE_H
#define IREGISTER_SERVICE_H

#include <memory>
#include <string>
#include "AccountInformation.h"
#include "IUserContext.h"

class IRegisterService {
public:
    virtual std::unique_ptr<IUserContext> registerAccount(const AccountInformation& info) = 0;
    virtual ~IRegisterService() = default;
};

#endif
