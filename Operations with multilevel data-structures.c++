#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <string>
using namespace std;

class course {
public:
	string name;
	int section;
	int credits;
	course() {}
	course(string n, int s, int c) { name = n; section = s; credits = c; }
	bool operator<(course c) { return (name < c.name); }
	bool operator==(course c) { return (name == c.name); }
};

void add_student(map<int, map<int, vector<course *> * > > &DB, int id);
void remove_student(map<int, map<int, vector<course *> * > > &DB, int id);
void add_course(map<int, map<int, vector<course *> * > > &DB, int semester, int id, course c); //20171 Spring semester of 2017; 20172: Fall semester of 2017

void drop_course(map<int, map<int, vector<course *> * >> &DB, int semester, int id, course c);
void print_student_semester_courses(map<int, map<int, vector<course *> * >> &DB, int semester, int id);
void print_student_all_courses(map<int, map<int, vector<course *> * > > &DB, int id);
void print_DB(map<int, map<int, vector<course *> * > > &DB);

int main() {
	map<int, map<int, vector<course *> *> > DB;
	add_student(DB, 11111);
	course C1("CIS554", 1, 3), C2("CSE674", 1, 3), C3("MAT296", 8, 4), C4("WRT205", 5, 3);
	
	add_course(DB, 20171, 11111, C1);
	add_course(DB, 20171, 11111, C4);
	add_course(DB, 20171, 11111, C3);
	add_course(DB, 20171, 11111, C2);
	print_student_semester_courses(DB, 20171, 11111);

	drop_course(DB, 20171, 11111, C1);
	print_student_semester_courses(DB, 20171, 11111);

	add_course(DB, 20172, 11111, C2);
	add_course(DB, 20172, 11111, C4);
	add_course(DB, 20172, 11111, C3);
	add_course(DB, 20172, 11111, C1);
	print_student_all_courses(DB, 11111);

	add_student(DB, 11112);
	add_course(DB, 20171, 11112, C2);
	add_course(DB, 20171, 11112, C4);
	add_course(DB, 20171, 11112, C3);
	add_course(DB, 20171, 11112, C1);
	print_student_semester_courses(DB, 20171, 11112);

	add_course(DB, 20172, 11112, C2);
	add_course(DB, 20172, 11112, C4);
	add_course(DB, 20172, 11112, C3);
	add_course(DB, 20172, 11112, C1);
	print_student_semester_courses(DB, 20172, 11112);

	print_student_all_courses(DB, 11112);

	print_DB(DB);
	remove_student(DB, 11111);
	
	print_DB(DB);

	getchar();
	getchar();
	return 0;
}

void add_student(map<int, map<int, vector<course *> * > > &DB, int id) {
	auto it1 = DB.find(id);
	if (it1 == DB.end()) {
		map<int, vector<course *>*> *l = new map<int, vector<course *>*>;
		DB[id] = *l;
  } else
  return;
}

void remove_student(map<int, map<int, vector<course *> * > > &DB, int id) {
	auto it1 = DB.find(id);
	if (it1 !=DB.end()) {
		auto it2 = it1->second;
		auto it3 = it2.begin();
		while (it3 != it2.end()) {
			auto it4 = it3->second;
			for (size_t i = 0; i < (*it4).size(); i++)
			{
				delete (*it4)[i];	
			}
			(*it4).clear();
			delete it4;
			it3++;
		}
		it2.clear();
	DB.erase(id);
	}
		return;
}

void add_course(map<int, map<int, vector<course *> * > > &DB, int semester, int id, course c) {
	auto it1 = DB.find(id);
	if (it1 != DB.end()) {
		auto it2 = it1->second;
		auto it3 = it2.find(semester);//we check if semester is present
		if (it3 == it2.end()) {//if semester not present, create new semester and new course
			vector<course *> *it4 = new vector<course *>;
			course *g = new course(c);
			(*it4).push_back(g);
			it2[semester] = it4;
		}
		else {//semester present, create new course alone
			auto it4 = it3->second;
			auto it5 = (*it4);
			for (size_t i = 0; i < it5.size(); i++) {
				if (*(it5[i]) == c) // course already exists so dont do anything
					return;
			}
			course *d = new course(c);
			it4->push_back(d);
			it2[semester] = it4;
		}
		DB[id] = it2;
	}
}

void drop_course(map<int, map<int, vector<course *> * > > &DB, int semester, int id, course c) {
	auto it1 = DB.find(id);
	if (it1 != DB.end()){
		auto it2 = it1->second;
		auto it3 = it2.find(semester);
		if (it3 != it2.end()) {
			auto it4 = it3->second;
			
			for (size_t i = 0; i < (*it4).size();i++){
				if (c == *(*it4)[i]) {
				
					(*it4).erase((*it4).begin()+i );
					break;
				}
			}
		}
	}
	return;
}

void print_student_semester_courses(map<int, map<int, vector<course *> * > > &DB, int semester, int id) {
	
	cout <<"\nstudent id = " << id << endl;
	auto it1 = DB.find(id);
	if (it1 != DB.end()) {
		auto it2 = it1->second;
		
		auto it3 = (it2.find(semester))->second;
		cout << "semester  = " << semester << endl;
		for (size_t i = 0; i < (*it3).size(); i++) {
			cout << (*it3)[i]->name << " "<< (*it3)[i]->section<<" "<< (*it3)[i]->credits<<"  ";
	
		}
	  cout << endl;
	}	
}
void print_student_all_courses(map<int, map<int, vector<course *> * > > &DB, int id) {
	
	cout << endl << "student id = " << id <<endl;
	auto it1 = DB.find(id);
	if (it1 != DB.end())
	{
		auto it2 = it1->second;
		auto it3 = it2.begin();
	
		while (it3 != it2.end()) {			
			cout << "semester = " << it3->first<<endl;
			auto it4 = it3->second;
			for (size_t i = 0; i < (*it4).size(); i++)
				cout << (*it4)[i]->name << " " << (*it4)[i]->section << " "<<(*it4)[i]->credits << "  ";
			cout << endl;
			it3++;
		}	
	}	
}

void print_DB(map<int, map<int, vector<course *> * > > &DB) {

	auto it1 = DB.begin();
	cout << endl;
	while (it1 != DB.end())
	{
		cout << "\nstudent id = " << it1->first;
		auto it2 = it1->second;
		auto it3 = it2.begin();
		while (it3 != it2.end()) {
			cout << "\nsemester = " << it3->first<<endl;
			auto it4 = it3->second;
			for (size_t i = 0; i < (*it4).size(); i++) {
				cout << (*it4)[i]->name << " " << (*it4)[i]->section << " " << (*it4)[i]->credits<<"  ";
			}
			it3++;
		}
		it1++;
	}
}
