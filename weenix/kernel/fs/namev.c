/******************************************************************************/
/* Important Spring 2023 CSCI 402 usage information:                          */
/*                                                                            */
/* This fils is part of CSCI 402 kernel programming assignments at USC.       */
/*         53616c7465645f5fd1e93dbf35cbffa3aef28f8c01d8cf2ffc51ef62b26a       */
/*         f9bda5a68e5ed8c972b17bab0f42e24b19daa7bd40e8305b1f7bd6c7208c1       */
/*         0e36230e913039b3046dd5fd0ba706a624d33dbaa4d6aab02c82fe09f561       */
/*         01b0fd977b0051f0b0ce0c69f7db857b1b5e007be2db6d42894bf93de848       */
/*         806d9152bd5715e9                                                   */
/* Please understand that you are NOT permitted to distribute or publically   */
/*         display a copy of this file (or ANY PART of it) for any reason.    */
/* If anyone (including your prospective employer) asks you to post the code, */
/*         you must inform them that you do NOT have permissions to do so.    */
/* You are also NOT permitted to remove or alter this comment block.          */
/* If this comment block is removed or altered in a submitted file, 20 points */
/*         will be deducted.                                                  */
/******************************************************************************/

#include "kernel.h"
#include "globals.h"
#include "types.h"
#include "errno.h"

#include "util/string.h"
#include "util/printf.h"
#include "util/debug.h"

#include "fs/dirent.h"
#include "fs/fcntl.h"
#include "fs/stat.h"
#include "fs/vfs.h"
#include "fs/vnode.h"

/* This takes a base 'dir', a 'name', its 'len', and a result vnode.
 * Most of the work should be done by the vnode's implementation
 * specific lookup() function.
 *
 * If dir has no lookup(), return -ENOTDIR.
 *
 * Note: returns with the vnode refcount on *result incremented.
 */
int lookup(vnode_t *dir, const char *name, size_t len, vnode_t **result)
{
        // NOT_YET_IMPLEMENTED("VFS: lookup");
        KASSERT(NULL != dir);
        dbg(DBG_PRINT, "(GRADING2A 2.a)\n");
        KASSERT(NULL != name);
        dbg(DBG_PRINT, "(GRADING2A 2.a)\n");
        KASSERT(NULL != result);
        dbg(DBG_PRINT, "(GRADING2A 2.a)\n");
        dbg(DBG_PRINT, "(GRADING2B)\n");
        int code = dir->vn_ops->lookup(dir, name, len, result);
        dbg(DBG_PRINT, "(GRADING2B)\n");
        return code;
}
/* When successful this function returns data in the following "out"-arguments:
 *  o res_vnode: the vnode of the parent directory of "name"
 *  o name: the `basename' (the element of the pathname)
 *  o namelen: the length of the basename
 *
 * For example: dir_namev("/s5fs/bin/ls", &namelen, &name, NULL,
 * &res_vnode) would put 2 in namelen, "ls" in name, and a pointer to the
 * vnode corresponding to "/s5fs/bin" in res_vnode.
 *
 * The "base" argument defines where we start resolving the path from:
 * A base value of NULL means to use the process's current working directory,
 * curproc->p_cwd.  If pathname[0] == '/', ignore base and start with
 * vfs_root_vn.  dir_namev() should call lookup() to take care of resolving each
 * piece of the pathname.
 *
 * Note: A successful call to this causes vnode refcount on *res_vnode to
 * be incremented.
 */
