# Project 2 Report: User-level Thread Library 🗣📚

# Summary 🌟

Once mention the operating system, those excellent tech products as well as fancy programming 
languages come to our mind immediately. However, most people even us as computer science major 
students often ignore how powerful the most fundamental but important tool, the shell, is.  
Shell, is a user interfaceto the Operating System's services and it gets input from the user, 
interprets the input as wellas launches the desired actions to the user. In this project, 
we implemented a simple shell that can perform basic command lines and user communication via 
the terminal to help us better understand how Unix system calls work.

# Implementation 🌟

The implementation of this program follows distinct steps:

1⃣️ Parsing the command line given by the users input into two pieces, command and arguments.

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
                  

                                 
