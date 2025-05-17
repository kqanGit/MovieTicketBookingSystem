#ifndef USER_CONTEXT_FACTORY_H
#define USER_CONTEXT_FACTORY_H

#include <memory>
#include "IUserContext.h"

class UserContextFactory {
public:
    // Mặc định: trả về nullptr, chỉ override ở class phù hợp
    virtual std::unique_ptr<IUserContext> CreateUser() { return nullptr; }
    // virtual std::unique_ptr<IUserContext> CreateUser(const AccountInformation& info) { return nullptr; }
    virtual ~UserContextFactory() = default;
};
#endif
