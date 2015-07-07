CREATE TABLE FLUGZEUGTYP(
FTID SMALLINT NOT NULL PRIMARY KEY,
ANZAHLPLAETZE SMALLINT NOT NULL
);

CREATE TABLE PERSON(
PID SMALLINT NOT NULL PRIMARY KEY,
NAME CHAR(30) NOT NULL
);

CREATE TABLE FLUGHAFEN(
FHID SMALLINT NOT NULL PRIMARY KEY,
STADT CHAR(20) NOT NULL,
STAAT CHAR(20) NOT NULL
);

CREATE TABLE FLUG(
FID SMALLINT NOT NULL PRIMARY KEY,
STARTDATUM DATE NOT NULL,
FLUGZEUGTYP SMALLINT NOT NULL,
STARTFLUGHAFEN SMALLINT NOT NULL,
ZIELFLUGHAFEN SMALLINT NOT NULL,
CONSTRAINT FGK_FLUGZEUGTYP FOREIGN KEY (FLUGZEUGTYP) REFERENCES FLUGZEUGTYP(FTID),
CONSTRAINT FGK_STARTFLUGHAFEN FOREIGN KEY (STARTFLUGHAFEN) REFERENCES FLUGHAFEN(FHID),
CONSTRAINT FGK_ZIELFLUGHAFEN FOREIGN KEY (ZIELFLUGHAFEN) REFERENCES FLUGHAFEN(FHID)
);


CREATE TABLE BUCHT(
PID SMALLINT NOT NULL,
FID SMALLINT NOT NULL,
PREIS SMALLINT NOT NULL,
CONSTRAINT FGK_PERSON FOREIGN KEY(PID) REFERENCES PERSON(PID),
CONSTRAINT FGK_FLUG FOREIGN KEY(FID) REFERENCES FLUG(FID),
PRIMARY KEY(PID, FID)
);

-- Datenstze Person

insert into Person(PID, Name) values
(2, 'Til'),
(3, 'Matthias'),
(4, 'Goetz'),
(5, 'Daniel'),
(6, 'Jan'),
(7, 'Veronica'),
(8, 'Tom'),
(9,'Franka'),
(10, 'Karoline'),
(11, 'Robert'),
(12, 'Will'),
(13, 'Brad'),
(14, 'Denzel');

-- Datenstze Flugzeugtyp
insert into Flugzeugtyp(FTID, AnzahlPlaetze) values
(1, 400),
(2, 300),
(3,50),
(4, 345),
(5, 78);

-- Daten fr Flughafen
insert into Flughafen(FHID, Stadt, Staat) values
(1, 'Berlin', 'Deutschland'),
(2,'Barcelona', 'Spanien'),
(3, 'Palma de Mallorca', 'Spanien'),
(4, 'Athen', 'Griechenland'),
(5, 'München', 'Deutschland'),
(6, 'Hamburg', 'Deutschland'),
(7, 'Dresden', 'Deutschland'),
(8, 'Rom', 'Italien'),
(9, 'Paris', 'Frankreich');


-- Flugdaten
insert into Flug(FID, Startdatum, Flugzeugtyp, Startflughafen, Zielflughafen) values
(1,'2015-04-02', 2, 5, 8),
(2,'2015-04-06', 4, 4, 1),
(3,'2015-07-21', 5, 1, 3),
(4,'2015-04-20', 4, 6, 8),
(5,'2015-02-15', 1, 1, 9),
(6,'2015-04-05', 2, 8, 5),
(7,'2015-06-07', 4, 1, 4),
(8,'2015-03-02', 3, 1, 6),
(9, '2015-04-07', 1, 6, 2),
(10,'2015-03-05', 3, 1, 6),
(11, '2015-04-20', 4, 9, 2),
(12, '2015-05-02', 3, 8, 4);

-- Datenstze bucht
insert into bucht(PID, FID, Preis) values
(7, 7, 100),
(7, 10, 130),
(11, 1, 330),
(11, 9, 220),
(11, 5, 300),
(11, 10, 90),
(11, 7, 230),
(14, 11, 700),
(2, 12, 300),
(3, 12, 300),
(4,12, 300);
