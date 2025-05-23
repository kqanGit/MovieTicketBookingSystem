#include "MovieMapper.h"
#include "Movie.h"
#include <memory>
#include "IMovie.h"
#include "MovieDTO.h"

MovieDTO MovieMapper::toDTO(const std::shared_ptr<IMovie>& movie) {
    MovieDTO dto;
    dto.id = movie->getId();
    dto.title = movie->getTitle();
    dto.genre = movie->getGenre();
    dto.rating = movie->getRating();
    return dto;
}

std::shared_ptr<IMovie> MovieMapper::toEntity(const MovieDTO& dto) {
    auto movie = std::make_shared<Movie>();
    movie->setId(dto.id);
    movie->setTitle(dto.title);
    movie->setGenre(dto.genre);
    movie->setRating(dto.rating);
    movie->setDescription(""); // Hoặc giá trị mặc định khác
    return movie;
}

void MovieMapper::updateFromDTO(std::shared_ptr<IMovie> movie, const MovieDTO& dto) {
    movie->setId(dto.id);
    movie->setTitle(dto.title);
    movie->setGenre(dto.genre);
    movie->setRating(dto.rating);
}

// void MovieMapper::updateFromDTO(std::shared_ptr<IMovie> movie, const MovieDTO& dto) {
//     // Cập nhật các trường có thể thay đổi từ DTO
//     // Lưu ý: Trong thực tế, bạn cần setter trong IMovie hoặc downcast
//     auto concreteMovie = std::dynamic_pointer_cast<Movie>(movie);
//     if (concreteMovie) {
//         concreteMovie->setTitle(dto.title);
//         concreteMovie->setGenre(dto.genre);
//         concreteMovie->setRating(dto.rating);
//     }
// }
