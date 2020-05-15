#include <iostream>
using namespace std;



class process
{
public:
  int id;               //each process will have its own unique id
  int waiting_time;     //holds total waiting time of process
  int tunraround_time;  //holds total turnaround time of process
  int response_time;    // holds response time of process
  int start_time;       //time when processs begins
  int end_time;         //time when entire process is completed
  int queue_pos;        //time when process will enter queue after I/O burst
  int burst_pos;        //index pointing to current position in process data array
  int burstSize;        //size of process data array
  int next_burst;       //burst size of next CPU burst to run in process
  int arr[32];          //process data array

  process();            //initialize process
  void setBurst(int i, int p[], int s);
  void Ttr();           //update turnaround time
  void Tw(int t);       //update waiting time
  void Tr(int t);       //update respnse time
  int addTime();        //return CPU burst
  bool endOB();         //returns true if process data is complete
  int setNextPos(int t);//update position in process data array for next burst
  void cpyArr(const int a[], int n);//copy array fuction
};


class Item
{
public:
	process p;   //stores all process data
	Item *next;  //pointer to next process in queue
	Item()      //initialize node
	{
		next = 0;
	}
	Item(process p)//initilize process to node
	{
		this->p = p;
		next = 0;
	}

};


class queueList
{

	int size;      //size of queue
	Item *front;   //pointer to front of queue
	Item *back;    //pointer to end of queue

public:
	queueList();                       //default constructor
	void push_back(Item *a);           //push node to back of queue
	Item * pop_front();                //pop node from front of queue
	void insert(Item *a, int t);       //insert node into queue at position t
	int getSize();                     //get size of queue
  bool isEmpty();                    //check if queue is empty
  int findPos(int e);                //returns position in queue that the the queue_pos of the node before it < e
                                    //and the queue_pos of the node after is > e
  void display(ostream &out, int t);//displays processes in queue
  void IOdisplay(ostream &out, int t);//displays processes in I/O burst
};




