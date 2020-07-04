A simple compiler built for the compiler class at university implementing of a subset of Go language. The resulting output is a file with MIPS instructions 

# The following Go language commands are implemented in this AST generator:

* Variable declarations (a:=0,a=1)
* Variable incrementation (++,--,+=,-=)
* Boolean and Integer values
* Expressions (a=b+c+2, flag = true, true == false, etc.)
* If/else command 
* For (3 args and 1 arg)
* fmt.Print() function
* fmt.Scan() function
* Function declarations
* Function calls


# Compiling
Make sure you have the following dependecies: `make, flex and bison`. Afterwards running `make all` will generate a file `compiler.o` which can be used like so:
```
./compiler.o < Examples/example1.go
./compiler.o < Examples/example2.go
./compiler.o < Examples/example3.go
```
The compiler will output the generated assembly to the standard output
http://courses.missouristate.edu/kenvollmar/mars/
# Running the compiled assembly
You can run the output on any MIPS emulator of your choosing. I recommend [MARS](http://courses.missouristate.edu/kenvollmar/mars/).


