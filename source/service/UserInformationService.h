#ifndef USER_INFORMATION_SERVICE_H
#define USER_INFORMATION_SERVICE_H

#include "IUserInformationService.h"
#include "../model/AccountInformation.h"
#include <string>

class UserInformationService : public IUserInformationService {
private:
    AccountInformation accountInfo;
public:
    UserInformationService(const AccountInformation& acc);
    void printAccountInfo() const;
    std::string getRole() const override;
};

#endif
