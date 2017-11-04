
 The AST Generator is an Abstract Syntatic Tree Generator for Go language programs.

		[How to run the AST Generator]

			- Navigate into the main folder /ast_generator
			- Using the bash/shell/cmd, execute the command "make"
			- All the files will be compiled and ready to be executed
			- To run the generator, use the command "./compiler.o"

		[Using the examples given and creating your own examples]

			- 3 examples are given within the original package of the generator
			- They are located under the folder /ast_generator/Examples
			- To generate the AST for each of them using the bash/shell/cmd, while in the main folder "/ast_generator", you can execute the following commands:

					"./compiler.o < Examples/example1.go"

					"./compiler.o < Examples/example2.go"

					"./compiler.o < Examples/example3.go"

			- If you intend to create your own example, feel free to do so. To test it, just execute the command:

					"./compiler.o < 'example_path' "
						 where 'example_path' represents the path to your own created example

		[Go commands that are implemented in the generator]

			The following Go language commands are implemented in this AST generator:

				- Variable declarations (a:=0,a=1)
				- Variable incrementation (++,--,+=,-=)
				- Boolean and Integer values
				- Expressions (a=b+c+2, flag = true, true == false, etc.)
				- If/else command 
				- For (3 args and 1 arg)
				- fmt.Print() function
				- fmt.Scan() function
				- Function declarations
				- Function calls