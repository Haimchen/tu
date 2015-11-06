-- Wir gehen davon aus, dass die Tabelle aus Aufgabe 5 "Kunden" heißt

-- Durchschnitt
-- SELECT AVG(k.Alter)
-- FROM Kunden k;


-- Median
SELECT altergeordnet.Alter
FROM
	(
	SELECT COUNT(k.ID) as anzahl
	FROM Kunden k	
	) as AnzahlKunden,
	(
	SELECT k.Alter
	FROM Kunden k
	ORDER BY k.Alter ASC
	) as altergeordnet
LIMIT 1
OFFSET AnzahlKunden.anzahl / 2;

-- Modus
SELECT k.Alter, COUNT(DISTINCT k.Alter) as Anzahl
FROM Kunden k
GROUP BY k.Alter
ORDER BY Anzahl DESC
LIMIT 1;