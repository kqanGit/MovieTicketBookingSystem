#ifndef _REGISTRY_H_
#define _REGISTRY_H_
#include <map>
#include <string>
#include <memory>
class ServiceRegistry {
private:
    inline static std::map<std::string, std::shared_ptr<void>> _services;
public:
    template<typename T>
    static void addSingleton(std::shared_ptr<T> service) {
        std::string typeName = typeid(T).name();
        _services[typeName] = service;
    }

    template<typename T>
    static std::shared_ptr<T> getSingleton() {
        std::string typeName = typeid(T).name();
        auto it = _services.find(typeName);
        if (it != _services.end()) {
            return std::static_pointer_cast<T>(it->second);
        }
        return nullptr;
    }
};

#endif