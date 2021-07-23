package main

import (
	"fmt"
	"github.com/warthog618/gpiod"
	"github.com/warthog618/gpiod/device/rpi"
	"log"
	"net/http"
	"os"
	"syscall"
	"time"
)

var readyOne = false
var readyTwo = false

func main() {
	c, err := gpiod.NewChip("gpiochip0")
	if err != nil {
		panic(err)
	}
	defer c.Close()

	l1, err := c.RequestLine(rpi.GPIO21,
		gpiod.WithPullUp,
		gpiod.WithBothEdges,
		gpiod.WithEventHandler(eventHandler(&readyOne)))
	if err != nil {
		fmt.Printf("RequestLine returned error: %s\n", err)
		if err == syscall.Errno(22) {
			fmt.Println("Note that the WithPullUp option requires kernel V5.5 or later - check your kernel version.")
		}
		os.Exit(1)
	}
	defer l1.Close()

	l2, err := c.RequestLine(rpi.GPIO20,
		gpiod.WithPullUp,
		gpiod.WithBothEdges,
		gpiod.WithEventHandler(eventHandler(&readyTwo)))
	if err != nil {
		fmt.Printf("RequestLine returned error: %s\n", err)
		if err == syscall.Errno(22) {
			fmt.Println("Note that the WithPullUp option requires kernel V5.5 or later - check your kernel version.")
		}
		os.Exit(1)
	}
	defer l2.Close()

	fs := http.FileServer(http.Dir("./assets"))

	http.HandleFunc("/is-ready", func(writer http.ResponseWriter, request *http.Request) {
		if readyOne && readyTwo {
			writer.Write([]byte("yes"))
		} else {
			writer.Write([]byte("no"))
		}
	})
	http.Handle("/", fs)

	log.Fatal(http.ListenAndServe(":7777", nil))
}

func eventHandler(b *bool) func(evt gpiod.LineEvent) {

	return func(evt gpiod.LineEvent) {
		t := time.Now()
		edge := "rising"
		if evt.Type == gpiod.LineEventFallingEdge {
			edge = "falling"
		}
		fmt.Printf("event:%3d %-7s %s (%s)\n",
			evt.Offset,
			edge,
			t.Format(time.RFC3339Nano),
			evt.Timestamp)

		*b = true

	}
}
