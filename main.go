package main

import (
	"fmt"
	"os"
	"strconv"
)

func main() {
	f, _ := os.Create("./list1.txt")

	defer f.Close()

	for i := 0; i < 256; i++ {
		n := int64(i)
		pattern := fmt.Sprintf("%08b\n", n)
		num := strconv.Itoa(i)
		f.WriteString(num + "    " + pattern)
	}
	fmt.Println("file written")

}
