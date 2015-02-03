package de.tuberlin.pes.swtpp.ha03.example.test;

import static org.junit.Assert.*;

import java.util.LinkedList;

import org.junit.Before;
import org.junit.Test;

import de.tuberlin.pes.swtpp.ha03.example.control.UserController;
import de.tuberlin.pes.swtpp.ha03.example.model.User;

public class UserControllerTest {

	LinkedList<User> users; 
	UserController controller;
	
	@Before
	public void setUp() throws Exception {
		// create test controller
		users = new LinkedList<User>();
		controller = new UserController(users);
		
		// Create test users;
		User u1 = new User("Me", "me1", "abc");
		User u2 = new User("You", "you1", "12345");
		
		users.add(u1);
		users.add(u2);
	}

	@Test
	public void testCheckUserExists() {
		if (!controller.checkUserExists("me1")) fail("Existing user not found"); 
		
		if (controller.checkUserExists("notexistinguser")) fail("Not existing user found");
	}

	@Test
	public void testCheckUserPwd() {
		fail("Not yet implemented");
	}

	@Test
	public void testFindUserByID() {
		fail("Not yet implemented");
	}

	@Test
	public void testCreateUser() {
		fail("Not yet implemented");
	}

}
