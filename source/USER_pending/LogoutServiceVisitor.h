#ifndef _LOGINSERVICEVISITOR_H_
#define _LOGINSERVICEVISITOR_H_
#include "IServiceVisitor.h"
#include "ILogoutService.h"
#include "ServiceRegistry.h"
#include <memory>
#include <map>

class Guest;
class User;
class Admin;


class LogoutServiceVisitor : public IServiceVisitor {
public:
    std::shared_ptr<ILogoutService> _service;
    LogoutServiceVisitor();
    std::shared_ptr<ILogoutService> getLogoutService();
    void service(Guest* role) override;
    void service(User* role) override;
    void service(Admin* role) override;
};

#endif