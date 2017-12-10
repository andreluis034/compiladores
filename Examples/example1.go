package main
import "fmt"

func main() {

	c:= fib(20);
	fmt.Print(c);
}
func fib(a) {
	if(a < 2) {
		return a;
	}
	c := fib(a-1);
	b := fib(a-2);
	return c + b;
}