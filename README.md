# JLU-SNL-COMPILER
This is the JLU-SNL-COMPILER project.

You can find all relevant code in the SNL-compiler folder.

The FinalProject folder within SNL-compiler contains all the code, and QT needs to be installed to open it.

The text_text folder within SNL-compiler includes some sample tests for our project.

Information about the SNL language can be found in the InstructionBook folder.

The interface is shown below:

![image](https://github.com/HanLi05869/JLU-SNL-COMPILER/blob/main/InterfaceImages/1.png)

The left side is for input text, and the right side is for output.

Clicking "Debug" will run the entire program.

Step 1 displays the token sequence after lexical analysis:
![image](https://github.com/HanLi05869/JLU-SNL-COMPILER/blob/main/InterfaceImages/2.png)

Recursion displays the syntax tree for recursive descent:
![image](https://github.com/HanLi05869/JLU-SNL-COMPILER/blob/main/InterfaceImages/3.png)

LL1old displays the syntax tree after LL1 method:
![image](https://github.com/HanLi05869/JLU-SNL-COMPILER/blob/main/InterfaceImages/4.png)

LL1new displays a more concise syntax tree:
![image](https://github.com/HanLi05869/JLU-SNL-COMPILER/blob/main/InterfaceImages/5.png)

Step 3 shows the result of semantic analysis:
![image](https://github.com/HanLi05869/JLU-SNL-COMPILER/blob/main/InterfaceImages/6.png)

Step 4 is for intermediate code generation:
![image](https://github.com/HanLi05869/JLU-SNL-COMPILER/blob/main/InterfaceImages/7.png)
