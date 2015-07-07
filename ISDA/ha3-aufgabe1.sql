-- http://sqlfiddle.com/#!9/f8b23


-- -- 1.
-- SELECT f.Flugzeugtyp, COUNT(f.Flugzeugtyp) AS AnzahlFluege
-- FROM Flug f
-- GROUP BY f.Flugzeugtyp
-- ORDER BY AnzahlFluege DESC
-- LIMIT 1;

-- -- 2. Wie  lauten  die Personen (PID,  Name),  die nach  Barcelona ODER  Rom geflogen  sind?
-- SELECT p.PID, p.Name
-- FROM Person p, Flug f, Flughafen fh, Bucht b
-- WHERE 
-- b.PID = p.PID AND
-- b.FID = f.FID AND
-- f.Zielflughafen = fh.FHID AND
-- (fh.Stadt = "Barcelona" OR fh.Stadt = "Rom")
-- GROUP BY p.PID;

-- -- 3.
-- SELECT p.PID, p.Name, b.Preis
-- FROM Person p, Bucht b
-- WHERE
-- b.PID = p.PID AND
-- b.FID = "10"
-- ORDER BY b.Preis ASC
-- LIMIT 1;

-- -- 4.
-- SELECT p.PID, p.Name
-- FROM Person p, Bucht b
-- WHERE
-- p.PID = b.PID AND
-- b.PID NOT IN (SELECT b.PID
-- FROM Bucht b, Flug f, Flughafen fh
-- WHERE 
-- b.FID = f.FID AND
-- f.Zielflughafen = fh.FHID AND
-- fh.Staat = "Spanien")
-- GROUP BY p.PID;

-- 5.
-- SELECT bothCities.PID, p.Name
-- FROM
-- (
--   SELECT b.PID, fh.Stadt
--   FROM Flug f, Flughafen fh, Bucht b
--   WHERE 
--   b.FID = f.FID AND
--   f.Zielflughafen = fh.FHID AND
--   fh.Stadt = "Athen" OR fh.Stadt = "Rom"
--   GROUP BY b.PID, fh.Stadt
-- ) as bothCities, Person p
-- WHERE
-- p.PID = bothCities.PID
-- GROUP BY bothCities.PID having COUNT(bothCities.Stadt) = 2;

-- -- 5. Version 2 - nicht in MYSQL
-- SELECT p.PID, p.Name
-- FROM (
--   SELECT b.PID
--   FROM Flug f, Flughafen fh, Bucht b
--   WHERE 
--   b.FID = f.FID AND
--   f.Zielflughafen = fh.FHID AND
--   fh.Stadt = "Athen"
--   GROUP BY b.PID
--   INTERSECT
--   SELECT b.PID
--   FROM Flug f, Flughafen fh, Bucht b
--   WHERE 
--   b.FID = f.FID AND
--   f.Zielflughafen = fh.FHID AND
--   fh.Stadt = "ROM"
--   GROUP BY b.PID
-- ) as bothCities, Person p
-- WHERE
-- p.PID = bothCities.PID
-- GROUP BY p.PID;



