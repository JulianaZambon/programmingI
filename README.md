# Programming I (CI1001)
Projects requested in the Programming I course (ci1001).

### Small Project 2
This project is a variant of implementation of the same problem from TP1. The challenge is to learn how to pass parameters using pointers (addresses) to avoid copying complex structures that can occupy a lot of space on the STACK.

### Small Project 3
This project involves implementing the program `libpilha.c` based on the provided `libpilha.h`. Your implementation should use a linked list with a head node that implements the Last In First Out (LIFO) policy, i.e., a policy that implements an Abstract Data Type (ADT) Stack. The challenge is to learn how to use dynamic allocation and keep the list always linked without losing pointers.

### Small Project 3b (Parentheses)
Implement a program that checks if an arithmetic expression entered by the user has balanced parentheses, considering that there are different types of "parentheses" such as regular parentheses ((, )), square brackets ([ ]), curly braces ({, }), among others. The program will read an expression from the standard input and verify if the symbols of precedence (parentheses, brackets, and braces) were correctly used.

### Small Project 4
This project involves implementing the program `libfila.c` based on the provided `libfila.h`. Your implementation should use a linked list with a head node that implements the First In First Out (FIFO) policy, i.e., a policy that implements an Abstract Data Type (ADT) Queue. The challenge is to learn how to use dynamic allocation and keep the list always linked without losing pointers. This time, notice that the queue type contains two pointers to the linked list, one that points to the head of the list (its beginning) and another that always points to the tail of the list (its end).

### Preparation 1 for T1
This preparatory work involves implementing the program `liblista_ordenada.c` based on the provided `liblista_ordenada.h`, and creating a tester for it as the main program. The challenges are:
- Learning to insert and remove items in an ordered way.
- Dealing with the difference between the node containing an integer and now containing a pointer to an integer. To test your implementation, build a file `testa_lista_ordenada.c`.

### Preparation 2 for T1
This preparatory work involves implementing the program `liblista_circular.c` based on the provided `liblista_circular.h`, and creating a tester for it as the main program. Essentially, it involves modifying Preparation 1 to implement a doubly linked circular list. You can make decisions as you see fit for this preparatory work. Tip: When creating the list, add an additional node to serve as a sentinel. If this sentinel is always the first in the list, your implementation will be simpler. The challenges are:
- Learning to implement a doubly linked circular list with a sentinel. To test your implementation, build a file `testa_lista_circular.c`.

### T1 - Part 01
This project involves implementing the program `libagenda.c` based on the provided `libagenda.h`. It mainly consists of adapting the programs you created in the preparation work 1 and 2, implementing linked ordered lists (one simple and one doubly circular), modified to meet the new specifications. The challenges are:
- Adapting the codes you've already created.
- Implementing the additional functions.

### T2 - Shell
Create a shell script with functions that answer questions about the performance of students in a course across different subjects over the years.
