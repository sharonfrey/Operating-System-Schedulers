#include <iostream>
using namespace std;



class process
{
public:
  int id;               //each process will have its own unique id
  int waiting_time;     //holds total waiting time of process
  int tunraround_time;  //holds total turnaround time of process
  int response_time;    //holds response time of process
  int time_quantum;     //time quantum of round robin scheduler
  int start_time;       //time when processs begins
  int end_time;         //time when entire process is completed
  int next_queue;       //tracker so process can be demoted to next queue when Tq expires
  int queue_pos;        //time when process will enter queue after I/O burst
  int burst_pos;        //index pointing to current position in process data array
  int interrupt;        //flag that is raised when process in lower queue is interrupted
  int burstSize;        //size of process data array
  int next_burst;       //burst size of next CPU burst to run in process
  int arr[32];          //process data array

  process();            //initialize process
  void setBurst(int i, int p[], int s);

  void operator=(const process &x);


  void Ttr();           //update turnaround time
  void Tw(int t);       //update waiting time
  void Tr(int t);       //update respnse time
  int addTime();        //return CPU burst
  int addTime(int n, int t);//return CPU burst with a check for interrupt
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
  Item * getfront();                 //returns value of node in front of queue
  void insert(Item *a, int t);       //insert node into queue at position t
	int getSize();                     //get size of queue
  bool isEmpty();                    //check if queue is empty
  bool isEmpty(int t);              //check if queue is empty at execution time t
  int findPos(int e);                //returns position in queue that the the queue_pos of the node before it < e
                                    //and the queue_pos of the node after is > e
  void display(ostream &out, int t);//displays processes in queue
  void IOdisplay(ostream &out, int t);//displays processes in I/O burst

  void push_front(Item *a);
  void swap(Item *a, Item *q);
  void bubbledown();
};




int main()
{
  //experiment variables (used for tracking time and other data)
  Item *pop;  //running process
  Item *temp; //temp process to compare processes
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
  queueList queue1;
  queueList queue2;
  queueList queue3;
  //push elements into queue
  queue1.push_back(new Item(p1));
  queue1.push_back(new Item(p2));
  queue1.push_back(new Item(p3));
  queue1.push_back(new Item(p4));
  queue1.push_back(new Item(p5));
  queue1.push_back(new Item(p6));
  queue1.push_back(new Item(p7));
  queue1.push_back(new Item(p8));



  //variables to keep track of multiple queues in mlfq
  int total = queue1.getSize(); //number of processes in queue
  int completed = 0;            //track how many processes are completed






  //mlfq algorithm
  while(completed != total)
  {
    //conditionals to pop element from queue in order of priority
    if (queue1.isEmpty(time))  //check if there are any processes in queue 1
    {
      if (queue2.isEmpty())    //check if there are any processes in queue 2
      {
        if (queue3.isEmpty())  //check if there are any processes in queue 3
        {
          pop = queue1.pop_front(); //if all 3 queues are empty at execution timethe next
          pop->p.time_quantum = 5;  //process will enter queue 1 after I/O burst completion
        }
        else                        //process is in queue 3
        {
          pop = queue3.pop_front();
          pop->p.time_quantum = 0;
        }
      }
      else                        //process is in queue 2
      {
        pop = queue2.pop_front();
        pop->p.time_quantum = 10;
      }
    }
    else                          //process is in queue 1
    {
      pop = queue1.pop_front();
      pop->p.time_quantum = 5;
    }

    //conditional to track any idle time in scheduler
    if(pop->p.queue_pos > time)
    {
      idle_time += (pop->p.queue_pos - time);
      time = pop->p.queue_pos;
    }

    //print context switch data
    cout << "Context Switch:  " << cs << endl
        << "\tExecution Time:  " << time << endl
        << "\tRunning Process:  " << pop->p.id << endl;
    cout << "Ready Queue 1:  " << endl;
    queue1.display(cout, time);
    cout << "Ready Queue 2:  " << endl;
    queue2.display(cout, time);
    cout << "Ready Queue 3:  " << endl;
    queue3.display(cout, time);
    cout << "Processes in I/O:  " << endl;
    queue1.IOdisplay(cout, time);

    cs++;




    //update waiting time and response time
    pop->p.Tw(time);
    pop->p.Tr(time);
    //look at the next process in queue 1 (in case of interrupt)
    temp = queue1.getfront();

    //update execution time to be current time + CPU burst size
    //this is where the execution time will be at for the next iteration of the loop
    if (temp == nullptr)       //if queue 1 is empty
      time += pop->p.addTime();//update execution time
    else
      time += pop->p.addTime(temp->p.queue_pos, time);//update execution time in regards to interrupt

    int e = pop->p.setNextPos(time);//e is the time process will end I/O burst and enter queue again
    //conditional to check if process is completed
    if (e != -1)//setNextPos returns -1 if process is complete
    {
      if (pop->p.next_queue == 1)       //if next queue process enters is queue 1
      {
        queue1.push_front(pop);
      }
      if (pop->p.next_queue == 2)       //if next queue process enters is queue 2
        queue2.push_back(pop);          //insert process to back of queue 2
      if (pop->p.next_queue == 3)       //if next queue process enters is queue 3
        queue3.push_back(pop);          //insert process to back of queue 3

      cout << endl << endl;
    }
    else                                //process is completed
    {
      //display end of process
      cout << "Process " << pop->p.id << " has completed its total execution" << endl << endl << endl;
      completed++;
      pop->p.end_time = time; //update end time
      pop->p.Ttr();           //update turnaround time

      //store wanted data in array for future calculations
      all_Tw[pop->p.id - 1] = pop->p.waiting_time;
      all_Ttr[pop->p.id - 1] = pop->p.tunraround_time;
      all_Tr[pop->p.id - 1] = pop->p.response_time;
    }
  }



  //calculating the averages
  for(int i = 0; i < total; i++)
  {
    avg_Tw += all_Tw[i];
    avg_Ttr += all_Ttr[i];
    avg_Tr += all_Tr[i];
  }

  avg_Tw = avg_Tw / total;
  avg_Ttr = avg_Ttr / total;
  avg_Tr = avg_Tr / total;
  cpu_u = time - idle_time;
  cpu_u = (cpu_u / time) * 100;

  //display end results of CPU Simulation
  cout << "Total time:  " << time << endl
  << "CPU Utilization:  " << cpu_u << "%" << endl << endl;

  for (int i = 0; i < total; i++)
  {
    cout << "Waiting Time P" << i+1 << ":  " << all_Tw[i] << endl;
  }
  cout << "Average Waiting Time:  " << avg_Tw << endl << endl;

  for (int i = 0; i < total; i++)
  {
    cout << "Turnaround Time P" << i+1 << ":  " << all_Ttr[i] << endl;
  }
  cout << "Average Turnaround Time:  " << avg_Ttr << endl << endl;

  for (int i = 0; i < total; i++)
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
  time_quantum = 5;
  start_time = 0;
  end_time = 0;
  next_queue = 2;
  queue_pos = 0;
  burst_pos = 0;
  interrupt = 0;
}

