SELECT DISTINCT(name)
FROM people
INNER JOIN directors
ON people.id = person_id
INNER JOIN movies
ON directors.movie_id = movies.id
INNER JOIN ratings
on ratings.movie_id = movies.id
WHERE rating >= 9.0;
