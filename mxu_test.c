#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

//asm volatile(".include \"mxu1_as_macros.s.h\" \n");
asm (".include \"mxu1_as_macros.s.h\" \n");

void mxu_init(void)
{
	printf("Enabling MIPS XBurst MXU SIMD instructions..\n");
	fflush(NULL);
    __asm__ __volatile__ (
    "ori    $t0, $zero, 1  \n"  // $t0 = 1
    "s32i2m xr16, $t0      \n"  // Enable MXU instructions
    "nop                   \n"  // There must be 3 non-MXU instructions
    "nop                   \n"  //  issued afterwards before MXU
    "nop                   \n"  //  instructions are ready for use.
    : // Output
    : // Input
    : // Clobber
	  "t0", "memory"
    );
}

void test_mxu1_rev1(void)
{
	uint32_t val1 = 0xfe;
	uint32_t val2 = 1;
	uint32_t *pval1 = &val1;
	uint32_t *pval2 = &val2;
	uint32_t res = 42;

	asm volatile (
	"s32ldd xr1, %[pval1], 0           \n"
	"s32ldd xr2, %[pval2], 0           \n"
	"d32add xr3, xr1, xr2, xr0, AA     \n"
	"s32m2i xr3, %[res]                \n"
    : // Output
	  [res]     "=r" (res)
    : // Input
	  [pval1]   "r"  (pval1),
	  [pval2]   "r"  (pval2)
    : // Clobber
	  "t0", "memory"
    );
	
	 printf("%s() 0x%x + 0x%x = %x\n", __func__, val1, val2, res);
	 fflush(NULL);
}

void test_mxu1_rev2(void)
{
	uint32_t val1 = 0xfe;
	uint32_t val2 = 1;
	uint32_t *pval1 = &val1;
	uint32_t *pval2 = &val2;
	uint32_t res = 42;

	 __asm__ __volatile__ (
	"s8ldd xr1,  %[pval1], 0, ptn0     \n"
	"s8ldd xr2,  %[pval2], 0, ptn0     \n"
	"d32add xr3, xr1, xr2, xr0, AA     \n"
	"s32m2i xr3, %[res]                \n"
    : // Output
	  [res]     "=r" (res)
    : // Input
	  [pval1]   "r"  (pval1),
	  [pval2]   "r"  (pval2)
    : // Clobber
	  "t0", "memory"
    );
	
	 printf("%s() 0x%x + 0x%x = %x\n", __func__, val1, val2, res);
	 fflush(NULL);
}

int main()
{
	mxu_init();

	printf("Testing MXU1 rev1 opcode..\n");
	fflush(NULL);
	test_mxu1_rev1();

	printf("Testing MXU1 rev2 opcode..\n");
	fflush(NULL);
	test_mxu1_rev2();

	return 0;
}
