#ifndef _MOVIEMANAGERSERVICEVISITOR_H_
#define _MOVIEMANAGERSERVICEVISITOR_H_
#include "IServiceVisitor.h"
#include "ServiceRegistry.h"
#include "IMovieManagerService.h"
#include <memory>
#include <map>

class Guest;
class User;
class Admin;

class MovieManagerServiceVisitor : public IServiceVisitor {
public:
    std::shared_ptr<IMovieManagerService> _service;
    MovieManagerServiceVisitor();
    std::shared_ptr<IMovieManagerService> getMovieManagerService();
    void service(Guest* role) override;
    void service(User* role) override;
    void service(Admin* role) override;
};

#endif