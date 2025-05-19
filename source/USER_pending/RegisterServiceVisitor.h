#ifndef _REGISTERSERVICEVISITOR_H_
#define _REGISTERSERVICEVISITOR_H_
#include "IServiceVisitor.h"
#include "ServiceRegistry.h"
#include "IRegisterService.h"  
#include <memory>
#include <map>

class Guest;
class User;
class Admin;


class RegisterServiceVisitor : public IServiceVisitor{
public:
    std::shared_ptr<IRegisterService> _service;
    RegisterServiceVisitor();
    std::shared_ptr<IRegisterService> getRegisterService();
    void service(std::shared_ptr<Guest> role) override;
    void service(std::shared_ptr<User> role) override;
    void service(std::shared_ptr<Admin> role) override;
};

#endif