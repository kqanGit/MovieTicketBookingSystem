classDiagram
direction TB
    class IMovie {
    }

    class Movie {
	    -int id
	    -string title
	    -string genre
	    -int duration
	    -string description
	    -float rating
	    -vector~ShowTime~ showTimes
    }

    class MovieDTO {
	    +int id
	    +string title
	    +string genre
	    +float rating
    }

    class IMovieRepository {
	    +getAllMovies() vector~MovieDTO~
	    +getMovieById(id) IMovie
	    +addMovie(movie)
	    +deleteMovie(id)
        +deleteShowTimes(moiveID, showTimeID)
        +deleteAllShowTimes(moiveID)
    }

    class MovieRepositorySQL {
	    -DatabaseConnection: dbConn
	    +getAllMovies()
	    +getMovieById(id)
	    +addMovie(movie)
	    +deleteMovie(id)
        +ddShowTimes(id, starttime, endtime)
        +deleteShowTimes(moiveID, showTimeID)
        +getShowTimesByMovieId(id)
        +deleteAllShowTimes(moiveID)
    }

    class IMovieViewerService {
	    +showAllMovies()
	    +showMovieDetail(id)
        +showMovieShowTimes(id)
    }

    class IMovieManagerService {
	    +addMovie(movie)
	    +deleteMovie(id)
        +deleteShowTimes(moiveID, showTimeID)
    }

    class MovieManagerService {
	    -IMovieRepository : repo
	    +addMovie(movie)
	    +deleteMovie(id)
        +deleteShowTimes(moiveID, showTimeID)
    }

    class MovieViewerService {
	    -IMovieRepository : repo
	    +showAllMovies()
	    +showMovieDetail(id)
        +showMovieShowTimes(id)
    }

    %% class DatabaseConnection {
	%%     -connectionString
	%%     -hEnv
	%%     -hDbc
	%%     +connect()
	%%     +disconnect()
	%%     +getConnection()
    %% }

	<<interface>> IMovie
	<<interface>> IMovieRepository

    IMovie <|.. Movie
    IMovie <.. IMovieManagerService
    MovieDTO <.. IMovieRepository
    IMovieRepository <|.. MovieRepositorySQL
    %% DatabaseConnection <.. MovieRepositorySQL : use
    Movie <.. MovieDTO : converted (by MovieMapper)
    MovieManagerService ..> IMovieRepository : use
    MovieViewerService ..> IMovieRepository : use
    IMovieManagerService <|.. MovieManagerService
    IMovieViewerService <|.. MovieViewerService