/**
 * @file AuthenticationRepositorySQL.h
 * @brief SQLite implementation of authentication repository interface
 * @author Movie Ticket Booking System Team
 * @date 2025
 * @version 1.0.0
 */

#ifndef AUTH_REPO_SQL_H
#define AUTH_REPO_SQL_H

#include "IAuthenticationRepository.h"
#include "DatabaseConnection.h"

/**
 * @class AuthenticationRepositorySQL
 * @brief SQLite-based implementation of user authentication data access
 * 
 * This class provides concrete implementation of authentication repository
 * operations using SQLite database backend. It handles user credential
 * storage, validation, and account management through SQL operations.
 * 
 * @details
 * Key Features:
 * - SQLite database integration for user authentication
 * - Secure password handling and validation
 * - User account creation and management
 * - Efficient credential lookup operations
 * 
 * Design Patterns:
 * - **Repository Pattern**: Concrete implementation of IAuthenticationRepository
 * - **Dependency Injection**: Accepts DatabaseConnection for data access
 * - **Data Access Object (DAO)**: Encapsulates database access logic
 * - **Strategy Pattern**: Implements specific authentication strategy
 * 
 * Security Considerations:
 * - Password hashing before storage
 * - SQL injection prevention through parameterized queries
 * - Secure credential validation
 * - Account information protection
 * 
 * @par Usage Example
 * @code
 * DatabaseConnection* db = DatabaseConnection::getInstance();
 * auto authRepo = std::make_unique<AuthenticationRepositorySQL>(db);
 * 
 * // Add new user
 * AccountInformation newUser{0, "john_doe", "hashed_password", 
 *                           "+1-555-0123", "john@email.com", "USER"};
 * authRepo->addUser(newUser);
 * 
 * // Authenticate user
 * auto user = authRepo->getUserByUserName("john_doe", "plain_password");
 * if (user.userID > 0) {
 *     // Authentication successful
 * }
 * @endcode
 * 
 * @note Passwords should be hashed before storage for security
 * @warning Ensure database connection is valid before use
 * 
 * @see IAuthenticationRepository
 * @see DatabaseConnection
 * @see AccountInformation
 * @see ILoginService
 */
class AuthenticationRepositorySQL : public IAuthenticationRepository {
private:
    /**
     * @brief Database connection for SQL operations
     * 
     * Pointer to the database connection used for all authentication
     * related database operations. Managed externally by caller.
     * 
     * @note Connection lifetime must exceed repository lifetime
     * @warning Do not delete this pointer - it's managed externally
     */
    DatabaseConnection* dbConn;

public:
    /**
     * @brief Constructor with database connection injection
     * 
     * Initializes the repository with a database connection for
     * performing authentication operations.
     * 
     * @param db Valid database connection pointer
     * 
     * @pre db != nullptr
     * @pre Database connection is established and functional
     * @post Repository is ready for authentication operations
     * 
     * @note Database connection is not owned by this repository
     * @warning Caller must ensure database connection remains valid
     * 
     * @see DatabaseConnection
     */
    AuthenticationRepositorySQL(DatabaseConnection* db) : dbConn(db) {}

    /**
     * @brief Add a new user account to the database
     * 
     * Creates a new user account in the authentication database with
     * the provided account information. Handles user registration process.
     * 
     * @param info Complete account information for the new user
     * 
     * @pre info.userName is unique in the database
     * @pre info.password is properly hashed
     * @pre info.gmail is valid email format
     * @post User account is created in database
     * @post User can authenticate with provided credentials
     * 
     * @throws std::runtime_error If user creation fails
     * @throws std::invalid_argument If account information is invalid
     * 
     * @par Database Operations
     * - Validates username uniqueness
     * - Inserts user record into users table
     * - Assigns unique user ID
     * - Stores encrypted/hashed password
     * 
     * @note Username must be unique across the system
     * @warning Password should be hashed before calling this method
     * 
     * @see AccountInformation
     * @see getUserByUserName()
     */
    void addUser(const AccountInformation& info) override;
    
    /**
     * @brief Authenticate user and retrieve account information
     * 
     * Validates user credentials and returns complete account information
     * if authentication is successful. Used for login operations.
     * 
     * @param user Username for authentication
     * @param pass Password for authentication (plain text)
     * @return AccountInformation User account data if authentication successful
     * 
     * @retval Valid AccountInformation with userID > 0 if authentication successful
     * @retval AccountInformation with userID = 0 if authentication failed
     * 
     * @pre user is non-empty string
     * @pre pass is non-empty string
     * @post If successful: returned account contains complete user data
     * @post If failed: returned account has userID = 0
     * 
     * @par Authentication Process
     * - Look up user by username
     * - Verify password against stored hash
     * - Return complete account information if valid
     * - Return empty account if invalid credentials
     * 
     * @note Password verification handles hashing internally
     * @warning Failed authentication should not reveal whether username exists
     * 
     * @see AccountInformation
     * @see addUser()
     * @see ILoginService
     * 
     * @par Example
     * @code
     * auto result = authRepo->getUserByUserName("john_doe", "mypassword");
     * if (result.userID > 0) {
     *     std::cout << "Welcome " << result.userName << "!" << std::endl;
     *     // Proceed with authenticated session
     * } else {
     *     std::cout << "Invalid credentials" << std::endl;
     * }
     * @endcode
     */
    AccountInformation getUserByUserName(const std::string& user, const std::string& pass) override;
    
    /**
     * @brief Virtual destructor for proper inheritance cleanup
     * 
     * Ensures proper cleanup when repository is destroyed through base pointer.
     * Database connection is not cleaned up as it's externally managed.
     * 
     * @note Database connection cleanup is caller's responsibility
     */
    virtual ~AuthenticationRepositorySQL() = default;
};

#endif
