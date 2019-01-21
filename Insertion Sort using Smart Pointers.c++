#include <iostream>
#include <memory>
using namespace std;

class node {
public:
	int value;
	//node * next;
	//node * previous;
	shared_ptr<node> next;
	weak_ptr<node> previous;
	//node(int i) { value = i; next = previous = nullptr; }
	node(int i) { value = i; }
	//node() { next = previous = nullptr; }
	node() {}
};

class doubly_linked_list {
public:
	//node * head;
	//node * tail;
	shared_ptr<node> head, tail;
	//doubly_linked_list() { head = tail = nullptr; }
	doubly_linked_list() {}
	void make_random_list(int m, int n);
	void print_forward();
	void print_backward();

	 //implement insertion_sort following the special requirements.
	void insertion_sort();
};

void doubly_linked_list::make_random_list(int m, int n) {

	for (int i = 0; i < m; i++) {
		//node * p1 = new node(rand() % n);
		shared_ptr<node> p1 = make_shared<node>(rand() % n);
		p1->previous = tail;
		//if (tail != nullptr ) tail->next = p1;
		if (tail) tail->next = p1;
		tail = p1;
		//if (head == nullptr) head = p1;
		if (!head) head = p1;
	}
}

void doubly_linked_list::print_forward() {
	cout << endl;
	//node * p1 = head;
	shared_ptr<node> p1 = head;
	//while (p1 != nullptr) {
	while (p1) {
		cout << p1->value << " ";
		p1 = p1->next;
	}
}

void doubly_linked_list::print_backward() {
	cout << endl;
	//node * p1 = tail;
	shared_ptr<node> p1 = tail;
	//while (p1 != nullptr) {
	while (p1) {
		cout << p1->value << " ";
		p1 = p1->previous.lock();
	}
}
void doubly_linked_list::insertion_sort() {
	if (head == tail) { return; }
	doubly_linked_list L2;
	L2.head = head;
	L2.tail = head;
	head = head->next;
	head->previous.reset();//makes nullptr
	L2.head->next.reset();
	while (head) {
		if (head->value <= L2.head->value) {
			shared_ptr<node>temp2 = head->next;
			head->next = L2.head;
			L2.head->previous = head;
			L2.head = head;
			head = temp2;
			if(head)
			head->previous.reset();
		}
		else if (head->value >= L2.tail->value) {
			head->previous = L2.tail;
			L2.tail->next = head;
			shared_ptr<node>temp2 = head->next;
			L2.tail = head;
			L2.tail->next.reset();
			head = temp2;
			if(head)
			head->previous.reset();
		}
		else
		for (shared_ptr<node> l2ptr = L2.head; l2ptr !=nullptr; l2ptr = l2ptr->next) {
			if (l2ptr->value >= head->value) {
				shared_ptr<node> temp2 = head->next;
				shared_ptr<node> temp = l2ptr->previous.lock();
				temp->next = head;
				head->next = l2ptr;
				l2ptr->previous = head;
				head->previous = temp;
				head = temp2;
				if(head)
				head->previous.reset();
				break;
			}
		}
	}
	head = L2.head;
	tail = L2.tail;
}

int main() {
	doubly_linked_list d1;
	d1.make_random_list(30, 10);
	d1.print_forward();
	d1.print_backward();
	d1.insertion_sort();
	d1.print_forward();
	d1.print_backward();
	getchar();
	getchar();
	return 0;
}
