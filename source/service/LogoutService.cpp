#include "LogoutService.h"
#include "../model/Guest.h"

std::unique_ptr<IUserContext> LogoutService::logout() {
    // return std::make_unique<Guest>();
    // Hoặc nếu muốn dùng GuestContextCreator:
    GuestContextCreator creator;
    return creator.CreateUser();
}