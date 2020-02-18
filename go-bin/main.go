package main

import (
	"bufio"
	"fmt"
	"io/ioutil"
	"net"
	"strings"
)

func main() {

	fmt.Println("Launching server...")

	ln,err := net.Listen("tcp", ":1984")
	if err != nil {
		fmt.Println(err)
		return
	}
	defer ln.Close()

	if err != nil {
		fmt.Println(err)
		return
	}

	for {
		conn, err := ln.Accept()
		if err != nil {
			fmt.Println(err)
			return
		}
		go handleConnection(conn)
	}
}

func handleConnection(conn net.Conn) {
	conn.Write([]byte("Welcome, human" + "\n"))
	fmt.Println("[+] Connection Received")

	authenticate(conn)
	conn.Close()
	return
}

func authenticate(conn net.Conn){
	conn.Write([]byte("What is your name: "))
	name, err := bufio.NewReader(conn).ReadString('\n')
	check(err)
	name = strings.TrimRight(name, "\r\n")
	success := checkName(name)
	if !success{
		fmt.Println("[-] Invalid user. Closing connection.")
		conn.Write([]byte("Intruder! intruder!" + "\n"))
		conn.Close()
		return
	}
	conn.Write([]byte("Enter the password: "))
	pwd, err := bufio.NewReader(conn).ReadString('\n')
	check(err)
	pwd = strings.TrimRight(pwd, "\r\n")
	success = checkPassword(pwd)
	if !success{
		fmt.Println("[-] Password incorrect. Closing connection.")
		conn.Write([]byte("Nice try You have pieced a lot of this together, though." + "\n"))
		conn.Close()
		return
	}

	jackpot(conn)
}

func jackpot(conn net.Conn){
	s, err := ioutil.ReadFile("../secrets/metaverse.txt")
	check(err)
	conn.Write([]byte("\nWelcome to the Metaverse. Here are your secrets:" + "\n\n"))
	conn.Write([]byte(string(s) + "\n\n"))
}

func checkName(name string) bool{
	fmt.Println("[+] Checking name")

	if len(name) != 12{
		return false
	}

	if name[:1] != "H" || name[1:] != "Protagonist"{
		return false
	}
	return true
}

func checkPassword(pwd string) bool{
	fmt.Println("[+] Checking password")
	if pwd == ""{
		return false
	}
	for i, c := range pwd{
		switch i {
			case 0:
				if int(c) != 104{
					return false
				}
			case 1:
				if int(c) != 52{
					return false
				}
			case 2:
				if int(c) != 99{
					return false
				}
			case 3:
				if int(c) != 107{
					return false
				}
			case 4:
				if int(c) != 49{
					return false
				}
			case 5:
				if int(c) != 57{
					return false
				}
			case 6:
				if int(c) != 56{
					return false
				}
			case 7:
				if int(c) != 52{
					return false
				}
			default:
				return false
			}
	}
	return true
}

func check(e error) {
	if e != nil {
		fmt.Println(e.Error())
		panic(e)
	}
}