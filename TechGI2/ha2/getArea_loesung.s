#	$a0 = &length[0]
#	$a1 = &width[0]
#	$a2 = n
#	$f0 = 0.0
#	$f12 = Rueckgaberegister 
#	Richtige Ausgabe auf der Console: 60.92449951

Area:
	add.s  $f12, $f0, $f0	# sum = 0 = $f12
	addi	$t0, $0, 0		# i = 0 = $t0

loop:	beq	$t0, $a2, endloop	# if i = n goto endloop

	sll	$t1, $t0, 2		# $t1 = 4*i
	add 	$t1, $a0, $t1		# t1 = &length[i]
	l.s	$f1, 0($t1)		# f1 = length[i]
	
	sll	$t2, $t0, 2		# $t2 = 4*i
	add 	$t2, $a1, $t2		# t2 = &width[i]
	l.s	$f2, 0($t2)		# f2 = width[i]
	
	mul.s	$f1, $f1, $f2		# f1 = f1 * f2
	add.s	$f12, $f12, $f1	# sum = sum + f1
	
	addi	$t0, $t0, 1		# i++
	j	loop
endloop:

	jr 	$ra

	
.data

length: .float 3.45, 4.36, 2.45, 1.8, 2.7
width:  .float 5.68, 3.2 , 1.23, 0.5, 8.69
n:   	.word 5

.text
.globl_main:

main:

	addi	$sp, $sp, -4		# save return adress
	sw	$ra, 0($sp)

	la	$a0, length	#load length adress
	la	$a1, width	#load width adress
	lw	$a2, n		#load n
	mtc1 	$0, $f0	#put 0.0 in $f0

	jal 	Area
	
	li 	$v0, 2 	#print content of $f12
	syscall

	lw	$ra, 0($sp)		# restore return adress
	addi	$sp, $sp, 4
	jr	$ra
