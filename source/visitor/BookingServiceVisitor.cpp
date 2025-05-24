#include "BookingServiceVisitor.h"
#include "../model/Guest.h"
#include "../model/User.h"
#include "../model/Admin.h"

BookingServiceVisitor::BookingServiceVisitor() {
    _service = nullptr;
}

std::shared_ptr<IBookingService> BookingServiceVisitor::getBookingService() {
    return _service;
}

void BookingServiceVisitor::service(std::shared_ptr<Guest> role) {
    _service = nullptr;
}

void BookingServiceVisitor::service(std::shared_ptr<User> role) {
    _service = ServiceRegistry::getSingleton<IBookingService>();
}

void BookingServiceVisitor::service(std::shared_ptr<Admin> role) {
    _service = ServiceRegistry::getSingleton<IBookingService>();
}

