.global _start

_start:
/* enable all clocks */
ldr r0, =0X020C4068
ldr r1, =0XFFFFFFFF
str r1, [r0]

ldr r0, =0X020C406C
str r1, [r0]

ldr r0, =0X020C4070
str r1, [r0]

ldr r0, =0X020C4074
str r1, [r0]

ldr r0, =0X020C407C
str r1, [r0]

ldr r0, =0X020C4080
str r1, [r0]

/*set GPIO_IO03 to reuse as GPO1_IO03*/
ldr r0, =0X020E0068
ldr r1, =0X5
str r1, [r0]

ldr r0, =0X020E02F4
ldr r1, =0X10B0
str r1, [r0]

/* set GPO1_IO03 output low level*/
ldr r0, =0X0209C000
ldr r1, =0
str r1, [r0]

loop:
    b loop
    