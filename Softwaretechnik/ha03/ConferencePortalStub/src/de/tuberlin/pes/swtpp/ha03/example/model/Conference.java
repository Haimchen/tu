package de.tuberlin.pes.swtpp.ha03.example.model;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;

public class Conference {
	private String name;
	private String id;
	private String description;
	private Date dateOfConference;
	private Date submitDeadline;
	private Date reviewDeadline;
	private int maxPages;
	private ArrayList<Paper> submittedPapers;
	
	private Boolean closed = false;
	private double minPointsAvg = 1;
	private double minPointsTotal = 3;
	private int minPapersPublished = 0;
	private double pointsAccepted = 3;
	private double pointsBorderline = 1;
	private double pointsRejected = -2;
	

	
	public Conference(String name, String id, String description, Date dateOfConference, Date submitDeadline, Date reviewDeadline, int maxPages) {
		this.name = name;
		this.id = id;
		this.description = description;
		this. dateOfConference = dateOfConference;
		this.submitDeadline = submitDeadline;
		this.reviewDeadline = reviewDeadline;
		this.maxPages = maxPages;
		this.submittedPapers = new ArrayList<Paper>();
				
	}
	
	
	public String getId() {
		return id;
	}

	public void setId(String id) {
		this.id = id;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public String getDescription() {
		return description;
	}

	public void setDescription(String description) {
		this.description = description;
	}

	public Date getDateOfConference() {
		return dateOfConference;
	}

	public void setDateOfConference(Date dateOfConference) {
		this.dateOfConference = dateOfConference;
	}

	public Date getFirstDeadline() {
		return submitDeadline;
	}

	public void setFirstDeadline(Date firstDeadline) {
		this.submitDeadline = firstDeadline;
	}

	public Date getSecondDeadline() {
		return reviewDeadline;
	}

	public void setSecondDeadline(Date secondDeadline) {
		this.reviewDeadline = secondDeadline;
	}

	public int getMaxPages() {
		return maxPages;
	}

	public void setMaxPages(int maxPages) {
		this.maxPages = maxPages;
	}

	public ArrayList<Paper> getSubmittedPapers() {
		return submittedPapers;
	}

	public void setSubmittedPapers(ArrayList<Paper> submittedPapers) {
		this.submittedPapers = submittedPapers;
	}
	
	public void addSubmittedPaper(Paper paper) {
		// TODO: check if paper already submitted?
		submittedPapers.add(paper);
	}

	public Boolean getClosed() {
		return closed;
	}

	public void setClosed(Boolean closed) {
		this.closed = closed;
	}

	public double getMinPointsAvg() {
		return minPointsAvg;
	}

	public void setMinPointsAvg(double minPointsAvg) {
		this.minPointsAvg = minPointsAvg;
	}

	public double getMinPointsTotal() {
		return minPointsTotal;
	}

	public void setMinPointsTotal(double minPointsTotal) {
		this.minPointsTotal = minPointsTotal;
	}

	public int getMinPapersPublished() {
		return minPapersPublished;
	}

	public void setMinPapersPublished(int minPapersPublished) {
		this.minPapersPublished = minPapersPublished;
	}

	public double getPointsAccepted() {
		return pointsAccepted;
	}

	public void setPointsAccepted(double pointsAccepted) {
		this.pointsAccepted = pointsAccepted;
	}

	public double getPointsBorderline() {
		return pointsBorderline;
	}

	public void setPointsBorderline(double pointsBorderline) {
		this.pointsBorderline = pointsBorderline;
	}

	public double getPointsRejected() {
		return pointsRejected;
	}

	public void setPointsRejected(double pointsRejected) {
		this.pointsRejected = pointsRejected;
	}
	
	public boolean equals(Conference otherConference){
		return this.getId().equalsIgnoreCase(otherConference.getId());
	}

	/**
	 * checks if Submission Deadline is in the future
	 * @return 
	 */
	public Boolean isSubmitDeadlineValid() {
		Calendar submitDeadlineCal = Calendar.getInstance();
		submitDeadlineCal.setTime(this.submitDeadline); 
		Calendar now = Calendar.getInstance();
		if(now.after(submitDeadlineCal)){
			return false;
		}
		return true;
	}

	/**
	 * checks if review deadline is at least 4 weeks later that submission deadline
	 * @return
	 */
	public Boolean isReviewDeadlineValid() {
		
		Calendar reviewDeadlineCal = Calendar.getInstance();
		reviewDeadlineCal.setTime(this.reviewDeadline); 
		Calendar submitDeadlineCal = Calendar.getInstance();
		submitDeadlineCal.setTime(this.submitDeadline);
		submitDeadlineCal.add(Calendar.DATE, 28);
		if(submitDeadlineCal.after(reviewDeadlineCal)){
			return false;
		}
		return true;
	}

	/**
	 * checks if Date of Conference is at least 4 weeks later than review deadline
	 * @return
	 */
	public Boolean isDateValid() {
		Calendar dateOfConferenceCal = Calendar.getInstance();
		dateOfConferenceCal.setTime(dateOfConference);
		Calendar reviewDeadlineCal = Calendar.getInstance();
		reviewDeadlineCal.setTime(this.reviewDeadline); 
		reviewDeadlineCal.add(Calendar.DATE, 28);
		if(reviewDeadlineCal.after(dateOfConferenceCal)){
			return false;
		}
		return true;
	}


	public Paper getPaperByTitle(String paperTitle) {
		String title;
		for(Paper paper : submittedPapers){
			title = paper.getTitle();
			if(title.equalsIgnoreCase(paperTitle)){
				return paper;
			}
		};
		return null;
	}

}
