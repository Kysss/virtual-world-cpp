#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
using namespace std;





class Room;

class Creature{
	protected:
	int id;
	int type;
	Room* room;

	
	public:
	
	string type_label[3] = {"PC", "Animal", "NPC"}; 
	
	Creature (int id, int type, Room* room){
		this->id = id;
		this->type = type;
		this->room = room;
		
	}
	
	Room* get_room(){
		return room;
	}
	int get_Id(){
		return id;
	}
	
	int get_Type(){
		return type;
	}
	
	string get_type_label(){
		return type_label[type];
	}
	
	
	void changeRoom(Room* newRoom){
		room = newRoom;
	}
	
	
	
	bool leaveRoom();
	void clean();
	void dirty();
	void make_comfortable();
};


class Room{
protected:
  int id;
  int type;
  Room* north;
  Room* south;
  Room* east;
  Room* west;
  int current_room_state;
  Creature* creatures[10] = {NULL};
 
public:
  Room(int id, int state){
	  this->id = id;
	  this->current_room_state = state;
	 // this->creatures = {NULL};
  }
  string state_label[3]= {"clean", "half-dirty", "dirty"}; 
  
  virtual ~Room(){
	  for(int i = 0; i < 10; i++){
		  if(creatures[i]!= NULL){
			  delete creatures[i];
			  creatures[i] = NULL;
		  }
	  }
	  cout << "destructor called" << endl;
  }
  
  string get_state_label(){
		return state_label[current_room_state];
	}
  void init_Neighbors(Room* neighbors[4]){
	  north = neighbors[0];
	  south = neighbors[1];
	  east = neighbors[2];
	  west = neighbors[3];
  }
 
  int clean(){
	  if(current_room_state == 0 ){
		  cout<< " The room is already clean." << endl;
	  }else{
		  current_room_state -= 1;
		  cout << " The room is cleaned." << endl;
		  
	  }
	  
  }
  
  
  int dirty(){
	  if(current_room_state == 2){
		  cout<< " The room is already dirty." << endl;
		  
		  }else{
		  current_room_state += 1;
		  cout << " The room is dirtied." << endl;
		  }
	 
  }
  
  
  
  
  
  void change_room_state(string line){
	  //cout << "change_room_state (string line) is called" << endl;
	  if(line == "clean"){
		  clean();
	  }else if (line == "dirty"){
		  dirty();
	  }
	  
	  
  }
  
  Room* getNorth(){
	  return north;
	  
  }
  
  Room* getSouth(){
	  return south;
  }
  
  Room* getEast(){
	  return east;
  }
  
  Room* getWest(){
	  return west;
  }
  int get_room_Id (){
	  return id;
  }
  
  int get_room_state(){
	  return current_room_state;
  }
  
  void change_room_state(string action, Creature *c);

  
  void add_Creature(Creature* c){
	  for(int i = 0; i<10; i++){
		  if(creatures[i] == NULL){
			  creatures[i] = c;
			  break;
		  }
	  }
  }
  
  void remove_Creature(Creature* c){
	  for(int i = 0; i < 10; i++){
		  if(creatures[i]!=NULL){
			  if(creatures[i]->get_Id() == c->get_Id()){
				  creatures[i] = NULL;
				  break;
			  }
		  }
	  }
  }
  
  bool has_Creature(Creature* c){
	  for(int i = 0; i <10; i++){
		  if(creatures[i]!=NULL){
			  if(creatures[i]->get_Id() == c->get_Id()){
				  return true;
			  }
		  }
	  }
	  return false;
  }
  
