mov a, 1337
mov b, 1338
cmp a, b
jneq false
true:	show 1
		jmp end
false:	show 0
		jmp end
end:	halt
