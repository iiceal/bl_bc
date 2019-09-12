#ifndef __BL_COMMON_H_
#define	__BL_COMMON_H_

#include <common.h>
#define BL_CMD_SHORT_NAME_LENGTH    (16)

struct BL_CNXT_ST {
    int notused;
};

typedef struct BL_CNXT_ST BL_CNXT_T;

typedef unsigned char* 	(*BL_CMD_GET_DESCRIPTION) (void);
typedef unsigned int 	(*BL_CMD_AUOTO_FN)(BL_CNXT_T *pCnxt, unsigned int argc, const char **);
typedef int 		(*BL_CMD_ON_ENTER_FN)(void);
typedef void 		(*BL_CMD_ON_EXIT_FN) (void);
typedef int (*BL_CMD_REG_MENU_FN)(DYN_SHELL_ITEM_T * p_shell);

typedef struct BL_CMD_ST {  
    unsigned char    			name[BL_CMD_SHORT_NAME_LENGTH];
    BL_CMD_GET_DESCRIPTION     	cmd_description;
    BL_CMD_GET_DESCRIPTION     	cmd_long_description;
    BL_CMD_AUOTO_FN  			cmd_entry_func;
//    const BL_CMD_IOMUX_SETTING_T*   	iomux;
}BL_CMD_T;

typedef struct BL_CMD_INFO_ST
{
   BL_CMD_T* 		p_cmd_descriptor;
   unsigned int  	result;
} BL_CMD_INFO_T;

typedef BL_CMD_T * (* BL_GET_CMD_DESCRIPTOR_T)(BL_CNXT_T *pCnxt);
extern BL_GET_CMD_DESCRIPTOR_T __cmd_descriptor_handle_begin[];
extern BL_GET_CMD_DESCRIPTOR_T __cmd_descriptor_handle_end[];

#define DECLARE_BL_CMD(get_case_descriptor_fn)  \
static BL_GET_CMD_DESCRIPTOR_T __descriptor_func_##get_case_descriptor_fn\
                                  __attribute__((used)) \
                            __attribute__((__section__("func.get_case_descriptor"))) = get_case_descriptor_fn

#define BL_SHELL_ADD_CMD(p_shell,  short_name, rep,help_info, long_desc, cmd_func)        \
        dyn_shell_add_command(p_shell, (unsigned char*)short_name, rep, (unsigned char*)help_info, (unsigned char*)long_desc, cmd_func)

#define BL_SHELL_ADD_SUB_SHELL(p_shell,  short_name,help_info, shell_func)        \
        dyn_shell_add_sub_shell(p_shell, (unsigned char*) short_name, (unsigned char*)help_info, shell_func)


#define RET_NOTTEST (0xFFFFFFFF)
#define RET_FAIL   (0xFFFF0000)
#define RET_NA     (0xFFFE0000)      //Not applicable.
#define RET_BLOCK  (0xFFFD0000)      //condition is not ready..
#define RET_SUCCESS (0)
#define RET_PASS   RET_SUCCESS

#endif //__BL_COMMON_H_
