### Webserver

#### Meist genutzte Server

* Apache
* Windows
* nginx

Quelle: http://news.netcraft.com/archives/category/web-server-survey/

Anmerkung: Die genauen Prozentzahlen variieren in unterschiedlichen Quellen, diese drei sind aber immer die wichtigsten.

#### XAMPP

Apache-Distribution, die für einfache Konfiguration entwickelt wurde. Enthalten ist auch PHP, Perl, SQL-Datenbank.
Nutzung für Testzwecke, nicht als produktiver Server!

#### Zugriff

Zugriff lokal (localhost) und über Netzwerk-Adresse funktioniert.
Zugriff über öffentliche IP funktioniert nicht (CONNECTION_REFUSED). Grund dafür ist vermutlich, dass der Router nicht entsprechend konfiguriert wurde um eingehende HTTP-Requests weiterzuleiten. Dazu müssten erst die entsprechenden Ports freigegeben werden. Zudem ist XAMPP als Testsystem konfiguriert.

#### Seite veröffentlichen
* Publish-Funktion von Thimble
* richtigen Server aufsetzen
* Cloud-Anbieter nutzen
