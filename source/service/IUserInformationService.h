#ifndef IUSERINFORMATIONSERVICE_H
#define IUSERINFORMATIONSERVICE_H

#include "../model/AccountInformation.h"

class IUserInformationService {
public:
    virtual ~IUserInformationService() = default;
    virtual void printAccountInfo() const = 0;
    virtual std::string getRole() const = 0;
};

#endif // IUSERINFORMATIONSERVICE_H