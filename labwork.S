  # labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.

  #
  # Please keep the two macros PUSH and POP unchanged
  #
.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm
  #
  # Please add your own code below this line
  #
	
	.data
	.align 2
mytime:	.word 0x5957
timstr:	.ascii "text more text lots of text\0"
	.text
main:
	# print timstr
	la	$a0,timstr
	li	$v0,4
	syscall
	nop
	# wait a little
	li	$a0, 2		# change argument for delay
	jal	delay
	nop
	# call tick
	la	$a0,mytime
	jal	tick
	nop
	# call your function time2string
	la	$a0,timstr
	la	$t0,mytime
	lw	$a1,0($t0)
	jal	time2string
	nop
	# print a newline
	li	$a0,10
	li	$v0,11
	syscall
	nop
	# go back and do it all again
	j	main
	nop
# tick: update time pointed to by $a0
tick:	lw	$t0,0($a0)	# get time
	addiu	$t0,$t0,1	# increase
	andi	$t1,$t0,0xf	# check lowest digit
	sltiu	$t2,$t1,0xa	# if digit < a, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0x6	# adjust lowest digit
	andi	$t1,$t0,0xf0	# check next digit
	sltiu	$t2,$t1,0x60	# if digit < 6, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0xa0	# adjust digit
	andi	$t1,$t0,0xf00	# check minute digit
	sltiu	$t2,$t1,0xa00	# if digit < a, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0x600	# adjust digit
	andi	$t1,$t0,0xf000	# check last digit
	sltiu	$t2,$t1,0x6000	# if digit < 6, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0xa000	# adjust last digit
tiend:	sw	$t0,0($a0)	# save updated result
	jr	$ra		# return
	nop

  # you can write your code for subroutine "hexasc" below this line
  #
.global hexasc
hexasc:
	PUSH $t0
	PUSH $ra
	andi $a0, $a0, 0xf
	li $t0, 9		
	ble $a0, $t0, less	# jumps to label less if $a0 can be expressed decimally
	nop
	addi $v0, $a0, 55	# adds 55 to $a0, 10 becomes A (65), 11 B (66) etc
	j donehexasc		# jumps to return
	nop
less:	
	addi $v0, $a0, 48	# adds 48 to $a0, 0 becomes 48 etc
donehexasc:
	POP $ra
	POP $t0
	jr $ra
	nop
.global time2string
time2string:
  PUSH $a0
    PUSH $ra
    move    $t0, $a0
    addi    $t1, $zero, 0x3a
    addi    $t2, $zero, 0x00
    move    $a0, $a1

    sb      $t2, 5($t0)    

    jal     hexasc
    nop
    sb      $v0, 4($t0)    #OO:O(X)

    srl     $a0, $a1, 4
    jal     hexasc
    nop
    sb      $v0, 3($t0)    #OO:(X)O

    sb      $t1, 2($t0)    #OO(:)OO
    
    srl     $a0, $a1, 8
    jal     hexasc
    nop
    sb      $v0, 1($t0)    #O(X):OO


    srl     $a0, $a1, 12
    jal     hexasc
    nop
    sb      $v0, 0($t0)    #(X)o:OO

    move    $a0, $t0

    POP $ra
    POP $a0
    jr    $ra
    nop


.global enable_interrupt
enable_interrupt:
	ei
	jr $ra
	nop