README für ConferencePortal(Stub)

Dieses Projekt dient als Einstieg in die Implementierungsaufgabe (HA3 SWTPP WS 2014/2015). 

Die Aufgabe soll als Servlet auf einem Apache Tomcat Web-Server abgeliefert. In eclipse ist dies 
ein "dynamic web project". 

Im Folgenden wird ein Überblick über die schon vorgegebenen Ressourcen 
gegeben. Danach folgen noch ein paar Hinweise zur Benutzung von JSP und Servlet. Außerdem sollten 
die Kommentare in den Beispieldateien beachtet werden. Hier sind auch noch einige Hinweise zu finden.

1) Projekt-Überblick
--------------------

---| ConferencePortal
   Der übergeordnete Projekt-Ordner. Hier befinden sich die Eclipse-Konfiguration und die Projekt-Dateien, 
   außerdem natürlich alle im Folgenden beschriebenen Ordner.
   
   ---| src
      Hier befinden sich alle reinen Java-Source-Code-Dateien, aufgeteilt in packets. In eclipse sind die 
      packets als Ganzes zu sehen (getrennt mit Punkten). Im Dateisystem sind das jeweils Unter-Ordner.
   
      ---| de.tuberlin.pes.swtpp.ha03.example.control
         Packet für die Controller-Klassen des Klassenmodells.
         
      ---| de.tuberlin.pes.swtpp.ha03.example.model
         Packet für die Entity-Klassen des Klassenmodells, d.h. das eigentliche Datenmodell. 
         
      ---| de.tuberlin.pes.swtpp.ha03.example.test
         Hier befinden sich die jUnit Testcases und Testsuiten, mit denen die anderen Klassen getestet werden.
         
      ---| de.tuberlin.pes.swtpp.ha03.example.web
         Hier befindet sich die Klasse ConferenceServlet, welche die zentrale Komponente in die Web-Anwendung auf Server-Seite ist. 
         Zu dem Servlet später mehr.
         
   ---| build
      Hier landen die kompilierten Klassen
      
   ---| WebContent
      In diesem Ordner werden die Web-Ressourcen des Projekts gespeichert, die der Apache Tomcat Server dann wie 
      ein Web-Server zur Verfügung stellt. Bilder, html-Seiten, css-Dateien sind also über eine URL erreichbar (je nach ProjektName
      z.B. http://localhost:8080/ConferencePortal/...) mit dem Pfad innerhalb dieses Ordners erreichbar. Wenn die Ressource
      innerhalb einer Page auf dem Tomcat verlinkt wird kann auch mit einem relativen Pfad gearbeitet werden. Z.B. <a href="meinbild.jpg">Bild</>
      Im webContent-Ordner liegen auch die JSP-Dateien dieses Beispiels.
  
      ---| jsp
      	  Die für das Projekt relevanten JSP-Dateien befinden sich hier
          
	      ---| index.jsp
	         Hier startet die Anwendung. Wenn für den Benutzer noch keine Infos vorliegen wird er auf diese Seite weitergeleitet
	
	      ---| head.jsp
	         Diese Seite wird von allen anderen eingebunden. Sie enthält den header und Feld für die Darstellung allgemeiner Fehlermeldungen.
	      
	      ---| main.jsp
	         Idee: Die allgemeinen Anwendungsfälle kann der Benutzer hier finden. Z.B. kann man hier Konferenzen anlegen,
	         Papers neu hochladen, Listen der zugeordneten Daten öffnen etc.. Das kann aber auch gerne anders strukturiert werden.
	
	      ---| conferenceDetail.jsp
	         Auf dieser Seite werden die Eingabefelder für neue und zu bearbeitende Konferenzen angezeigt. Diese Seite
	         enthält Beispiele für die meisten Eingabefelder, die jeweils in den Kommentaren erklärt sind. Sie wird im Beispiel erreicht wenn
	         auf der main.jsp vom Benutzer "neue Konferenz" oder "Konferenz bearbeiten" gewählt wird.

2) Grundsätzliches zu Servlet/JSP

Ein Servlet ist ein "Behältnis" für eine dynamische Web Anwendung, die auf einem Application Server wie dem Apache Tomcat 
ausgeführt werden kann. Kern davon ist das Servlet (in unserem Fall ConferenceServlet), diese Klasse implementiert die Klasse HttpServlet 
und ist im deployment descriptor (Konfiguration des Projekts) registriert. Hierdurch wird dem Apache 
Web Server signalisiert, dass er das Servlet für HTTP-requests bereit stellen soll. Alle Anfragen (HTTP requests) des
Benutzers an die URL des Servlets (http://localhost:8080/ConferencePortal/ConferenceServlet) werden an die Servlet-Klasse weitergeleitet.
Die Anfragen im Beispiel werden an die doGet()-Methode von ConferenceServlet weitergeleitet.

Wichtig: 
Die Servlet-Klasse wird einmal vom Server erstellt. Egal welcher Benutzer/welche Session/welcher Browser etc. - man landet immer in dieser
Servlet-Klasse. Die Attribute sind also immer sichtbar. Sie eignen sich nur für globale Variablen und die Daten, die nicht vom aktuellen Zustand der
Session eines Benutzers abhängen.

2.1) Der Request und die Session

In dem Objekt "HttpServletRequest request" stecken alle Informationen, die der Application Server aus dem Request herausgesucht hat oder
die er der aktuellen Benutzersession zugeordnet hat. Genauer:

- mit request.getParameter("ParameterName):String kriegt man die Parameter aus dem Request, d.h. die Daten die der Benutzer gerade abgeschickt hat.
- mit request.getSession() kriegt man die Session für den Benutzer, in der die dem WebServer bekannten Session-Daten gespeichert sind. Hier
  überleben also die Daten verschiedene Aufrufe von doGet() für den gleichen Benutzer. Die Daten, die man z.B. für die nächste JSP-Seite oder "für nach dem
  nächsten Klick des Benutzers" speichern möchte, gehen hier rein. Beispiel:
  request.getSession().setAttribute("currentUser", meinaktuellerBenutzer) speichert den aktuell durch Login/Passwort ermittelten Benutzer
  User meinaktuellerBenutzer = (User) request.getSession().getAttribute("currentUser") gibt ihn wieder zurück.
- mit request.getSession.getAttribute kann man also auch Daten zu den JSP-Seiten schicken, z.B. auch Listen ob Objekten etc.

2.2) JSP

In der Servlet-Methode doGet kann man die im System nötigen Aktionen durchführen, die der Request auslöst. Beim Use-Case createConference würde man also
eine Konferenz anlegen. Der Benutzer erwartet aber eine Antwort (In Form von Browser-lesbarem Zeug, also prinzipiell HTML). Diese kann man theoretisch
in der doGet-Methode zusammenbauen, was aber eher unpraktisch ist. 

Die JSP-Seiten (Java Server Pages) kürzen das etwas ab: Hier kann man in eine normale
HTML-Seite mit speziellen Tags Java-Code einbauen und man hat auch wie in der Servlet-Methode doGet Zugriff auf den letzten Request und die aktuelle Session. 
Der Tomcat durchsucht die JSP-Seite nach den JSP-Tags und führt diese Code-Snippets aus. Diese Java-Anteile werden dann durch ihre Ausgaben ersetzt und 
ergeben für den Browser eine ganz nochmale HTML-Seite.
Eine Erklärung der JSP-Tags und der verfügbaren Objekte sind hier recht gut aufgelistet:
http://de.wikipedia.org/wiki/JavaServer_Pages
