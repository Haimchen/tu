-- Getestet mit MySQL 5.6


-- 1.
SELECT f.Flugzeugtyp, COUNT(f.Flugzeugtyp) AS AnzahlFluege
FROM Flug f
GROUP BY f.Flugzeugtyp
ORDER BY AnzahlFluege DESC;
LIMIT 1;

-- 2. 
SELECT p.PID, p.Name
FROM Person p, Flug f, Flughafen fh, Bucht b
WHERE 
b.PID = p.PID AND
b.FID = f.FID AND
f.Zielflughafen = fh.FHID AND
(fh.Stadt = "Barcelona" OR fh.Stadt = "Rom")
GROUP BY p.PID;

-- 3.
SELECT p.PID, p.Name, b.Preis
FROM Person p, Bucht b
WHERE
b.PID = p.PID AND
b.FID = "10"
ORDER BY b.Preis ASC
LIMIT 1;

-- 4.
SELECT p.PID, p.Name
FROM Person p, Bucht b
WHERE
p.PID = b.PID AND
b.PID NOT IN (SELECT b.PID
FROM Bucht b, Flug f, Flughafen fh
WHERE 
b.FID = f.FID AND
f.Zielflughafen = fh.FHID AND
fh.Staat = "Spanien")
GROUP BY p.PID;

-- 5.
SELECT bothCities.PID, p.Name
FROM
(
  SELECT b.PID, fh.Stadt
  FROM Flug f, Flughafen fh, Bucht b
  WHERE 
  b.FID = f.FID AND
  f.Zielflughafen = fh.FHID AND
  (fh.Stadt = "Athen" OR fh.Stadt = "Rom")
  GROUP BY b.PID, fh.Stadt
) as bothCities, Person p
WHERE
p.PID = bothCities.PID
GROUP BY bothCities.PID having COUNT(bothCities.Stadt) = 2;


-- 6.
SELECT COUNT(DISTINCT FluegeAthen.FID) as AnzahlFluege, COUNT(DISTINCT b.PID) as AnzahlPersonen
FROM  
Bucht b INNER JOIN (
SELECT f.FID
FROM Flug f, Flughafen fh
WHERE
f.Zielflughafen = fh.FHID AND
fh.Stadt = "Athen" AND
YEAR(f.Startdatum) = 2015
) as FluegeAthen
ON b.FID = FluegeAthen.FID; 

-- 7.
SELECT p.Name
FROM
	((SELECT personen.PID, COUNT(DISTINCT personen.Staat) as AnzahlStaaten
	FROM
		(
		SELECT b.PID, fh.Staat
		FROM Bucht b, Flug f, Flughafen fh
		WHERE 
		b.FID = f.FID AND
		f.Zielflughafen = fh.FHID
		GROUP BY b.PID, fh.Staat
		) as personen
	GROUP BY personen.PID) as bla),
	(
	SELECT COUNT(DISTINCT fh.Staat) as Anzahl
	FROM Flughafen fh
	) as staaten,
	Person p
WHERE
bla.AnzahlStaaten = staaten.Anzahl AND
p.PID = bla.PID;

-- 8.
SELECT fh.Stadt, COUNT(fh.Stadt) as Anzahl
FROM
	(SELECT b.PID, f.Zielflughafen
	FROM Bucht b, Flug f, Flughafen fh
	WHERE 
	b.FID = f.FID AND
	f.Startflughafen = fh.FHID AND
	fh.Staat = "Deutschland" AND
	MONTH(f.Startdatum) = 4 AND
	YEAR(f.Startdatum) = 2015
	) as staaten 
	JOIN Flughafen fh ON (staaten.zielflughafen = fh.FHID)
GROUP BY fh.Stadt
ORDER BY Anzahl DESC
LIMIT 1;






