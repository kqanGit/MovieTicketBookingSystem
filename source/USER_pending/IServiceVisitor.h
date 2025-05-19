#ifndef ISERVICEVISITOR_H
#define ISERVICEVISITOR_H
#include "IVisitor.h"
#include <memory>

class Guest;
class User;
class Admin;

class IServiceVisitor : public IVisitor {
public:
    virtual void service(std::shared_ptr<Guest> role) = 0;
    virtual void service(std::shared_ptr<User> role) = 0;
    virtual void service(std::shared_ptr<Admin> role) = 0;
};

#endif 