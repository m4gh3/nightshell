.text
.global _ndl_runtime_resolver0_

_ndl_runtime_resolver0_:
	push	{r0,r1,r2,r3,r4}		//save params and allign stack to 8
	mov 	r0, lr					//lr should be the start of the .got
	ldr 	r1, [lr, #-8 ]			//r1 should point to the header
	sub 	r2, ip, r0
	sub     r2, #4
	lsr 	r2, #2					//plt id
	bl 	_ndl_runtime_resolver1_		//call to c
	push 	{r0}					//pushes return value
	add 	sp, sp, #4
	pop 	{r0-r4}
	pop 	{lr}
	ldr pc, [sp, #-28]				//load the return value into pc to jump!
