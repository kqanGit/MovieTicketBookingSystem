// Interfaces.h
#ifndef INTERFACES_H
#define INTERFACES_H
#include <string>
#include <optional>
#include "AccountInformation.h"
class IUserInformationService { public: virtual ~IUserInformationService() = default; };
class IMovieViewerService { public: virtual ~IMovieViewerService() = default; };
class IMovieManagerService { public: virtual ~IMovieManagerService() = default; };
class IBookingService { public: virtual ~IBookingService() = default; }; 
class IViewBookingHistoryService { public: virtual ~IViewBookingHistoryService() = default; };
class ILoginService { public: virtual bool login(const std::string&, const std::string&) = 0; virtual ~ILoginService() = default; };
class ILogoutService { public: virtual void logout() = 0; virtual ~ILogoutService() = default; };
class IRegisterService { public: virtual bool registerUser(const AccountInformation&) = 0; virtual ~IRegisterService() = default; };
#endif

//??????????????????/