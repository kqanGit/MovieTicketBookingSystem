// Guest.h
#ifndef GUEST_H
#define GUEST_H
#include "IVisitor.h"
#include "IUserContext.h"
#include "IServiceVisitor.h"
#include <memory> // Thêm để sử dụng unique_ptr

class Guest : public IUserContext, public std::enable_shared_from_this<Guest> {
public:
    void accept(std::shared_ptr<IVisitor> service) override;
};

#endif