#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;



////------------global variable area ----------////
static int global_room_counter = 0;
static int global_creature_counter = 0;



////-------------------------------------------////
class Creature{
  
protected:
  int creature_id;
  int room_id;
  int type;

public:
  Creature(int id, int t, int room): creature_id(id), type(t), room_id(room){
	  global_creature_counter +=1;
	  
  }
  const string creature_label[3] = {"PC", "Animal", "NPC"};
  
  int get_room_Id(){
    return room_id;
  }

  int get_Type(){
    return type;
  }
  
  void print_description(){
    cout << "creature " << creature_id << " is in room " << room_id << " with type: " 
	<< creature_label[type] <<" \n" ;
  }

  void print_type_and_room(){
	  cout << creature_label[type] << " " << room_id;
  }
  void leave_room(string neighbor){
    //newRoom = get Creature.currentRoom.randomNeighbor
    //this.room_id = newRoom
  }
  
  /*
  void change_room_state(string command){
		if(command == "clean"){
			existing_rooms.at(room_id).clean();
		}else if (command == "dirty"){
			existing_rooms.at(room_id).dirty();
		}
	}*/
};


vector<Creature> existing_creatures;


class Room{
protected:

  int id;
  int neighbors_id[4] = {-1,-1,-1,-1};
  int current_room_state;
 // const string room_state_label[3] = {"dirty", "half-dirty", "clean"};
 // Room* neighbor_array[4];
  vector <Creature> cre_arr;
 
public:
  const string neighbor_label[4] = {"north","south","east","west"};
  const string state_label[3] = {"clean", "half-dirty" , "dirty"};

  
 /* int clean(){
	  if(current_room_state == 0 ){
		  cout<< "The room is already clean.";
	  }else{
		  current_room_state -= 1;
	  }
	  
  }*/
  
  /*
  int dirty(){
	  if(current_room_state == 2){
		  cout<< "The room is already dirty.";
		  
		  }else{
		  current_room_state += 1;
		  }
	  
  }*/
  int get_room_Id (){
	  return id;
  }
  void look_inside(){
    //print info
    cout << "\nRoom "  << id << ", " << state_label[current_room_state] << ", " ;
	for(int i = 0; i < 4; i++){
		if(neighbors_id[i]!= -1){
			cout << "neighbor " << neighbors_id[i] << " " << 
			"to the " << neighbor_label[i] << ", " ;
		}
	}
	cout << "contains: ";
	for(int i = 0; i < cre_arr.size(); i++){
		cout << " \n";
		cre_arr.at(i).print_type_and_room();
	}
	
  }
  
  void put_creature(Creature c){
	  cre_arr.push_back(c);
	  
  }
  Room(int room_id, int state, int north_id, int south_id, int east_id, int west_id){
	  id = room_id;
	  neighbors_id[0] = north_id;
	  neighbors_id[1] = south_id;
	  neighbors_id[2] = east_id;
	  neighbors_id[3] = west_id;
	  current_room_state = state;
	  global_room_counter +=1;
  }

};

vector<Room> existing_rooms;



class PC: public Creature{

protected:

int respect_value = 40;

public:
  PC(int id, int type, int room): Creature(id, type, room){};
  
void print_type_and_room(){
	  cout << creature_label[type];
  }
  
int get_respect_value (){
	return respect_value;
} 

vector<string> pc_parse_user_command(string line){
	string delimiter = ":";
	size_t pos = 0;
	string token;
	vector<string>vect;
	while ((pos = line.find(delimiter)) != string :: npos) {
		token = line.substr(0, pos);
		vect.push_back(token);
//		cout << token << std::endl;
		line.erase(0, pos + delimiter.length());
		}
		vect.push_back(line);
		//cout << s << std::endl;
		//cout<< "\n size " <<vect.size() ;
		return vect;
}





void process_user_command (vector<string> parsed_input){
	/*if(parsed_input.size() == 1){
		if(parsed_input.at(0) == "clean" || parsed_input.at(0) == "dirty"){
			change_room_state(parsed_input.at(0));
		}
		if(parsed_input.at(0) == "north" || 
		parsed_input.at(0) == "south" ||
		parsed_input.at(0) == "east" ||
		parsed_input.at(0) == "west"){
		leave_room(parsed_input.at(0));
		}
		if(parsed_input.at(0) == "look"){
			//get the room it stays in and call look_inside(); 
		}
		
	}else if(parsed_input.size()== 2){
		
	}*/
}

		
};

class NPC: public Creature{
	 //{"dirty", "half-dirty", "clean"};
protected:
  int preference = 1;// 1 and down half-dirty/dirty
public:
  NPC(int id, int type, int room): Creature(id, type, room){};


};

