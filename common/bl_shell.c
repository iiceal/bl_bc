#include <common.h>

#if 0
#define	strcasecmp  strcmp

////////////////////////////////////////////////////////////////////////

#define GETCH(a)   do{*a = (unsigned char)getc();}while(0)

//#define DBG_PRINTF  printf
#define DBG_PRINTF(...)  while(0)

/************************************************************
 *      Shell Operations 
 ***********************************************************/
#define MAX_ARGS		      20
#define MAX_COMMANDS		  256

static int hist_max = 0;
static int hist_add_idx = 0;
static int hist_cur = -1;
unsigned hist_num = 0;

static char*	hist_list[HIST_MAX];
static char	hist_lines[HIST_MAX][MAX_COMMANDS];

static void putnstr(const char *msg, const unsigned int len)
{
	int i;
	for(i = 0; i < len; i++)
        putc((unsigned char)msg[i]);

}

static void hist_init(void)
{
	int	i;

	hist_max = 0;
	hist_add_idx = 0;
	hist_cur = -1;
	hist_num = 0;

	for (i = 0; i < HIST_MAX; i++) {
		hist_list[i] = hist_lines[i];
		hist_list[i][0] = '\0';
	}
}

static void cread_add_to_hist(char*   line)
{
	strcpy(hist_list[hist_add_idx], line);

	if (++hist_add_idx >= HIST_MAX)
		hist_add_idx = 0;

	if (hist_add_idx > hist_max)
		hist_max = hist_add_idx;

	hist_num++;
}

static char* hist_prev(void)
{
	char*   ret;
	int	    old_cur;

	if (hist_cur < 0)
		return (NULL);

	old_cur = hist_cur;
	if (--hist_cur < 0)
		hist_cur = hist_max;

	if (hist_cur == hist_add_idx) {
		hist_cur = old_cur;
		ret = NULL;
	}
	else
		ret = hist_list[hist_cur];

	return (ret);
}

static char* hist_next(void)
{
	char*   ret;

	if (hist_cur < 0)
		return (NULL);

	if (hist_cur == hist_add_idx)
		return (NULL);

	if (++hist_cur > hist_max)
		hist_cur = 0;

	if (hist_cur == hist_add_idx) {
		ret = "";
	}
	else
		ret = hist_list[hist_cur];

	return (ret);
}

#define BEGINNING_OF_LINE() {			\
	while (num) {				\
		putc(CTL_BACKSPACE);		\
		num--;					\
	}						\
}

#define	ERASE_TO_EOL() {			\
	if (num < eol_num) {				\
		int	tmp;				\
		for (tmp = num; tmp < eol_num; tmp++)	\
		putc(' ');		\
		while (tmp-- > num)			\
		putc(CTL_BACKSPACE);	\
		eol_num = num;				\
	}						\
}


#define REFRESH_TO_EOL() { 			\
	if (num < eol_num) {				\
		wlen = eol_num - num;			\
		putnstr(buf + num, wlen);	\
		num = eol_num;				\
	}						\
}

static void cread_add_char(
		char	    ichar,
		int		    insert,
		unsigned int*	    num,
		unsigned int*	    eol_num,
		char*	    buf,
		unsigned int	    len)
{
	unsigned int	wlen;

	/* room ??? */
	if (insert || *num == *eol_num) {
		if (*eol_num > len - 1) {
            putc((unsigned char)'\a');
			return;
		}
		(*eol_num)++;
	}

	if (insert) {
		wlen = *eol_num - *num;
		if (wlen > 1) {
			memmove(&buf[*num+1], &buf[*num], wlen-1);
		}

		buf[*num] = ichar;
		putnstr(buf + *num, wlen);
		(*num)++;
		while (--wlen) {
			putc(CTL_BACKSPACE);
		}
	}
	else {
		/* echo the character */
		wlen = 1;
		buf[*num] = ichar;
		putnstr(buf + *num, wlen);
		(*num)++;
	}
}

static void cread_add_str(
		char*	    str,
		int		    strsize,
		int		    insert,
		unsigned int*	    num,
		unsigned int*	    eol_num,
		char*	    buf,
		unsigned int	    len)
{
	while (strsize--) {
		cread_add_char(*str, insert,
				num, eol_num, buf, len);
		str++;
	}
}

/*
 ************************************************************************
 *
 * 
 ************************************************************************
 */
