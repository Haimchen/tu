CREATE TABLE ZEIT(
	ZEITID INT PRIMARY KEY,
	JAHR INT NOT NULL,
	MONAT INT NOT NULL,
	TAG INT NOT NULL,
	WOCHE INT NOT NULL
);

CREATE TABLE GUTSCHEIN(
	GUTSCHEINID INT PRIMARY KEY,
	WERT INT NOT NULL
);

CREATE TABLE VERKAEUFER(
	VERKAEUFERID INT PRIMARY KEY,
	NAME VARCHAR(50)
);

CREATE TABLE TRANSAKTIONEN(
	ZEITID INT,
	KUNDENID INT,
	GUTSCHEINID INT,
	FILIALID INT,
	VERKAEUFERID INT,
	ABTEILUNGSID INT,
	EINKAUFID INT,
	PRIMARY KEY(ZEITID, KUNDENID, GUTSCHEINID, FILIALID, VERKAEUFERID, ABTEILUNGSID, EINKAUFID)
);

CREATE TABLE EINKAUF(
	EINKAUFID INT PRIMARY KEY,
	EINKAUFSWERT INT
);

CREATE TABLE KUNDE(
	KUNDENID INT PRIMARY KEY,
	NAME VARCHAR(50),
	PLZ INT,
	STRASSE VARCHAR(50)
);

CREATE TABLE FILIALE(
	FILIALID INT PRIMARY KEY,
	STADT VARCHAR(30)
);

CREATE TABLE ABTEILUNG(
	ABTEILUNGSID INT PRIMARY KEY
);