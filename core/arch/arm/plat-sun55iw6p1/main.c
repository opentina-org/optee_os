// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2026, Allwinner Technology Co., Ltd.
 * All rights reserved.
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

#include <console.h>
#include <drivers/gic.h>
#include <drivers/serial8250_uart.h>
#include <kernel/panic.h>
#include <kernel/interrupt.h>
#include <kernel/tee_common_otp.h>
#include <mm/core_memprot.h>
#include <platform_config.h>
#include <stdint.h>
#include <tee/entry_std.h>
#include <tee/entry_fast.h>

register_phys_mem_pgdir(MEM_AREA_IO_NSEC, CONSOLE_UART_BASE, SERIAL8250_UART_REG_SIZE);

static struct serial8250_uart_data console_data;

void plat_console_init(void)
{
	uint32_t port_num, uart_base;

	/*
	 * Read uart_port from sunxi tee head if present.
	 * boot0 prepends sunxi_tee_head.bin at TEE_RAM_START; its first word is
	 * the ARM32 branch instruction SUNXI_TEE_HEAD_MAGIC (0xEA0003FD).
	 * Without the AW head (non-AW boot flow), TEE_RAM_START holds OP-TEE
	 * code -- the magic will not match and we fall back to UART0 (port 0).
	 */
	if (*(volatile uint32_t *)(TEE_RAM_START) == SUNXI_TEE_HEAD_MAGIC)
		port_num = *(volatile uint8_t *)(TEE_RAM_START + 0x10C);
	else
		port_num = 0; /* no AW head: default to UART0 */
	uart_base = UART0_BASE + port_num * (UART1_BASE - UART0_BASE);
	serial8250_uart_init(&console_data, uart_base, CONSOLE_UART_CLK_IN_HZ, CONSOLE_BAUDRATE);
	register_serial_console(&console_data.chip);

	DMSG("register uart-%d as console\n", port_num);
}

TEE_Result tee_otp_get_hw_unique_key(struct tee_hw_unique_key *hwkey)
{
	return TEE_ERROR_NO_DATA;
}

int tee_otp_get_die_id(uint8_t *buffer, size_t len)
{
	return TEE_ERROR_NO_DATA;
}
