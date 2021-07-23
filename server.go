package main

import (
	"log"
	"net/http"
	"time"
)

func main() {
	fs := http.FileServer(http.Dir("./assets"))

	isReady := false

	http.HandleFunc("/is-ready", func(writer http.ResponseWriter, request *http.Request) {
		if isReady {
			writer.Write([]byte("yes"))
		} else {
			writer.Write([]byte("no"))
		}
	})
	http.Handle("/", fs)

	go func() {
		time.AfterFunc(time.Minute, func() {
			isReady = true
		})
	}()

	log.Fatal(http.ListenAndServe(":7777", nil))
}