int main()
{
  //experiment variables (used for tracking time and other data)
  Item *pop;
  int time = 0;//execution time
  int cs = 0; //context switch counter
  //store times for each process
  int all_Tw [8];
  int all_Ttr [8];
  int all_Tr [8];
  //time when cpu is not running
  int idle_time = 0;
  //average times of each process
  double avg_Tw = 0;
  double avg_Ttr = 0;
  double avg_Tr = 0;
  //percentage of cpu usage
  double cpu_u = 0;

  //create process
  //declare process data array
  //get size of process data array
  //set process data in process data structure
  process p1;
  int burst1[] = {5, 27, 3, 31, 5, 43, 4, 18, 6, 22, 4, 26, 3, 24, 4};
  int n1 = sizeof(burst1)/sizeof(burst1[0]);
  p1.setBurst(1, burst1, n1);

  process p2;
  int burst2[] = {4, 48, 5, 44, 7, 42, 12, 37, 9, 76, 4, 41, 9, 31, 7, 43, 8};
  int n2 = sizeof(burst2)/sizeof(burst2[0]);
  p2.setBurst(2, burst2, n2);

  process p3;
  int burst3[] = {8, 33, 12, 41, 18, 65, 14, 21, 4, 61, 15, 18, 14, 26, 5, 31, 6};
  int n3 = sizeof(burst3)/sizeof(burst3[0]);
  p3.setBurst(3, burst3, n3);

  process p4;
  int burst4[] = {3, 35, 4, 41, 5, 45, 3, 51, 4, 61, 5, 54, 6, 82, 5, 77, 3};
  int n4 = sizeof(burst4)/sizeof(burst4[0]);
  p4.setBurst(4, burst4, n4);

  process p5;
  int burst5[] = {16, 24, 17, 21, 5, 36, 16, 26, 7, 31, 13, 28, 11, 21, 6, 13, 3, 11, 4};
  int n5 = sizeof(burst5)/sizeof(burst5[0]);
  p5.setBurst(5, burst5, n5);

  process p6;
  int burst6[] = {11, 22, 4, 8, 5, 10, 6, 12, 7, 14, 9, 18, 12, 24, 15, 30, 8};
  int n6 = sizeof(burst6)/sizeof(burst6[0]);
  p6.setBurst(6, burst6, n6);

  process p7;
  int burst7[] = {14, 46, 17, 41, 11, 42, 15, 21, 4, 32, 7, 19, 16, 33, 10};
  int n7 = sizeof(burst7)/sizeof(burst7[0]);
  p7.setBurst(7, burst7, n7);

  process p8;
  int burst8[] = {4, 14, 5, 33, 6, 51, 14, 73, 16, 87, 6};
  int n8 = sizeof(burst8)/sizeof(burst8[0]);
  p8.setBurst(8, burst8, n8);


  //initialize queue
  queueList queue;
  //push elements into queue
  queue.push_back(new Item(p1));
  queue.push_back(new Item(p2));
  queue.push_back(new Item(p3));
  queue.push_back(new Item(p4));
  queue.push_back(new Item(p5));
  queue.push_back(new Item(p6));
  queue.push_back(new Item(p7));
  queue.push_back(new Item(p8));




  //fcfs algorithm
  while(!queue.isEmpty())
  {
    pop = queue.pop_front(); //get the next element in queue

    //conditional to track any idle time in scheduler
    if(pop->p.queue_pos > time)
    {
      idle_time += (pop->p.queue_pos - time); //update idle time
      time = pop->p.queue_pos;                //update execution time
    }

    //print context switch data
    cout << "Context Switch:  " << cs << endl
        << "\tExecution Time:  " << time << endl
        << "\tRunning Process:  " << pop->p.id << endl;
    cout << "Ready Queue:  " << endl;
    queue.display(cout, time);
    cout << "Processes in I/O:  " << endl;
    queue.IOdisplay(cout, time);

    cs++;


    //update waiting time and response time
    pop->p.Tw(time);
    pop->p.Tr(time);

    //update execution time to be current time + CPU burst size
    //this is where the execution time will be at for the next iteration of the loop
    time += pop->p.addTime();

    //conditional to check if process is completed
    if(!(pop->p.endOB()))
    {
      int e = pop->p.setNextPos(time); //e is the time process will end I/O burst and enter queue again
      int in = queue.findPos(e);//in is the position in queue node pop will be inserted at
                                //queue is in ascending order based on queue_pos(process variable) or e(main variable)
      queue.insert(pop, in); //insert process into queue at position in
      cout << endl << endl;
    }
    else //process is completed
    {
      //display end of process
      cout << "Process " << pop->p.id << " has completed its total execution" << endl << endl << endl;
      pop->p.end_time = time; //update end time
      pop->p.Ttr();           //update turnaround time

      //store wanted data in array for future calculations
      all_Tw[pop->p.id - 1] = pop->p.waiting_time;
      all_Ttr[pop->p.id - 1] = pop->p.tunraround_time;
      all_Tr[pop->p.id - 1] = pop->p.response_time;
    }
  }



  //calculating the averages
  for(int i = 0; i < 8; i++)
  {
    avg_Tw += all_Tw[i];
    avg_Ttr += all_Ttr[i];
    avg_Tr += all_Tr[i];
  }

  avg_Tw = avg_Tw / 8;
  avg_Ttr = avg_Ttr / 8;
  avg_Tr = avg_Tr / 8;
  cpu_u = time - idle_time;
  cpu_u = (cpu_u / time) * 100;

  //display end results of CPU Simulation
    cout << "Total time:  " << time << endl
    << "CPU Utilization:  " << cpu_u << "%" << endl << endl;

    for (int i = 0; i < 8; i++)
    {
      cout << "Waiting Time P" << i+1 << ":  " << all_Tw[i] << endl;
    }
    cout << "Average Waiting Time:  " << avg_Tw << endl << endl;

    for (int i = 0; i < 8; i++)
    {
      cout << "Turnaround Time P" << i+1 << ":  " << all_Ttr[i] << endl;
    }
    cout << "Average Turnaround Time:  " << avg_Ttr << endl << endl;

    for (int i = 0; i < 8; i++)
    {
      cout << "Response Time P" << i+1 << ":  " << all_Tr[i] << endl;
    }
    cout << "Average Response Time:  " << avg_Tr << endl << endl;






  return 0;
}





process::process()
{         //set default values to variables in node
  id = 0;
  waiting_time = 0;
  response_time = -1;
  tunraround_time = 0;
  start_time = 0;
  end_time = 0;
  queue_pos = 0;
  burst_pos = 0;
}

void process::setBurst(int i, int p[], int s)
{       //initialize id, next_burst, and arr variables
  id = i;
  next_burst = p[0];
  cpyArr(p, s);
}

