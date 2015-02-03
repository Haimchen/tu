README f�r ConferencePortal(Stub)

Dieses Projekt dient als Einstieg in die Implementierungsaufgabe (HA3 SWTPP WS 2014/2015). 

Die Aufgabe soll als Servlet auf einem Apache Tomcat Web-Server abgeliefert. In eclipse ist dies 
ein "dynamic web project". 

Im Folgenden wird ein �berblick �ber die schon vorgegebenen Ressourcen 
gegeben. Danach folgen noch ein paar Hinweise zur Benutzung von JSP und Servlet. Au�erdem sollten 
die Kommentare in den Beispieldateien beachtet werden. Hier sind auch noch einige Hinweise zu finden.

1) Projekt-�berblick
--------------------

---| ConferencePortal
   Der �bergeordnete Projekt-Ordner. Hier befinden sich die Eclipse-Konfiguration und die Projekt-Dateien, 
   au�erdem nat�rlich alle im Folgenden beschriebenen Ordner.
   
   ---| src
      Hier befinden sich alle reinen Java-Source-Code-Dateien, aufgeteilt in packets. In eclipse sind die 
      packets als Ganzes zu sehen (getrennt mit Punkten). Im Dateisystem sind das jeweils Unter-Ordner.
   
      ---| de.tuberlin.pes.swtpp.ha03.example.control
         Packet f�r die Controller-Klassen des Klassenmodells.
         
      ---| de.tuberlin.pes.swtpp.ha03.example.model
         Packet f�r die Entity-Klassen des Klassenmodells, d.h. das eigentliche Datenmodell. 
         
      ---| de.tuberlin.pes.swtpp.ha03.example.test
         Hier befinden sich die jUnit Testcases und Testsuiten, mit denen die anderen Klassen getestet werden.
         
      ---| de.tuberlin.pes.swtpp.ha03.example.web
         Hier befindet sich die Klasse ConferenceServlet, welche die zentrale Komponente in die Web-Anwendung auf Server-Seite ist. 
         Zu dem Servlet sp�ter mehr.
         
   ---| build
      Hier landen die kompilierten Klassen
      
   ---| WebContent
      In diesem Ordner werden die Web-Ressourcen des Projekts gespeichert, die der Apache Tomcat Server dann wie 
      ein Web-Server zur Verf�gung stellt. Bilder, html-Seiten, css-Dateien sind also �ber eine URL erreichbar (je nach ProjektName
      z.B. http://localhost:8080/ConferencePortal/...) mit dem Pfad innerhalb dieses Ordners erreichbar. Wenn die Ressource
      innerhalb einer Page auf dem Tomcat verlinkt wird kann auch mit einem relativen Pfad gearbeitet werden. Z.B. <a href="meinbild.jpg">Bild</>
      Im webContent-Ordner liegen auch die JSP-Dateien dieses Beispiels.
  
      ---| jsp
      	  Die f�r das Projekt relevanten JSP-Dateien befinden sich hier
          
	      ---| index.jsp
	         Hier startet die Anwendung. Wenn f�r den Benutzer noch keine Infos vorliegen wird er auf diese Seite weitergeleitet
	
	      ---| head.jsp
	         Diese Seite wird von allen anderen eingebunden. Sie enth�lt den header und Feld f�r die Darstellung allgemeiner Fehlermeldungen.
	      
	      ---| main.jsp
	         Idee: Die allgemeinen Anwendungsf�lle kann der Benutzer hier finden. Z.B. kann man hier Konferenzen anlegen,
	         Papers neu hochladen, Listen der zugeordneten Daten �ffnen etc.. Das kann aber auch gerne anders strukturiert werden.
	
	      ---| conferenceDetail.jsp
	         Auf dieser Seite werden die Eingabefelder f�r neue und zu bearbeitende Konferenzen angezeigt. Diese Seite
	         enth�lt Beispiele f�r die meisten Eingabefelder, die jeweils in den Kommentaren erkl�rt sind. Sie wird im Beispiel erreicht wenn
	         auf der main.jsp vom Benutzer "neue Konferenz" oder "Konferenz bearbeiten" gew�hlt wird.

2) Grunds�tzliches zu Servlet/JSP