  void creatureLeft();
  void look_inside(){
    //print info
    cout << "\nRoom "  << id  << ", " << "state:" << get_state_label() << "; ";
	if(north!=NULL){
		cout << "neighbor " << north->get_room_Id() << " to the north; ";
		
	}
	if(south!=NULL){
		cout<< "neighbor " << south->get_room_Id() << " to the south; ";
		
	}
	if(east!=NULL){
		cout<< "neighbor " << east->get_room_Id() << " to the east; ";
		
	}
	
	if(west!=NULL){
		cout << "neighbor " << west->get_room_Id() << " to the west; "; 
	}
	cout << "contains: " << endl;
	for(int i = 0; i < 10; i++){
		
		if(creatures[i]!=NULL){
			if(creatures[i]->get_Type() == 0){
				//PC
				cout << creatures[i]->get_type_label() << endl;
			}else if (creatures[i]->get_Type() == 1){
				//Animal
				cout << creatures[i]->get_type_label() << " " << creatures[i]->get_Id() << endl;
				
			}else{
				//NPC
				cout << creatures[i]->get_type_label() << " " << creatures[i]->get_Id() << endl;
			}
			
		}
	}
	cout << "\n" ;
  }

};

class NPC: public Creature{
	protected:
	public:
//	const string type_label[3] = {"PC", "Animal", "NPC"};
	NPC(int id, int type, Room* room): Creature(id,type,room){};
	
	void print_info(){
		cout << "NPC "  << this->get_Id();
	}
	bool react(string action, Creature* c);
	
	
	
	
};

class Animal: public Creature{
	protected:
	public:

	Animal(int id, int type, Room* room): Creature(id,type,room){};
	
	void print_info(){
		cout << "Animal "  << this->get_Id();
	}
	
	bool react(string action, Creature* c);
	
	
	
	
};


class PC: public Creature{
	protected:
	int respect = 40;
	
	
	public:
	PC(int id, int type, Room* room): Creature(id,type,room){};
	int get_respect(){
		return respect;
	}
	void print_info(){
		cout << "PC " ;
	}
	void update_respect(int difference){
		respect = respect + difference;
	}
	bool react(string action, Creature* c);
	
	
};

extern PC* pc = NULL;
static int room_counter = 0;
static int cre_counter = 0;

//if were forced to leave to a neighbor room due to the PC action,
//check current room state and make corresponding room state changes 
void Creature:: make_comfortable(){
	int type = this->get_Type();
	if(type ==1 ){
		if(this->get_room()->get_state_label()=="dirty"){
			cout << "Animal " << this->get_Id() << " does not like the room state: so it cleaned it. ";
			this->get_room()->clean();
		}
	}else if(type ==2){
		if(this->get_room()->get_state_label()=="clean"){
			cout << "NPC " << this->get_Id() << " does not like the room state: so it dirtied it. ";
			this->get_room()->dirty();
		}
	}
}

//leaving the room *randomly* 
//in this case making it check in the order or N,S,E,and W
//return true if leaveRoom is successful
//return false if no room is available to go to
bool Creature:: leaveRoom(){
	Room* oldRoom = this->room;
	if(room->getNorth()!=NULL){
		//check isFull () :
		oldRoom->getNorth()->add_Creature(this);
		this->changeRoom(oldRoom->getNorth());
		oldRoom->remove_Creature(this);
		cout << this->get_type_label() << " " << this->get_Id() << " leaves to the north." << endl;
		this->make_comfortable();
		return true;
	}
	if(room->getSouth()!=NULL){
		oldRoom->getSouth()->add_Creature(this);
		this->changeRoom(oldRoom->getSouth());
		oldRoom->remove_Creature(this);
		cout<< this->get_type_label() << " " << this->get_Id() << " leaves to the south." << endl;
		this->make_comfortable();

		return true;
	}
	if(room->getEast()!=NULL){
		oldRoom->getEast()->add_Creature(this);
		this->changeRoom(oldRoom->getEast());
		oldRoom->remove_Creature(this);
		cout<< this->get_type_label() << " " << this->get_Id() << " leaves to the east." << endl;
		this->make_comfortable();

		return true;
	}
	if(room->getWest()!=NULL){
		oldRoom->getWest()->add_Creature(this);
		this->changeRoom(oldRoom->getWest());
		oldRoom->remove_Creature(this);
		cout<< this->get_type_label() << " " << this->get_Id() << " leaves to the west." << endl;
		this->make_comfortable();

		return true;
	}
	
	
	return false;
	
	
}

//creature was being forced to clean
void Creature::clean(){
	//cout << "Creature: clean is called " << endl;
	this->room->change_room_state("clean", this);
}

