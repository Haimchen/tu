<head>
<!--  <base href="https://www.pes.tu-berlin.de/" /> -->

<title>Konferenz-Verwaltungsportal SWTPP</title>

<link rel="stylesheet" type="text/css" href="conference.css">

<!-- jquery 
 -->
<link rel="stylesheet" href="//code.jquery.com/ui/1.11.2/themes/smoothness/jquery-ui.css">
<script src="//code.jquery.com/jquery-1.10.2.js"></script>
<script src="//code.jquery.com/ui/1.11.2/jquery-ui.js"></script>

<link rel="stylesheet" href="http://jqueryui.com/resources/demos/style.css">

 
<!-- jquery local (zum testen) 
<link rel="stylesheet" href="jquery.css">
<script src="jquery.js"></script>
<script src="jquery_ui.js"></script>
-->

<div id="home"><img src="https://www.pes.tu-berlin.de/fileadmin/Aperto_design/img/logo_01.gif" width="219" height="75" alt="TU Berlin" title="TU Berlin" /></div>

<!-- General error messages found in the session are displayed here. -->
<% if (session.getAttribute("errortext") != null ) { %>
<br/>
<br/>

<h2> <font color="#FF0000"> Fehlermeldung:
<%	out.println(session.getAttribute("errortext").toString()); %>
</font> </h2>

<br/>
<br/>

<% }%>

<h1> Konferenzverwaltungs-Portal SWTPP</h1>

</head>