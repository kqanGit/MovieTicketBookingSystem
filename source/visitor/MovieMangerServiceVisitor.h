#ifndef _MOVIEMANAGERSERVICEVISITOR_H_
#define _MOVIEMANAGERSERVICEVISITOR_H_
#include "IServiceVisitor.h"
#include "../core/ServiceRegistry.h"
#include "../service/IMovieManagerService.h"
#include <memory>
#include <map>

class Guest;
class User;
class Admin;

class MovieManagerServiceVisitor : public IServiceVisitor {
private:
    std::shared_ptr<IMovieManagerService> _service;
public:
    MovieManagerServiceVisitor();
    std::shared_ptr<IMovieManagerService> getMovieManagerService();
    void service(std::shared_ptr<Guest> role) override;
    void service(std::shared_ptr<User> role) override;
    void service(std::shared_ptr<Admin> role) override;
};

#endif