//creature was being forced to dirty
void Creature:: dirty(){
	//cout << "Creature:dirty is called." << endl;
	this->room->change_room_state("dirty", this);
}

//return a boolean
//react by updating PC respect
//if room state is not within the preference, leave the room,
//if PC calls command, do normal react
//if PC calls the creature to do certain actions, do vigorious react 
bool Animal::react(string action, Creature* c){
		if(c->get_Id()!=this->get_Id()){
			//normal react 
			if(action == "clean"){
				cout << this->get_Id() << " licks your face 1 time." << endl;
				pc->update_respect(1);
			}else{
				cout << this->get_Id() << " growls at you 1 time. " << endl;
				pc->update_respect(-1);
				if(this->get_room()->get_state_label() == "dirty"){
					 return this->leaveRoom();
					//TODO;
				}
				
			}
			return true;
		
		
		}else{
			//vigorous react when the PC commands the Animal itself to do actions 
			if(action == "clean"){
				cout << this->get_Id() << " licks your face 3 times!" << endl;
				pc->update_respect(3);
			}else{
				cout << this->get_Id() << " growls at you 3 times!" << endl;
				pc->update_respect(-3);
				if(this->get_room()->get_state_label() == "dirty"){
					
					return this->leaveRoom();
				}
			}
			return true;
			
			
		}
	}
	
bool PC:: react(string action, Creature* c){
	return true;
}

bool NPC:: react(string action, Creature* c){
	if(c->get_Id()!=this->get_Id()){
			//normal react
			// if the caller is pc itself
			if(action == "dirty"){
				cout << this->get_Id() << " smiles at you." << endl;
				pc->update_respect(1);
			}else{
				cout << this->get_Id() << " grumbles at you. " << endl;
				pc->update_respect(-1);
				if(this->get_room()->get_state_label() == "clean"){
					 return this->leaveRoom();
					
				}
				
			}
			return true;
			
		}else{
			//vigorous react
			if(action == "dirty"){
				cout << this->get_Id() << "smiles at you for a long time." << endl;
				pc->update_respect(3);
			}else{
				cout << this->get_Id() << "grumbles a lot!" << endl;
				pc->update_respect(-3);
				if(this->get_room()->get_state_label() == "clean"){
					
					return this->leaveRoom();
				}
			}
			return true;
			
			
		}
}


void Room:: creatureLeft(){
	cout << "Creature Left called." << endl;
	for(int i = 0; i <10; i ++){
		if(creatures[i]!= NULL){
			if(creatures[i]->get_Type() == 0){
				  //PC
				  PC* pc;
				  pc = static_cast<PC*>(creatures[i]);
				  pc->react("bad", pc); //will be unhappy but won't do any action yet.
				 
				  
			  }else if (creatures[i]->get_Type() == 1){
				  //Animal
				  Animal* animal;
				  animal = static_cast<Animal*>(creatures[i]);
				  animal->react("bad", pc); // will react bad once
				  
			  }else{
				  //NPC
				  NPC* npc;
				  npc = static_cast<NPC*>(creatures[i]);
				  npc->react("bad", pc); //will react bad once
				 
			  }
			
			
		}
	}
	
}
  void Room :: change_room_state(string action, Creature* c){
	 // cout << "change_room_state (string action, creature c) is called " << endl;
	  //add conditions what if the room is already clean or already dirty
	  if(action == "clean"){
		  current_room_state-=1 ;
	  }else{
		  current_room_state+=1;
	  }
	  
	  for(int i = 0; i < 10; i++){
		  if(creatures[i]!=NULL){
			  if(creatures[i]->get_Type() == 0){
				  //PC
				  PC* pc;
				  pc = static_cast<PC*>(creatures[i]);
				 // cout << "PC stay?!" <<endl;
				  if(!pc->react(action,c)){
					  c= pc;
				  //returns false when the creatures tries to leave but 
				  //rooms are full
				  cout << "There is no where for " << creatures[i]->get_Id() << " to stay. It left the house!" << endl;
				  delete creatures[i]; //deleting the pointer for the creature that left
				  creatures[i] = NULL;
				  this->creatureLeft();
				  }
				  
			  } else if (creatures[i]->get_Type() == 1){
				  //Animal
				  Animal* animal;
				  animal = static_cast<Animal*>(creatures[i]);
				  if(!animal->react(action,c)){
				  //returns false when the creatures tries to leave but 
				  //rooms are full
				  cout << "There is no where for " << creatures[i]->get_Id() << " to stay. It left the house!" << endl;
				  delete creatures[i];
				  creatures[i] = NULL;
				  this->creatureLeft();
				  }
			  }else{
				  //NPC
				  NPC* npc;
				  npc = static_cast<NPC*>(creatures[i]);
				  if(!npc->react(action,c)){
				  //returns false when the creatures tries to leave but 
				  //rooms are full
				  cout << "There is no where for " << creatures[i]->get_Id() << " to stay. It left the house!" << endl;
				  delete creatures[i];
				  creatures[i] = NULL;
				  this->creatureLeft();
				  }
			  }
				 	
		  } 
		  
	  }
	  
  }
  
  vector<string> parse_user_command(string line){
	string delimiter = ":";
	size_t pos = 0;
	string token;
	vector<string>vect;
	while ((pos = line.find(delimiter)) != string :: npos) {
		token = line.substr(0, pos);
		vect.push_back(token);

		line.erase(0, pos + delimiter.length());
		}
		vect.push_back(line);

		return vect;
	}
  
