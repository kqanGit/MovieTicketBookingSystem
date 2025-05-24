#ifndef ADMIN_H
#define ADMIN_H
#include "IVisitor.h"
#include "IServiceVisitor.h"
#include "IUserContext.h"
#include "UserInformationService.h"
#include <memory> 
#include <string>

class Admin : public IUserContext {
private:
    std::shared_ptr<UserInformationService> infoService;
public:
    Admin(const AccountInformation& acc);
    void accept(std::shared_ptr<IVisitor> service) override;
    std::shared_ptr<IUserInformationService> getUserInformationService() const override;
};

#endif
