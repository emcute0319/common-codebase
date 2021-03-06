/******************************************************************************
 *
 * (C) Copyright 2007
 *    Panda Xiong, yaxi1984@gmail.com
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 * History:
 *    2007.03.27    Panda Xiong       Create
 *
******************************************************************************/

#define _CLI_KERNEL_SOURCE_

#include "../inc/cli_api.h"


/* at first, there is no registerd command;
 * and, this is always point to the last command.
 */
static CLI_REG_CMD_T *p_cmd_end;


/******************************************************************************
 * FUNCTION NAME:
 *      None
 * DESCRIPTION:
 *      check this command has been registed or not.
 *      if registerd, return GT_TRUE; else, return GT_FALSE.
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
GT_BOOL CLI_REG_IsCmdRegisterd(IN const CLI_REG_CMD_T *cmd)
{
    CLI_REG_CMD_T *cmd_loop;

    if ((cmd == NULL) || (cmd->cmd_name == NULL))
    {
        return GT_FALSE;
    }

    for (cmd_loop=p_cmd_end; cmd_loop!=NULL; cmd_loop=cmd_loop->prev_cmd)
    {
		if (cmd_loop->cmd_name == NULL)
		{
			continue;
		}

        if (strcmp(cmd_loop->cmd_name, cmd->cmd_name) == 0)
        {
            /* this command has been registerd, return fail */
            return GT_TRUE;
        }
    }

    return GT_FALSE;
}


/******************************************************************************
 * FUNCTION NAME:
 *      None
 * DESCRIPTION:
 *      None
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
GT_BOOL CLI_REG_RegisterCmd(IN CLI_REG_CMD_T *cmd)
{
    /* if the command has no command name, register fail */
    if ((cmd == NULL) || (cmd->cmd_name == NULL))
    {
        return GT_FALSE;
    }

    if (p_cmd_end == NULL)
    {
        p_cmd_end           = cmd;
        p_cmd_end->prev_cmd = NULL;
        p_cmd_end->next_cmd = NULL;
    }
    else
    {
        /* first, we should check this command has been registerd or not. */
        if (CLI_REG_IsCmdRegisterd(cmd))
        {
            return GT_TRUE;
        }

        /* this is a new command, register it. */
        cmd->prev_cmd       = p_cmd_end;
        p_cmd_end->next_cmd = cmd;
        p_cmd_end           = p_cmd_end->next_cmd;
        p_cmd_end->next_cmd = NULL;
    }

    return GT_TRUE;
}


/******************************************************************************
 * FUNCTION NAME:
 *      None
 * DESCRIPTION:
 *      None
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
GT_BOOL CLI_REG_DeRegisterCmd(IN CLI_REG_CMD_T *cmd)
{
    CLI_REG_CMD_T *cmd_loop;

    /* if the command has no command name, register fail */
    if ((cmd == NULL) || (cmd->cmd_name == NULL))
    {
        return GT_FALSE;
    }

    /* if there is no registerd command, de-register command success. */
    if (p_cmd_end == NULL)
    {
        return GT_TRUE;
    }

    for (cmd_loop=p_cmd_end; cmd_loop!=NULL; cmd_loop=cmd_loop->prev_cmd)
    {
        /* this command has been registerd, de-register it */
        if (strcmp(cmd_loop->cmd_name, cmd->cmd_name) == 0)
        {
            if (cmd_loop->prev_cmd != NULL)     /* not the head command */
            {
                cmd_loop->prev_cmd->next_cmd = cmd_loop->next_cmd;

                if (cmd_loop->next_cmd != NULL)
                {
                    /* not the tail command */
                    cmd_loop->next_cmd->prev_cmd = cmd_loop->prev_cmd;
                }
                else
                {
                    /* the tail command */
                    p_cmd_end = cmd_loop->prev_cmd;
                }
            }
            else    /* head command */
            {
                cmd_loop->next_cmd->prev_cmd = NULL;
            }

            /* reset the de-registered command to initial status */
            cmd_loop->prev_cmd = NULL;
            cmd_loop->next_cmd = NULL;

            break;
        }
    }

    /*  if this command is no registerd, or is de-registerd, return success. */
    return GT_TRUE;
}


