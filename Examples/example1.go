package main
import "fmt"

func main() {

	c:= fib(20);
	fmt.Print(c);
}
func fact(x) { 
	if x == 0 {
		return 1;
	}
	y := fact(x-1);
	return x *y;
}