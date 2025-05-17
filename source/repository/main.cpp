#include "DatabaseConnection.h"
#include "MovieRepositorySQL.h"
#include "MovieManagerService.h"
#include "MovieViewerService.h"
#include "Movie.h"
#include <iostream>
#include <memory>
#include <filesystem>

int main() {
    try {        // 1. Tạo kết nối database
        std::string dbPath = "MovieTicketBookingSystem.db"; // Thay đổi đường dẫn phù hợp
        // auto dbConn = DatabaseConnection::getInstance();
        
        // if (!dbConn->connect(dbPath)) {
        //     std::cerr << "Failed to connect to database" << std::endl;
        //     return 1;
        // }

        // 2. Khởi tạo repository và services
        auto movieRepo = std::make_shared<MovieRepositorySQL>(dbPath);
        
        // Service cho quản lý phim (admin)
        MovieManagerService managerService(movieRepo);
        
        // Service cho xem phim (user)
        MovieViewerService viewerService(movieRepo);

        // // 3. Thêm phim mới với các suất chiếu
        // auto newMovie = std::make_shared<Movie>(
        //     1, // ID sẽ được tự động tạo bởi database
        //     "Inception",
        //     "Sci-Fi",
        //     148, // duration in minutes
        //     "A thief who steals corporate secrets through the use of dream-sharing technology...",
        //     8.8f
        // );        std::vector<std::string> showTimes = {
        //     "2023-12-25,19:00:00,21:28:00",
        //     "2023-12-25,12:00:00,14:28:00",  // Sửa thành 12:00 đến 14:28
        //     "2023-12-26,14:00:00,16:28:00"
        // };

        // std::cout << "Adding new movie..." << std::endl;
        // managerService.addMovie(newMovie, showTimes);
        // std::cout << "Movie added successfully!" << std::endl;

        // // 4. Hiển thị tất cả phim
        // std::cout << "\nAll Movies:" << std::endl;
        // auto allMovies = viewerService.showAllMovies();
        // for (const auto& movie : allMovies) {
        //     std::cout << "ID: " << movie.id 
        //               << ", Title: " << movie.title
        //               << ", Genre: " << movie.genre
        //               << ", Rating: " << movie.rating << std::endl;
        // }

        // // 5. Xem chi tiết phim
        // std::cout << "\nMovie Details:" << std::endl;
        // int movieIdToView = 1; // ID của phim vừa thêm
        // auto movieDetail = viewerService.showMovieDetail(movieIdToView);
        // if (movieDetail) {
        //     std::cout << "Title: " << movieDetail->getTitle() << std::endl;
        //     std::cout << "Genre: " << movieDetail->getGenre() << std::endl;
        //     std::cout << "Duration: " << movieDetail->getDuration() << " minutes" << std::endl;
        //     std::cout << "Description: " << movieDetail->getDescription() << std::endl;
        //     std::cout << "Rating: " << movieDetail->getRating() << std::endl;
        // } else {
        //     std::cout << "Movie not found!" << std::endl;
        // }

        // 6. Xóa phim (ví dụ)
        // std::cout << "\nDeleting movie..." << std::endl;
        // managerService.deleteMovie(movieIdToView);
        // std::cout << "Movie deleted successfully!" << std::endl;

        // 7. Xem suất chiếu của phim
        std::cout << "\nShowtimes for Movie ID " << std::endl;
        std::vector <std::string> showTimes = viewerService.showMovieShowTimes(1);
        for(auto time: showTimes){
            std::cout << time << std::endl;
        }

        // 8. Xóa tất cả suất chiếu của phim
        std::cout << "\nDeleting showtimes for Movie ID " << std::endl;
        managerService.deleteShowTime(1, 6);
        std::cout << "Showtimes deleted successfully!" << std::endl;

        std::cout << "\nShowtimes for Movie ID " << std::endl;
        std::vector <std::string> afterDelete = viewerService.showMovieShowTimes(1);
        for(auto time: afterDelete ){
            std::cout << time << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}