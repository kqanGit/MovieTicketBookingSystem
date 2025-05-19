#include "MovieMangerServiceVisitor.h"

MovieManagerServiceVisitor::MovieManagerServiceVisitor() {
    _service = nullptr;
}

std::shared_ptr<IMovieManagerService> MovieManagerServiceVisitor::getMovieManagerService() {
    return _service;
}

void MovieManagerServiceVisitor::service(Guest* role) {
    _service = nullptr;
}

void MovieManagerServiceVisitor::service(User* role) {
    _service = nullptr;
}

void MovieManagerServiceVisitor::service(Admin* role) {
    _service = ServiceRegistry::getSingleton<IMovieManagerService>();
}