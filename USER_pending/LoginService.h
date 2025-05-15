// LoginService.h
#ifndef LOGIN_SERVICE_H
#define LOGIN_SERVICE_H

#include "ILoginService.h"
#include "IAuthenticationRepository.h"
#include "UserContextFactory.h"
#include <memory>

class LoginService : public ILoginService {
private:
    IAuthenticationRepository* repo;
    UserContextFactory* userFactory;
    UserContextFactory* adminFactory;
public:
    LoginService(IAuthenticationRepository* r, UserContextFactory* uf, UserContextFactory* af)
        : repo(r), userFactory(uf), adminFactory(af) {}

    std::unique_ptr<IUserContext> login(const std::string& username, const std::string& password) override;
};

#endif