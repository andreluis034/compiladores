package main
import "fmt"

func main() {
	c := 20;

	fib(5);

}
func fib(a) {
	if(a > 0) {
		fib(a-1);
	}
	fmt.Print(a);
}