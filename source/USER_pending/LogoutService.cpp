// LogoutService.cpp
#include "LogoutService.h"

std::unique_ptr<IUserContext> LogoutService::logout() {
    GuestContextCreator guestFactory(repo);
    return guestFactory.CreateUser({});
}