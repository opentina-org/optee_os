// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2026, Allwinner Technology Co., Ltd.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef PLATFORM_CONFIG_H
#define PLATFORM_CONFIG_H

/* Make stacks aligned to data cache line length */
#define STACK_ALIGNMENT		64

#ifdef ARM64
#ifdef CFG_WITH_PAGER
#error "Pager not supported for ARM64"
#endif
#endif /*ARM64*/

/* system config */
#define CFG_TEE_CORE_NB_CORE	4

/* Location of TZDRAM
 *+----------------------------+
 *| NAME			| Addr list					|
 *| SHARED MEM		| TZDRAM_BASE - TEE_SHMEM_SIZE -> TZDRAM_BASE	|
 *| OPTEE OS		| TZDRAM_BASE -> TZDRAM_BASE + TEE_RAM_VA_SIZE	|
 *| TA MEM			| TZDRAM_BASE + TEE_RAM_VA_SIZE -> TZDRAM_BASE + TZDRAM_SIZE |
 */
#define TZDRAM_BASE		0x48600000
#define TZDRAM_SIZE		0x00A00000

/* Full GlobalPlatform test suite requires TEE_SHMEM_SIZE to be at least 2MB */
#define TEE_SHMEM_SIZE		0x400000
#define TEE_SHMEM_START		(TZDRAM_BASE - 0x400000)

/* TEE OS 1M */
#define TEE_RAM_VA_SIZE	(1024 * 1024)
#define TEE_RAM_PH_SIZE	TEE_RAM_VA_SIZE
#define TEE_RAM_START	TZDRAM_BASE

/*
 * Allwinner boot0 prepends a 4 KB sunxi tee head (sunxi_tee_head.bin) at
 * TEE_RAM_START before the OP-TEE binary.  TEE_LOAD_ADDR is offset by the
 * head size so that after:
 *   cat sunxi_tee_head.bin tee.bin > sunxi_tee.bin
 * boot0 loads sunxi_tee.bin at TEE_RAM_START and _start lands at
 * TEE_LOAD_ADDR automatically.
 */
#define SUNXI_TEE_HEAD_SIZE	0x1000
/*
 * Magic: the first 4 bytes of sunxi_tee_head.bin are the ARM32 instruction
 * "B +0xFF4" (0xEA0003FD).  This is used at runtime to detect whether the
 * head was prepended before reading head fields (e.g. uart_port).
 */
#define SUNXI_TEE_HEAD_MAGIC	0xEA0003FDU
#ifndef TEE_LOAD_ADDR
#define TEE_LOAD_ADDR	(TEE_RAM_START + SUNXI_TEE_HEAD_SIZE)
#endif

/* TA MEM 9M */
#define TA_RAM_SIZE		ROUNDDOWN((TZDRAM_SIZE - TEE_RAM_VA_SIZE), CORE_MMU_PGDIR_SIZE)
#define TA_RAM_START	ROUNDUP((TZDRAM_BASE + TEE_RAM_VA_SIZE), CORE_MMU_PGDIR_SIZE)

#define HEAP_SIZE		(128 * 1024)

#ifdef CFG_WITH_LPAE
#define MAX_XLAT_TABLES		5
#endif
/* end of system config */

/* register configs */
#define SUNXI_GIC600_BASE	0x03400000
#define GIC_BASE			SUNXI_GIC600_BASE
#define GICD_BASE			(SUNXI_GIC600_BASE + 0x0)
#define GIC_MAX_INTS		(287 + 1)

#define UART0_BASE				0x02600000
#define UART1_BASE				0x02601000
#define CONSOLE_UART_BASE		UART0_BASE
#define CONSOLE_BAUDRATE		115200
#define CONSOLE_UART_CLK_IN_HZ	24000000

#define DRAM0_BASE		0x40000000
#define DRAM0_SIZE		0x100000000
/* end of register configs */

#endif /*PLATFORM_CONFIG_H*/