static int cread_line_alt(
		char*	    buf,
		unsigned int*	    len)
{
	unsigned int	num = 0;
	unsigned int	eol_num = 0;
//    unsigned int	rlen;
	unsigned int	wlen;
	char	ichar;
	int		insert = 1;
	int		esc_len = 0;
	int		rc = 0;
#define ESC_BUFF_SIZE	  (8)
	char	esc_save[ESC_BUFF_SIZE];

	while (1) {
            GETCH(&ichar);

		if ((ichar == '\n') || (ichar == '\r')) {
			printf("\n\r");
			break;
		}

		/*
		 * handle standard linux xterm esc sequences for arrow keys, etc.
		 */
		if (esc_len != 0) {
			esc_save[esc_len++] = ichar;  //Accept the char;
			switch(esc_len)
			{
				case 2:    //two char: \x1b, [ 
					if(ichar != '[')
					{// second char is not '[', unknow escape seq. ignore the first '\x1b'
						esc_len = 0;
						break;
					}
					else
						continue;
				case 3:  //three char: \x1b, [, ch0
					switch (ichar) {		
						case 'D':	/* <-- key */
							ichar = CTL_CH('b');
							esc_len = 0;
							break;
		
						case 'C':	/* --> key */
							ichar = CTL_CH('f');
							esc_len = 0;
							break;	/* pass off to ^F handler */
		
						case 'H':	/* Home key */
							ichar = CTL_CH('a');
							esc_len = 0;
							break;	/* pass off to ^A handler */
		
						case 'A':	/* up arrow */
							ichar = CTL_CH('p');
							esc_len = 0;
							break;	/* pass off to ^P handler */
		
						case 'B':	/* down arrow */
							ichar = CTL_CH('n');
							esc_len = 0;
							break;	/* pass off to ^N handler */
						default:
							if(ichar >=64 && ichar <= 126)  /* between '@' and '~': the final char */
							{//unknow escape seq. ignore it, and drop the final char.
								esc_len = 0;
							}
						}
						if(esc_len)
						 		continue;  //continu the loop
						else
						 	break;  //break the switch
				default:  //more chars: \x1b, [, ch0, ch1, ....
				  if( (ichar <64 || ichar > 126) &&  esc_len < ESC_BUFF_SIZE -1 )
				  {//no final char and no overflow: receiving more char.
				      continue;
				  }
				  				  
				  if(0 == memcmp(&esc_save[2],"2~",esc_len - 2))
				  {//Home
							ichar = CTL_CH('a');
							esc_len = 0;
							break;	/* pass off to ^A handler */				  	
				  }
				  if(0 == memcmp(&esc_save[2],"5~",esc_len - 2))
				  {//End
							ichar = CTL_CH('e');
							esc_len = 0;
							break;	
				  }
#if 0				  
				  if(0 == memcmp(&esc_save[2],"1~",esc_len - 2))
				  {//Insert
							//ichar = CTL_CH('a');
							esc_len = 0;
							continue;	
				  }
				  if(0 == memcmp(&esc_save[2],"3~",esc_len - 2))
				  {//PageUp
							//ichar = CTL_CH('a');
							esc_len = 0;
							continue;	
				  }
				  if(0 == memcmp(&esc_save[2],"6~",esc_len - 2))
				  {//PageDown
							//ichar = CTL_CH('a');
							esc_len = 0;
							continue;	
				  }				  
#endif				  
				  
				  if(ichar >=64 && ichar <= 126)
				  {//unknow escap seq. ignore it.
							esc_len = 0;
							continue;					  	
				  } 
					cread_add_str(esc_save, esc_len, insert,
							&num, &eol_num, buf, *len);
					esc_len = 0;				  
				}
			}

		switch (ichar) {
			case 0x1b:
				if (esc_len == 0) {
					esc_save[esc_len] = ichar;
					esc_len = 1;
				}

				else {
					printf("impossible condition #876\n");
					esc_len = 0;
				}
				break;

			case CTL_CH('a'):
				BEGINNING_OF_LINE();
				break;

			case CTL_CH('f'):
				if (num < eol_num) {
					putc(buf[num]);
					num++;
				}
				break;

			case CTL_CH('b'):
				if (num) {
					putc(CTL_BACKSPACE);
					num--;
				}
				break;

			case CTL_CH('u'):
				for (wlen = 0; wlen < num; wlen++)
					putc (CTL_BACKSPACE);
				for (wlen = 0; wlen < eol_num; wlen++)
					putc (' ');
				for (wlen = 0; wlen < eol_num; wlen++)
					putc (CTL_BACKSPACE);
				num = eol_num = 0;
				break;

			case CTL_CH('d'):
				if (num < eol_num) {
					wlen = eol_num - num - 1;
					if (wlen) {
						memmove(&buf[num], &buf[num+1], wlen);
						putnstr(buf + num, wlen);
					}

					putc(' ');
					do {
						putc(CTL_BACKSPACE);
					}
					while (wlen--);
					eol_num--;
				}
				break;

			case CTL_CH('k'):
				ERASE_TO_EOL();
				break;

			case CTL_CH('e'):
				REFRESH_TO_EOL();
				break;

			case CTL_CH('o'):
				insert = !insert;
				break;

			case CTL_CH('x'):
				BEGINNING_OF_LINE();
				ERASE_TO_EOL();
				break;

			case DEL:
			case DEL7:
			case 8:
				if (num) {
					wlen = eol_num - num;
					num--;
					memmove(&buf[num], &buf[num+1], wlen);
					putc(CTL_BACKSPACE);
					putnstr(buf + num, wlen);
					putc(' ');
					do {
						putc(CTL_BACKSPACE);
					}
					while (wlen--);
					eol_num--;
				}
				break;

			case CTL_CH('p'):
			case CTL_CH('n'):
				{
					char*   hline;

					esc_len = 0;

					if (ichar == CTL_CH('p'))
						hline = hist_prev();
					else
						hline = hist_next();

					if (!hline) {
						putc('\a');
						continue;
					}

					/* nuke the current line */
					/* first, go home */
					BEGINNING_OF_LINE();

					/* erase to end of line */
					ERASE_TO_EOL();

					/* copy new line into place and display */
					strcpy(buf, hline);
					eol_num = strlen(buf);
					REFRESH_TO_EOL();
					continue;
				}


			default:
				cread_add_char(ichar,insert,
						&num, &eol_num, buf, *len);
				break;
		}

	}
	*len = eol_num;
	buf[eol_num] = '\0';		/* lose the newline */

	if (buf[0] && buf[0] != CREAD_HIST_CHAR)
		cread_add_to_hist(buf);
	hist_cur = hist_add_idx;

	return (rc);
}


 void GetCmdExAlt(
		char*   buf,
		unsigned int  len)
{
	static int	initted = 0;

	if (!initted) {
		hist_init();
		initted = 1;
	}

	cread_line_alt(buf, &len);
}
 void hide_cursor(void)
{
	printf("\x1b\x5b?25l");
}

 void show_cursor(void)
{
	printf("\x1b\x5b?25h");	
}

 int get_args(char *s, char **argv){
	int args = 0;

	if (!s || *s=='\0') return 0;
	while (args < MAX_ARGS){
		while (*s == ' ' || *s == '\t') s++;

		if (*s=='\0'){
			argv[args] = 0;
			return args;
		}

		argv[args++] = s;


		while (*s && *s != ' ' && *s != '\t') s++;
		if (*s=='\0'){
			argv[args] = 0;
			return args;
		}
		*s++ = '\0';
	}
	return args;
}
void * dyn_shell_malloc(int len)
{
	static int malloc_len = 0;
	malloc_len += len;
	DBG_PRINTF("malloc total_len = %d\n", malloc_len);
	return malloc(len);
}

