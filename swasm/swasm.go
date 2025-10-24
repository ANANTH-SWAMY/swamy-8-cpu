package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

const REGISTERS int = 4
const MEMORY_SIZE int = 32

const ADD byte = 0b00100000
const SUB byte = 0b01000000
const JMP byte = 0b01100000
const ISZ byte = 0b10000000
const STP byte = 0b11100000

type Instruction struct {
	Opcode  byte
	Operand byte
}

func write_binary(file *os.File) {
	bin_file, err := os.Create("output.bin")
	if err != nil {
		fmt.Printf("Error: Unable to create binary file: output.bin\n")
		return
	}
	defer bin_file.Close()

	instructions := []Instruction{}

	scanner := bufio.NewScanner(file)

	for scanner.Scan() {
		line := scanner.Text()
		line = strings.ToUpper(line)

		fields := strings.Fields(line)

		var instr Instruction

		if fields[0] == "STP" {
			instr.Opcode = STP
		} else {
			if len(fields) != 2 {
				fmt.Printf("Error: Invalid instruction format: %v\n", line)
				return
			}

			operand, err := strconv.Atoi(fields[1])
			if err != nil {
				fmt.Printf("Error: Invalid operand: %v\n", fields[1])
				return
			}

			switch fields[0] {
			case "ADD":
				instr.Opcode = ADD

				if operand < 0 || operand >= REGISTERS {
					fmt.Printf("Error: Invalid register: %v\n", operand)
					return
				}

				instr.Operand = byte(operand)

			case "SUB":
				instr.Opcode = SUB
				if operand < 0 || operand >= REGISTERS {
					fmt.Printf("Error: Invalid register: %v\n", operand)
					return
				}

				instr.Operand = byte(operand)

			case "JMP":
				instr.Opcode = JMP

				if operand < 0 || operand >= MEMORY_SIZE {
					fmt.Printf("Error: Invalid address: %v\n", operand)
					return
				}

				instr.Operand = byte(operand)

			case "ISZ":
				instr.Opcode = ISZ

				if operand < 0 || operand >= REGISTERS {
					fmt.Printf("Error: Invalid register: %v\n", operand)
					return
				}

				instr.Operand = byte(operand)

			default:
				fmt.Printf("Error: Unknown instruction: %v\n", fields[0])
				return
			}
		}

		instructions = append(instructions, instr)
	}

	for _, instr := range instructions {
		binary := instr.Opcode | instr.Operand

		_, err := bin_file.Write([]byte{binary})
		if err != nil {
			fmt.Printf("Error: Unable to write to binary file: output.bin\n")
			return
		}
	}
}

func main() {
	if len(os.Args) < 2 {
		fmt.Printf("Usage: %v <file>\n", os.Args[0])
		return
	}

	file, err := os.Open(os.Args[1])
	if err != nil {
		fmt.Printf("Error: Unable to open file: %v\n", os.Args[1])
		return
	}
	defer file.Close()

	write_binary(file)
}
