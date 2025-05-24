#include "LogoutServiceVisitor.h"

LogoutServiceVisitor::LogoutServiceVisitor() {
    _service = nullptr;
}

void LogoutServiceVisitor::service(std::shared_ptr<Guest> role) {
    _service = nullptr;
}

void LogoutServiceVisitor::service(std::shared_ptr<User> role) {
    _service = ServiceRegistry::getSingleton<ILogoutService>();
}

void LogoutServiceVisitor::service(std::shared_ptr<Admin> role) {
    _service = ServiceRegistry::getSingleton<ILogoutService>();
}

std::shared_ptr<ILogoutService> LogoutServiceVisitor::getLogoutService() {
    return _service;
}

