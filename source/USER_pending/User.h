#ifndef USER_H
#define USER_H
#include "IVisitor.h"
#include "IServiceVisitor.h"
#include "IUserContext.h"
#include <memory>

class User : public IUserContext, public std::enable_shared_from_this<User> {
public:
    void accept(std::shared_ptr<IVisitor> service) override;
};

#endif