int main(){
	
	/*Room r(1,1);
	cout << r.get_room_Id();
	Creature c(0, 5,&r);
	cout << c.get_Type();
	PC pc (0,5,&r);
	NPC npc(0,4,&r);
	Animal a (1,2,&r);
	pc.print_info();
	npc.print_info();
	a.print_info();*/
	
	
	//------------------------creating rooms-----------------------
	string number_of_rooms;
	int room_size;
	cout << "Welcome. Before we start the game, we needed you to enter the room and creature info..." << endl;
	cout << "How many rooms?" << endl;
	cin >> number_of_rooms;
	room_size = stoi(number_of_rooms);
//	cout << room_size;
	
	Room* existing_rooms[room_size] = {NULL}; //array of created rooms
	int neighbors[room_size][4];
	cout<< "Please enter 5 numbers describing those rooms, with 5 numbers each row, in order." << endl;
	for(int i = 0; i < room_size; i ++){
		string sstate, sn, ss, se,sw;
		cout<< "Please enter the information for room " << i << ":" << endl;
		cin >> sstate >> sn >> ss>> se>> sw;
		
		//int rs;
		//rs = stoi(sstate);
		existing_rooms[i] = new Room(i, stoi(sstate));
		
		
		neighbors[i][0] = stoi(sn);
		neighbors[i][1] = stoi(ss);
		neighbors[i][2] = stoi(se);
		neighbors[i][3] = stoi(sw);
		
	}
	
	for(int i = 0; i < room_size; i ++){
		Room* neighs[4] = {NULL};
		for(int j = 0; j < 4; j ++){
			// cout<< neighbors[i][j] << "  " ;
			if(neighbors[i][j]!= -1){
				neighs[j] = existing_rooms[neighbors[i][j]];
			}
		}
		existing_rooms[i]->init_Neighbors(neighs);
		/*for (int i = 0; i < room_size; i++){
			delete neighs[i];
			} */
	}
	//--------------------------end creating rooms---------------------------
	//-------------------------creating and putting creatures----------------
	string number_of_creatures;
	
	cout << "How many creatures do you want to put into those rooms?" << endl;
	cin >> number_of_creatures;
	int creature_size;
	creature_size = stoi(number_of_creatures);
	Creature* existing_creatures[creature_size] = {NULL};
	
	cout << "Please enter 2 numbers describing the creature," << endl;
	cout << "with creature type followed by the room number. " << endl;
	for(int i = 0; i < creature_size; i++){
		cout << "Enter the informaion for creature " << i << ":" << endl;
		string stype, sroom;
		cin >> stype >> sroom;
		if(stoi(stype) == 0){
			//PC
			existing_creatures[i] = new PC(i, stoi(stype), existing_rooms[stoi(sroom)]);
			pc = static_cast<PC*>(existing_creatures[i]);
		}else if(stoi(stype) == 1){
			//Animal
			existing_creatures[i] = new Animal(i, stoi(stype), existing_rooms[stoi(sroom)]);
		}else {
			//NPC
			existing_creatures[i] = new NPC(i, stoi(stype), existing_rooms[stoi(sroom)]);
			
		}
		existing_rooms[stoi(sroom)]->add_Creature(existing_creatures[i]);
	}
	
	//-----------------------------------end putting in creatures------------------------
	//cout << "room id:" << existing_rooms[0]->get_room_Id();
	//cout << "room id:" << existing_rooms[1]->get_room_Id();
	//cout << "room id:" << existing_rooms[2]->get_room_Id();
	
	/*existing_rooms[0]->look_inside();
	existing_rooms[1]->look_inside();
	existing_rooms[2]->look_inside();
	
	cout << pc->get_respect();
	cout << pc->get_room()->get_room_Id();*/
	//----------------------------start taking in commands ----------------------------------
	
	string command;
	while(command!= "exit" && pc->get_respect()<80 && pc->get_respect()>0){
		//cout << "Welcome!" << endl;
		cout << "Enter a command:" << endl;
		cin >> command;
		if(command == "help"){
			cout << "Your goal of this game is to achieve a respect of 80. " << endl;
			cout << "You will lose if your respect value falls below 0" << endl;
			cout << "Available commands are: clean, dirty, north, south, east, west. " << endl;
			cout << "In addition to these, [creature id]:[commands] " << endl;
			cout << "is also available to force other creatures to do actions. For example, " << endl;
			cout << "'2:clean' will force creature whose ID is 2 to clean the current room. " << endl;
		
		} else if(command == "exit"){

			cout << "GoodBye!" << endl;
			break;
		} else if(command == "look"){
			pc->get_room()->look_inside();
			
		} else if(command == "north"){
			if(pc->get_room()->getNorth()!=NULL){
				Room* oldRoom = pc->get_room();
				Room* newRoom = existing_rooms[pc->get_room()->getNorth()->get_room_Id()];
				pc->changeRoom(newRoom);
				newRoom->add_Creature(pc);
				oldRoom->remove_Creature(pc);
				cout << "You have moved to the north, Room " << newRoom->get_room_Id() << "." << endl;
			}else{
				cout << "There is no room to the north. Please try again.!" << endl;
			}
		} else if(command == "south"){
			if(pc->get_room()->getSouth()!=NULL){
				Room* oldRoom = pc->get_room();
				Room* newRoom = existing_rooms[pc->get_room()->getSouth()->get_room_Id()];
				pc->changeRoom(newRoom);
				newRoom->add_Creature(pc);
				oldRoom->remove_Creature(pc);
				cout << "You have moved to the south, Room " << newRoom->get_room_Id() << "." << endl;
			}else{
				cout << "There is no room to the south. Please try again.!" << endl;
			}
		} else if(command == "east"){
			if(pc->get_room()->getEast()!=NULL){
				Room* oldRoom = pc->get_room();
				Room* newRoom = existing_rooms[pc->get_room()->getEast()->get_room_Id()];
				pc->changeRoom(newRoom);
				newRoom->add_Creature(pc);
				oldRoom->remove_Creature(pc);
				cout << "You have moved to the east, Room " << newRoom->get_room_Id() << "." << endl;
			}else{
				cout << "There is no room to the east. Please try again.!" << endl;
			}
		} else if(command == "west"){
			if(pc->get_room()->getWest()!=NULL){
				Room* oldRoom = pc->get_room();
				Room* newRoom = existing_rooms[pc->get_room()->getWest()->get_room_Id()];
				pc->changeRoom(newRoom);
				newRoom->add_Creature(pc);
				oldRoom->remove_Creature(pc);
				cout << "You have moved to the west, Room " << newRoom->get_room_Id() << "." << endl;
			}else{
				cout << "There is no room to the west. Please try again.!" << endl;
			}
		} else if(command == "clean"){
			if(pc->get_room()->get_state_label()!="clean"){
				pc->get_room()->change_room_state("clean", pc);
				cout << "Respect is now " << pc->get_respect()<< endl;
			}else{
				cout<< "The room is already clean." << endl;
			}
			
		} else if(command == "dirty"){
			if(pc->get_room()->get_state_label()!="dirty"){
				pc->get_room()->change_room_state("dirty", pc);
				cout << "Respect is now " << pc->get_respect()<< endl;
			}else{
			cout << "The room is already dirty. " << endl;
			} 
		}else{
			// --------------------assumming if command consists of a semicolon--------------
			vector<string> c = parse_user_command(command);
			//c[0] is the creature id
			//c[1] is the action
			int cc_id = stoi(c[0]);
			string action = c[1];
			if(action == "clean"){
				if(pc->get_room()->get_state_label()!="clean"){
					existing_creatures[cc_id]->clean(); //calls change_room_state within clean()
					cout << "Respect is now " << pc->get_respect() << endl;
				}else{
					cout << "The room is already clean." << endl;
				}
				
			}else if (action == "dirty"){
				if(pc->get_room()->get_state_label()!="dirty"){
					existing_creatures[cc_id]->dirty();
					cout << "Respect is now " << pc->get_respect() << endl;
				}else{
					cout<< "The room is already clean" << endl;
				}
				
			}else if (action == "north"){
				
				int nr_id = existing_creatures[cc_id]->get_room()->getNorth()->get_room_Id();
				Room* newRoom = existing_rooms[nr_id];
				Room* oldRoom = existing_creatures[cc_id]->get_room();
				existing_creatures[cc_id]->changeRoom(newRoom);
				newRoom->add_Creature(existing_creatures[cc_id]);
				oldRoom->remove_Creature(existing_creatures[cc_id]);
				
				cout << cc_id << " moves to the north." << endl;
				existing_creatures[cc_id]->make_comfortable();
				
			}else if (action == "south"){
				
				int nr_id = existing_creatures[cc_id]->get_room()->getSouth()->get_room_Id();
				Room* newRoom = existing_rooms[nr_id];
				Room* oldRoom = existing_creatures[cc_id]->get_room();
				existing_creatures[cc_id]->changeRoom(newRoom);
				newRoom->add_Creature(existing_creatures[cc_id]);
				oldRoom->remove_Creature(existing_creatures[cc_id]);
				
				cout << cc_id << " moves to the south." << endl;
				existing_creatures[cc_id]->make_comfortable();
			}else if (action == "east"){
				
				int nr_id = existing_creatures[cc_id]->get_room()->getEast()->get_room_Id();
				Room* newRoom = existing_rooms[nr_id];
				Room* oldRoom = existing_creatures[cc_id]->get_room();
				existing_creatures[cc_id]->changeRoom(newRoom);
				newRoom->add_Creature(existing_creatures[cc_id]);
				oldRoom->remove_Creature(existing_creatures[cc_id]);
				
				cout << cc_id << " moves to the east." << endl;
				existing_creatures[cc_id]->make_comfortable();
			}else if (action == "west"){

				int nr_id = existing_creatures[cc_id]->get_room()->getWest()->get_room_Id();
				Room* newRoom = existing_rooms[nr_id];
				Room* oldRoom = existing_creatures[cc_id]->get_room();
				existing_creatures[cc_id]->changeRoom(newRoom);
				newRoom->add_Creature(existing_creatures[cc_id]);
				oldRoom->remove_Creature(existing_creatures[cc_id]);
				
				cout << cc_id << " moves to the west." << endl;
				existing_creatures[cc_id]->make_comfortable();
			}else{
				cout << "Sorry, either the creature does not exist or the command is not available. Please try again." 
				<< endl;
			}
			
			
			
		}
		
		if(pc->get_respect() <0){
				cout << "Sorry, you lost! Maybe try again later ... " << endl;
			}
			if(pc->get_respect() > 80){
				cout << "You won! You are given a piece of chocolate as a price!" << endl;
			}
		
		
		
	}
	return 0;
	
}
