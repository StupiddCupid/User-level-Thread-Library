# Project 2 Report: User-level Thread Library ⛓📚 

# Summary 🌟
As we all know, thread is a single execution sequence in computer science that represents 
a separately schedulable task. The appearance of thread greatly accomplishes our idea that 
we want to place concurrent computations within the same address space and eliminate 
duplication of the address space and most of the environment while running a program. Thus,
in this project, we implement a user-level thread library for linux that can rovide a 
complete interface for applications to create and run independent threads concurrently via 
high-quality C code.

# Implementation 🌟

The implementation of this program is consist of three parts.

Part 1️⃣ Queue API

To implement thread and its performance, we chose one of the most used containers ,queue, by 
a given API. To better manipulate our simple FIFO queue and make each operation effectively, 
we utilized linked list -a underlying data structure- for our queue implementation. By taking 
the advantage of 'struct' data type，we create three struct functions with their members to 
construct the basic structure of our queue:

1. struct queue, containing the length of the queue and two pointers of type 'struct Node', 
one pointing to the front node and the other one pointing to the tail node.

2. struct Node, containing one pointer called 'key' for storing the value of the node and 
another pointer call 'next' with type 'struct Node*' representing the ➡️.

3. struct Node* newNode with its parameter 'value', allocating a space for each new node, 
initializing its value and connecting its next node.

Now, It is time to implement 7 basic queue operations: create(), enqueue(), dequeue(),
delete(), iterate(), getLength() and destory().

✔️In create() function, we first allocate a space for our queue and seted its front node and 
tail node to be null. Also, the default length of the queue is initialized to be zero. 

✔️In enqueue() function, we create a new node with the passed value by using 'struct Node*' that 
we created at the begining. Then, we noticed there should be three cases when adding a new node 
to our queue. 
   case1:   Our queue is empty, which means the length of it is currently zero. Thus, we simply 
            set front ptr and tail prt pointing to the new node, and increase the length.
   case2:   Our queue has length of 1, which means our front node and tail node are currently the 
            same. Then, we move the tail node pointing to the new node and make the next node of 
            the front node pointing to the new node. Increseing the length at the end.
   case3:   Our queue has a size bigger than 1, which is the most common case we need to handle.
            We keep the position of the front node and set the tail pointer pointining to the new 
            node. Finally, making the new node to be the tail node.
      
✔️In dequeue() function, we have two cases. Before dealing with the different cases, we noticed
that we should store the node that will be dequeued later. Thus, we create a new pointer called
current node taking the job of storing dequeued node. Now, one case is that there is only one node
left in our queue which means no more node is in our queue after dequeue. So, we simply set the
front and tail to be null indicating that no more node lefts. The other case is when more than one 
node is in our queue, we simply reset our front node to be the next node. 

✔️In delete() function, we would like to find the first node that has the data we want to delete and 
delete it. Thus, using a loop comes into our mind. By having a while loop, we can iterating the loop 
until we reach the node we want to delete. To finish delete, we also reset front and tail pointer in 
different cases like enqueue() function.

✔️In iterate() function, it should iterate through all items in the queue and calls the given callback 
function on each item. So, using a while loop that contiune call the callback function is how we dealed
with iterate() function.

✔️In destory() function, we just free the queue that we created in order to destory the queue. 
✔️In getLength() function, we just return the member 'length' of struct queue.

Testing😼: to check whether our coding parts is correctly constructed, we created our own tester which 
containing one array as our data pool. By calling create() function to create a queue and calling 
enqueue & dequeue, we compared the result with our designed output. If they do not match with each 
other, an assertion will occur and give a test failure o us. Also, to check iterator() function, we
create our callback function containing delete() function, which could check our delete() function
and iterator function at once. 

Part 2️⃣ User-level thread library



Part 3️⃣ Semaphore API

# Limitations🌟
   
# The End🌟
Thanks for watching:D

                                 
