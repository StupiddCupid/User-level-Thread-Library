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

Now, We would like to implement 7 basic queue operations: create(), enqueue(), dequeue(),
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

Part 2️⃣ User-level thread library



Part 3️⃣ Semaphore API
To better recognize the command and execute each arguments correctly, we created a 'Parser' function 
via data structure called 'struct Parser' to seperate command and arguments. First, we create two 
arrays, array 'command' for saving command and the other one 'args' for saving arguments. We use 
one 'for loop' to traverse the whole line of user input and seperate command and its arguments by 
finding the first whitespace in the user input. In other words, the array 'command' will keep 
appending character from the input until it meets the first whitespace. For example, "echo line1" 
will be parsed into array 'command' which contains "echo" and array 'args' which contains "line1". 

Also, in this function, we create a int variable 'redirection' for regogniazing output and input 
redirection. The varible 'redirection' has value '0' as its default value, meaning no need for 
redirection. To recognize input or output redirection, it is easy for us to check whether there 
is ‘>' or '< in the user input. Once we detect output redirection sign '>' inside of our for 
loop, the variable 'redirection' will be assigned to be '1'; Similarly, the 'redirection' will 
be assigned to be'-1' if we detect input redirection sign '<'.

Final, we get all the information we need for execution from the user input, now it is time for 
us to save the information we gain and further determine the type of command and the content of 
arguments we grab from the input. At the end of our 'Parser' function, we pass the command and 
its arguments into a 'Struct Command' for saving the information. 
   
2⃣️ Store the command line information via struct and organize the information.

By taking the advantage of 'struct' data，we create a struct function 'createCommand' for storing 
the command and its arguments. In the 'struct Command', we build 6 instance' sdata for an 
instance 'Command':   
                   <sub> array 'original_cmd'----------------size of CMDLINE_MAX---------------->(contains the user input)
   
                   <sub> array 'cmd'         ----------------size of CMDLINE_MAX---------------->(contains command that should be execute)
                      
                   <sub> array 'filename'    ----------------size of ARGUMENT_MAX--------------->(contains file's name)
                      
                   <sub> array pointer '*arguments'----------size of ARGUMENT_MAX--------------->(contains arguments)
                      
                   <sub> int variable 'num_of_arguments' --------------->(contains the nums of arguments) 
                      
                   <sub> int variable 'redirection'      --------------->(use it as a boolean for detecing out/in redirection)
                                                                                                                
                                 
By getting the information passed from function 'Parser', we fill out those six varibles. To do so, we can easily 
get any one of these data of the user input and preform the command more straitforwardly and conveniently. Finally,
we could go back to the main function to do the actual execution🤸‍♂️


3⃣️ Perform and execute the user input between the command and its arguments.
                      
We classify commands into three different categories, 1.build-in commands(cd, pwd, etc)
                                                      2.regular commands(echo, date,etc)
                                                      3.output/input refirection('>', '<')
                                                         
For build-in commands, we create a function named 'builtin_cmd', which will perform 'pwd', 'cd', 
'pushd' , 'popd' and 'dirs'. We defined 5 if statements to detect each of those build-in commands firstly. 
 Once we recognize the command we are going to perform, then the rest of task will be took inside of
 the if statement and do the corresponding execution.
   
For regular commands aka system calls, following what we learned from the lectures, 
we use fork() to create a clone of the parent process and let it wait for the child process to fully 
complete and get its exit value.
   
For out/int redirection, we create two function output_redirector and input_redirector. 
In order to get input from a file or pass output into a file, we use dup2() to rearrage file 
descriptor of the standard input/output of the terminal.

After setting up the above three functions that perform the actual execution, 
Now, going back to the main function, we can preform each command with its diverse 
type and content of arguments. We use if statement to check the type of the command and pass
the command into corresponding categories function to do the actual execution.
                                                 
                      
# Limitations🌟
Although our simple shell can do majority of the commands, we still have not figured out how to implement pipeline.
We tried to do it using linked list, but we failed at this moment. Thus, we have to continue working on it.
   
# The End🌟
Thanks for watching:D
   
![This is an image](https://www.istockphoto.com/vector/turtle-hiding-in-shell-gm1068845128-285910920)
                  

                                 
