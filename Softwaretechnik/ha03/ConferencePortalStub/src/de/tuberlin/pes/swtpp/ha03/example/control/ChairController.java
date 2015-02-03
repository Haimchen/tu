package de.tuberlin.pes.swtpp.ha03.example.control;

import java.util.Date;
import java.util.LinkedList;

import de.tuberlin.pes.swtpp.ha03.example.model.Conference;
import de.tuberlin.pes.swtpp.ha03.example.model.User;

/**
 * This class implements all conference-chair-related use cases.
 */
public class ChairController {

	// list of conferences in the system
	private LinkedList<Conference> conferences;
	
	// list of users registered in the system
	private LinkedList<User> users;

	/**
	 * The conference chair requires visibility to the global lists of conferences and users. The lists must not be null.
	 * 
	 * @param conferences
	 * @param users
	 */
	public ChairController(LinkedList<Conference> conferences, LinkedList<User> users) {
		this.conferences = conferences;
		this.users = users;
	}
	
	/**
	 * Returns conference object with given ID.
	 * 
	 * @param id
	 * @return
	 */
	public Conference getConference(String id) {
		for (Conference c: conferences) {
			if (c.getId().toLowerCase().equals(id.toLowerCase())) return c;
		}
		return null;
	}
	
	/**
	 * Implements the Use Case "create conference". The conference was successfully registered in the system when the result is "". If the data supplied did not meet the requirements, 
	 * the result holds the first error message.
	 * 
	 * @param chair
	 * @param id
	 * @param name
	 * @param description
	 * @param dateOfConference
	 * @param submitDeadline
	 * @param reviewDeadline
	 * @param maxPages
	 * @return error message or empty String
	 */
	public String createConference(User chair, String id, String name, String description, Date dateOfConference, Date submitDeadline, Date reviewDeadline, int maxPages) 
	{
		// TODO check parameters
		
		Conference conference = new Conference(name, id);
		
		chair.addHostedConference(conference);
		conferences.add(conference);
		
		// TODO more...
		
		return "";
	}
	
	/**
	 * Implements the Use Case "create conference", but the dates which are entered are not compared to current date. Just for debugging: Create conferences that are already behind deadlines.
	 * 
	 * @param chair
	 * @param id
	 * @param name
	 * @param description
	 * @param dateOfConference
	 * @param submitDeadline
	 * @param reviewDeadline
	 * @param maxPages
	 * @return
	 */
	public String createConferenceDEBUG(User chair, String id, String name, String description, Date dateOfConference, Date submitDeadline, Date reviewDeadline, int maxPages) {
		// TODO implement ... 
		return "Not yet implemented";
	}
	
}
