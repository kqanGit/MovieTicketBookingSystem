#ifndef ILOGINSERVICE_H
#define ILOGINSERVICE_H

#include <string>
#include <memory>
#include "../model/AccountInformation.h"
#include <optional>

class ILoginService {
public:
    virtual std::optional<AccountInformation> authenticate(const std::string& username, const std::string& password) = 0;
    virtual ~ILoginService() = default;
};

#endif