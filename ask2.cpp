#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <cstdlib>

using namespace std;

class Student;
class Order;
class Class;

class Student {
	private:
		string name;
		int Class;	// number of class
		string gender;
		int behavior; // 0 = quite
	public:
		Student(string nam, int C, string gen); // constructor
		~Student() { /*cout << "student deleted" << endl;*/ }; // destructor
		void print(); // print student's info
		string getGEN(); // returns gender
		int getClass();
		int getBEHAVE();
		void printNAME();
		int behave(); // sets student behavior 1 and returns 0 if it was already 1 or 1 if it was 0
		void good();
};

class Order {
	private:
		Student *s1;
		Student *s2;
		int couple; // number of couple in the order (helps understand which gender is s1 & s2)
		// couple%2 = 0 (s1=M | s2=F) , couple%2=1 (s1=F | s2=M)
		Order* next;
	public:
		Order(int i=1);
		~Order() { /*cout << "sequence deleted" << endl;*/ };
		void enter(Student* st);
		void print();
		int solo(); // returns 0 if no solo students, 1 if solo male, 2 if solo female.
		Student* getLAST();
		int coups(); // return how many couples are in the class
		void getINFO(int &ppl, int &coup, int &f);
		Student* getBAD(); // returns a "bad" student (-> behavior=1)
		Student* swapRANDOM(Student* st); // swaps bad student (st) with random student, and returns random student too in order to re-enter in class
		int sumST(); // return sum of students in the sequence
};

class Class {
	private:
		int num; // number of classes
		Order** sequence; // array of pointers, showing in each sequence of students
		int* coups;
		int* points; // points
	public:
		Class(int K);
		~Class() { /*cout << "class deleted" << endl;*/ };
		void enter(Student* st);
		void print();
		void normalization(); // normalize each class so that if there is a solo male & a solo female, we match them.
		void changes();
		void infoCLASS(int Tquiet, int Tmessy); // gives info if class is quiet or messy
};








//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////








Student::Student(string nam, int C, string gen) {
	name = nam;
	Class = C;
	gender = gen;
	behavior = 0;
	//this->print();
}

Order::Order(int i) {
	s1 = NULL;
	s2 = NULL;
	couple = i;
	next = NULL;
}

Class::Class(int K) {
	num = K;
	sequence = (Order**)malloc(sizeof(Order*)*K);
	coups = (int*)malloc(sizeof(int)*K);
	points = (int*)malloc(sizeof(int)*K);
	for (int i=0 ; i<K ; ++i)
	{
		sequence[i] = new Order(1);
		coups[i] = 1;
		points[i] = 0;
	}
}








//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////








void Student::print() {
	cout << name << " -> " << Class << " -> " << gender << /*" -> " << behavior <<*/ endl;
}

void Student::printNAME() {
	cout << name;
}

string Student::getGEN() { return gender; }

int Student::getClass() { return Class; }

int Student::getBEHAVE() { return behavior; }

//void Student::zero() { behavior = 0; }

void Order::enter(Student* st) {
	int flag=0;
	string gen = st->getGEN();
	Order* n = this;
	if(gen=="Male") //if student is male
	{
		while(flag!=1)
		{
			//cout << n->couple << " ";
			if((n->couple)%2==1) // in couple 1,3,5,... male students, get assigned in s1
			{
				if(n->s1==NULL)
				{
					n->s1 = st;
					flag=1;
				}
			}
			else // in couple 2,4,6,... male students, get assigned in s2
			{
				if(n->s2==NULL)
				{
					n->s2 = st;
					flag=1;
				}
			}

			if(flag==0)
			{
				if(n->next!=NULL)
					n = n->next;
				else
				{
					n->next = new Order(n->couple + 1);
					n = n->next;
				}
			}
		}
	}
	else // if student is female
	{
		while(flag!=1)
		{
			//cout << n->couple << " ";
			if((n->couple)%2==1) // in couple 1,3,5,... female students, get assigned in s2
			{
				if(n->s2==NULL)
				{
					n->s2 = st;
					flag=1;
				}
			}
			else // in couple 2,4,6,... male students, get assigned in s1
			{
				if(n->s1==NULL)
				{
					n->s1 = st;
					flag=1;
				}
			}

			if(flag==0)
			{
				if(n->next!=NULL)
					n = n->next;
				else
				{
					n->next = new Order(n->couple + 1);
					n = n->next;
				}
			}
		}
	}
	//cout << endl;
}