static DYN_SHELL_ITEM_T* dyn_shell_alloc_item(void)
{
    DYN_SHELL_ITEM_T*  p_item;
    DBG_PRINTF("[dyn_shell_alloc_item] allocat %d bytes...",sizeof(DYN_SHELL_ITEM_T));
    p_item = (DYN_SHELL_ITEM_T*)dyn_shell_malloc(sizeof(DYN_SHELL_ITEM_T));
    DBG_PRINTF("with result %08x\r\n",p_item);
    if(NULL == p_item)
    	return 0;
    memset(p_item, 0, sizeof(DYN_SHELL_ITEM_T));
    DBG_PRINTF("[dyn_shell_alloc_item] return with ret %08x\r\n",p_item);
    return p_item;
}

#define STRING_BUFFER_BLOCK_SIZE (16)
static unsigned char* str_cpy(const unsigned char *src)
{
	unsigned int len;
	unsigned char* ret;
	if(NULL == src)
		return NULL;
	len = strlen((char*)src) + 1;
	len = (len + STRING_BUFFER_BLOCK_SIZE -1) & ( - STRING_BUFFER_BLOCK_SIZE);
	
	ret = (unsigned char*) dyn_shell_malloc(len);
	
	if(NULL == ret)
		return NULL;
  strcpy((char*)ret, (char*)src);
  return ret;		
}

 int dyn_shell_append_command(DYN_SHELL_ITEM_T ** pp_pos, 
                                     const unsigned char *cmdstr,
                                     unsigned char rep,
                                     const unsigned char *short_usage,
                                     const unsigned char *detail_usage,
                                     DYN_SHELL_CMD_FUNC_T func)
{
    DYN_SHELL_ITEM_T*  p_shell_item;
    
    DBG_PRINTF("[dyn_shell_append_command] begin, *pp_pos is %08x\r\n",*pp_pos);    
    
    if(NULL == (p_shell_item = dyn_shell_alloc_item()))
    {
    	  DBG_PRINTF("[dyn_shell_append_command] no mem space\r\n");
        return 0;
    }
    
    p_shell_item->cmdstr = (const char *)str_cpy(cmdstr);
    p_shell_item->short_usage = (const char *)str_cpy(short_usage);
    p_shell_item->cmd.detail_usage = (const char *)str_cpy(detail_usage);
    p_shell_item->cmd.rep = rep;
    p_shell_item->cmd.func = func;    
    
    while(*pp_pos && !(*pp_pos)->is_sub_shell)
    {
    	pp_pos = &((*pp_pos)->p_next);
    }
    p_shell_item->p_next = *pp_pos;    
    *pp_pos = p_shell_item;    
    
    DBG_PRINTF("[dyn_shell_append_command] command@0x%08x added\r\n", p_shell_item);
    
    return 0;
}

 int dyn_shell_add_command(DYN_SHELL_ITEM_T * p_shell, 
                                     const unsigned char *cmdstr,
                                     unsigned char rep,
                                     const unsigned char *short_usage,
                                     const unsigned char *detail_usage,
                                     DYN_SHELL_CMD_FUNC_T func)
{
    if(NULL == p_shell || !p_shell->is_sub_shell)      
    	return 0;	
    	
	  dyn_shell_append_command(&p_shell->sub_shell.p_cmd_list, cmdstr, rep, short_usage, detail_usage, func);
	      
    return 0;
}

 int dyn_shell_init(DYN_SHELL_ITEM_T ** pp_pos, 
                                     const unsigned char *banner)
{
  	
    DYN_SHELL_ITEM_T*  p_shell_item;
    
    DBG_PRINTF("[dyn_shell_init] begin\r\n");    
    
    if(NULL == (p_shell_item =dyn_shell_alloc_item()))
        return 0;    
    
    p_shell_item->is_sub_shell = 1;
    p_shell_item->cmdstr = (const char *)str_cpy(banner);
    
    DBG_PRINTF("[dyn_shell_init] shell initialized @0x%08x added\r\n", p_shell_item);
    
    *pp_pos = p_shell_item;
    
    return 0;
}


 int dyn_shell_append_sub_shell(DYN_SHELL_ITEM_T ** pp_pos, 
                                     const unsigned char *cmdstr,
                                     const unsigned char *short_usage,
                                     DYN_SHELL_CREAT_SUB_SHELL_FUNC_T func)
{
    DYN_SHELL_ITEM_T*  p_shell_item;
    
    DBG_PRINTF("[dyn_shell_append_sub_shell] begin\r\n");    
    
    if(NULL == (p_shell_item =dyn_shell_alloc_item()))
        return 0;    
    
    p_shell_item->is_sub_shell = 1;
    p_shell_item->cmdstr = (const char *)str_cpy(cmdstr);
    p_shell_item->short_usage = (const char *)str_cpy(short_usage);
    
    p_shell_item->sub_shell.p_parent = *pp_pos;
    p_shell_item->sub_shell.create_func = func;
    
    while(*pp_pos)
    {
    	pp_pos = &((*pp_pos)->p_next);
    }
    p_shell_item->p_next = *pp_pos;    
    *pp_pos = p_shell_item;    
    
    DBG_PRINTF("[dyn_shell_append_sub_shell] sub_shell@0x%08x added\r\n", p_shell_item);
    
    return 0;	
}

 int dyn_shell_add_sub_shell(DYN_SHELL_ITEM_T * p_shell, 
                                     const unsigned char *cmdstr,
                                     const unsigned char *short_usage,
                                     DYN_SHELL_CREAT_SUB_SHELL_FUNC_T func)
{
    if(NULL == p_shell || !p_shell->is_sub_shell)      
    	return 0;
    return dyn_shell_append_sub_shell(&p_shell->sub_shell.p_cmd_list,cmdstr,short_usage,func);
}