void process::cpyArr(const int a[], int n)
{     //standard copy array function
  this->burstSize = n;
  for(int i = 0; i < n; i++)
    this->arr[i] = a[i];
}

void process::Ttr()
{   //calculate turnaround time
  tunraround_time = end_time - start_time;
}

void process::Tw(int t)
{   //calculate waiting time
  waiting_time = waiting_time + (t - queue_pos);
}

void process::Tr(int t)
{   //calculate response time
  if (response_time == -1)
    response_time = t - start_time;
}

int process::addTime()
{    //return burst size
  return next_burst;
}

bool process:: endOB()
{       //if array index is at the last CPU burst (end of process) return true
  if ((burstSize - burst_pos) == 1)
    return true;
  return false;
}

int process:: setNextPos(int t)
{
  queue_pos = t + arr[burst_pos + 1]; //queue_pos will hold time when process enters queue after I/O burst
  burst_pos += 2;                     //update burst_pos to next burst in array
  next_burst = arr[burst_pos];        //next_burst variable holds the next CPU burst of the process
  return queue_pos;
}

queueList::queueList()
{           //set default values to queue
	front = back = nullptr;
	size = 0;
}

void queueList::push_back(Item *a)
{           //push process to the back of queue
	if(front == nullptr)//if queue is empty
		front = a;        //node a will be front of queue

	if (back != nullptr)//if queue is not empty
		back->next = a;  //back node pointer to next is node a

	a->next = nullptr; //node a pointer to next will be null(back of queue)
	back = a;          //back node in queue will be node a

	size++;        //update queue size
}

Item * queueList::pop_front()
{
  size--;         //update queue size
  Item *q = front;//store node q as front node
  if (size == 0)  //if last process in queue
  {               //set front and back node to nullptr
    front = nullptr;
    back = nullptr;
  }
  else
  {
    front = front->next;//front pointer is now pointing to next process in queue
  }
	return q;      //return process in front of queue
}
void queueList::insert(Item *a, int t)
{
  Item *p = front;  //declare iterator that will start at front of queue
	for (int i = 1; i < t; i++)
	{
		p = p->next;  //traverse queue up to position t
	}
	a->next = p->next; //insert node a into at iterator pointer
	p->next = a;

	size++;         //update queue size
}
int queueList::getSize()
{       //return size of queue
	return size;
}

bool queueList::isEmpty()
{       //returns true if queue is empty
  if (size == 0)
    return true;
  return false;
}

int queueList::findPos(int e)
{
  Item *t = front;    //declare iterator that will start at front of queue

  int pos = 0;        //keep track of position in queue
  while(t != nullptr)//traverse queue using iterator t
  {
                        //e is the queue_pos of a node we want to insert into queue (that is stored in ascending order)
    if(t->p.queue_pos > e)//if e > iterator's queue_pos, it will enter queue too soon (before it should enter the queue)
      return pos;
    pos++;            //pos will increase at each iteration if e is too large
    t = t->next;
  }
  return pos;
}
void queueList::display(ostream &out, int t)
{
  int empty = 0;    //variable to keep track if queue is empty
	Item *w = front;  //declare iterator that will start at front of queue
	for (int i = 0; i < size; i++)
	{
		if (w == nullptr) //condition in case linked list breaks
			break;
    if ( w->p.queue_pos <= t) //if process is in queue at execution time t
    {             //display process in queue information
      cout << "\tProcess:  " << w->p.id << "\tBurst:  " << w->p.next_burst <<  endl;
      empty++;
    }
		w = w->next;
	}
  if(empty == 0) //if no elements in queue at execution time t
    cout << "\tEmpty Queue" << endl;
}
void queueList::IOdisplay(ostream &out, int t)
{
  int empty = 0;     //variable to keep track if I/O is empty
	Item *w = front;  //declare iterator that will start at front of queue
	for (int i = 0; i < size; i++)
	{
		if (w == nullptr) //condition in case linked list breaks
			break;
    if ( w->p.queue_pos > t)//if process is not in queue at execution time t
    {             //display process in queue information
      cout << "\tProcess:  " << w->p.id << "\tRemaining Time:  " << w->p.queue_pos - t <<  endl;
      empty++;
    }

		w = w->next;
	}
  if(empty == 0)//if no elements in I/O at execution time t
    cout << "\tNo Process in I/O" << endl;
}