void process::setBurst(int i, int p[], int s)
{       //initialize id, next_burst, and arr variables
  id = i;
  cpyArr(p, s);
  next_burst = p[0];
}


void process::cpyArr(const int a[], int n)
{     //standard copy array function
  this->burstSize = n;
  for(int i = 0; i < n; i++)
    this->arr[i] = a[i];
}

void process::operator=(const process &x)
{
  id = x.id;
  waiting_time = x.waiting_time;
  tunraround_time = x.tunraround_time;
  response_time = x.response_time;
  time_quantum = x.time_quantum;
  start_time = x.start_time;
  end_time = x.end_time;
  next_queue = x.next_queue;
  queue_pos = x.queue_pos;
  burst_pos = x.burst_pos;
  next_burst = x.next_burst;
  interrupt = x.interrupt;
  cpyArr(x.arr, x.burstSize);
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
{    //return time process is able to run
  if (time_quantum == 0 || next_burst <= time_quantum)//if full burst can run
  {
    cout << "jsut to make sure   " << next_burst << endl;
    return next_burst;
  }
      //retun size of burst
  else          //if burst is larger than time time_quantum
    return time_quantum;//return time quantum size
}
int process::addTime(int n, int t)
{   //return time process is able to run (with a check for an interrupt)
  if(time_quantum == 5 || (n >= (addTime() + t))) //if there is no interrupt
    return addTime();
  else
  {
    interrupt = 1;           //raise interrupt flag
    int inter_size = n - t; //calculate length process can run before interrupt
    next_burst = next_burst - inter_size;//update process burst size
    return inter_size;      //return process run time
  }
}

bool process:: endOB()
{       //if array index is at the last CPU burst (end of process) return true
  if ((burstSize - burst_pos) == 1)
    return true;
  return false;
}

int process:: setNextPos(int t)
{
  if (interrupt == 1)//if there is an interrupt
  {                //burst is already updated when interrupt occurs
    interrupt = 0; //reset interrupt flag
    queue_pos = t;//update time when process enters queue
    return queue_pos;
  }

  if (time_quantum == 0 || next_burst <= time_quantum)//if process completes burst
  {
    if(endOB())     //check if process is completed
      return -1;
    queue_pos = t + arr[burst_pos + 1];//queue_pos will hold time when process enters queue after I/O burst
    burst_pos += 2;                     //update burst_pos to next burst in array
    next_burst = arr[burst_pos];        //next_burst variable holds the next CPU burst of the process
    next_queue = 1;                   //update next queue process will enter
    return queue_pos;
  }
  else
  {
    if (time_quantum == 5)      //if process is in queue 1
      next_queue = 2;          //it will be pushed to queue 2
    if (time_quantum == 10)    //if process is in queue 2
      next_queue = 3;         //it will be pushed to queue 3

    next_burst = next_burst - time_quantum;//update time left in process CPU burst
    queue_pos = t;            //update time when process enters queue
    return queue_pos;
  }

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
Item * queueList::getfront()
{       //return value of node in front of the queue
	return front;
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
{      //return size of queue
	return size;
}

bool queueList::isEmpty()
{       //returns true if queue is empty
  if (size == 0)
    return true;
  return false;
}

bool queueList::isEmpty(int t)
{       //returns true if queue is empty at execution time t
  if (isEmpty() || front->p.queue_pos > t)
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

void queueList::push_front(Item *a)
{         //push process to front of queue
	a->next = front; //node a pointer to next points to front node of queue
	front = a;     //update front pointer
	size++;        //update size
  bubbledown();
}
void queueList::swap(Item *a, Item *q)
{
	process temp = a->p;
	a->p = q->p;
	q->p = temp;
}

void queueList::bubbledown()
{
  Item *itr = front;
  Item *next;
  while(itr->next != nullptr)
  {
    next = itr->next;
    if(itr->p.queue_pos >= next->p.queue_pos)
      swap(itr, next);
    itr = itr->next;
  }
}
