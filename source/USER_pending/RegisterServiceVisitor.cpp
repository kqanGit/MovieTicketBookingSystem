#include "RegisterServiceVisitor.h"

RegisterServiceVisitor::RegisterServiceVisitor() {
    _service = nullptr;
}

std::shared_ptr<IRegisterService> RegisterServiceVisitor::getRegisterService() {
    return _service;
}

void RegisterServiceVisitor::service(Guest* role) {
    _service = ServiceRegistry::getSingleton<IRegisterService>();
}

void RegisterServiceVisitor::service(User* role) {
    _service = nullptr;
}

void RegisterServiceVisitor::service(Admin* role) {
    _service = nullptr;
}
