#ifndef _GLOBAL_MACROS_H_
#define _GLOBAL_MACROS_H_

// =============================================================================
//  MACROS
// =============================================================================
#define REG_READ_UINT32( _reg_ )		(*(volatile unsigned int*)(_reg_))
#define REG_WRITE_UINT32( _reg_, _val_) 	((*(volatile unsigned int*)(_reg_)) = (unsigned int)(_val_))

#endif

