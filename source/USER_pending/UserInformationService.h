#ifndef USER_INFORMATION_SERVICE_H
#define USER_INFORMATION_SERVICE_H

#include "IUserInformationService.h"
#include "AccountInformation.h"
#include <string>

class UserInformationService : public IUserInformationService {
private:
    AccountInformation accountInfo;
public:
    UserInformationService(const AccountInformation& acc);
    void printAccountInfo() const;
};

#endif