class Animal: public Creature{
	 //{"dirty", "half-dirty", "clean"};
	protected:
	int preference = 1 ;// 1 and up /half dirty or clean
	public:
	Animal(int id, int type, int room):Creature(id, type, room){};
	
};

// global variables
/*void play(){
	 string line;
  ifstream myfile ("input.txt");
  if(myfile.is_open()){
	  while(getline(myfile,line)){
		  cout << line << '\n';
	  }
	  myfile.close();
  }else{
	  cout << "Unable to open file";
  }
	
	//return 0;
	
}*/




static vector<int> parse_string_into_integers(string line){
	vector<int> vect;
	int i;
	stringstream ss(line);
	while(ss>>i){
		vect.push_back(i);
		if(ss.peek()==' ')
			ss.ignore();
	}
	return vect;
	//printing double check
	/*for (i = 0; i <vect.size(); i++){
		cout << vect.at(i) << " followed by ";
	}*/
}



static void play(){
	string line;
	int number_of_rooms = 0;
	int number_of_creatures = 0;
	ifstream myfile("input.txt");
	if(myfile.is_open()){
		getline(myfile,line); 
		//get n of rooms
		//create rooms using the following n lines of input
		number_of_rooms = atoi(line.c_str());
		cout << number_of_rooms << " rooms are waiting to be described. \n";
		for(int i = 0; i < number_of_rooms; i++){
			getline(myfile,line);
			vector<int> room_param = parse_string_into_integers(line);
			Room newRoom (global_room_counter, 
			room_param.at(0), 
			room_param.at(1), 
			room_param.at(2), 
			room_param.at(3),
			room_param.at(4)
			);
			existing_rooms.push_back(newRoom);
			//TODO: check this again
			//Room *rm = &newRoom;
			//existing_rooms.push_back(rm);
		}
		//get n of creatures
		//create Creatures using the following n lines of input;
		getline(myfile,line);
		number_of_creatures = atoi(line.c_str());
		cout << number_of_creatures << " Creatures waiting to be added to the room \n";
		for(int i = 0; i < number_of_creatures; i ++){
			getline(myfile,line);
			vector<int> creature_param = parse_string_into_integers(line);
			
			if(creature_param.at(0)==0){
				PC newCreature = PC(global_creature_counter, creature_param.at(0),
					creature_param.at(1));
				existing_rooms.at(creature_param.at(1)).put_creature(newCreature);
				existing_creatures.push_back(newCreature);
			}else if(creature_param.at(0)==1){
				Animal newCreature= Animal(global_creature_counter, creature_param.at(0),
					creature_param.at(1));
				existing_rooms.at(creature_param.at(1)).put_creature(newCreature);
				existing_creatures.push_back(newCreature);
			}else{
				NPC newCreature = NPC(global_creature_counter, creature_param.at(0),
					creature_param.at(1));
				existing_rooms.at(creature_param.at(1)).put_creature(newCreature);
				existing_creatures.push_back(newCreature);
			}
			
			
			
		}
		//end taking the virtual world inputs. Start parsing user commands;
		
		
		while(getline(myfile,line)){ //compare
			if(line.compare("exit")!=0){
				//process user command should be part of only the PC object
				//process_user_command(line);
			}
		}
	}else{
		cout << "Unable to open file";
	}
	
}



int main () {
  //const int global_room_counter = 0;
	
  /*Creature cre (1);
  cout << "Type of this creature:" << cre.get_Type() <<"\n" ;*/
  //PC pc(0,1,0);
  //pc.print_type_and_room();
 /* pc.print_Type();
 
/* NPC npc(1,2,1);
  npc.print_Type();
  /*
  Room room1 (0,0,-1,1,-1,2);
  room1.look_inside();
  */

/*
  Room *newRoom = new Room(global_room_counter, 
			0, 1, -1, -1, 2
			);
		cout << "global room counter" << global_room_counter ;
			//existing_rooms.push_back(newRoom);
			//cout << existing_rooms.at(0)->look_inside() << "hello";*/
  /*Room rm = Room(global_room_counter, -1,1,1,2,1);
  cout << "global room counter" << global_room_counter ;*/
  
  play();
  /*for(int i = 0; i < existing_creatures.size(); i++){
  existing_creatures.at(i).print_description();}*/

  
  for(int i = 0; i < existing_rooms.size(); i++){
  existing_rooms.at(i).look_inside();
  }
  /*
 PC pc(0,1,0);
 vector<string> vector = pc.pc_parse_user_command("2:clean");
 cout << "\n" << vector.at(0) << " and " << vector.at(1);

 Animal animal(0,1,0);
 animal.print_type_and_room();
 PC pc2(0,1,0);
 pc2.print_type_and_room();*/
  
  return 0;
}
