#ifndef _CPU_H_
#define _CPU_H_

#define REG_FLAGS_CF_MASK		0x00000001
#define REG_FLAGS_PF_MASK		0x00000004
#define REG_FLAGS_AF_MASK		0x00000010
#define REG_FLAGS_ZF_MASK		0x00000040
#define REG_FLAGS_SF_MASK		0x00000080
#define REG_FLAGS_TF_MASK		0x00000100
#define REG_FLAGS_IF_MASK		0x00000200
#define REG_FLAGS_DF_MASK		0x00000400
#define REG_FLAGS_OF_MASK		0x00000800

#define REG_FLAGS_IOPL_MASK		0x00003000
#define REG_FLAGS_IOPL_EXT(x)		((x & REG_FLAGS_IOPL_MASK) >> 12)
#define REG_FLAGS_IOPL_SYTH(x)		((x << 12) & REG_FLAGS_IOPL_MASK)

#define REG_FLAGS_NT_MASK		0x00004000
#define REG_FLAGS_RF_MASK		0x00010000
#define REG_FLAGS_VM_MASK		0x00020000
#define REG_FLAGS_AC_MASK		0x00040000
#define REG_FLAGS_VIF_MASK		0x00080000
#define REG_FLAGS_VIP_MASK		0x00100000
#define REG_FLAGS_ID_MASK		0x00200000

static inline uint64_t get_cpu_physical_address_width(void)
{
	uint64_t result; 
	__asm__(
		"mov $0x80000008, %%eax\n\t"
		"cpuid\n\t"
		"mov %%eax, %0\n\t"
		:"=m" (result)
		:
		:
	);
	return result & 0xff;
}
static inline uint64_t get_cpu_linear_address_width(void)
{
	uint64_t result; 
	__asm__(
		"mov $0x80000008, %%eax\n\t"
		"cpuid\n\t"
		"mov %%eax, %0\n\t"
		:"=m" (result)
		:
		:
	);
	return result >> 8;
}



static inline void __halt__(void)
{
	__asm__ ("hlt");
}
static inline void __enable_interrupt__(void)
{
	__asm__("sti");
}
	
static inline void __disable_interrupt__(void)
{
	__asm__ ("cli");
}
static inline uint64_t __get_idt_addr__(void)
{
	uint64_t result[2] = {0};
	
	__asm__ (
		"lea %0, %%rax\r\n"
		"sidt 6(%%rax)"
		: "=m" (result)
		:
	);
	
	return result[1];
}
static inline uint64_t __read_msr__(uint32_t addr)
{
	uint64_t result;
	
	__asm__ (
		"mov %0, %%ecx\r\n"
		"rdmsr\r\n"
		"lea %1, %%rdi\r\n"
		"mov %%eax, (%%rdi)\r\n"
		"mov %%edx, 4(%%rdi)\r\n"
		: "=m" (addr)
		: "m" (result)
	);
	
	return result;
}
static inline void __write_msr__(uint32_t addr, uint64_t data)
{
	__asm__ (
		"mov %0, %%ecx\r\n"
		"lea %1, %%rdi\r\n"
		"mov (%%rdi), %%eax\r\n"
		"mov 4(%%rdi), %%edx\r\n"
		"wrmsr\r\n"
		: 
		: "m" (addr), "m" (data)
	);
}
/**
 * Execute CPUID instruction and return information,
 * with input value EAX
 */
typedef struct cpu_id_info
{
	uint32_t eax;
	uint32_t ebx;
	uint32_t ecx;
	uint32_t edx;
}cpu_id_info;
static inline cpu_id_info __cpuid__(uint32_t eax)
{
	cpu_id_info ret;
	__asm__(
		"mov %0, %%eax\r\n"
		"cpuid\r\n"
		"lea %1, %%rdi\r\n"
		"mov %%eax, (%%rdi)\r\n"
		"mov %%ebx, 4(%%rdi)\r\n"
		"mov %%ecx, 8(%%rdi)\r\n"
		"mov %%edx, 12(%%rdi)\r\n"
		:
		: "m" (eax), "m" (ret)
	);
	return ret;
}
static inline uint64_t __read_cr0__()
{
	uint64_t cr0;
	__asm__ (
		"mov %%cr0, %%rax\r\n"
		"mov %%rax, %0\r\n"
		: "=m" (cr0)
		:
	);
	return cr0;
}
static inline uint64_t __read_cr3__()
{
	uint64_t cr3;
	__asm__ (
		"mov %%cr3, %%rax\r\n"
		"mov %%rax, %0\r\n"
		: "=m" (cr3)
		:
	);
	return cr3;
}
static inline uint64_t __read_rflags__(void)
{
	uint64_t rflags;
	__asm__ (
		"pushfq\r\n"
		"pop %0\r\n"
		: "=m" (rflags)
		:
	);
	return rflags;	
}
#endif
