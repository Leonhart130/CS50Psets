SELECT name
FROM people
INNER JOIN stars
ON people.id = stars.person_id
JOIN movies
ON stars.movie_id = movies.id
WHERE title = "Toy Story";
