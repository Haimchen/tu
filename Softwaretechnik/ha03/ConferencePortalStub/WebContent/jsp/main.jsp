<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
    pageEncoding="ISO-8859-1"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="de" lang="de">

<%@ page import="de.tuberlin.pes.swtpp.ha03.example.model.User" %>
<%@ page import="de.tuberlin.pes.swtpp.ha03.example.model.Conference" %>

<%@include file="head.jsp" %>

<% 
// initialize local variables
User currentUser = (User) session.getAttribute("currentUser"); %>

<body>

<br/>
<h2> Angemeldeter User: <% out.println(currentUser.getName()); %></h2>
<br/>
<h2> Für Conference Chairs</h2>
<div >
	<br/>
	<br/>
	
	<h3> Meine Konferenzen </h3>
		
	<table class="regular">
	<% for (Conference c: currentUser.getHostedConferences()) { %>
		<tr>
			<td align="left"> 
				<%=c.getName() %>
			</td>
			<td width="20 pt"> 
				
			</td>
			<td width="60 pt">
				<form action="/ConferencePortal/ConferenceServlet" method="GET">
			     	<input type="hidden" name="usecase" value="ChairConferenceDetail"/>
			     	<input type="hidden" name="selectedConference" value="<%= c.getId() %>"/>
					<input type="submit" value="Details"/>
				</form>
			</td>
		</tr>
	<% } %>
	</table>
	
	<br/>
	   <form action="/ConferencePortal/ConferenceServlet" method="GET">	
	     	<input type="hidden" name="usecase" value="ChairCreateConferencePage"/>
	     	
			<input type="submit" value="Neue Konferenz anlegen"/>
	
	    </form>
	<br/>
	<br/>
</div>
<h2> Für Autoren</h2>
<br/>
<br/>
<h3> Meine Papers </h3>
	
	Hier könnten die Papers des Benutzers(Autor) aufgelistet werden.
	
	
<h2> Für Reviewer</h2>
<br/>
<br/>
<h3> Papers für Review zugewiesen </h3>
<br/>

    Hier könnten die dem Benutzer zugewiesenen Review angezeigt werden.

<br/>
<h2> Allgemein </h2>
<br/>
 <form action="/ConferencePortal/ConferenceServlet" method="GET">	
     	<input type="hidden" name="usecase" value="UserLogoff"/>
     	
<input type="submit" value="Log Off"/>
    </form>
<br/>
<br/>
</body>
</html>