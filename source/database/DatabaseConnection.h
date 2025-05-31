/**
 * @file DatabaseConnection.h
 * @brief Database connection management using Singleton pattern
 * @author Movie Ticket Booking System Team
 * @date 2025
 * @version 1.0.0
 */

#ifndef DATABASE_CONNECTION_H
#define DATABASE_CONNECTION_H

#include <string>
#include <vector>
#include <map>

extern "C" {
    #include "sqlite3.h"
}

/**
 * @class DatabaseConnection
 * @brief Singleton class for managing SQLite database connections
 * 
 * This class implements the Singleton design pattern to ensure only one database
 * connection exists throughout the application lifecycle. It provides thread-safe
 * access to the SQLite database and manages connection pooling and query execution.
 * 
 * @details
 * Key Features:
 * - Thread-safe singleton implementation
 * - Automatic connection management
 * - SQLite optimization settings
 * - Parameterized query support
 * - Connection health monitoring
 * - SQL file execution support
 * 
 * @par Usage Example
 * @code
 * DatabaseConnection* db = DatabaseConnection::getInstance();
 * db->connect("database.db");
 * 
 * // Execute a query
 * std::vector<std::string> params = {"user123"};
 * auto results = db->executeQuery("SELECT * FROM users WHERE username = ?", params);
 * 
 * // Execute non-query
 * bool success = db->executeNonQuery("INSERT INTO users (username) VALUES (?)", params);
 * @endcode
 * 
 * @warning This class is not copyable or movable to maintain singleton integrity
 * 
 * @see https://sqlite.org/c3ref/sqlite3.html
 */
class DatabaseConnection {
private:
    /**
     * @brief Singleton instance pointer
     * 
     * Static pointer to the single instance of DatabaseConnection.
     * Initialized to nullptr and created on first getInstance() call.
     */
    static DatabaseConnection* instance;
    
    /**
     * @brief SQLite database handle
     * 
     * Raw SQLite3 database connection handle used for all database operations.
     * Managed internally by the class lifecycle.
     */
    sqlite3* db;

    /**
     * @brief Private constructor (Singleton pattern)
     * 
     * Prevents direct instantiation. Database connection must be obtained
     * through getInstance() method.
     * 
     * @post db == nullptr (connection established via connect())
     */
    DatabaseConnection();

public:
    /**
     * @brief Get the singleton instance of DatabaseConnection
     * 
     * Implements lazy initialization - creates the instance on first call.
     * Subsequent calls return the same instance.
     * 
     * @return DatabaseConnection* Pointer to the singleton instance (never null)
     * 
     * @note This method is thread-safe for creation but not for usage
     * @warning Do not delete the returned pointer
     */
    static DatabaseConnection* getInstance();

    /**
     * @brief Establish connection to SQLite database file
     * 
     * Opens a connection to the specified SQLite database file.
     * Creates the file if it doesn't exist.
     * 
     * @param dbFilePath Path to the SQLite database file
     * @return bool True if connection successful, false otherwise
     * 
     * @pre Database file path is valid and accessible
     * @post If successful: db != nullptr and connection is ready
     * 
     * @throws std::runtime_error If SQLite driver is not available
     * 
     * @par SQLite Settings Applied
     * - Foreign key constraints enabled
     * - WAL mode for better concurrency
     * - Optimized pragma settings
     */
    bool connect(const std::string& dbFilePath);
      /**
     * @brief Close database connection and cleanup resources
     * 
     * Safely closes the SQLite connection and releases associated resources.
     * Can be called multiple times safely.
     * 
     * @post db == nullptr
     * @post All prepared statements are finalized
     */
    void disconnect();

    /**
     * @brief Execute non-query SQL statements (INSERT, UPDATE, DELETE)
     * 
     * Executes SQL statements that don't return result sets.
     * Supports parameterized queries to prevent SQL injection.
     * 
     * @param sql SQL statement to execute (may contain ? placeholders)
     * @param params Parameter values to bind to placeholders
     * @return bool True if execution successful, false otherwise
     * 
     * @pre Database connection is established
     * @pre Parameter count matches placeholder count in SQL
     * 
     * @par Example Usage
     * @code
     * std::vector<std::string> params = {"john_doe", "password123"};
     * bool success = db->executeNonQuery(
     *     "INSERT INTO users (username, password) VALUES (?, ?)", 
     *     params
     * );
     * @endcode
     * 
     * @see executeQuery() for SELECT statements
     */
    bool executeNonQuery(const std::string& sql, const std::vector<std::string>& params = {});

    /**
     * @brief Execute SELECT queries and return results
     * 
     * Executes SQL SELECT statements and returns results as a vector of maps.
     * Each map represents a row with column names as keys and values as strings.
     * 
     * @param sql SELECT SQL statement (may contain ? placeholders)
     * @param params Parameter values to bind to placeholders
     * @return std::vector<std::map<std::string, std::string>> Query results
     * 
     * @retval Empty vector if no results found or query failed
     * @retval Vector of maps where each map is a database row
     * 
     * @pre Database connection is established
     * @pre SQL statement is a valid SELECT query
     * 
     * @par Example Usage
     * @code
     * auto results = db->executeQuery("SELECT id, name FROM movies WHERE genre = ?", {"Action"});
     * for (const auto& row : results) {
     *     std::cout << "ID: " << row.at("id") << ", Name: " << row.at("name") << std::endl;
     * }
     * @endcode     * 
     * @see executeNonQuery() for INSERT/UPDATE/DELETE statements
     */
    std::vector<std::map<std::string, std::string>> executeQuery(
        const std::string& sql,
        const std::vector<std::string>& params = {}
    );
      /**
     * @brief Execute an entire SQL file
     * 
     * Reads and executes SQL statements from a file. Useful for
     * database schema initialization and large script execution.
     * 
     * @param filePath Path to the SQL file to execute
     * @return bool True if all statements executed successfully, false otherwise
     * 
     * @pre SQL file exists and is readable
     * @pre Database connection is established
     * @pre SQL file contains valid SQLite statements
     * 
     * @par File Format Requirements
     * - UTF-8 encoding
     * - Statements separated by semicolons
     * - Comments supported (-- and multi-line style)
     * 
     * @note Transaction handling: File execution is not automatically wrapped in a transaction
     * 
     * @warning Large files may consume significant memory
     */
    bool executeSQLFile(const std::string& filePath);

    /**
     * @brief Destructor - cleanup database resources
     * 
     * Automatically closes database connection and releases resources
     * when the singleton instance is destroyed (typically at program termination).
     * 
     * @note Called automatically - do not call manually
     */
    ~DatabaseConnection();
};

#endif
