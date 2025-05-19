#ifndef ADMIN_H
#define ADMIN_H
#include "IVisitor.h"
#include "IServiceVisitor.h"
#include "IUserContext.h"
#include <memory> 
#include <string>

class Admin : public IUserContext {
public:
    void accept(std::shared_ptr<IVisitor> service) override;
};

#endif
