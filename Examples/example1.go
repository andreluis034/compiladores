package main
import "fmt"

func main() {

	a := fib(3);
	fmt.Print(a);
}
func fib(a) {
	if(a < 2) {
		return a;
	}
	c := fib(a-1);
	b := fib(a-2);
	return c + b;
}