package de.tuberlin.pes.swtpp.ha03.example.control;

import java.util.LinkedList;

import de.tuberlin.pes.swtpp.ha03.example.model.User;

/**
 * This class implements all general user-related use cases.
 */
public class UserController {

	private LinkedList<User> users;
	
	/**
	 * This controller needs the global list of users. The list must not be null. 
	 * @param users
	 */
	public UserController(LinkedList<User> users) {
		this.users = users;
	}
	
	/**
	 * Returns true if a user with the given ID exists in the list of users.
	 * @param id
	 * @return
	 */
	public boolean checkUserExists(String id) {
		for (User u: users) if (u.getId().toLowerCase().equals(id.toLowerCase())) return true;
		
		return false;
	}
	
	/**
	 * Checks user credentials.
	 * 
	 * @param id
	 * @param pwd
	 * @return true if User/Pwd exist
	 */
	public boolean checkUserPwd(String id, String pwd) {
		for (User u: users) 
			if (u.getId().toLowerCase().equals(id.toLowerCase())) 
				if (u.getPwd().equals(pwd)) return true; 
				else return false;
		
		return false;
	}
	
	/**
	 * Returns user object with given ID or null if it does not exist.
	 * 
	 * @param id
	 * @return
	 */
	public User findUserByID(String id) {
		for (User u: users) if (u.getId().toLowerCase().equals(id.toLowerCase())) return u;
		
		return null;
	}
	
	/**
	 * Use Case create user. All data supplied must not be empty. The id is a unique key. The user can not be created if the ID exists already. The created user is returned. 
	 * 
	 * @param id
	 * @param name
	 * @param pwd
	 * @param contactInfo
	 * @return
	 */
	public synchronized User createUser(String id, String name, String pwd, String contactInfo) {
		
		if (id == null || name == null || pwd == null || contactInfo == null || id == "" || name == "" || pwd == "") return null;
		
		if (checkUserExists(id)) return null;
			
		User u = new User(name, id, pwd);
		
		u.setContactInfo(contactInfo);
		
		users.add(u);
		
		return u;
	}
	
}
