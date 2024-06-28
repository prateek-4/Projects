## Compiler for Language of Expressions (E++) 

– Built a compiler which takes input from E++ and outputs an equivalent program in a machine language Targ
– Made a storage and retrieval system for memory addresses of variables using *AVL* Trees
– Implemented a min-heap strategy to map variables to least available memory addresses at compilation time

## Building the Compiler
From the main parent directory run the following command to build the compiler.
> **make**

If the build process is successfull then a binary file named e++ would have been generated in the bin folder.

*Note:* For windows systems, it won’t work, you need to install GNU make command first.**

## Running the compiler
You can test the compiler by running it on a set of E++ expressions. Write the set of valid E++ expressions
in a file (suppose it’s a.txt), in the tokenized form i.e between each token there should be an at least one
space character separating the two tokens. Also note that the empty lines are ignored. See the samples
folder for examples.
To compile the set of E++ expressions in the file, place the built compiler in the same directory as the a.txt
and run the command (eg is for MacOS/Linux):
> **./e++ a.txt**

If there are no errors in the process, then a file named targ.txt, would be generated containing the targ
commands for a.txt .

## Testing the Targ Syntax
  In the directory tests/targ there are binaries for each OS. Copy the respective binary file to the same
folder in which your filename.txt is there, containing the Targ commands. Run the following command (eg
is for MacOS/Linux):
> **./targ++ filename.txt**

• If you get the permission denied error, then run ”sudo chmod +x targ++” only once to make system
trust the executable.

• For MacOS, if the issue persists still or you get unidentified developer error then open the binary by
right-clicking on it and now you will be able to run the binary normally by ./targ++”.

*• Note:* The binaries only checks that the commands are syntactically correct or not. Neither their
semantics nor their logical output is being checked.

## E++ syntax
See the samples folder for examples on syntax.

## Contact
Contact Information
If you have any questions or suggestions, please contact at [4.prateek.mourya@gmail.com].
