#ifndef __BL_SHELL_H__
#define __BL_SHELL_H__

#define MAX_MENU_ITEM 20
#define CTL_CH(c)	((c) - 'a' + 1)
#define	HIST_MAX    (2)
#define CTL_BACKSPACE	('\b')
#define	DEL		((char)255)
#define	DEL7		((char)127)
#define CREAD_HIST_CHAR	('!')
#define add_idx_minus_one() ((hist_add_idx == 0) ? hist_max : hist_add_idx-1)
	
#define BL_MAX_CMDS_NUM  (40)
#define BL_MAX_STRING  (256)

typedef struct SHELL_CMD_T 
{
  const char *cmd;
  const char *usage;
  int (*func)(struct SHELL_CMD_T *cptr, unsigned int argc, const char **);
}shell_cmd_t;

////////////////////////////////////////////////////////////////////////

typedef struct DYN_SHELL_ITEM_ST DYN_SHELL_ITEM_T;
typedef int (*DYN_SHELL_CREAT_SUB_SHELL_FUNC_T)(DYN_SHELL_ITEM_T * p_shell);
typedef int (*DYN_SHELL_CMD_FUNC_T)(struct SHELL_CMD_T *cptr, unsigned int argc, const char **);

struct DYN_SHELL_ITEM_ST 
{	
  unsigned int is_sub_shell;
  const char *cmdstr;
  const char *short_usage;
  union {
  	struct {
  		const char *detail_usage;  		
  		unsigned char rep;    		
	        DYN_SHELL_CMD_FUNC_T func;      
  	}cmd;
  	struct {
  		DYN_SHELL_ITEM_T *p_parent;
  		DYN_SHELL_ITEM_T *p_cmd_list;
  		DYN_SHELL_CREAT_SUB_SHELL_FUNC_T  create_func;
  	}sub_shell;
  };
  DYN_SHELL_ITEM_T* p_next;
};

//exterm DYN_SHELL_ITEM_T* g_p_root_shell;
//exterm DYN_SHELL_ITEM_T* g_p_common_cmd_shell;

extern int dyn_shell_add_command(DYN_SHELL_ITEM_T * p_shell, 
                                     const unsigned char *cmdstr,
                                     unsigned char 	 rep,
                                     const unsigned char *short_usage,
                                     const unsigned char *detail_usage,
                                     DYN_SHELL_CMD_FUNC_T func);
                                     
extern int dyn_shell_add_sub_shell(DYN_SHELL_ITEM_T * p_shell, 
                                     const unsigned char *cmdstr,
                                     const unsigned char *short_usage,
                                     DYN_SHELL_CREAT_SUB_SHELL_FUNC_T func);
                                     
int dyn_shell_init(DYN_SHELL_ITEM_T ** pp_shell, const unsigned char *banner);

extern int dyn_shell_load(DYN_SHELL_ITEM_T * p_shell, DYN_SHELL_ITEM_T * p_basic_cmds);

#endif //__BL_SHELL_H__
