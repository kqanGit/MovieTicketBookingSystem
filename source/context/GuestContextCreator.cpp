// GuestContextCreator.cpp
#include "GuestContextCreator.h"
#include "Guest.h"

std::unique_ptr<IUserContext> GuestContextCreator::CreateUser() {
    return std::make_unique<Guest>();
}

std::unique_ptr<IUserContext> GuestContextCreator::CreateUser(const AccountInformation& info) {
    throw std::logic_error("GuestContextCreator should not be called with AccountInformation");
}
