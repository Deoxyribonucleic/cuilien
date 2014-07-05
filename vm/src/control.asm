jmp main

print:
	push a
	mov a, sp
	add a, 8
	show [a]
	pop a
	ret

main:
	mov a, 1
	mov b, 2
	mov c, 3
	mov d, 4
	push 1337
	call print
	show a
	show b
	show c
	show d
	halt
