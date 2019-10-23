#include <unwind.h>
#include <bl_common.h>

static _Unwind_Reason_Code stack_func(struct _Unwind_Context* ctx, void* arg)
{
	_Unwind_Ptr rs;
	int *depth = (int *)arg;

	printf("#%d: pc at %08x\n", *depth, _Unwind_GetIP(ctx));
	(*depth)++;

 	rs = _Unwind_GetRegionStart (ctx);
	printf("\tBase+off: 0x%08x+0x%x\n", (u32)rs, _Unwind_GetGR(ctx, 15) - (u32)rs);
	printf("\tpc: 0x%08x  lr:0x%08x  sp:0x%08x  ip:0x%08x\n",
		_Unwind_GetGR(ctx, 15), _Unwind_GetGR(ctx, 14),
		_Unwind_GetGR(ctx, 13), _Unwind_GetGR(ctx, 12));

	printf("\tr0: 0x%08x  r1: 0x%08x  r2: 0x%08x  r3: 0x%08x\n", 
		_Unwind_GetGR(ctx, 0), _Unwind_GetGR(ctx, 1), 
		_Unwind_GetGR(ctx, 2), _Unwind_GetGR(ctx, 3));

	printf("\tr4: 0x%08x  r5: 0x%08x  r6: 0x%08x  r7: 0x%08x\n",
		_Unwind_GetGR(ctx, 4), _Unwind_GetGR(ctx, 5), 
		_Unwind_GetGR(ctx, 6), _Unwind_GetGR(ctx, 7));

	printf("\tr8: 0x%08x  r9: 0x%08x  r10: 0x%08x  r11: 0x%08x\n",
		_Unwind_GetGR(ctx, 8), _Unwind_GetGR(ctx, 9), 
		_Unwind_GetGR(ctx, 10), _Unwind_GetGR(ctx, 11)); 

  return(_URC_NO_REASON);
}

void dump_stack(void)
{
	printf("%s enter!\n", __func__);
	int ret = 0;
	int depth = 0;

	ret = _Unwind_Backtrace(stack_func, &depth);
	printf("ret = %d \n", ret);
}
