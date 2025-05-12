#include "DatabaseConnection.h"
#include <stdexcept>
#include <iostream>
int main() {
    DatabaseConnection* db = DatabaseConnection::getInstance();
    if (!db->connect("MovieTicketBookingSystem.db")) {
        std::cerr << "Failed to connect to database\n";
        return 1;
    }

    // Test the connection
    std::cout << "Database connection successful.\n";

    db->executeSQLFile("MovieTicketBookingSystem.sql");
}