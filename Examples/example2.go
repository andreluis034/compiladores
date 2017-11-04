//bool to integer example in go

package main

import "fmt"

func main() {

	converted_value := 0;
	bool_value := false;

	fmt.Scan(bool_value);

	if true == bool_value {

		converted_value = 1;

	}

	else {

		converted_value = 0;

	}

	fmt.Print(converted_value);

}