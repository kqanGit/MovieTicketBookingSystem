#ifndef LOGINSERVICEVISITOR_H
#define LOGINSERVICEVISITOR_H
#include "IServiceVisitor.h"
#include "../service/ILoginService.h"
#include "../core/ServiceRegistry.h"
#include <map>
#include <memory>

class Guest;
class User;
class Admin;

class LoginServiceVisitor : public IServiceVisitor {
private:
    std::shared_ptr<ILoginService> _service;
public:
    LoginServiceVisitor();
    std::shared_ptr<ILoginService> getLoginService();
    void service(std::shared_ptr<Guest> role) override;
    void service(std::shared_ptr<User> role) override;
    void service(std::shared_ptr<Admin> role) override;
};

#endif