static DYN_SHELL_ITEM_T * dyn_shell_get_sub_items(DYN_SHELL_ITEM_T * p_cur_shell)
{
    if(p_cur_shell->is_sub_shell)
    {
       	return p_cur_shell->sub_shell.p_cmd_list;
           
    } else        
      	return p_cur_shell;	
}

static int dyn_shell_print_help_string(unsigned int is_sub_shell, const char* cmd_name, const char * help_string)
{
	int i;
	int c;
	int t;
	const int align_block_size = 16;
	
	if(is_sub_shell)
	  c = printf("[%s]: ",cmd_name);
	else
		c = printf("%s: ",cmd_name);
	
	t = (c + align_block_size -1) &( -align_block_size);
	
	for(i = c; i< t; i++)
		printf(" ");
	printf("%s\r\n",help_string);
   return 0;
	
}

static int dyn_shell_do_print_help(DYN_SHELL_ITEM_T * p_cur_shell,DYN_SHELL_ITEM_T * p_basic_cmds, unsigned int argc, const char **argv)
{
//    const shell_cmd_t	*cptr;
    argc = argc;
    argv = argv;
    
    DYN_SHELL_ITEM_T *p_item;
    
    
    if(argc>0)
    {         
      p_item = dyn_shell_get_sub_items(p_cur_shell);

	    while(p_item)
  	  {
  	  	if (!strcasecmp(argv[1], (const char*)p_item->cmdstr))
  	  	{
  	  		  if(p_item->cmd.detail_usage)
              printf("%s:\t\t%s\r\n",p_item->cmdstr,p_item->cmd.detail_usage);
           else
              printf("No help information for %s\r\n", p_item->cmdstr);
  	  		  return 0;
  	  	}
    	    	p_item = p_item->p_next;
    	}
    	p_item = dyn_shell_get_sub_items(p_basic_cmds);    
    	while(p_item)
    	{
  	  	if (!strcasecmp(argv[1], (const char*)p_item->cmdstr))
  	  	{
  	  		  printf("%s:\t\t%s\r\n",p_item->cmdstr,p_item->cmd.detail_usage);
  	  		  return 0;
  	  	}    		
      	  	p_item = p_item->p_next;
    	}            	
    }

    printf("The following commands are supported :\r\n");    
       
    p_item = dyn_shell_get_sub_items(p_cur_shell);
    while(p_item)
    {
        if(!p_item->is_sub_shell)
        {
        	dyn_shell_print_help_string(0, p_item->cmdstr,p_item->cmd.detail_usage);
        }
        	p_item = p_item->p_next;
    }
    p_item = dyn_shell_get_sub_items(p_basic_cmds);    
    while(p_item)
    {
        if(!p_item->is_sub_shell)
        {
        	dyn_shell_print_help_string(0, p_item->cmdstr,p_item->cmd.detail_usage);
        }    	
        	p_item = p_item->p_next;
    }        
    
    
     p_item = dyn_shell_get_sub_items(p_cur_shell);
    while(p_item)
    {
        if(p_item->is_sub_shell)
        {
        	dyn_shell_print_help_string(1, p_item->cmdstr,p_item->cmd.detail_usage);
        }
        	p_item = p_item->p_next;
    }
    p_item = dyn_shell_get_sub_items(p_basic_cmds);    
    while(p_item)
    {
        if(p_item->is_sub_shell)
        {
        	dyn_shell_print_help_string(1, p_item->cmdstr,p_item->cmd.detail_usage);
        }    	
        	p_item = p_item->p_next;
    }
    
    printf("exit:           Exit\r\n");
    return 1;
}

