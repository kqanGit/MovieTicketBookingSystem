#include "MovieViewerServiceVisitor.h"

MovieViewerServiceVisitor::MovieViewerServiceVisitor() {
    _service = nullptr;
}

void MovieViewerServiceVisitor::service(std::shared_ptr<Guest> role) {
    _service = ServiceRegistry::getSingleton<IMovieViewerService>();
}

void MovieViewerServiceVisitor::service(std::shared_ptr<User> role) {
    _service = ServiceRegistry::getSingleton<IMovieViewerService>();
}

void MovieViewerServiceVisitor::service(std::shared_ptr<Admin> role) {
    _service = ServiceRegistry::getSingleton<IMovieViewerService>();
}

std::shared_ptr<IMovieViewerService> MovieViewerServiceVisitor::getMovieViewerService() {
    return _service;
}