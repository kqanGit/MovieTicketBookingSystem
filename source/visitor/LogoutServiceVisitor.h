#ifndef _LOGINSERVICEVISITOR_H_
#define _LOGINSERVICEVISITOR_H_
#include "IServiceVisitor.h"
#include "../service/ILogoutService.h"
#include "../core/ServiceRegistry.h"
#include <memory>
#include <map>

class Guest;
class User;
class Admin;


class LogoutServiceVisitor : public IServiceVisitor {
private:
    std::shared_ptr<ILogoutService> _service;
public:
    LogoutServiceVisitor();
    std::shared_ptr<ILogoutService> getLogoutService();
    void service(std::shared_ptr<Guest> role) override;
    void service(std::shared_ptr<User> role) override;
    void service(std::shared_ptr<Admin> role) override;
};

#endif