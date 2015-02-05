package de.tuberlin.pes.swtpp.ha03.example.control;

import java.util.Calendar;
import java.util.Date;
import java.util.LinkedList;

import de.tuberlin.pes.swtpp.ha03.example.model.Conference;
import de.tuberlin.pes.swtpp.ha03.example.model.Paper;
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
	 * Returns User object with given ID.
	 * 
	 * @param id
	 * @return
	 */
	public User getUserById(String id) {
		for (User user: users) {
			if (user.getId().equalsIgnoreCase(id)) return user;
		}
		return null;
	}
	/**
	 * Implements the Use Case "create conference". The conference was successfully registered in the system when the result is "". 
	 * If the data supplied did not meet the requirements, 
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
		// create Conference
		Conference newConference = new Conference(name, id, description, dateOfConference, submitDeadline, reviewDeadline, maxPages);
		
		// check if ID is unique
		for(Conference conference : conferences){
			Boolean equals = newConference.equals(conference);
			if(equals){
				return "ID already exists";
			}
		}
		
		// check Dates
		Boolean submitDeadlineValid = newConference.isSubmitDeadlineValid();
		if(!submitDeadlineValid){
			return "Submission Deadline is in the past";
		}
		
		Boolean reviewDeadlineValid = newConference.isReviewDeadlineValid();
		if(!reviewDeadlineValid){
			return "Review Deadline is to close to Submission Deadline.";
		}
		
		Boolean dateOfConferenceValid = newConference.isDateValid();
		if(!dateOfConferenceValid){
			return "Date of Conference is to close to Review Deadline!";
		}
	
		// Conference is valid and can be added
		chair.addHostedConference(newConference);
		conferences.add(newConference);
		
		return "";
	}
	
	/**
	 * Implements the Use Case "create conference", but the dates which are entered are not compared to current date. 
	 * Just for debugging: Create conferences that are already behind deadlines.
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
	
	/**
	 * Implements the use case "assign reviewer".
	 * @param reviewer
	 * @param paper
	 */
	public String assignReviewer(String reviewerId, String paperTitle, Conference conference){
		User reviewer = getUserById(reviewerId);
		Paper paper = conference.getPaperByTitle(paperTitle);
		
		User author = paper.getAuthor();
		if (author!= null && author.equals(reviewer)){
			return "Author can not be added as reviewer!";
		}
		
		reviewer.addPaperToReview(paper);
		return "";
	}
	
}
