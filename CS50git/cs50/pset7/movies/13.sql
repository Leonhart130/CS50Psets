SELECT name
FROM people
JOIN stars
ON people.id = stars.person_id
JOIN movies
ON movie.id = stars.movie_id
WHERE