int dir_namev(const char *pathname, size_t *namelen, const char **name,
              vnode_t *base, vnode_t **res_vnode)
{
        // NOT_YET_IMPLEMENTED("VFS: dir_namev");
        KASSERT(NULL != pathname);
        dbg(DBG_PRINT, "(GRADING2A 2.b)\n");
        KASSERT(NULL != namelen);
        dbg(DBG_PRINT, "(GRADING2A 2.b)\n");
        KASSERT(NULL != name);
        dbg(DBG_PRINT, "(GRADING2A 2.b)\n");
        KASSERT(NULL != res_vnode);
        dbg(DBG_PRINT, "(GRADING2A 2.b)\n");
        dbg(DBG_PRINT, "(GRADING2B)\n");

        vnode_t *dir_vnode;
        if (*pathname == '/')
        {
                dbg(DBG_PRINT, "(GRADING2B)\n");
                dir_vnode = vfs_root_vn;
        }
        else if (base == NULL)
        {
                dbg(DBG_PRINT, "(GRADING2B)\n");
                dir_vnode = curproc->p_cwd;
        }
        KASSERT(NULL != dir_vnode);
        dbg(DBG_PRINT, "(GRADING2A 2.b)\n");

        int is_last_slash = 1;
        int last_index = strlen(pathname) - 1;
        int name_len = 0;

        for (int i = strlen(pathname) - 1; i >= 0; i--)
        {

                if (is_last_slash == 1 && pathname[i] == '/')
                {
                        dbg(DBG_PRINT, "(GRADING2B)\n");
                        last_index -= 1;
                        continue;
                }
                else if (pathname[i] == '/')
                {
                        dbg(DBG_PRINT, "(GRADING2B)\n");
                        break;
                }
                dbg(DBG_PRINT, "(GRADING2B)\n");
                is_last_slash = 0;
                last_index -= 1;
                name_len += 1;
                if (name_len > NAME_LEN)
                {
                        dbg(DBG_PRINT, "(GRADING2B)\n");
                        return -ENAMETOOLONG;
                }
                dbg(DBG_PRINT, "(GRADING2B)\n");
        }
        *namelen = name_len;
        if (*namelen == 0)
        {
                dbg(DBG_PRINT, "(GRADING2B)\n");
                *name = NULL;
        }
        else
        {
                if (last_index < 0)
                {
                        dbg(DBG_PRINT, "(GRADING2B)\n");
                        *name = &pathname[0];
                }
                else
                {
                        dbg(DBG_PRINT, "(GRADING2B)\n");
                        *name = &pathname[last_index] + 1;
                }
        }

        int name_start = 0;
        int name_end = 0;
        vnode_t *cur_node = NULL;
        vnode_t *pre_node = NULL;
        for (int f = name_end; f < last_index; f++)
        {
                dbg(DBG_PRINT, "(GRADING2B)\n");
                if (pathname[name_end] == '/')
                {
                        name_end += 1;
                        continue;
                }
                else
                {
                        dbg(DBG_PRINT, "(GRADING2B)\n");
                        break;
                }
        }

        while (last_index > name_end)
        {
                name_start = name_end;
                int each_length = 0;
                for (int f = name_end; f < last_index; f++)
                {
                        dbg(DBG_PRINT, "(GRADING2B)\n");
                        if (pathname[name_end] == '/')
                        {
                                dbg(DBG_PRINT, "(GRADING2B)\n");
                                break;
                        }
                        else
                        {
                                dbg(DBG_PRINT, "(GRADING2B)\n");
                                name_end += 1;
                                each_length += 1;
                                continue;
                        }
                }

                int is_valid = lookup(dir_vnode, &pathname[name_start], name_end - name_start, &cur_node);
                if (pre_node != NULL)
                {
                        dbg(DBG_PRINT, "(GRADING2B)\n");
                        vput(pre_node);
                }
                if (is_valid < 0)
                {
                        dbg(DBG_PRINT, "(GRADING2B)\n");
                        return is_valid;
                }
                pre_node = cur_node;
                dir_vnode = cur_node;
                for (int f = name_end; f < last_index; f++)
                {
                        dbg(DBG_PRINT, "(GRADING2B)\n");
                        if (pathname[name_end] == '/')
                        {
                                dbg(DBG_PRINT, "(GRADING2B)\n");
                                name_end += 1;
                                each_length += 1;
                                continue;
                        }
                        else
                        {
                                dbg(DBG_PRINT, "(GRADING2B)\n");
                                break;
                        }
                }
        }
        if (cur_node == NULL)
        {
                dbg(DBG_PRINT, "(GRADING2B)\n");
                *res_vnode = dir_vnode;
                vref(*res_vnode);
        }
        else
        {
                if (dir_vnode->vn_ops->lookup == NULL)
                {
                        dbg(DBG_PRINT, "(GRADING2B)\n");
                        vput(cur_node);
                        return -ENOTDIR;
                }
                dbg(DBG_PRINT, "(GRADING2B)\n");
                *res_vnode = cur_node;
        }

        return 0;
}
/* This returns in res_vnode the vnode requested by the other parameters.
 * It makes use of dir_namev and lookup to find the specified vnode (if it
 * exists).  flag is right out of the parameters to open(2); see
 * <weenix/fcntl.h>.  If the O_CREAT flag is specified and the file does
 * not exist, call create() in the parent directory vnode. However, if the
 * parent directory itself does not exist, this function should fail - in all
 * cases, no files or directories other than the one at the very end of the path
 * should be created.
 *
 * Note: Increments vnode refcount on *res_vnode.
 */
