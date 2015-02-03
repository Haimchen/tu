<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
    pageEncoding="ISO-8859-1"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="de" lang="de">

<!-- Import java classes of our project -->
<%@ page import="de.tuberlin.pes.swtpp.ha03.example.model.User" %>
<%@ page import="de.tuberlin.pes.swtpp.ha03.example.model.Conference" %>
<%@ page import="java.text.SimpleDateFormat" %>


<%@include file="head.jsp" %>

<!-- This little javascript function opens the datepicker window. The necessary javascript files are included in head.jsp. Make sure to be online while testing. -->
<script>
$(function() {
$( "#date1picker" ).datepicker();
});
$(function() {
$( "#date2picker" ).datepicker();
});
$(function() {
$( "#date3picker" ).datepicker();
});
</script>

<!-- here we open a regular java scriptlet where we can initialize our variables -->
<% 
// load current conference if there already is one.
Conference currentConference = (Conference) session.getAttribute("currentConference");

// for date formatting...
SimpleDateFormat s;

%>

<body>

<br/>
<!-- in this little scriptlet we write the content to "out" which is then part of the output html. Later we see that the %= -tag does the same with the result of the expression. -->
<h2> Angemeldeter User: <% out.println(((User) session.getAttribute("currentUser")).getName()); %></h2>
<br/>
<h2>
<!-- if the current conference is still null, this page creates a new one. Otherwise this is just to edit. At this point we just want to display the current conference name. -->
<% if (currentConference != null) {%>
 Details für Konferenz 
<% out.println(" " + currentConference.getName()); %>

<% } else {  %>
Neue Konferenz erstellen
<% } %>
</h2>

<% 

// if there is no current conference we show the following html form
if (currentConference == null) {%>
 <form action="/ConferencePortal/ConferenceServlet" method="GET">

		<table class="regular">
			<tr>
				<td >Konferenz-Titel</td>
				<td width="20"></td>
				<!-- fill the default value with the current value of the parameter. This may already exist because the user entered invalid data before. -->
				<td><input type="text" name="confName" value="<%= request.getParameter("confName") != null? request.getParameter("confName") : "" %>"  />
			</tr>
			<tr>
				<td>Konferenz-ID</td>
				<td></td>
				<td><input type="text" name="confID" value="<%= request.getParameter("confID") != null? request.getParameter("confID") : "" %>" />
			</tr>
			<tr>
				<td>Beschreibungstext</td>
				<td></td>
				<td><input type="text" name="confDescription" value="<%= request.getParameter("confDescription") != null? request.getParameter("confDescription") : "" %>"/>
			</tr>
			<tr>
				<td>Datum der Konferenz</td>
				<td></td>
				<!-- the datepicker1 if triggers the javascript function that opens the datepicker window.. -->
				<td><input type="text" name="confDate" id="date1picker" value="<%= request.getParameter("confDate") != null? request.getParameter("confDate") : "" %>"/>
			</tr>
			<tr>
				<td>Deadline für Paper-Submits</td>
				<td></td>
				<td><input type="text" name="confSubmitDeadline"
					id="date2picker" value="<%= request.getParameter("confSubmitDeadline") != null? request.getParameter("confSubmitDeadline") : "" %>"/>
			</tr>
			<tr>
				<td>Deadline für Paper-Reviews</td>
				<td></td>
				<td><input type="text" name="confReviewDeadline"
					id="date3picker" value="<%= request.getParameter("confReviewDeadline") != null? request.getParameter("confReviewDeadline") : "" %>"/>
			</tr>
			<tr>
				<td>Maximale Seitenanzahl</td>
				<td></td>
				<td><input type="number" name="confMaxPages" value="<%= request.getParameter("confMaxPages") != null? request.getParameter("confMaxPages") : "5" %>" />
			</tr>
			<tr>
				<td></td>
				<td></td>
				<td></td>
			</tr>
		</table>
		
		<br/>
		<br/>

	<input type="hidden" name="usecase"	value="ChairCreateConference" />
	<input type="submit" value="Create Conference" />
	</form>

<% } else { %>

Hier könnten Konferenz-Use-Cases für eine bestehende Konferenz untergebracht oder schon abgeschlossene Konferenzen angezeigt werden.

<% } %>

<form>
	<!-- For page navigation: Go back to main page. Here the use case is empty because then the servlet redirects correctly. -->
	<input type="hidden" name="usecase"	value="" />
	<input type="submit" value="Go back" />
</form>    
<br/>
<br/>
</body>
</html>