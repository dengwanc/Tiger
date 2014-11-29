BEGIN L12
L14
mov $0x2, 103
push 103
mov $0x4, 104
push 104
mov 0(ebp), 105
push 105
call L11
jmp L13
L13
END L12

BEGIN L11
L16
mov $0x6, 115
add $9, 115
jmp L15
L15
END L11

