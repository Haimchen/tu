package de.tuberlin.pes.swtpp.ha03.example.web;

import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.LinkedList;

import javax.servlet.RequestDispatcher;
import javax.servlet.ServletContext;
import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import de.tuberlin.pes.swtpp.ha03.example.control.ChairController;
import de.tuberlin.pes.swtpp.ha03.example.control.UserController;
import de.tuberlin.pes.swtpp.ha03.example.model.Conference;
import de.tuberlin.pes.swtpp.ha03.example.model.User;


/**
 * Servlet implementation class ConferenceServlet
 */
@WebServlet("/ConferenceServlet")
public class ConferenceServlet extends HttpServlet {
	private static final long serialVersionUID = 1L;
	
	// relative paths to the jsp pages
	private static String INDEX_PAGE  = "/jsp/index.jsp";
	private static String MAIN_PAGE  = "/jsp/main.jsp";
	private static String CONFERENCEDETAIL_PAGE  = "/jsp/conferenceDetail.jsp";
	// TODO ...
	
	// data model of the conference system (global lists)
	private static LinkedList<User> users;
	private static LinkedList<Conference> conferences;
	// TODO ...
	
	// controller classes
	private static UserController userController;
	private static ChairController chairController;
	// TODO ...
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public ConferenceServlet() {
        super();
        
        // initialization of data model
        users = new LinkedList<User>();
        conferences = new LinkedList<Conference>();
    	// TODO ...
        
        // create controllers
        userController = new UserController(users);
        chairController = new ChairController(conferences, users);
    	// TODO ...
       
        // just for testing; create data so that we don't have to start new every time
        createDebuggingData();
    }

	/**
	 * We implement all conference functionality as HTTP get requests; The web server calls this method every time a html form action is received.
	 * 
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
			
		// What we do here in this method (for every request) is the following:
		// - get the parameters (user inputs) out of the request parameters: request.getParameter(...)
		// - find out what the current state is (session attributes): request.getSession().getAttribute(...)
		// - perform the action that has to be done (use case)
		// - change the session state so that we "remember" what has been done request.getSession().setAttribute(...)
		// ---> the session state we read again when this method is called. 
		// Also it is available in the JSP Pages, so we can pass data to the JSP pages.
		
		
		// this is to initialize and forward the servlet session data
		// with sc.getRequestDispatcher(page) we can select the page the user is forwarded to
		ServletContext sc = getServletContext();
		RequestDispatcher rd;
		String nextJSPPage = INDEX_PAGE; // define next page to forward the user to. Default: Index page

	    // in request.getSession() we find the data that needs to be saved as long as the user session lives. We can use the Attributes to pass data to the JSP Pages.
	    // the parameters in the request are those values entered in the html forms of the jsp pages
		
		// no error at first
		// we use the errortext attribute to display general error messages in the page head
		request.getSession().removeAttribute("errortext");

	    // see what we have to do
	    String usecase = request.getParameter("usecase"); 

	    
	    // here we implement the page navigation and start the use cases by passing the requests to the controller classes
	    // The parameter usecase is used in every html form to help us find out here what to do
	    
	    //////////////////////////
	    // Start of workflow -> no usecase selected
	    //////////////////////////
	    if (usecase == null || usecase.equals("")) {
	    	// if there is no user -> start with index page to log on or register
	    	if (request.getSession().getAttribute("currentUser") != null) {
	    		nextJSPPage = MAIN_PAGE;
	    	}
	    	
	    //////////////////////////
        // 	User login/register stuff 
	    //////////////////////////	    	
	    } else if (usecase.equals("UserLogin")) {
	    	// normal log in request : check user ID and password
	    	// if ok remember user and forward to main page, else try again
	    	if (userController.checkUserPwd(request.getParameter("id"), request.getParameter("password"))) {
	    		request.getSession().setAttribute("currentUser", userController.findUserByID(request.getParameter("id")));
	    		nextJSPPage = MAIN_PAGE;
	    	} else {
	    		request.getSession().setAttribute("errortext", "User/Pwd unbekannt.");
	    		// go to index page
	    	}
	    } else if (usecase.equals("UserLogoff")) {
	    	// log of just disconnects the current user and goes back to start
	    	request.getSession().removeAttribute("currentUser");
	    	// go to index page
	    } else if (usecase.equals("UserRegister")) {
	    	// register a new user with the selected data. the createUser method returns error messages when the data given was not correct
	    	User u = userController.createUser(request.getParameter("id"), request.getParameter("name"), request.getParameter("password"), request.getParameter("contactInfo"));
	    	
	    	if (u != null) {
	    		request.getSession().setAttribute("currentUser", u);
	    		nextJSPPage = MAIN_PAGE;
	    	} else {
	    		request.getSession().setAttribute("errortext", "Could not create user. Try again.");
	    		// go to index page
	    	}
	    	
		//////////////////////////
	    // 	Real Chair/Reviewer/Actor Use Cases 
		//////////////////////////	 
	    } else {
	    	// for all other use cases we need a session
	    	// that is: the current user data must be saved in the session object
	    	// if not: we ignore all request and force the user to log on
	    	if (request.getSession().getAttribute("currentUser") != null) {

	    		// now the real use cases
	    		// pass on to function that makes it a little easier to read
    			nextJSPPage = handleRequestForAuthenticatedUser(request, usecase);
	    	}
	    }
	    
	    // forward the user to the next selected JSP page
	    rd = sc.getRequestDispatcher(nextJSPPage);
	    rd.forward(request, response);
		
	}

	
	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	/* We don't need this yet
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// TODO Auto-generated method stub
	}
	*/
	
