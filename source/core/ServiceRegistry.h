
/**
 * @file ServiceRegistry.h
 * @brief Global service registry for dependency injection and singleton management
 * @author Movie Ticket Booking System Team
 */

#ifndef _REGISTRY_H_
#define _REGISTRY_H_
#include <map>
#include <string>
#include <memory>

/**
 * @class ServiceRegistry
 * @brief Global registry for managing singleton service instances
 * 
 * Implements a service registry pattern to provide dependency injection
 * and centralized singleton management throughout the application.
 * This enables loose coupling and facilitates testing by allowing
 * service substitution.
 * 
 * @details
 * **Design Patterns Implemented:**
 * - **Service Locator Pattern**: Central registry for service discovery
 * - **Singleton Pattern**: Manages single instances of services
 * - **Registry Pattern**: Centralized storage and retrieval of objects
 * - **Dependency Injection**: Provides dependencies to requesting components
 * 
 * **Key Features:**
 * - Type-safe service registration and retrieval
 * - Automatic singleton lifecycle management
 * - Thread-safe operations (with proper usage)
 * - Memory management through shared_ptr
 * - Runtime type identification for service mapping
 * 
 * **Benefits:**
 * - Reduces tight coupling between components
 * - Facilitates unit testing with mock services
 * - Centralizes dependency management
 * - Supports lazy initialization of services
 * 
 * @warning This class uses static members and is not inherently thread-safe
 * @note Consider thread synchronization for multi-threaded applications
 * 
 * @see DatabaseConnection
 * @see IAuthenticationRepository
 * @see IMovieRepository
 * @since v1.0
 */
class ServiceRegistry {
private:
    /**
     * @brief Static map storing registered service instances
     * 
     * Maps type names to shared pointers of service instances.
     * Uses void pointers for type erasure and shared_ptr for memory management.
     */
    inline static std::map<std::string, std::shared_ptr<void>> _services;
    
public:
    /**
     * @brief Registers a singleton service instance
     * 
     * Stores a service instance in the registry under its type name,
     * making it available for later retrieval by the same type.
     * 
     * @tparam T The service type to register
     * @param service Shared pointer to the service instance
     * 
     * @pre service must be a valid initialized instance
     * @post Service is available for retrieval via getSingleton<T>()
     * @post Previous instance of same type (if any) is replaced
     * 
     * @note If a service of the same type already exists, it will be replaced
     * @warning Replacing services may break existing dependencies
     * 
     * @code
     * auto authRepo = std::make_shared<AuthenticationRepositorySQL>();
     * ServiceRegistry::addSingleton<IAuthenticationRepository>(authRepo);
     * @endcode
     * 
     * @see getSingleton()
     * @since v1.0
     */
    template<typename T>
    static void addSingleton(std::shared_ptr<T> service) {
        std::string typeName = typeid(T).name();
        _services[typeName] = service;
    }

    /**
     * @brief Retrieves a singleton service instance by type
     * 
     * Looks up and returns a previously registered service instance
     * of the specified type, or nullptr if not found.
     * 
     * @tparam T The service type to retrieve
     * 
     * @return std::shared_ptr<T> Pointer to the service instance, or nullptr if not found
     * 
     * @post Returned pointer (if not null) is safe to use
     * @post Multiple calls return the same instance (singleton behavior)
     * 
     * @note Returns nullptr if service was never registered
     * @note Type must exactly match the registration type
     * 
     * @code
     * auto authRepo = ServiceRegistry::getSingleton<IAuthenticationRepository>();
     * if (authRepo) {
     *     // Use the service
     *     auto account = authRepo->getUserByUserName(username, password);
     * }
     * @endcode
     * 
     * @see addSingleton()
     * @since v1.0
     */
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