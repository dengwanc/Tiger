L18
L15
mov $0x1, 102
mov 102, 101
mov 0(ebp), 105
mov -4(105), 104
mov $0x13, 107
cmp 104, 107
jl L12
L13
mov $0x0, 108
mov 108, 101
L12
mov $0x0, 111
cmp 101, 111
je L14
L16
mov $0x0, 112
mov 0(ebp), 113
mov 112, -4(113)
jmp L15
L14
mov 0(ebp), 117
mov -12(117), 116
jmp L17
L17
