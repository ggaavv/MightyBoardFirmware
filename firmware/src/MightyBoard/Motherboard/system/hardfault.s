	.syntax unified
	.cpu cortex-m3
	.fpu softvfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 1
	.eabi_attribute 34, 1
	.eabi_attribute 18, 4
	.thumb
	.file	"hardfault.c"
	.text
.Ltext0:
	.cfi_sections	.debug_frame
	.section	.text.hard_fault_handler_c,"ax",%progbits
	.align	2
	.global	hard_fault_handler_c
	.thumb
	.thumb_func
	.type	hard_fault_handler_c, %function
hard_fault_handler_c:
.LFB55:
	.file 1 "src/MightyBoard/Motherboard/system/hardfault.c"
	.loc 1 9 0
	.cfi_startproc
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
.LVL0:
	push	{r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
.LCFI0:
	.cfi_def_cfa_offset 40
	.cfi_offset 14, -4
	.cfi_offset 11, -8
	.cfi_offset 10, -12
	.cfi_offset 9, -16
	.cfi_offset 8, -20
	.cfi_offset 7, -24
	.cfi_offset 6, -28
	.cfi_offset 5, -32
	.cfi_offset 4, -36
	.cfi_offset 3, -40
	.loc 1 19 0
	ldr	r5, [r0, #0]
.LVL1:
	.loc 1 20 0
	ldr	r4, [r0, #4]
.LVL2:
	.loc 1 21 0
	ldr	r6, [r0, #8]
.LVL3:
	.loc 1 22 0
	ldr	r7, [r0, #12]
.LVL4:
	.loc 1 24 0
	ldr	r8, [r0, #16]
.LVL5:
	.loc 1 25 0
	ldr	r9, [r0, #20]
.LVL6:
	.loc 1 26 0
	ldr	sl, [r0, #24]
.LVL7:
	.loc 1 27 0
	ldr	fp, [r0, #28]
.LVL8:
	.loc 1 29 0
	movw	r0, #:lower16:.LC0
.LVL9:
	movt	r0, #:upper16:.LC0
	bl	puts
	.loc 1 30 0
	movw	r0, #:lower16:.LC1
	movt	r0, #:upper16:.LC1
	mov	r1, r5
	bl	printf
	.loc 1 31 0
	movw	r0, #:lower16:.LC2
	movt	r0, #:upper16:.LC2
	mov	r1, r4
	bl	printf
	.loc 1 32 0
	movw	r0, #:lower16:.LC3
	movt	r0, #:upper16:.LC3
	mov	r1, r6
	bl	printf
	.loc 1 33 0
	movw	r0, #:lower16:.LC4
	movt	r0, #:upper16:.LC4
	mov	r1, r7
	bl	printf
	.loc 1 34 0
	movw	r0, #:lower16:.LC5
	movt	r0, #:upper16:.LC5
	mov	r1, r8
	bl	printf
	.loc 1 35 0
	movw	r0, #:lower16:.LC6
	movt	r0, #:upper16:.LC6
	mov	r1, r9
	bl	printf
	.loc 1 36 0
	movw	r0, #:lower16:.LC7
	movt	r0, #:upper16:.LC7
	mov	r1, sl
	bl	printf
	.loc 1 37 0
	movw	r0, #:lower16:.LC8
	movt	r0, #:upper16:.LC8
	mov	r1, fp
	bl	printf
	.loc 1 38 0
	movw	r3, #60728
	movt	r3, 57344
	ldr	r1, [r3, #0]
	movw	r0, #:lower16:.LC9
	movt	r0, #:upper16:.LC9
	bl	printf
	.loc 1 39 0
	movw	r3, #60712
	movt	r3, 57344
	ldr	r1, [r3, #0]
	movw	r0, #:lower16:.LC10
	movt	r0, #:upper16:.LC10
	bl	printf
	.loc 1 40 0
	movw	r3, #60716
	movt	r3, 57344
	ldr	r1, [r3, #0]
	movw	r0, #:lower16:.LC11
	movt	r0, #:upper16:.LC11
	bl	printf
	.loc 1 41 0
	movw	r3, #60720
	movt	r3, 57344
	ldr	r1, [r3, #0]
	movw	r0, #:lower16:.LC12
	movt	r0, #:upper16:.LC12
	bl	printf
	.loc 1 42 0
	movw	r3, #60732
	movt	r3, 57344
	ldr	r1, [r3, #0]
	movw	r0, #:lower16:.LC13
	movt	r0, #:upper16:.LC13
	bl	printf
	.loc 1 43 0
	mov	r3, #60672
	movt	r3, 57344
	ldr	r1, [r3, #36]
	movw	r0, #:lower16:.LC14
	movt	r0, #:upper16:.LC14
	bl	printf
.L2:
	b	.L2
	.cfi_endproc
.LFE55:
	.size	hard_fault_handler_c, .-hard_fault_handler_c
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	.ascii	"\012\012[Hard fault handler - all numbers in hex]\000"
.LC1:
	.ascii	"R0 = %x\012\000"
	.space	3
.LC2:
	.ascii	"R1 = %x\012\000"
	.space	3
.LC3:
	.ascii	"R2 = %x\012\000"
	.space	3
.LC4:
	.ascii	"R3 = %x\012\000"
	.space	3
.LC5:
	.ascii	"R12 = %x\012\000"
	.space	2
.LC6:
	.ascii	"LR [R14] = %x  subroutine call return address\012\000"
	.space	1
.LC7:
	.ascii	"PC [R15] = %x  program counter\012\000"
.LC8:
	.ascii	"PSR = %x\012\000"
	.space	2
.LC9:
	.ascii	"BFAR = %x\012\000"
	.space	1
.LC10:
	.ascii	"CFSR = %x\012\000"
	.space	1
.LC11:
	.ascii	"HFSR = %x\012\000"
	.space	1
.LC12:
	.ascii	"DFSR = %x\012\000"
	.space	1
.LC13:
	.ascii	"AFSR = %x\012\000"
	.space	1
.LC14:
	.ascii	"SCB_SHCSR = %x\012\000"
	.text
.Letext0:
	.file 2 "c:\\program files\\codesourcery\\sourcery_codebench_lite_for_arm_eabi\\bin\\../lib/gcc/arm-none-eabi/4.6.3/../../../../arm-none-eabi/include/stdint.h"
	.file 3 "src/MightyBoard/Motherboard/system/core_cm3.h"
	.file 4 "<built-in>"
	.section	.debug_info,"",%progbits
.Ldebug_info0:
	.4byte	0x302
	.2byte	0x2
	.4byte	.Ldebug_abbrev0
	.byte	0x4
	.uleb128 0x1
	.4byte	.LASF35
	.byte	0x1
	.4byte	.LASF36
	.4byte	.LASF37
	.4byte	0
	.4byte	0
	.4byte	.Ldebug_ranges0+0
	.4byte	.Ldebug_line0
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.4byte	.LASF0
	.uleb128 0x3
	.4byte	.LASF4
	.byte	0x2
	.byte	0x2a
	.4byte	0x3b
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.4byte	.LASF1
	.uleb128 0x2
	.byte	0x2
	.byte	0x5
	.4byte	.LASF2
	.uleb128 0x2
	.byte	0x2
	.byte	0x7
	.4byte	.LASF3
	.uleb128 0x3
	.4byte	.LASF5
	.byte	0x2
	.byte	0x4f
	.4byte	0x5b
	.uleb128 0x2
	.byte	0x4
	.byte	0x5
	.4byte	.LASF6
	.uleb128 0x3
	.4byte	.LASF7
	.byte	0x2
	.byte	0x50
	.4byte	0x6d
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.4byte	.LASF8
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.4byte	.LASF9
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.4byte	.LASF10
	.uleb128 0x4
	.byte	0x4
	.byte	0x5
	.ascii	"int\000"
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.4byte	.LASF11
	.uleb128 0x5
	.4byte	0x62
	.uleb128 0x6
	.byte	0x74
	.byte	0x3
	.byte	0xf3
	.4byte	0x1b0
	.uleb128 0x7
	.4byte	.LASF12
	.byte	0x3
	.byte	0xf5
	.4byte	0x1b0
	.byte	0x2
	.byte	0x23
	.uleb128 0
	.uleb128 0x7
	.4byte	.LASF13
	.byte	0x3
	.byte	0xf6
	.4byte	0x90
	.byte	0x2
	.byte	0x23
	.uleb128 0x4
	.uleb128 0x7
	.4byte	.LASF14
	.byte	0x3
	.byte	0xf7
	.4byte	0x90
	.byte	0x2
	.byte	0x23
	.uleb128 0x8
	.uleb128 0x7
	.4byte	.LASF15
	.byte	0x3
	.byte	0xf8
	.4byte	0x90
	.byte	0x2
	.byte	0x23
	.uleb128 0xc
	.uleb128 0x8
	.ascii	"SCR\000"
	.byte	0x3
	.byte	0xf9
	.4byte	0x90
	.byte	0x2
	.byte	0x23
	.uleb128 0x10
	.uleb128 0x8
	.ascii	"CCR\000"
	.byte	0x3
	.byte	0xfa
	.4byte	0x90
	.byte	0x2
	.byte	0x23
	.uleb128 0x14
	.uleb128 0x8
	.ascii	"SHP\000"
	.byte	0x3
	.byte	0xfb
	.4byte	0x1c5
	.byte	0x2
	.byte	0x23
	.uleb128 0x18
	.uleb128 0x7
	.4byte	.LASF16
	.byte	0x3
	.byte	0xfc
	.4byte	0x90
	.byte	0x2
	.byte	0x23
	.uleb128 0x24
	.uleb128 0x7
	.4byte	.LASF17
	.byte	0x3
	.byte	0xfd
	.4byte	0x90
	.byte	0x2
	.byte	0x23
	.uleb128 0x28
	.uleb128 0x7
	.4byte	.LASF18
	.byte	0x3
	.byte	0xfe
	.4byte	0x90
	.byte	0x2
	.byte	0x23
	.uleb128 0x2c
	.uleb128 0x7
	.4byte	.LASF19
	.byte	0x3
	.byte	0xff
	.4byte	0x90
	.byte	0x2
	.byte	0x23
	.uleb128 0x30
	.uleb128 0x9
	.4byte	.LASF20
	.byte	0x3
	.2byte	0x100
	.4byte	0x90
	.byte	0x2
	.byte	0x23
	.uleb128 0x34
	.uleb128 0x9
	.4byte	.LASF21
	.byte	0x3
	.2byte	0x101
	.4byte	0x90
	.byte	0x2
	.byte	0x23
	.uleb128 0x38
	.uleb128 0x9
	.4byte	.LASF22
	.byte	0x3
	.2byte	0x102
	.4byte	0x90
	.byte	0x2
	.byte	0x23
	.uleb128 0x3c
	.uleb128 0xa
	.ascii	"PFR\000"
	.byte	0x3
	.2byte	0x103
	.4byte	0x1da
	.byte	0x2
	.byte	0x23
	.uleb128 0x40
	.uleb128 0xa
	.ascii	"DFR\000"
	.byte	0x3
	.2byte	0x104
	.4byte	0x1b0
	.byte	0x2
	.byte	0x23
	.uleb128 0x48
	.uleb128 0xa
	.ascii	"ADR\000"
	.byte	0x3
	.2byte	0x105
	.4byte	0x1b0
	.byte	0x2
	.byte	0x23
	.uleb128 0x4c
	.uleb128 0x9
	.4byte	.LASF23
	.byte	0x3
	.2byte	0x106
	.4byte	0x1f4
	.byte	0x2
	.byte	0x23
	.uleb128 0x50
	.uleb128 0x9
	.4byte	.LASF24
	.byte	0x3
	.2byte	0x107
	.4byte	0x20e
	.byte	0x2
	.byte	0x23
	.uleb128 0x60
	.byte	0
	.uleb128 0xb
	.4byte	0x90
	.uleb128 0xc
	.4byte	0x30
	.4byte	0x1c5
	.uleb128 0xd
	.4byte	0x89
	.byte	0xb
	.byte	0
	.uleb128 0x5
	.4byte	0x1b5
	.uleb128 0xc
	.4byte	0x62
	.4byte	0x1da
	.uleb128 0xd
	.4byte	0x89
	.byte	0x1
	.byte	0
	.uleb128 0xb
	.4byte	0x1df
	.uleb128 0x5
	.4byte	0x1ca
	.uleb128 0xc
	.4byte	0x62
	.4byte	0x1f4
	.uleb128 0xd
	.4byte	0x89
	.byte	0x3
	.byte	0
	.uleb128 0xb
	.4byte	0x1f9
	.uleb128 0x5
	.4byte	0x1e4
	.uleb128 0xc
	.4byte	0x62
	.4byte	0x20e
	.uleb128 0xd
	.4byte	0x89
	.byte	0x4
	.byte	0
	.uleb128 0xb
	.4byte	0x213
	.uleb128 0x5
	.4byte	0x1fe
	.uleb128 0xe
	.4byte	.LASF25
	.byte	0x3
	.2byte	0x108
	.4byte	0x95
	.uleb128 0xf
	.byte	0x1
	.4byte	.LASF38
	.byte	0x1
	.byte	0x8
	.byte	0x1
	.4byte	.LFB55
	.4byte	.LFE55
	.4byte	.LLST0
	.4byte	0x2da
	.uleb128 0x10
	.4byte	.LASF39
	.byte	0x1
	.byte	0x8
	.4byte	0x2da
	.4byte	.LLST1
	.uleb128 0x11
	.4byte	.LASF26
	.byte	0x1
	.byte	0xa
	.4byte	0x89
	.4byte	.LLST2
	.uleb128 0x11
	.4byte	.LASF27
	.byte	0x1
	.byte	0xb
	.4byte	0x89
	.4byte	.LLST3
	.uleb128 0x11
	.4byte	.LASF28
	.byte	0x1
	.byte	0xc
	.4byte	0x89
	.4byte	.LLST4
	.uleb128 0x11
	.4byte	.LASF29
	.byte	0x1
	.byte	0xd
	.4byte	0x89
	.4byte	.LLST5
	.uleb128 0x11
	.4byte	.LASF30
	.byte	0x1
	.byte	0xe
	.4byte	0x89
	.4byte	.LLST6
	.uleb128 0x11
	.4byte	.LASF31
	.byte	0x1
	.byte	0xf
	.4byte	0x89
	.4byte	.LLST7
	.uleb128 0x11
	.4byte	.LASF32
	.byte	0x1
	.byte	0x10
	.4byte	0x89
	.4byte	.LLST8
	.uleb128 0x11
	.4byte	.LASF33
	.byte	0x1
	.byte	0x11
	.4byte	0x89
	.4byte	.LLST9
	.uleb128 0x12
	.byte	0x1
	.4byte	.LASF40
	.byte	0x4
	.byte	0
	.byte	0x1
	.4byte	0x82
	.byte	0x1
	.uleb128 0x13
	.4byte	0x2e0
	.uleb128 0x14
	.byte	0
	.byte	0
	.uleb128 0x15
	.byte	0x4
	.4byte	0x89
	.uleb128 0x15
	.byte	0x4
	.4byte	0x2e6
	.uleb128 0xb
	.4byte	0x2eb
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.4byte	.LASF34
	.uleb128 0x16
	.4byte	.LASF41
	.byte	0x3
	.2byte	0x487
	.4byte	0x300
	.byte	0x1
	.byte	0x1
	.uleb128 0x5
	.4byte	0x50
	.byte	0
	.section	.debug_abbrev,"",%progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x52
	.uleb128 0x1
	.uleb128 0x55
	.uleb128 0x6
	.uleb128 0x10
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0x35
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0x13
	.byte	0x1
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0x26
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xe
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xf
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0x6
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x10
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x11
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x12
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3c
	.uleb128 0xc
	.byte	0
	.byte	0
	.uleb128 0x13
	.uleb128 0x5
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x14
	.uleb128 0x18
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x15
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x16
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3c
	.uleb128 0xc
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_loc,"",%progbits
.Ldebug_loc0:
.LLST0:
	.4byte	.LFB55
	.4byte	.LCFI0
	.2byte	0x2
	.byte	0x7d
	.sleb128 0
	.4byte	.LCFI0
	.4byte	.LFE55
	.2byte	0x2
	.byte	0x7d
	.sleb128 40
	.4byte	0
	.4byte	0
.LLST1:
	.4byte	.LVL0
	.4byte	.LVL9
	.2byte	0x1
	.byte	0x50
	.4byte	0
	.4byte	0
.LLST2:
	.4byte	.LVL1
	.4byte	.LVL9
	.2byte	0x2
	.byte	0x70
	.sleb128 0
	.4byte	.LVL9
	.4byte	.LFE55
	.2byte	0x1
	.byte	0x55
	.4byte	0
	.4byte	0
.LLST3:
	.4byte	.LVL2
	.4byte	.LVL9
	.2byte	0x2
	.byte	0x70
	.sleb128 4
	.4byte	.LVL9
	.4byte	.LFE55
	.2byte	0x1
	.byte	0x54
	.4byte	0
	.4byte	0
.LLST4:
	.4byte	.LVL3
	.4byte	.LVL9
	.2byte	0x2
	.byte	0x70
	.sleb128 8
	.4byte	.LVL9
	.4byte	.LFE55
	.2byte	0x1
	.byte	0x56
	.4byte	0
	.4byte	0
.LLST5:
	.4byte	.LVL4
	.4byte	.LVL9
	.2byte	0x2
	.byte	0x70
	.sleb128 12
	.4byte	.LVL9
	.4byte	.LFE55
	.2byte	0x1
	.byte	0x57
	.4byte	0
	.4byte	0
.LLST6:
	.4byte	.LVL5
	.4byte	.LVL9
	.2byte	0x2
	.byte	0x70
	.sleb128 16
	.4byte	.LVL9
	.4byte	.LFE55
	.2byte	0x1
	.byte	0x58
	.4byte	0
	.4byte	0
.LLST7:
	.4byte	.LVL6
	.4byte	.LVL9
	.2byte	0x2
	.byte	0x70
	.sleb128 20
	.4byte	.LVL9
	.4byte	.LFE55
	.2byte	0x1
	.byte	0x59
	.4byte	0
	.4byte	0
.LLST8:
	.4byte	.LVL7
	.4byte	.LVL9
	.2byte	0x2
	.byte	0x70
	.sleb128 24
	.4byte	.LVL9
	.4byte	.LFE55
	.2byte	0x1
	.byte	0x5a
	.4byte	0
	.4byte	0
.LLST9:
	.4byte	.LVL8
	.4byte	.LVL9
	.2byte	0x2
	.byte	0x70
	.sleb128 28
	.4byte	.LVL9
	.4byte	.LFE55
	.2byte	0x1
	.byte	0x5b
	.4byte	0
	.4byte	0
	.section	.debug_aranges,"",%progbits
	.4byte	0x1c
	.2byte	0x2
	.4byte	.Ldebug_info0
	.byte	0x4
	.byte	0
	.2byte	0
	.2byte	0
	.4byte	.LFB55
	.4byte	.LFE55-.LFB55
	.4byte	0
	.4byte	0
	.section	.debug_ranges,"",%progbits
.Ldebug_ranges0:
	.4byte	.LFB55
	.4byte	.LFE55
	.4byte	0
	.4byte	0
	.section	.debug_line,"",%progbits
.Ldebug_line0:
	.section	.debug_str,"MS",%progbits,1
.LASF23:
	.ascii	"MMFR\000"
.LASF16:
	.ascii	"SHCSR\000"
.LASF33:
	.ascii	"stacked_psr\000"
.LASF4:
	.ascii	"uint8_t\000"
.LASF39:
	.ascii	"hardfault_args\000"
.LASF31:
	.ascii	"stacked_lr\000"
.LASF0:
	.ascii	"signed char\000"
.LASF30:
	.ascii	"stacked_r12\000"
.LASF1:
	.ascii	"unsigned char\000"
.LASF8:
	.ascii	"long unsigned int\000"
.LASF3:
	.ascii	"short unsigned int\000"
.LASF17:
	.ascii	"CFSR\000"
.LASF41:
	.ascii	"ITM_RxBuffer\000"
.LASF35:
	.ascii	"GNU C 4.6.3\000"
.LASF14:
	.ascii	"VTOR\000"
.LASF18:
	.ascii	"HFSR\000"
.LASF2:
	.ascii	"short int\000"
.LASF37:
	.ascii	"C:\\Users\\gavin\\git\\MightyBoardFirmware\\firmwar"
	.ascii	"e\000"
.LASF25:
	.ascii	"SCB_Type\000"
.LASF11:
	.ascii	"unsigned int\000"
.LASF19:
	.ascii	"DFSR\000"
.LASF10:
	.ascii	"long long unsigned int\000"
.LASF13:
	.ascii	"ICSR\000"
.LASF32:
	.ascii	"stacked_pc\000"
.LASF15:
	.ascii	"AIRCR\000"
.LASF24:
	.ascii	"ISAR\000"
.LASF21:
	.ascii	"BFAR\000"
.LASF5:
	.ascii	"int32_t\000"
.LASF38:
	.ascii	"hard_fault_handler_c\000"
.LASF9:
	.ascii	"long long int\000"
.LASF12:
	.ascii	"CPUID\000"
.LASF34:
	.ascii	"char\000"
.LASF40:
	.ascii	"printf\000"
.LASF20:
	.ascii	"MMFAR\000"
.LASF36:
	.ascii	"src/MightyBoard/Motherboard/system/hardfault.c\000"
.LASF7:
	.ascii	"uint32_t\000"
.LASF6:
	.ascii	"long int\000"
.LASF26:
	.ascii	"stacked_r0\000"
.LASF27:
	.ascii	"stacked_r1\000"
.LASF28:
	.ascii	"stacked_r2\000"
.LASF29:
	.ascii	"stacked_r3\000"
.LASF22:
	.ascii	"AFSR\000"
	.ident	"GCC: (Sourcery CodeBench Lite 2012.03-56) 4.6.3"
