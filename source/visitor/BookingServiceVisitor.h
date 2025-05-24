#ifndef _BOOKINGSERVICEVISITOR_H_
#define _BOOKINGSERVICEVISITOR_H_
#include "IServiceVisitor.h"
#include "../core/ServiceRegistry.h"
#include "../service/IBookingService.h"
#include <memory>
#include <map>

class Guest;
class User;
class Admin;

class BookingServiceVisitor : public IServiceVisitor {
private:
    std::shared_ptr<IBookingService> _service;
public:
    BookingServiceVisitor();
    std::shared_ptr<IBookingService> getBookingService();
    void service(std::shared_ptr<Guest> role) override;
    void service(std::shared_ptr<User> role) override;
    void service(std::shared_ptr<Admin> role) override;
};
#endif