Ein Servlet ist ein "Beh�ltnis" f�r eine dynamische Web Anwendung, die auf einem Application Server wie dem Apache Tomcat 
ausgef�hrt werden kann. Kern davon ist das Servlet (in unserem Fall ConferenceServlet), diese Klasse implementiert die Klasse HttpServlet 
und ist im deployment descriptor (Konfiguration des Projekts) registriert. Hierdurch wird dem Apache 
Web Server signalisiert, dass er das Servlet f�r HTTP-requests bereit stellen soll. Alle Anfragen (HTTP requests) des
Benutzers an die URL des Servlets (http://localhost:8080/ConferencePortal/ConferenceServlet) werden an die Servlet-Klasse weitergeleitet.
Die Anfragen im Beispiel werden an die doGet()-Methode von ConferenceServlet weitergeleitet.

Wichtig: 
Die Servlet-Klasse wird einmal vom Server erstellt. Egal welcher Benutzer/welche Session/welcher Browser etc. - man landet immer in dieser
Servlet-Klasse. Die Attribute sind also immer sichtbar. Sie eignen sich nur f�r globale Variablen und die Daten, die nicht vom aktuellen Zustand der
Session eines Benutzers abh�ngen.

2.1) Der Request und die Session

In dem Objekt "HttpServletRequest request" stecken alle Informationen, die der Application Server aus dem Request herausgesucht hat oder
die er der aktuellen Benutzersession zugeordnet hat. Genauer:

- mit request.getParameter("ParameterName):String kriegt man die Parameter aus dem Request, d.h. die Daten die der Benutzer gerade abgeschickt hat.
- mit request.getSession() kriegt man die Session f�r den Benutzer, in der die dem WebServer bekannten Session-Daten gespeichert sind. Hier
  �berleben also die Daten verschiedene Aufrufe von doGet() f�r den gleichen Benutzer. Die Daten, die man z.B. f�r die n�chste JSP-Seite oder "f�r nach dem
  n�chsten Klick des Benutzers" speichern m�chte, gehen hier rein. Beispiel:
  request.getSession().setAttribute("currentUser", meinaktuellerBenutzer) speichert den aktuell durch Login/Passwort ermittelten Benutzer
  User meinaktuellerBenutzer = (User) request.getSession().getAttribute("currentUser") gibt ihn wieder zur�ck.
- mit request.getSession.getAttribute kann man also auch Daten zu den JSP-Seiten schicken, z.B. auch Listen ob Objekten etc.

2.2) JSP

In der Servlet-Methode doGet kann man die im System n�tigen Aktionen durchf�hren, die der Request ausl�st. Beim Use-Case createConference w�rde man also
eine Konferenz anlegen. Der Benutzer erwartet aber eine Antwort (In Form von Browser-lesbarem Zeug, also prinzipiell HTML). Diese kann man theoretisch
in der doGet-Methode zusammenbauen, was aber eher unpraktisch ist. 

Die JSP-Seiten (Java Server Pages) k�rzen das etwas ab: Hier kann man in eine normale
HTML-Seite mit speziellen Tags Java-Code einbauen und man hat auch wie in der Servlet-Methode doGet Zugriff auf den letzten Request und die aktuelle Session. 
Der Tomcat durchsucht die JSP-Seite nach den JSP-Tags und f�hrt diese Code-Snippets aus. Diese Java-Anteile werden dann durch ihre Ausgaben ersetzt und 
ergeben f�r den Browser eine ganz nochmale HTML-Seite.
Eine Erkl�rung der JSP-Tags und der verf�gbaren Objekte sind hier recht gut aufgelistet:
http://de.wikipedia.org/wiki/JavaServer_Pages
