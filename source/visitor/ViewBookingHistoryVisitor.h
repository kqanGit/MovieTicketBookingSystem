#ifndef VIEW_BOOKING_HISTORY_VISITOR_H
#define VIEW_BOOKING_HISTORY_VISITOR_H

#include "IServiceVisitor.h"
#include "IViewBookingHistoryService.h"
#include "ServiceRegistry.h"
#include <memory>

class Guest;
class User;
class Admin;

class ViewBookingHistoryVisitor : public IServiceVisitor {
private:
    std::shared_ptr<IViewBookingHistoryService> _service;
public:
    ViewBookingHistoryVisitor() : _service(nullptr) {}

    std::shared_ptr<IViewBookingHistoryService> getHistoryService() {
        return _service;
    }

    void service(std::shared_ptr<Guest> role) override {
        _service = nullptr;
    }

    void service(std::shared_ptr<User> role) override {
        _service = ServiceRegistry::getSingleton<IViewBookingHistoryService>();
    }

    void service(std::shared_ptr<Admin> role) override {
        _service = ServiceRegistry::getSingleton<IViewBookingHistoryService>();
    }
};

#endif