void Class::enter(Student* st) {
	Order* n;
	//cout << "Belongs to class " << st->getClass() << endl;
	n = sequence[st->getClass() - 1];
	n->enter(st);
}

void Order::print() {
	Order *n = this;

	while(n!=NULL)
	{
		if(n->s2==NULL)
		{
			(n->s1)->printNAME();
			cout << endl;
		}
		else if (n->s1==NULL)
		{
			(n->s2)->printNAME();
			cout << endl;
		}
		else
		{
			(n->s1)->printNAME();
			cout << " - ";
			(n->s2)->printNAME();
			cout << endl;
		}
		n = n->next;
	}
}


void Class::print() {
	int i=0;
	for( i=0 ; i<num ; ++i)
	{
		cout << "->Class " << i+1 << "<-" << endl;
		sequence[i]->print();
		cout << endl;
	}
}

int Order::solo() {
	Order *n = this;

	while(n->next!=NULL)
		n = n->next;

	if(n->couple%2==1)
	{
		if(n->s2==NULL) // no female
			return 1;
		else if (n->s1==NULL) // no male
			return 2;
		else
			return 0;
	}
	else
	{
		if(n->s2==NULL) // no male
			return 2;
		else if (n->s1==NULL) // no female
			return 1;
		else
			return 0;
	}
}

Student* Order::getLAST() {
	Student* st;
	Order* n = this;
	Order* temp;
	int flag=0;
	while(flag==0)
	{
		temp = n->next;
		if(temp->next!=NULL)
			n = n->next;
		else
		{
			n->next=NULL;
			if(temp->s1!=NULL)
			{
				st = temp->s1;
				delete temp;
				return st;
			}
			else
			{
				st = temp->s2;
				delete temp;
				return st;
			}
		}
	}
}

void Class::normalization() {
	Student* st;
	//Order* n;
	int i,j;
	int* check = (int*)malloc(sizeof(int)*num);
	for(i=0 ; i<num ; ++i)
	{
		check[i] = sequence[i]->solo();
		//cout << check[i] << endl;
	}
	for(i=0 ; i<num ; ++i)
	{
		for(j=i+1 ; j<num ; ++j)
		{
			if(check[i]==1 && check[j]==2)
			{
				st = sequence[j]->getLAST();
				//st->printNAME();
				cout << endl;
				//n = seuqnce[j];
				sequence[i]->enter(st);
				check[i]=0;
				check[j]=0;
				break;
			}
		}
	}
	for(i=0 ; i<num ; ++i)
	{
		coups[i] = sequence[i]->coups();
		//cout << "Class " << i << " has " << coups[i] << endl;
	}
}

int Order::coups() {
	Order* n = this;
	while(n->next!=NULL)
		n = n->next;

	return n->couple;
}

int Student::behave() {
	if (behavior==0)
	{
		behavior=1;
		return 1;
	}

	return 0;
}

void Student::good() { this->behavior = 0; }

void Order::getINFO(int &ppl, int &coup, int &f) {
	Order* n = this;
	int count=0;
	int c1,c2;

	while (n!=NULL)
	{
		c1=0;
		c2=0;
		if((n->s1)->getBEHAVE()==1)
		{
			++ppl;
			c1=1;
		}

		if((n->s2)->getBEHAVE()==1)
		{
			++ppl;
			c2=1;
		}

		if(c1+c2==2)
		{
			++coup;
			++count;
			if (count>=3)
				f=1;
		}
		else
			count=0;

		n = n->next;
	}
}