/******************************************************************************
 * FUNCTION NAME:
 *      None
 * DESCRIPTION:
 *      None
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
CLI_REG_CMD_T *CLI_REG_SearchCommand(IN const GT_UI8 *cmd)
{
    CLI_REG_CMD_T *cmd_loop;

	if (cmd == NULL)
	{
		return NULL;
	}

    for (cmd_loop=p_cmd_end; cmd_loop!=NULL; cmd_loop=cmd_loop->prev_cmd)
    {
        if (strcmp(cmd_loop->cmd_name, cmd) == 0)
        {
            /* find command */
            return cmd_loop;
        }
    }

    /* not find command, return NULL */
	return NULL;
}


/******************************************************************************
 * FUNCTION NAME:
 *      None
 * DESCRIPTION:
 *      None
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
CLI_REG_CMD_T *CLI_REG_GetNextCommand(IN const CLI_REG_CMD_T *cmd)
{
    CLI_REG_CMD_T *cmd_loop;

    if (cmd == NULL)
    {
        /* if input NULL, return the first registered command */
        cmd_loop = p_cmd_end;
        while (cmd_loop->prev_cmd != NULL)
        {
            cmd_loop = cmd_loop->prev_cmd;
        }

        return cmd_loop;
    }
    else
    {
        /* else, return the next command */
        return cmd->next_cmd;
    }
}


/******************************************************************************
 * FUNCTION NAME:
 *      None
 * DESCRIPTION:
 *      None
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
CLI_REG_CMD_T *CLI_REG_GetPrevCommand(IN const CLI_REG_CMD_T *cmd)
{
    if (cmd == NULL)
    {
        /* if input NULL, return the last registered command */
        return p_cmd_end;
    }
    else
    {
        /* else, return the prev. command */
        return cmd->prev_cmd;
    }
}


/******************************************************************************
 * FUNCTION NAME:
 *      None
 * DESCRIPTION:
 *      None
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
CLI_REG_CMD_T *CLI_REG_GetNextMatchCommand
(
    IN const CLI_REG_CMD_T *cmd,
    IN const GT_UI8        *match_str
)
{
    CLI_REG_CMD_T *cmd_loop;
    GT_UI32        match_cmd_len;

    if (match_str == NULL)
    {
        return NULL;
    }

    match_cmd_len = strlen(match_str);
    cmd_loop      = (CLI_REG_CMD_T *)cmd;

    if (cmd_loop == NULL)
    {
        /* if input NULL, move to the first registered command */
        cmd_loop = p_cmd_end;
        while (cmd_loop->prev_cmd != NULL)
        {
            cmd_loop = cmd_loop->prev_cmd;
        }
    }

    /* search for the next match command */
    while (cmd_loop->next_cmd != NULL)
    {
        cmd_loop = cmd_loop->next_cmd;

        if ((cmd_loop->cmd_name != NULL)
                && (memcmp(cmd_loop->cmd_name, match_str, match_cmd_len) == 0))
        {
            return cmd_loop;
        }
    }

    return NULL;
}


/******************************************************************************
 * FUNCTION NAME:
 *      None
 * DESCRIPTION:
 *      None
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
GT_UI32 CLI_REG_GetMatchCommandNum(IN const GT_UI8 *match_str)
{
    CLI_REG_CMD_T *cmd_loop;
    GT_UI32        count;

    if (match_str == NULL)
    {
        return 0;
    }

    cmd_loop = NULL;
    count    = 0;
    while ((cmd_loop = CLI_REG_GetNextMatchCommand(cmd_loop, match_str)) != NULL)
    {
        count++;
    }

    return count;
}


/******************************************************************************
 * FUNCTION NAME:
 *      None
 * DESCRIPTION:
 *      None
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
GT_BOOL CLI_REG_Init(void)
{
	p_cmd_end = NULL;
	return GT_TRUE;
}

