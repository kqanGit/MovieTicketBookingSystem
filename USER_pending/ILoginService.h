#ifndef ILOGINSERVICE_H
#define ILOGINSERVICE_H

#include <string>
#include <memory>
#include "IUserContext.h"

class ILoginService {
public:
    virtual std::unique_ptr<IUserContext> login(const std::string& username, const std::string& password) = 0;
    virtual ~ILoginService() = default;
};

#endif