Student* Order::getBAD() {
	Order* n = this;
	Student* st = NULL;

	while(n!=NULL) // assign bad student to a TEMPstudent, set his pointer NULL and his behavior to good (=0)
	{
		if(n->s1!=NULL && ((n->s1)->getBEHAVE() == 1) )
		{
			st = n->s1;
			n->s1 = NULL;
			st->good();
			break;
		}
		else if (n->s2!=NULL && ((n->s2)->getBEHAVE() == 1) )
		{
			st = n->s2;
			n->s2 = NULL;
			st->good();
			break;
		}

		n = n->next;
	}

	return st;
}


Student* Order::swapRANDOM(Student* st) {
	Order* n = this;
	Student* temp = NULL;
	int rndm;
	int flag=0;

	while(1)
	{
		if(n==NULL)
		{
			n = this;
			continue;
		}

		rndm=rand()%2;
		if(rndm==0 || n->s1==NULL || n->s2==NULL)
		{
			n = n->next;
			continue;
		}
		else
		{
			if((st->getGEN()) == (n->s1)->getGEN())
			{
				(n->s1)->good();
				temp = n->s1;
				n->s1 = st;
				return temp;
			}
			else
			{
				(n->s2)->good();
				temp = n->s2;
				n->s2 = st;
				return temp;
			}
		}
	}
}



void Class::changes() {
	int** info; // j=0 -> bad ppl | j=1 -> bad couples | j=2 -> if more there are more than 2 sequencial couples (0 or 1)
	info = (int**)malloc(sizeof(int*)*num);
	for(int i=0 ; i<num ; ++i)
		info[i] = (int*)malloc(sizeof(int)*3);

	int ppl, coup, f;
	//int counter=0;
	for(int i=0 ; i<num ; ++i) // we get info for each class ^^
	{
		ppl=0;
		coup=0;
		f=0;
		sequence[i]->getINFO(ppl,coup,f);
		//cout << ppl << "	" << coup << "	" << f << "	";
		info[i][0] = ppl;
		info[i][1] = coup;
		info[i][2] = f;
		//counter++;

		if(coup<=2) // if couples <= 2 then points in class are, as many as the bad students
			points[i] += ppl;
		else
			points[i] += ppl*2; // ppl*2 because for each move, class gets 2 negative points

		//cout << points[i] << endl;
	}

	cout << endl << endl;


	for(int i=0 ; i<num ; ++i) // for each class, we check each student, one-by-one and decide what to with him
	{
		Student* st=NULL;
		Student* TEMPst=NULL;
		int stop=0; // flag which indicates when all students' behavior is 0 for this class.
		if(info[i][1]<=2) // couples <= 2
		{
			//cout << "1" << endl;
			while(stop!=1)
			{
				st = sequence[i]->getBAD();
				if (st==NULL)
					break;
				else
				{
					st->printNAME();
					cout << " -> ";
					TEMPst = sequence[i]->swapRANDOM(st);
					TEMPst->printNAME();
					cout << endl;
					sequence[i]->enter(TEMPst);
				}
			}
		}
		else if(info[i][2]==0) // non-sequencial couples
		{
			//cout << "2" <<endl;
			while(stop!=1)
			{
				st = sequence[i]->getBAD();
				if(st==NULL)
					break;
				else
				{
					st->printNAME();
					cout << " -> ";
					if(i+1<num)
					{
						TEMPst = sequence[i+1]->swapRANDOM(st);
						sequence[i]->enter(TEMPst);
					}
					else
					{
						TEMPst = sequence[0]->swapRANDOM(st);
						sequence[i]->enter(TEMPst);
					}
					TEMPst->printNAME();
					cout << endl;
				}
			}
		}
		else // sequencial couples
		{
			//cout << "3" <<endl;
			while(stop!=1)
			{
				st = sequence[i]->getBAD();
				if(st==NULL)
					break;
				else
				{
					st->printNAME();
					cout << " -> ";
					int rndm = rand()%num;
					while (rndm==i)
						rndm = rand()%2;

					TEMPst = sequence[rndm]->swapRANDOM(st);
					sequence[i]->enter(TEMPst);
					TEMPst->printNAME();
					cout << endl;
				}
			}

		}
	}
	cout << endl << endl;
}

