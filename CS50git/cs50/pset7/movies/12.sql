SELECT DISTINCT(title)
FROM movies
JOIN stars
ON movies.id = stars.movie_id
JOIN people
ON stars.person_id = people.id
WHERE name IN ("Johnny Depp" AND "Helena Bonham Carter")
ORDER BY title;