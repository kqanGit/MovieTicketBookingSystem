#include "MovieMangerServiceVisitor.h"
#include "../model/Guest.h"
#include "../model/User.h"
#include "../model/Admin.h"

MovieManagerServiceVisitor::MovieManagerServiceVisitor() {
    _service = nullptr;
}

std::shared_ptr<IMovieManagerService> MovieManagerServiceVisitor::getMovieManagerService() {
    return _service;
}

void MovieManagerServiceVisitor::service(std::shared_ptr<Guest> role) {
    _service = nullptr;
}

void MovieManagerServiceVisitor::service(std::shared_ptr<User> role) {
    _service = nullptr;
}

void MovieManagerServiceVisitor::service(std::shared_ptr<Admin> role) {
    _service = ServiceRegistry::getSingleton<IMovieManagerService>();
}