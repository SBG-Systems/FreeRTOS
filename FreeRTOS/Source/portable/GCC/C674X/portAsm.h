; Keep in mind that the ABI mandates that the stack pointer be 8-byte aligned,
; and that the memory at *SP be available.
;
; In addition, don't save B15:B14 since B15 is the stack pointer itself, and B14
; is the data-page pointer used for near accesses, set once during startup by
; the C runtime, and never changed. This allows using A15 as a temporary stack
; pointer for parallel doubleword loads and stores.
;
; See C6000 EABI (SPRAB89a), 4.4 Frame Layout.

.macro portSAVE_CONTEXT rp, tsr
	stdw	.d2t1	a15:a14, *--b15
	mv		.d1x	b15, a15

	stdw	.d1t1	a1:a0, *--a15
||	stdw	.d2t2	b1:b0, *--b15[2]
	stdw	.d1t1	a3:a2, *--a15[2]
||	stdw	.d2t2	b3:b2, *--b15[2]
||	mvc		.s2		\tsr, b0
	stdw	.d1t1	a5:a4, *--a15[2]
||	stdw	.d2t2	b5:b4, *--b15[2]
||	mvc		.s2		ilc, b1
	stdw	.d1t1	a7:a6, *--a15[2]
||	stdw	.d2t2	b7:b6, *--b15[2]
||	mvc		.s2		rilc, b2
	stdw	.d1t1	a9:a8, *--a15[2]
||	stdw	.d2t2	b9:b8, *--b15[2]
||	mvc		.s2		fadcr, b3
	stdw	.d1t1	a11:a10, *--a15[2]
||	stdw	.d2t2	b11:b10, *--b15[2]
||	mvc		.s2		faucr, b4
	stdw	.d1t1	a13:a12, *--a15[2]
||	stdw	.d2t2	b13:b12, *--b15[2]
||	mvc		.s2		fmcr, b5
	stdw	.d1t1	a17:a16, *--a15[2]
||	stdw	.d2t2	b17:b16, *--b15[2]
||	mvc		.s2		\rp, b6
	stdw	.d1t1	a19:a18, *--a15[2]
||	stdw	.d2t2	b19:b18, *--b15[2]
||	mv		.s1x	b4, a0
	stdw	.d1t1	a21:a20, *--a15[2]
||	stdw	.d2t2	b21:b20, *--b15[2]
||	mv		.s1x	b5, a1
	stdw	.d1t1	a23:a22, *--a15[2]
||	stdw	.d2t2	b23:b22, *--b15[2]
||	mv		.s1x	b6, a2
	stdw	.d1t1	a25:a24, *--a15[2]
||	stdw	.d2t2	b25:b24, *--b15[2]
	stdw	.d1t1	a27:a26, *--a15[2]
||	stdw	.d2t2	b27:b26, *--b15[2]
	stdw	.d1t1	a29:a28, *--a15[2]
||	stdw	.d2t2	b29:b28, *--b15[2]
	stdw	.d1t1	a31:a30, *--a15[2]
||	stdw	.d2t2	b31:b30, *--b15[2]

	stdw	.d1t1	a1:a0, *--a15[2]
||	stdw	.d2t2	b1:b0, *--b15[2]
||	mvkl	.s2		pxCurrentTCB, b4
	stdw	.d1t1	a3:a2, *--a15[2]
||	stdw	.d2t2	b3:b2, *--b15[2]
||	mvkh	.s2		pxCurrentTCB, b4

	ldw		.d2t2	*b4, b0
	mv		.d1x	b15, a15
	addk	.s2		-8, b15
	nop 2
	stw		.d2t1	a15, *b0
.endm

.macro portRESTORE_CONTEXT rp, tsr
	mvkl	.s2		pxCurrentTCB, b4
	mvkh	.s2		pxCurrentTCB, b4
	ldw		.d2t2	*b4, b0
	nop 4
	ldw		.d2t2	*b0, b15
	mvk		.l1		8, a15
	nop 3
	add		.d1x	a15, b15, a15

	lddw	.d2t2	*b15++[2], b3:b2
	lddw	.d1t1	*a15++[2], a3:a2
	lddw	.d2t2	*b15++[2], b1:b0
	lddw	.d1t1	*a15++[2], a1:a0
	nop 1
	mvc		.s2		b3, fadcr
	mvc		.s2		b2, rilc
||	mv		.d2x	a2, b6
	mvc		.s2		b1, ilc
	mvc		.s2		b0, \tsr
||	mv		.d2x	a1, b5

	lddw	.d2t2	*b15++[2],		b31:b30
||	lddw	.d1t1	*a15++[2],		a31:a30
||	mv		.s2x	a0, b4
	lddw	.d2t2	*b15++[2],		b29:b28
||	lddw	.d1t1	*a15++[2],		a29:a28
||	mvc		.s2		b6, \rp
	lddw	.d2t2	*b15++[2],		b27:b26
||	lddw	.d1t1	*a15++[2],		a27:a26
||	mvc		.s2		b5, fmcr
	lddw	.d2t2	*b15++[2],		b25:b24
||	lddw	.d1t1	*a15++[2],		a25:a24
||	mvc		.s2		b4, faucr
	lddw	.d2t2	*b15++[2], b23:b22
||	lddw	.d1t1	*a15++[2], a23:a22
	lddw	.d2t2	*b15++[2], b21:b20
||	lddw	.d1t1	*a15++[2], a21:a20
	lddw	.d2t2	*b15++[2], b19:b18
||	lddw	.d1t1	*a15++[2], a19:a18
	lddw	.d2t2	*b15++[2], b17:b16
||	lddw	.d1t1	*a15++[2], a17:a16
	lddw	.d2t2	*b15++[2], b13:b12
||	lddw	.d1t1	*a15++[2], a13:a12
	lddw	.d2t2	*b15++[2], b11:b10
||	lddw	.d1t1	*a15++[2], a11:a10
	lddw	.d2t2	*b15++[2], b9:b8
||	lddw	.d1t1	*a15++[2], a9:a8
	lddw	.d2t2	*b15++[2], b7:b6
||	lddw	.d1t1	*a15++[2], a7:a6
	lddw	.d2t2	*b15++[2], b5:b4
||	lddw	.d1t1	*a15++[2], a5:a4
	lddw	.d2t2	*b15++[2], b3:b2
||	lddw	.d1t1	*a15++[2], a3:a2
	lddw	.d2t2	*b15++[2], b1:b0
||	lddw	.d1t1	*a15, a1:a0

	b		.s2		\rp
	lddw	.d2t1	*b15++, a15:a14
	nop 4
.endm
