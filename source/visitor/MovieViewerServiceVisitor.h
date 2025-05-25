#ifndef _MOVIEVIEWERSERVICEVISITOR_H_
#define _MOVIEVIEWERSERVICEVISITOR_H_
#include "IServiceVisitor.h"
#include "../service/IMovieViewerService.h"
#include "../core/ServiceRegistry.h"
#include <memory>
#include <map>

class Guest;
class User;
class Admin;


class MovieViewerServiceVisitor : public IServiceVisitor {
private:
    std::shared_ptr<IMovieViewerService> _service;
public:
    MovieViewerServiceVisitor();
    std::shared_ptr<IMovieViewerService> getMovieViewerService();
    void service(std::shared_ptr<Guest> role) override;
    void service(std::shared_ptr<User> role) override;
    void service(std::shared_ptr<Admin> role) override;
};


#endif 