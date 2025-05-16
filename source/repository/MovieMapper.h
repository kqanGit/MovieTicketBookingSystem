#ifndef MOVIEMAPPER_H
#define MOVIEMAPPER_H

#include "IMovie.h"
#include "MovieDTO.h"
#include <memory>

class MovieMapper {
public:
    // Chuyển từ IMovie sang MovieDTO
    static MovieDTO toDTO(const std::shared_ptr<IMovie>& movie);
    
    // Chuyển từ MovieDTO sang Movie (tạo đối tượng mới)
    static std::shared_ptr<IMovie> toEntity(const MovieDTO& dto);
    
    // Cập nhật đối tượng Movie từ DTO
    static void updateFromDTO(std::shared_ptr<IMovie> movie, const MovieDTO& dto);
};

#endif // MOVIEMAPPER_H