int Order::sumST()
{
	Order* n = this;
	int sum=0;
	while(n!=NULL)
	{
		if(n->s1!=NULL)
			++sum;
		if(n->s2!=NULL)
			++sum;
		n = n->next;
	}
	return sum;
}

void Class::infoCLASS(int Tquiet, int Tmessy)
{
	if (Tquiet<0 || Tquiet>100 || Tmessy<0 || Tmessy>100)
	{
		cout << "Wrong percentage!" << endl;
		return;
	}
	float Quiet,Messy;
	Quiet = ((float)Tquiet)/100;
	Messy = ((float)Tmessy)/100;
	//cout << "Tquiet = " << Quiet << " & Tmessy = " << Messy << endl;
	for(int i=0 ; i<num ; ++i)
	{
		int sum = sequence[i]->sumST();
		//cout << "Class " << i+1 << " has " << sum << " students" << endl;
		if((float)points[i] <= ((float)sum*Quiet))
			cout << "Class " << i+1 << " -> What a quiet class!" << endl;
		else if((float)points[i] >= ((float)sum*Messy))
			cout << "Class " << i+1 << " -> What a mess!" << endl;
		else
			cout << "Class " << i+1 << " -> Please be quiet!" << endl;
	}
}







//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////








int main (int argc, char* argv[]) {
	int i,M=0,FM=0;
	string s;
	Student** st;
	Student** Male;
	Student** Female;
	st = (Student**)malloc(sizeof(Student*)*36);

	ifstream inputFile;
	inputFile.open("students.txt");
	if (!inputFile)
	{
		cout << "Cannot find input file" << endl;
		exit(-1);
	}

	int infoC,j=0;
	string token,nam,gen;
	string delimiter = "-";
	string info;
	i=0;

	while( getline (inputFile,s))
	{
		size_t pos = 0;
		size_t last = 0;
		size_t next = 0;
		j=0;
		while ((next = s.find(delimiter, last)) != string::npos)
		{
			if (j==0)
			{
				info = s.substr(last, next-last);
				//cout << info << endl;
				last = next + 1;
				++j;
				nam = info;
			}
			else
			{
				info = s.substr(last, next-last);
				stringstream geek(info);
				infoC=0;
				geek >> infoC;
				//cout << test << endl;
				last = next + 1;
				++j;
			}
		}

		info = s.substr(last);
		gen = info;
		//cout << test << endl;

		/*if(gen=="Male")
			++M;
		else
			++FM;*/

		st[i] = new Student(nam,infoC,gen);
		++i;

	}
	inputFile.close();

	int K, L, Tquiet, Tmessy;

	K = atoi(argv[1]);
	L = atoi(argv[2]);
	Tquiet = atoi(argv[3]);
	Tmessy = atoi(argv[4]);

	//cout << endl << "Found " << M << " males and " << FM << " females" << endl;

	//split(st,Male,Female,M,FM,K);

	Class c(K);


	for(i=0 ; i<36 ; ++i)
	{
		c.enter(st[i]);
	}

	c.print();
	cout << endl;

	c.normalization();
	cout << endl;

	c.print();
	cout << endl;

	cout << endl << endl;

	long now;
	now=time(NULL); // now is current time
	srand((unsigned int)now); // definition of rand() -> gave here current time as seed

	int ppl; //how many students we choose every time
	int choose; // chose student
	int stdnt;

	for(i=1 ; i<=L ; ++i)
	{
		ppl = rand()%36;
		j=0;
		while(j<=(ppl))//
		{
			stdnt = rand()%36;
			choose = st[stdnt]->behave();
			if (choose==1)
			{
				++j;
				st[stdnt]->printNAME();
				cout << endl;
			}
		}
		cout << endl;

		c.changes();
		c.print();
	}

	cout << endl << endl;
	c.infoCLASS(Tquiet,Tmessy);
	cout << endl;

	for(i=0 ; i<36 ; ++i)
		delete st[i];
	delete st;
}
