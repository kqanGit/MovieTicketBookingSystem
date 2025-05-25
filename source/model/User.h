#ifndef USER_H
#define USER_H
#include "../visitor/IVisitor.h"
#include "../visitor/IServiceVisitor.h"
#include "../context/IUserContext.h"
#include "../service/UserInformationService.h"
#include <memory>

class User : public IUserContext, public std::enable_shared_from_this<User> {
private:
    std::shared_ptr<UserInformationService> infoService;
public:
    User(const AccountInformation& acc);
    void accept(std::shared_ptr<IVisitor> service) override;
    std::shared_ptr<IUserInformationService> getUserInformationService() const override;
};

#endif
