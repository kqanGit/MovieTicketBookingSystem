#include "LoginServiceVisitor.h"

LoginServiceVisitor::LoginServiceVisitor() {
    _service = nullptr;
}

void LoginServiceVisitor::service(std::shared_ptr<Guest> role) {
    _service = ServiceRegistry::getSingleton<ILoginService>();
}

void LoginServiceVisitor::service(std::shared_ptr<User> role) {
    _service = nullptr;
}

void LoginServiceVisitor::service(std::shared_ptr<Admin> role) {
    _service = nullptr;
}

std::shared_ptr<ILoginService> LoginServiceVisitor::getLoginService() {
    return _service;
}