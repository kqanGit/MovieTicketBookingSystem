/*
* TEST PLAN FOR MOVIEVIEWERSERVICE WITH DATABASE
* ----------------------------------------------
*
* 1. PURPOSE:
*    - Verify the functionality of the MovieViewerService class with actual database integration
*    - Ensure that movie listing, movie detail retrieval, and showtime viewing functions work correctly
*    - Validate proper interaction between the service layer and the repository layer
*
* 2. TEST CASES:
*    2.1. CanShowAllMovies:
*         - Description: Test the function to retrieve all movies from the database
*         - Expected output: List of MovieDTO objects with correct data from the database
*         - Condition: Database has been initialized with sample movie data
*
*    2.2. CanShowMovieDetail:
*         - Description: Test the function to get detailed information about a specific movie
*         - Input: Movie ID = 1 (Avengers)
*         - Expected output: A Movie object with correct details for movie with ID 1
*         - Condition: Database has the movie with ID 1
*
*    2.3. CanShowMovieDetailForNonExistentMovie:
*         - Description: Test the behavior when requesting details for a non-existent movie
*         - Input: Movie ID = 999 (does not exist)
*         - Expected output: nullptr
*         - Condition: Database does not contain a movie with ID 999
*
*    2.4. CanShowMovieShowTimes:
*         - Description: Test the function to retrieve showtimes for a specific movie
*         - Input: Movie ID = 1
*         - Expected output: List of showtime strings with correct format
*         - Condition: Database has showtimes for the movie with ID 1
*
* 3. TEST ENVIRONMENT SETUP:
*    - Each test run, the database will be used with sample data from database.sql
*    - MovieViewerService is initialized with an instance of MovieRepositorySQL for each test
*    - The database connection will be closed after completing all tests
*
* 4. ASSUMPTIONS:
*    - The database.sql file contains the necessary sample data for test cases
*    - No external interference with the database during testing
*/

#include <gtest/gtest.h>
#include "../service/MovieViewerService.h"
#include "../repository/MovieRepositorySQL.h"
#include "../model/Movie.h"
#include "../model/ShowTime.h"
#include "../repository/IMovieRepository.h"
#include "../database/DatabaseConnection.h"
#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <filesystem>

class MovieViewerServiceDBTest : public ::testing::Test {
protected:
    std::shared_ptr<IMovieRepository> repo;
    std::unique_ptr<MovieViewerService> service;
    
    void SetUp() override {
        // Initialize the repository with the test database
        repo = std::make_shared<MovieRepositorySQL>("database.db");
        // Create the service with the repository
        service = std::make_unique<MovieViewerService>(repo);
    }
    
    void TearDown() override {
        // Clean up resources
    }
};

// Test Case 2.1: Test showing all movies from the database
TEST_F(MovieViewerServiceDBTest, CanShowAllMovies) {
    // Call the function being tested
    auto movies = service->showAllMovies();
    
    // Print movie information for debugging
    std::cout << "Number of movies retrieved: " << movies.size() << std::endl;
    for (const auto& movie : movies) {
        std::cout << "Movie ID: " << movie.id << ", Title: " << movie.title 
                  << ", Genre: " << movie.genre << ", Rating: " << movie.rating << std::endl;
    }
    
    // Verify the results
    ASSERT_FALSE(movies.empty()) << "Movie list should not be empty";
    
    // Check for the existence of the "Avengers" movie
    bool foundAvengers = false;
    for (const auto& movie : movies) {
        if (movie.title == "Avengers" && movie.id == 1) {
            foundAvengers = true;
            EXPECT_EQ(movie.genre, "Action");
            break;
        }
    }
    EXPECT_TRUE(foundAvengers) << "Avengers movie should exist in the database";
}

// Test Case 2.2: Test showing movie details for an existing movie
TEST_F(MovieViewerServiceDBTest, CanShowMovieDetail) {
    // Call the function being tested
    auto movie = service->showMovieDetail(1); // Avengers
    
    // Verify the results
    ASSERT_NE(movie, nullptr) << "Movie with ID 1 should exist";
    
    // Check movie details
    EXPECT_EQ(movie->getId(), 1);
    EXPECT_EQ(movie->getTitle(), "Avengers");
    EXPECT_EQ(movie->getGenre(), "Action");
    EXPECT_FLOAT_EQ(movie->getRating(), 8.5f);
    
    std::cout << "Movie ID: " << movie->getId() << ", Title: " << movie->getTitle() 
              << ", Genre: " << movie->getGenre() << ", Rating: " << movie->getRating() << std::endl;
}

// Test Case 2.3: Test showing movie details for a non-existent movie
TEST_F(MovieViewerServiceDBTest, CanShowMovieDetailForNonExistentMovie) {
    // Redirect cout to capture output
    testing::internal::CaptureStdout();
    
    // Call the function being tested with a non-existent movie ID
    auto movie = service->showMovieDetail(999);
    
    // Get captured output
    std::string output = testing::internal::GetCapturedStdout();
    
    // Verify the results
    EXPECT_EQ(movie, nullptr) << "Non-existent movie should return nullptr";
    EXPECT_EQ(output, "Movie not found.\n") << "Error message should be printed";
}

// Test Case 2.4: Test showing movie showtimes
TEST_F(MovieViewerServiceDBTest, CanShowMovieShowTimes) {
    // Call the function being tested
    auto showtimes = service->showMovieShowTimes(1); // Showtimes for Avengers
    
    // Print showtimes for debugging

    
    // Verify the results
    ASSERT_FALSE(showtimes.empty()) << "Movie should have at least one showtime";
    
    // Check the format of the first showtime
   
}

int main(int argc, char **argv) {
    // Reset database
    
    auto db = DatabaseConnection::getInstance();

    const std::string dbPath = "database.db";
    
    // Remove the existing database file if it exists  
    if (std::filesystem::exists(dbPath)) {
        std::cout << "Removing existing database file..." << std::endl;
        std::filesystem::remove(dbPath);
    }

    if (!db->connect(dbPath)) {
        std::cerr << "Failed to connect to database" << std::endl;
        return 1;
    }   

    db->executeSQLFile("database.sql");

    // Initialize Google Test

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
