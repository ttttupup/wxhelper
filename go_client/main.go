package main

import (
	"go_client/tcpserver"
	"log"
)

func main() {
	log.SetFlags(log.LstdFlags | log.Lshortfile)
	tcpserver.Listen(19099)
}
