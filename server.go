package main

import (
	"log"
	"net/http"
)

func main() {
	fs := http.FileServer(http.Dir("./assets"))

	http.Handle("/", fs)

	log.Fatal(http.ListenAndServe(":7777", nil))
}
