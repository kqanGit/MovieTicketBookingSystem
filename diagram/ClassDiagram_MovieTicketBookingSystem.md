```mermaid
classDiagram
    class DatabaseConnection {
        +connect()
        +disconnect()
        +getConnection()
        -connectionString
        -hEnv
        -hDbc
    }

    class IMovie {
        <<interface>>
        +int id()
        +string title()
        +string genre()
        +int duration()
        +string description()
        +float rating()
    }

    class Movie {
        -int id
        -string title
        -string genre
        -int duration
        -string description
        -float rating
        +id()
        +title()
        +genre()
        +duration()
        +description()
        +rating()
    }

    class IMovieRepository {
        <<interface>>
        +getAllMovies() : List~IMovie~
        +getMovieById(id) : IMovie
    }

    class MovieRepositorySQL {
        -dbConn : DatabaseConnection
        +getAllMovies()
        +getMovieById(id)
    }

    IMovie <|.. Movie
    IMovieRepository <|.. MovieRepositorySQL
    DatabaseConnection <.. MovieRepositorySQL : use

```