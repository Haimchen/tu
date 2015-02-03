package de.tuberlin.pes.swtpp.ha03.example.model;

public class Conference {
	private String name;
	private String id;

	// TODO: just examples .... implement more.... 
	
	public Conference(String name, String id) {
		this.name = name;
		this.id = id;
				
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

}
