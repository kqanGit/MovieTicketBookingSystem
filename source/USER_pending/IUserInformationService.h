#ifndef IUSERINFORMATIONSERVICE_H
#define IUSERINFORMATIONSERVICE_H

#include "AccountInformation.h"

class IUserInformationService {
public:
    virtual ~IUserInformationService() = default;
    virtual void printAccountInfo() const = 0;
};

#endif // IUSERINFORMATIONSERVICE_H