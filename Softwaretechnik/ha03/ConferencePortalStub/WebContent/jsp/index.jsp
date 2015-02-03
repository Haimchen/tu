<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
    pageEncoding="ISO-8859-1"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="de" lang="de">

<!-- We include a simple header file that contains a logo and some CSS /JavaScipt includes which should be the same in all JSP pages. 
	 Also, the errortext Attribute is displayed in the head.jsp.
-->
<%@include file="head.jsp" %>

<!-- Start with the body of the html as the rest is in head.jsp. -->
<body id="ts">

<br/>
<h2> Schon registriert? Bitte anmelden </h2>
<br/>

	<!-- A form of this kind is forwarded by the browser to our servlet. All form inputs are parameters in the request that arrives in the doGet method of the servlet. -->
    <form action="/ConferencePortal/ConferenceServlet" method="GET">
     	<table class="regular">
     	
			<tr>
				<!-- simple text field in the name -->
				<td class="regular">ID</td>
				<td></td>
				<td><input type="text" name="id" /></td>
			</tr>
			<tr>
			    <!--  To make it simple, the passwort is just submitted unencrypted in the request. Of course this would be done differently in a real application. -->
				<td>Passwort</td>
				<td></td>
				<td><input type="password" name="password" /></td>
			</tr>
			<tr>
				<!--  this is the button that launches the action. -->
				<td />
				<td width="20" />
				<td><input type="submit" value="Login" /></td>
			</tr>
			<tr><td>
			<!--  Another important parameter for all our requests: The use case. Here we want to tell the servlet that this is a login request. -->
			<input type="hidden" name="usecase" value="UserLogin"/>

			</td></tr>
		</table>
	</form>
		

	

<br/>
<br/>
<h2> Neuer User? Bitte registrieren.</h2>
<br/>
     <form action="/ConferencePortal/ConferenceServlet" method="GET">

     	
     	<input type="hidden" name="usecase" value="UserRegister"/>
     	
     	<table class="regular">
     	<tr>
      		<td>Name</td>
      		<td width="20" />
      		<td><input type="text" name="name" /></td>
      	</tr>
      	<tr>
      		<td>Kontakt Info</td>
			<td width="20" />
      		<td><input type="text" name="contactInfo" /></td>
      	</tr>
     	<tr>
      		<td>ID</td>
      		<td width="20" />
      		<td><input type="text" name="id" /></td>
      	</tr>
      	<tr>
      		<td>Passwort</td>
      		<td width="20" />
      		<td><input type="password" name="password"/></td>
      	</tr>
      	<tr>
      		<td/>
      		<td width="20" />
      		<td><input type="submit" value="Register"/></td>
      	</tr>
     	</table>

     </form>
<br/>
<br/>
</body>
</html>