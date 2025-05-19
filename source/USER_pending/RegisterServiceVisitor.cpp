#include "RegisterServiceVisitor.h"

RegisterServiceVisitor::RegisterServiceVisitor() {
    _service = nullptr;
}

std::shared_ptr<IRegisterService> RegisterServiceVisitor::getRegisterService() {
    return _service;
}

void RegisterServiceVisitor::service(std::shared_ptr<Guest> role) {
    _service = ServiceRegistry::getSingleton<IRegisterService>();
}

void RegisterServiceVisitor::service(std::shared_ptr<User> role) {
    _service = nullptr;
}

void RegisterServiceVisitor::service(std::shared_ptr<Admin> role) {
    _service = nullptr;
}
