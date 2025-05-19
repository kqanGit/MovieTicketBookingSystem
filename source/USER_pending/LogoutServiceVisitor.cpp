#include "LogoutServiceVisitor.h"

LogoutServiceVisitor::LogoutServiceVisitor() {
    _service = nullptr;
}

void LogoutServiceVisitor::service(Guest* role) {
    _service = nullptr;
}

void LogoutServiceVisitor::service(User* role) {
    _service = ServiceRegistry::getSingleton<ILogoutService>();
}

void LogoutServiceVisitor::service(Admin* role) {
    _service = ServiceRegistry::getSingleton<ILogoutService>();
}

std::shared_ptr<ILogoutService> LogoutServiceVisitor::getLogoutService() {
    return _service;
}

