package tcpserver

import (
	"bufio"
	"log"
	"net"
	"strconv"
)

func Listen(port int) {
	p := strconv.Itoa(port)
	adress := "127.0.0.1:" + p
	ln, err := net.Listen("tcp", adress)
	if err != nil {
		log.Fatal(err)
	}
	defer ln.Close()
	log.Println("tcp server started")
	for {
		conn, err := ln.Accept()
		if err != nil {
			log.Println(err)
			continue
		}
		go handle(conn)
	}
}

func handle(conn net.Conn) {
	defer func() {
		if err := recover(); err != nil {
			log.Println("发生了未处理的异常", err)
		}
	}()
	defer conn.Close()
	scanner := bufio.NewScanner(conn)
	for scanner.Scan() {
		line := scanner.Bytes()
		log.Println("收到消息：", string(line))
	}
	if err := scanner.Err(); err != nil {
		log.Println("错误：", err)
	}
}
