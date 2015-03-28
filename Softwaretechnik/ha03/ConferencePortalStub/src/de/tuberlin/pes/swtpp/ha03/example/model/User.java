package de.tuberlin.pes.swtpp.ha03.example.model;

import java.util.ArrayList;
import java.util.LinkedList;

public class User {
	private String name = "";
	private String id = "";
	private String pwd = "";
	private String contactInfo = "";

	private LinkedList<Conference> hostedConferences;
	private ArrayList<Paper> writtenPapers;
	private ArrayList<Review> writtenReviews;
	private ArrayList<Paper> papersToReview;


	public User(String name, String id, String pwd) {
		this.name = name;
		this.id = id;
		this.pwd = pwd;
		
		hostedConferences = new LinkedList<Conference>();
		papersToReview = new ArrayList<Paper>();
		writtenPapers = new ArrayList<Paper>();
		writtenReviews = new ArrayList<Review>();
	}
	
	public Boolean equals(User otherUser){
		return otherUser.getId().equals(this.getId());
	}
	
	public void addPaperToReview(Paper paper){
		if (isReviewingPaper(paper)){
			return;
		}
		this.papersToReview.add(paper);
	}
	
	public Boolean isReviewingPaper(Paper paper){
		return papersToReview.contains(paper);
	}
	
	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}
	public String getId() {
		return id;
	}
	public void setId(String id) {
		this.id = id;
	}
	public String getPwd() {
		return pwd;
	}
	public void setPwd(String pwd) {
		this.pwd = pwd;
	}
	public String getContactInfo() {
		return contactInfo;
	}
	public void setContactInfo(String contactInfo) {
		this.contactInfo = contactInfo;
	}

	public LinkedList<Conference> getHostedConferences() {
		return hostedConferences;
	}
	public void addHostedConference(Conference conference) {
		this.hostedConferences.add(conference);
	}
	
	/**
	 * This message can be used "informs" a user. The system does not have to actually send a mail (or else). 
	 * 
	 * @param message
	 */
	public void informUser(String message) {
		System.out.println("User " + id + "(" + name + ") -> Message: " + message);
	}

	public ArrayList<Paper> getWrittenPapers() {
		return writtenPapers;
	}

	public void setWrittenPapers(ArrayList<Paper> writtenPapers) {
		this.writtenPapers = writtenPapers;
	}

	public ArrayList<Paper> getPapersToReview() {
		return papersToReview;
	}

	public void setPapersToReview(ArrayList<Paper> papersToReview) {
		this.papersToReview = papersToReview;
	}

	public ArrayList<Review> getWrittenReviews() {
		return writtenReviews;
	}

	public void setWrittenReviews(ArrayList<Review> writtenReviews) {
		this.writtenReviews = writtenReviews;
	}
}