static void dyn_shell_print_banner(DYN_SHELL_ITEM_T * p_cur_shell)
{
	  DBG_PRINTF("[dyn_shell_print_banner] banner of %08x\r\n",p_cur_shell);
    if(p_cur_shell->is_sub_shell)
    {
    	if(NULL!= p_cur_shell->sub_shell.p_parent)
    	{
        dyn_shell_print_banner(p_cur_shell->sub_shell.p_parent);
        printf("/");    		
    	}
    	printf((const char*)p_cur_shell->cmdstr);    	
    }
    else
    printf("BP2016");
        
}

 int dyn_shell_load(DYN_SHELL_ITEM_T * p_cur_shell,DYN_SHELL_ITEM_T * p_basic_cmds)
{
    char cmd[MAX_COMMANDS];
    char	*argv[MAX_ARGS + 1] = {NULL};
    int argc;
    const shell_cmd_t	*cptr = NULL;
    
    DYN_SHELL_ITEM_T *p_item;
    //LOG("enter");
    
    DBG_PRINTF("[dyn_shell_load] begin with p_cur_shell:%08x\r\n",p_cur_shell);
    show_cursor();
    while(1) 
    {
    	  dyn_shell_print_banner(p_cur_shell);
        printf("> ");
        GetCmdExAlt(cmd, sizeof(cmd));
        if (!cmd[0]) continue;

        argc = get_args(cmd, argv);
        
        if (!strcasecmp(argv[0], "exit"))
        {
        	if(p_cur_shell->is_sub_shell && NULL !=p_cur_shell->sub_shell.p_parent)
        	{
        		 p_cur_shell = p_cur_shell->sub_shell.p_parent;
        		 continue;
        		 
        	} else
        	{
        		return 0;
        	}
        }                        
                

       	p_item = dyn_shell_get_sub_items(p_cur_shell);
        
        DBG_PRINTF("[dyn_shell_load] look through the list of current shell @0x%08x\r\n",p_item);
        
        while(p_item)
        {
        	DBG_PRINTF("[dyn_shell_load]   p_item:0x%08x, %s\r\n", p_item,p_item->cmdstr);
        	if (!strcasecmp(argv[0], (const char*)p_item->cmdstr))
        	{
        		if(p_item->is_sub_shell)
        		{
        			if(NULL == p_item->sub_shell.p_cmd_list)
        			{
        				 if(NULL != p_item->sub_shell.create_func)
        				    p_item->sub_shell.create_func(p_item);
        			}
        			if(NULL != p_item->sub_shell.p_cmd_list)
        			{
        			   //dyn_shell_load(p_item->sub_shell.p_cmd_list, p_basic_cmds);
        			   p_item->sub_shell.p_parent = p_cur_shell;
        			   p_cur_shell = p_item;
        			   break;
        			}
        		} else
        		{
        			hide_cursor();
        			    (p_item->cmd.func)((shell_cmd_t *)cptr, (unsigned int)argc, (const char **)argv);
        			show_cursor();
        			break;
        		}
        	}
        	p_item = p_item->p_next;
        }
        if ( NULL == p_item)
        {
        	p_item = dyn_shell_get_sub_items(p_basic_cmds);
        	
        	DBG_PRINTF("[dyn_shell_load] look through the list of basic command @0x%08x\r\n",p_item);
	        while(p_item)
	        {
	        	DBG_PRINTF("[dyn_shell_load]   p_item:0x%08x\r\n", p_item);
	        	if (!strcasecmp(argv[0], (const char*)p_item->cmdstr))
	        	{
	        		if(p_item->is_sub_shell)
	        		{
	        		} else
	        		{
	        			hide_cursor();
	        			    (p_item->cmd.func)((shell_cmd_t *)cptr, (unsigned int)argc, (const char **)argv);
	        			show_cursor();
	        			break;
	        		}
	        	}
	        	p_item = p_item->p_next;
	        }        	
        }
        if ( NULL == p_item )
        {
            if (!strcasecmp(argv[0], "help") || !strcasecmp(argv[0], "?"))
            {
                dyn_shell_do_print_help(p_cur_shell,p_basic_cmds, argc, (const char **)argv);
            } 
            else 
            {
                printf("\tUnknown command : %s\r\n", argv[0]);
            }
        }        
    }
}
#endif
