### Aufgabe 4 - Relationaler Entwurf

Raum(<u>RNummer</u>)

>Wir haben uns entschieden, die Relation für *Raum*  nicht wegzulassen, weil es eventuell auch Räume ohne Aktenschränke geben kann

Aktenschrank(<u>ANummer</u>, <u>RNummer</u>, Sicherheitsstufe)

Beamter(<u>Personalkennziffer</u>, Name, Geburtsdatum, Tel-Nr.)

Notiz(<u>ID</u>, Betreff, Status, Aktenzeichen, Zeitstempel)

>Hier haben wir den Entity-Typ *Notiz* sowie den Relationship-Typ *anheften* zusammengefasst, da es sich um eine 1:n-Beziehung handelt. Das Aktenzeichen ist der Schlüssel für das Dokument, an welches die Notiz angeheftet wird.

Dokument(<u>Aktenzeichen</u>, ErstellDatum, SchrankNummer, RaumNummer)

>Wir haben uns entschieden, das *Dokument* und die zugehörigen Spezialisierungen im ER-Stil umzusetzen. Außerdem haben wir in der Relation für das *Dokument* auch den Relationship-Typ *ablegen* integriert, da es sich wieder um eine 1:n-Beziehung handelt. Deswegen enthält die Relation auch die beiden Schlüsselattribute des *Aktenschranks*.

Einfaches\_Dokument(<u>Aktenzeichen</u>, Art, ÜbergeordnetesDokument)

>Das *Einfache Dokument* erbt das Schlüsselattribut von der Generalisierung *Dokument*. Die Aggregations-Beziehung zum *Komplexen Dokument* wird über den Verweis *ÜbergeordnetesDokument* ausgedrückt, welche das Aktenzeichen des Komplexen Dokuments referenziert, zu dem das Einfache Dokument gehört.

Komplexes\_Dokument(<u>Aktenzeichen</u>, Status)

Bericht(<u>Aktenzeichen</u>, Autor, Mappe)

>Auch der *Bericht* erbt das Schlüsselattribut *Aktenzeichen* von *Dokument*, in diesem Fall indirekt über die Generalisierung *Komplexes Dokument*. Die Aggregation ist in dem Verweis auf die *Mappe* (also das Aktenzeichen der Mappe) enthalten.

Mappe(<u>Aktenzeichen</u>, Vorgang)

bearbeitet(<u>Aktenzeichen</u>, Personalkennziffer)

>Da *bearbeitet* eine n:m-Beziehung ist, ist eine eigene Relation notwendig.

##### Alternative Umsetzung mit einer möglichsten kleinen Zahl von Relationen:

>Wir haben uns für die Umsetzung der Generalisierung/Spezialisierung im ER-Stil entschieden, da uns dies als die übersichtlichste Variante erschien. Eine Umsetzung im null-Stil wäre aber genauso möglich, wenn das Ziel eine möglichst kleine Anzahl von Relationen ist. Dafür wäre andererseits mehr Speicher nötig, um die nicht benötigten Zeilen mit den null-Werten zu speichern.
>In diesem Fall könnte man die Relationen *Einfaches\_Dokument*, *Komplexes\_Dokument*, *Mappe* und *Bericht* wegglassen und *Dokument* folgendermaßen verändern:

Dokument(<u>Aktenzeichen</u>, ErstellDatum, SchrankNummer, RaumNummer, Komplexität, Art, ÜbergeordnetesDokument, Status, Vorgang, Autor, Mappe)

>Dabei können alle Attribute nach Komplexität auch den Wert null haben.
>Die übrigen Relationen bleiben unverändert.