	private String handleRequestForAuthenticatedUser(HttpServletRequest request, String usecase) {
		// See comment at start of doGet to see what has to be done in every use case
		SimpleDateFormat formatter = new SimpleDateFormat("MM/dd/yyyy");
		String result;
		
		// The return value: Next page in JSP Navigation
		String nextJSPPage;

		switch(usecase) {
	    		//////////////////////////
	    	    // 	Conference Chair use cases
	    		//////////////////////////
			case("ChairCreateConferencePage"): 
	    			// open conference detail page for new conference
	    			// just init values and redirect to the details page
				nextJSPPage = CONFERENCEDETAIL_PAGE;
	    			request.getSession().removeAttribute("currentConference");
				break;
			case("ChairConferenceDetail"): 
		    			// open create conference page for existing conference
				nextJSPPage = CONFERENCEDETAIL_PAGE;
	    		request.getSession().setAttribute("currentConference", chairController.getConference(request.getParameter("selectedConference"))); 
	    		break;
			case("ChairCreateConference"): 
	    			// receive data for new conference
	    			
	    			// result is feedback of conference controller
	    			try {
	    				// try to create conference
	    				result = chairController.createConference((User) request.getSession().getAttribute("currentUser"), 
		                         request.getParameter("confID"), 
		                         request.getParameter("confName"), 
		                         request.getParameter("confDescription"), 
		                         formatter.parse(request.getParameter("confDate")),
		                         formatter.parse(request.getParameter("confSubmitDeadline")),
		                         formatter.parse(request.getParameter("confReviewDeadline")),
                                Integer.parseInt(request.getParameter("confMaxPages")));	
	    			} catch (Exception e) {
	    				result = "Ungültige Eingabe: " + e.getMessage();
	    			}
	    			
		    		if (result == "") {
		    			// if ok just go to main page
	    			nextJSPPage = MAIN_PAGE;
		    		} else {
		    			// else try again
	    			nextJSPPage = CONFERENCEDETAIL_PAGE;
		    			request.getSession().setAttribute("errortext", result);
		    		}
	    		break;
	    		// TODO: More conference use cases
	    		
	    		//////////////////////////
	    	    // 	Author use cases
	    		//////////////////////////

	    		// TODO: Author use cases

	    		//////////////////////////
	    	    // 	Reviewer use cases
	    		//////////////////////////

	    		// TODO: Reviewer use cases
	    		
			default:
	    		//System.out.println("Else: " + usecase);
				request.getSession().setAttribute("errortext", "Unbekannte Funktion");
				nextJSPPage = INDEX_PAGE;
	    		break;
	    	}
		
		return nextJSPPage;
	}

	
	/**
	 * Just some debugging data for testing.
	 */
	private void createDebuggingData() {
		try {
		
		SimpleDateFormat formatter = new SimpleDateFormat("MM/dd/yyyy");
		
		User u1 = userController.createUser("u1", "Test User 1", "u1", "no address");
		User u2 = userController.createUser("u2", "Test User 2", "u2", "no address");
		User u3 = userController.createUser("u3", "Test User 3", "u3", "no address");
		
		chairController.createConference(u1,
										"c1", 
				                        "Test Conference 1", 
				                        "Test Conference Description 1", 
				                        formatter.parse("11/01/2015"), 
				                        formatter.parse("05/01/2015"), 
				                        formatter.parse("06/01/2015"), 
				                        4);
		
		chairController.createConference(u1,
				"c2", 
                "Test Conference 2", 
                "Test Conference Description 2", 
                formatter.parse("12/01/2015"), 
                formatter.parse("05/01/2015"), 
                formatter.parse("07/01/2015"), 
                6);
		
		chairController.createConferenceDEBUG(u1,
				"c3", 
                "Test Conference 3", 
                "Test Conference Description 3", 
                formatter.parse("5/01/2015"), 
                formatter.parse("01/01/2015"), 
                formatter.parse("04/01/2015"), 
                4);
		
		chairController.createConferenceDEBUG(u1,
				"c4", 
                "Test Conference 4", 
                "Test Conference already after review", 
                formatter.parse("5/01/2015"), 
                formatter.parse("12/01/2014"), 
                formatter.parse("01/01/2015"), 
                4);
		
		} catch (Exception e) {
			// just a debug method; dont care about Exceptions for now..
		}
	}

}
