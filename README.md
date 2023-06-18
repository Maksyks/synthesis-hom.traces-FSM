# synthesis-hom.traces-FSM
The program finds homing traces for finite state machine
Input data is a file.txt that contains a description of the FSM in the format: 
the number of: states, input symbols, output symbols, the number of transitions; a list of transitions. 
This order is important. 
example:
3
7
2
18
0 0 0 1
0 1 0 1
0 2 0 1 
0 3 0 1 
0 4 0 1
0 5 1 0
1 0 1 1 
1 1 1 1 
1 2 1 1 
1 3 1 1 
1 4 1 1 
1 6 0 1
1 6 2 0
2 0 2 1 
2 1 2 0
2 2 2 0 
2 3 2 0
2 4 2 0

The output data is the received homing traces in the format: state to which the FSM will switch - the trace
example:
0 - 1/1 6/1 
0 - 6/1 
1 - 1/1 5/0 
2 - 1/1 6/0 

