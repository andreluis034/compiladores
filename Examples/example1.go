package main
import "fmt"

func main() {
	c := 20;

	//
	fib(20);
	if(c>0) {
		fib(0);
	}

}
func fib(a) {
	if(a > 0) {
		fib(a-1);
	}
	fmt.Print(a);
}