int open_namev(const char *pathname, int flag, vnode_t **res_vnode, vnode_t *base)
{
        // NOT_YET_IMPLEMENTED("VFS: open_namev");
        // call dir_namev, check if pathname[0] == '/', it's absolute path. Otherwise, it's a relative path
        // relative to base object in dir_namev
        vnode_t *dir_vnode;
        const char *name = NULL;
        size_t namelen = 0;

        int dir_namev_return = dir_namev(pathname, &namelen, &name, base, &dir_vnode);

        if (dir_namev_return != 0)
        {
                dbg(DBG_PRINT, "(GRADING2B)\n");

                return dir_namev_return;
        }
        if (namelen == 0)
        {
                dbg(DBG_PRINT, "(GRADING2B)\n");

                //  set res_vnode to parent directory of res_vnode
                *res_vnode = dir_vnode;
                return 0;
        }
        if (dir_namev_return == 0)
        {
                if (!(name[0] == '/' && namelen == 1 && dir_vnode == vfs_root_vn))
                {

                        // check whether the file exist. If not exist, call create()
                        int lookup_return = lookup(dir_vnode, name, namelen, res_vnode);
                        if (lookup_return == -ENOENT)
                        {
                                if (flag & O_CREAT)
                                {

                                        KASSERT(NULL != dir_vnode->vn_ops->create);
                                        dbg(DBG_PRINT, "(GRADING2A 2.c)\n");
                                        dbg(DBG_PRINT, "(GRADING2B)\n");
                                        lookup_return = dir_vnode->vn_ops->create(dir_vnode, name, namelen, res_vnode);
                                }
                                dbg(DBG_PRINT, "(GRADING2B)\n");

                                vput(dir_vnode);
                                return lookup_return;
                        }
                        dbg(DBG_PRINT, "(GRADING2B)\n");
                }
        }
        // check if pathname requires a directory
        if (pathname[strlen(pathname) - 1] == '/' && !S_ISDIR((*res_vnode)->vn_mode))
        {
                dbg(DBG_PRINT, "(GRADING2B)\n");

                vput((*res_vnode));
                vput(dir_vnode);

                return -ENOTDIR;
        }
        dbg(DBG_PRINT, "(GRADING2B)\n");

        //  lookup() has set res_vnode VNode and increased res_vnode vn_refcount
        vput(dir_vnode);

        return 0;
}

#ifdef __GETCWD__
/* Finds the name of 'entry' in the directory 'dir'. The name is writen
 * to the given buffer. On success 0 is returned. If 'dir' does not
 * contain 'entry' then -ENOENT is returned. If the given buffer cannot
 * hold the result then it is filled with as many characters as possible
 * and a null terminator, -ERANGE is returned.
 *
 * Files can be uniquely identified within a file system by their
 * inode numbers. */
int lookup_name(vnode_t *dir, vnode_t *entry, char *buf, size_t size)
{
        NOT_YET_IMPLEMENTED("GETCWD: lookup_name");
        return -ENOENT;
}

/* Used to find the absolute path of the directory 'dir'. Since
 * directories cannot have more than one link there is always
 * a unique solution. The path is writen to the given buffer.
 * On success 0 is returned. On error this function returns a
 * negative error code. See the man page for getcwd(3) for
 * possible errors. Even if an error code is returned the buffer
 * will be filled with a valid string which has some partial
 * information about the wanted path. */
ssize_t
lookup_dirpath(vnode_t *dir, char *buf, size_t osize)
{
        NOT_YET_IMPLEMENTED("GETCWD: lookup_dirpath");

        return -ENOENT;
}
#endif /* __GETCWD__ */