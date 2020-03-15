1 算术运算符(逻辑 加减乘除 置位)
2 取址表达式(LSL, LSR, ASR, ROR, RRX)
3 赋值寄存器(立即数 寄存器>寄存器 寄存器>内存 内存>寄存器)
4 函数(入栈 跳转 返回 出栈)
5 条件跳转()
6 特殊寄存器(pc lr)
7 内存初始化(DCD)
8 多寄存器操作(LDMIA STMIA LDMIB STMIB LDMDA STMDA LDMDB STMDB)

# 1. arithmetic
add sub orr ands umull
cmp
0. 	AND regd, rega, argb ; regd ← rega & argb
1. 	EOR regd, rega, argb ; regd ← rega ^ argb
2. 	SUB regd, rega, argb ; regd ← rega - argb
3. 	RSB regd, rega, argb ; regd ← argb - rega
4. 	ADD regd, rega, argb ; regd ← rega + argb
5. 	ADC regd, rega, argb ; regd ← rega + argb + carry
6. 	SBC regd, rega, argb ; regd ← rega - argb - !carry
7. 	RSC regd, rega, argb ; regd ← argb - rega - !carry

8. 	TST rega, argb 		 ; set flags for rega & argb
9. 	TEQ rega, argb 		 ; set flags for rega ^ argb
10. CMP rega, argb 		 ; set flags for rega − argb
11. CMN rega, argb 		 ; set flags for rega + argb

12. ORR regd, rega, argb ; regd ← rega | argb
13. MOV regd, arg 		 ; regd ← arg
14. BIC regd, rega, argb ; regd ← rega & ~argb
15. MVN regd, arg 		 ; regd ← ~argb

# 2. address expression
[Rn, #±imm] 	Immediate offset
Address accessed is imm more/less than the address found in Rn. Rn does not change.
[Rn] 	Register
Address accessed is value found in Rn. This is just shorthand for [Rn, #0].
[Rn, ±Rm, shift] 	Scaled register offset
Address accessed is sum/difference of the value in Rn and the value in Rm shifted as specified. Rn and Rm do not change values.
[Rn, ±Rm] 	Register offset
Address accessed is sum/difference of the value in Rn and the value in Rm. Rn and Rm do not change values. This is just shorthand for [Rn, ±Rm, LSL #0].
[Rn, #±imm]! 	Immediate pre-indexed
Address accessed is as with immediate offset mode, but Rn's value updates to become the address accessed.
[Rn, ±Rm, shift]! 	Scaled register pre-indexed

Address accessed is as with scaled register offset mode, but Rn's value updates to become the address accessed.
[Rn, ±Rm]! 	Register pre-indexed

Address accessed is as with register offset mode, but Rn's value updates to become the address accessed.
[Rn], #±imm 	Immediate post-indexed

Address accessed is value found in Rn, and then Rn's value is increased/decreased by imm.
[Rn], ±Rm, shift 	Scaled register post-indexed

Address accessed is value found in Rn, and then Rn's value is increased/decreased by Rm shifted according to shift.
[Rn], ±Rm 	Register post-indexed

Address accessed is value found in Rn, and then Rn's value is increased/decreased by Rm. This is just shorthand for [Rn], ±Rm, LSL #0.
# 3. value transfer
`MOV r0, #10`   give immediate value
`LDR R2, [R1]`  load register; load address indicated by R1 to R2
`STR R2, [R0]`  store register; store R2 to address indicated by R0

# 4. function
1. `PUSH    {r5-r7,lr}`  ; pass argument
2. `BL  destination`     ; call function
    Places the return address in the arm_lr.
    Sets the PC to the address of the subroutine.
3. `BX      lr`          ; return
    return from the subroutine
4. `POP    {r5-r7,pc}`   ; release argument from stack

# 5. condition jump
0. 	BEQ ; equal ; Z
1. 	BNE ; not equal ; !Z
2. 	BCS or BHS ; carry set / unsigned higher or same ; C
3. 	BCC or BLO ; carry clear / unsigned lower ; !C
4. 	BMI ; minus / negative ; N
5.  BPL ; plus / positive or zero ; !N
6. 	BVS ; overflow set ; V
7. 	BVC ; overflow clear ; !V
8. 	BHI ; unsigned higher ; C && !Z
9. 	BLS ; unsigned lower or same ; !C || Z
10. BGE ; signed greater than or equal ; N == V
11. BLT ; signed less than ; N != V
12. BGT ; signed greater than ; !Z && (N == V)
13. BLE ; signed greater than or equal ; Z || (N != V)
14. BAL or omitted 		always 		true
# 6. registers
# 7. memory initialize
primes  DCD   2, 3, 5, 7, 11, 13, 17, 19
ADD R0, PC, #primes  ; load address of primes[0] into R0
LDR R1, [R0, #16]    ; load primes[4] into R1
# 8. Multiple-register
LDMIA, STMIA 	Increment after
                We start loading from the named address and into increasing addresses.
LDMIB, STMIB 	Increment before
                We start loading from four more than the named address and into increasing addresses.
LDMDA, STMDA 	Decrement after
                We start loading from the named address and into decreasing addresses.
LDMDB, STMDB 	Decrement before
                We start loading from four less than the named address and into decreasing addresses.