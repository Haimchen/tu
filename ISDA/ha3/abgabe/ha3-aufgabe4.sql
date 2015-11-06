-- Getestet mit MYSQL 5.6

-- 1.
SELECT COUNT(DISTINCT t.GutscheinID)
FROM Transaktionen t;


-- 2.
SELECT SUM(restwerte.rest)
FROM
	(
	SELECT t.GutscheinID, (g.Wert - e.Einkaufswert) as rest
	FROM
		Transaktionen t,
		Gutschein g,
		Einkauf e
	WHERE 
		t.einkaufid IS NOT NULL AND
		t.GutscheinID = g.GutscheinID AND
		t.einkaufid = e.einkaufid
	) as restwerte
WHERE
restwerte.rest > 0;

-- 3.
SELECT v.Name, SUM(g.Wert) as verkaufteGutscheine
FROM 
	Gutschein g 
	JOIN Transaktionen t ON (g.GutscheinID = t.GutscheinID)
	JOIN Verkaeufer v ON (t.verkaeuferid = v.verkaeuferid)
	JOIN Zeit z ON (t.Zeitid = z.Zeitid)
WHERE
	-- Wir gehen davon aus, dass TODAY() eine ZeitID liefert
	-- Woche(ZEITID) liefert dann das Attribut Woche aus einer ZeitID
	z.Woche = WOCHE(TODAY()) AND
	t.einkaufid IS NULL
GROUP BY v.Name ;
ORDER BY verkaufteGutscheine DESC; 


-- 4.
SELECT t.Abteilungsid, k.PLZ, z.Jahr, COUNT(*) as Anzahl
FROM Transaktionen t
	JOIN Kunde k ON (t.Kundenid = k.Kundenid)
	JOIN Zeit z ON (t.Zeitid = z.Zeitid)
WHERE
	t.einkaufid IS NOT NULL
GROUP BY t.Abteilungsid, k.PLZ, z.Jahr;