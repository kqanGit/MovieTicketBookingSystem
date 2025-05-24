#ifndef IREGISTER_SERVICE_H
#define IREGISTER_SERVICE_H

#include <string>
#include "AccountInformation.h"

class IRegisterService {
public:
    // Phương thức đăng ký user mới, chỉ trả về kết quả thành công/thất bại
    // không tạo hoặc trả về context (tuân theo separation of concerns)
    virtual bool registerUser(const AccountInformation& info) = 0;
    virtual ~IRegisterService() = default;
};

#endif
