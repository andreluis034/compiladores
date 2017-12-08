package main

import "fmt"

func dobra(c){
    c:=c*2;
    return c;
}


func main() {

    x:=20;

    fmt.Scan(x);

    x:=dobra(x);

    fmt.Print(x);
}