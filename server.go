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

func main() {
	c, err := gpiod.NewChip("gpiochip0")
	if err != nil {
		panic(err)
	}
	defer c.Close()

	l, err := c.RequestLine(rpi.GPIO21,
		gpiod.WithPullUp,
		gpiod.WithBothEdges,
		gpiod.WithEventHandler(eventHandler))
	if err != nil {
		fmt.Printf("RequestLine returned error: %s\n", err)
		if err == syscall.Errno(22) {
			fmt.Println("Note that the WithPullUp option requires kernel V5.5 or later - check your kernel version.")
		}
		os.Exit(1)
	}
	defer l.Close()

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

func eventHandler(evt gpiod.LineEvent) {
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
}
