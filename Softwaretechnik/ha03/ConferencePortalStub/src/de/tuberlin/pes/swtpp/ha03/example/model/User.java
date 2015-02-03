package de.tuberlin.pes.swtpp.ha03.example.model;

import java.util.LinkedList;

public class User {
	private String name = "";
	private String id = "";
	private String pwd = "";
	private String contactInfo = "";

	private LinkedList<Conference> hostedConferences;
	// TODO: add more attibutes that represent associations of class model

	public User(String name, String id, String pwd) {
		this.name = name;
		this.id = id;
		this.pwd = pwd;
		
		hostedConferences = new LinkedList<Conference>();
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
}
