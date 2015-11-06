----------
1. License

Please note that all resources (binaries, plugins, documentation, any downloads,
etc.) provided to you are property of the IOLITE GmbH or the Technische
Universität Berlin. You are granted the license to use them for the purpose of
the project / cooperation / teaching course with IOLITE GmbH or Technische
Universität Berlin. Redistribution, re-publishing or re-use in any form and for
any commercial or non-commercial purpose are prohibited.

If you have any questions regarding the use of the provided resources, please
contact Grzegorz Lehmann (grzegorz.lehmann@iolite.de).

----------
2. Introduction

This readme gives an overview of how to get started with IOLITE development.

There are two main ways to extend IOLITE. You can implement:
1. Apps - software modules installed into IOLITE and receive access to the home
  environment via a set of APIs. In their nature IOLITE apps are very similar to
  smartphone apps. You implement them to provide end-users with a new use case /
  functionality in their smart environments.
2. Drivers - software modules responsible for communication with devices /
  sensors / services in the smart environment. Examples drivers are Philips hue
  driver, EIB/KNX driver, OpenWeatherMap driver. You implement drivers if you
  want IOLITE to communicate with a new device, protocol or webservice.

This ZIP provides all resources necessary for app and driver development.
  
----------
3. Prerequisites

To work with IOLITE you will need:
- JDK 6.0 or higher version
- Eclipse IDE, Eclipse Modeling Tools edition. It is important to install this
  edition of Eclipse so that you have the Eclipse Modeling Framework (EMF)
  plugins installed. Current download link:
  http://www.eclipse.org/downloads/packages/eclipse-modeling-tools/mars1
- Maven is recommended: https://maven.apache.org
  Please note that user credentials needed to access the Maven repository will
  be published on the channel of your teaching course.

----------
4. Resources of this package
This section lists all resources you need to work with IOLITE.

-----
4.1. IOLITE Binary
In order to work with IOLITE you need an IOLITE binary.

Before the binary is executed, please copy the '/.iolite' directory into your
<user-home> directory.

To run IOLITE, open a console and execute the following command:
'java -jar iolite.jar'

Please note that IOLITE starts a webserver on port 80 per default. On some
systems this may be impossible. A custom port number can be configured in the
IOLITERuntime.xml. More on this in FAQ below.

After the of start IOLITE, you can access the Home Control Center user interface
via a browser under the address:
http://localhost/

(if you reconfigure the port number, the address is http://localhost:<port>/)

Please note that the runtime has a default user:
- Username: debug
- Password: test1234

-----
4.2. Example Driver
The provided '/.iolite' folder contains an example driver that installs a few
virtual devices to IOLITE.

-----
4.3. Example App
The provided '/.iolite' folder contains an example application under
'/.iolite/apps/example-app.jar'. The JAR files contains source files, so it can
be used as a coding example.

The app does not feature any particular functionality other than testing IOLITE
APIs. It can be installed via the HCC.
- open the HCC user interface in your browser
- Login
- Go to 'IOLITE Apps' (bottom navigation bar)
- Go to 'Store' (top navigation bar)
- You should see 'Example App' element available. Click on the 'arrow down' icon
  to install the app.
- Go to 'Installed' (top navigation bar)
- Click on the 'Example App' icon to open its user interface. The user interface
  contains some developer logs from the APIs.

-----
4.4. SDK
The IOLITE SDK is available as Eclipse plugin. The update site is:
http://iolite.de/update-site

4.5. Documents
This ZIP package contains several documents about IOLITE in the /docs folder:
- 'IOLITE App Development Guide.pdf' - provides all necessary information about
  IOLITE app development. Check it for further details.
- 'IOLITE Driver Development Guide.pdf' - provides all necessary information
  about IOLITE driver development. Check it for further details.
- 'IOLITE Home Control Center User Guide.pdf' - overview of the Home Control
  Center user interface and features.
  
----------
5. FAQ

- How to change the port number of IOLITE web server?
  Open the '<user-home>/.iolite/IOLITERuntime.xml' file and change the value of 
  the 'http.port' property of the '<http>' service node (~line 140). You should
  also change the 'announce.dnssd.port' property value of the '<discovery>'
  service node.
  
- How to reset IOLITE, e.g. if something goes wrong?
  Delete the '<user-home>/.iolite/FileStorageService' directory.