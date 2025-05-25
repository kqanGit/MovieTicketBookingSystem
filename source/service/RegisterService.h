#ifndef REGISTER_SERVICE_H
#define REGISTER_SERVICE_H

#include "IRegisterService.h"
#include "../repository/IAuthenticationRepository.h"

class RegisterService : public IRegisterService {
private:
    IAuthenticationRepository* repo;
public:
    RegisterService() : repo(nullptr) {} // Default constructor
    RegisterService(IAuthenticationRepository* r) : repo(r) {} // Parameterized constructor
    
    // Chỉ cần một phương thức duy nhất để đăng ký người dùng, đúng với SoC
    bool registerUser(const AccountInformation& info) override;
};

#endif
