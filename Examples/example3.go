package main

import "fmt"

func dobra(x){
    x:=x*2;
    return x;
}


func main() {

    x:=0;

    fmt.Scan(x);

    x:=dobra(x);

    fmt.Print(x);
}