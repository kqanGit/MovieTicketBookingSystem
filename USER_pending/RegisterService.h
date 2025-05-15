#ifndef REGISTER_SERVICE_H
#define REGISTER_SERVICE_H

#include "IRegisterService.h"
#include "IAuthenticationRepository.h"
#include "UserContextFactory.h"

class RegisterService : public IRegisterService {
private:
    IAuthenticationRepository* repo;
    UserContextFactory* factory;
public:
    RegisterService(IAuthenticationRepository* r, UserContextFactory* f) : repo(r), factory(f) {}

    std::unique_ptr<IUserContext> registerAccount(const AccountInformation& info) override;
};

#endif
