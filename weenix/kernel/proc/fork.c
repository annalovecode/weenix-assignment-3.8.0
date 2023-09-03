/******************************************************************************/
/* Important Spring 2023 CSCI 402 usage information:                          */
/*                                                                            */
/* This fils is part of CSCI 402 kernel programming assignments at USC.       */
/*         53616c7465645f5fd1e93dbf35cbffa3aef28f8c01d8cf2ffc51ef62b26a       */
/*         f9bda5a68e5ed8c972b17bab0f42e24b19daa7bd408305b1f7bd6c7208c1       */
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

#include "types.h"
#include "globals.h"
#include "errno.h"

#include "util/debug.h"
#include "util/string.h"

#include "proc/proc.h"
#include "proc/kthread.h"

#include "mm/mm.h"
#include "mm/mman.h"
#include "mm/page.h"
#include "mm/pframe.h"
#include "mm/mmobj.h"
#include "mm/pagetable.h"
#include "mm/tlb.h"

#include "fs/file.h"
#include "fs/vnode.h"

#include "vm/shadow.h"
#include "vm/vmmap.h"

#include "api/exec.h"

#include "main/interrupt.h"

/* Pushes the appropriate things onto the kernel stack of a newly forked thread
 * so that it can begin execution in userland_entry.
 * regs: registers the new thread should have on execution
 * kstack: location of the new thread's kernel stack
 * Returns the new stack pointer on success. */
static uint32_t
fork_setup_stack(const regs_t *regs, void *kstack)
{
        /* Pointer argument and dummy return address, and userland dummy return
         * address */
        uint32_t esp = ((uint32_t)kstack) + DEFAULT_STACK_SIZE - (sizeof(regs_t) + 12);
        *(void **)(esp + 4) = (void *)(esp + 8);         /* Set the argument to point to location of struct on stack */
        memcpy((void *)(esp + 8), regs, sizeof(regs_t)); /* Copy over struct */
        return esp;
}

/*
 * The implementation of fork(2). Once this works,
 * you're practically home free. This is what the
 * entirety of Weenix has been leading up to.
 * Go forth and conquer.
 */
int do_fork(struct regs *regs)
{
        // NOT_YET_IMPLEMENTED("VM: do_fork");
        KASSERT(regs != NULL);
        dbg(DBG_PRINT, "(GRADING3A 7.a)\n");
        KASSERT(curproc != NULL);
        dbg(DBG_PRINT, "(GRADING3A 7.a)\n");
        KASSERT(curproc->p_state == PROC_RUNNING);
        dbg(DBG_PRINT, "(GRADING3A 7.a)\n");
        proc_t *child_proc = NULL;
        child_proc = proc_create("child_proc");
        vmmap_t *child_vm = vmmap_clone(curproc->p_vmmap);
        dbg(DBG_PRINT, "(GRADING3A)\n");
        child_proc->p_vmmap = child_vm;
        child_proc->p_vmmap->vmm_proc = child_proc;
        child_vm->vmm_proc = child_proc;

        vmarea_t *curr = NULL;
        vmarea_t *iter = NULL;
        list_iterate_begin(&child_vm->vmm_list, curr, vmarea_t, vma_plink)
        {
                dbg(DBG_PRINT, "(GRADING3A)\n");
                iter = vmmap_lookup(curproc->p_vmmap, curr->vma_start);

                if (!((curr->vma_flags & MAP_TYPE) == MAP_PRIVATE))
                {
                        dbg(DBG_PRINT, "(GRADING3D 1)\n");

                        curr->vma_obj = iter->vma_obj;
                        curr->vma_obj->mmo_ops->ref(curr->vma_obj);
                }
                else
                {
                        dbg(DBG_PRINT, "(GRADING3A)\n");

                        mmobj_t *p_shadow = shadow_create();
                        p_shadow->mmo_shadowed = iter->vma_obj;
                        p_shadow->mmo_un.mmo_bottom_obj = mmobj_bottom_obj(iter->vma_obj);

                        mmobj_t *ch_shadow = shadow_create();
                        ch_shadow->mmo_shadowed = iter->vma_obj;
                        ch_shadow->mmo_un.mmo_bottom_obj = mmobj_bottom_obj(iter->vma_obj);

                        ch_shadow->mmo_shadowed->mmo_ops->ref(ch_shadow->mmo_shadowed);
                        ch_shadow->mmo_un.mmo_bottom_obj->mmo_ops->ref(ch_shadow->mmo_un.mmo_bottom_obj);
                        p_shadow->mmo_un.mmo_bottom_obj->mmo_ops->ref(p_shadow->mmo_un.mmo_bottom_obj);

                        curr->vma_obj = ch_shadow;
                        iter->vma_obj = p_shadow;
                }
                list_insert_tail(mmobj_bottom_vmas(iter->vma_obj), &curr->vma_olink);
                dbg(DBG_PRINT, "(GRADING3A)\n");
        }
        list_iterate_end();

        kthread_t *child_thr = kthread_clone(curthr);
        child_thr->kt_proc = child_proc;
        KASSERT(child_proc->p_state == PROC_RUNNING);
        dbg(DBG_PRINT, "(GRADING3A 7.a)\n");
        KASSERT(child_proc->p_pagedir != NULL);
        dbg(DBG_PRINT, "(GRADING3A 7.a)\n");
        KASSERT(child_thr->kt_kstack != NULL);
        dbg(DBG_PRINT, "(GRADING3A 7.a)\n");
        list_insert_tail(&child_proc->p_threads, &child_thr->kt_plink);

        child_thr->kt_ctx.c_pdptr = child_proc->p_pagedir;
        regs->r_eax = 0;
        child_thr->kt_ctx.c_eip = (uint32_t)userland_entry;
        child_thr->kt_ctx.c_esp = fork_setup_stack(regs, child_thr->kt_kstack);
        child_thr->kt_ctx.c_kstack = (uintptr_t)child_thr->kt_kstack;
        child_thr->kt_ctx.c_kstacksz = DEFAULT_STACK_SIZE;

        pt_unmap_range(curproc->p_pagedir, USER_MEM_LOW, USER_MEM_HIGH);
        tlb_flush_all();

        for (int i = 0; i < NFILES; i++)
        {
                dbg(DBG_PRINT, "(GRADING3A)\n");
                child_proc->p_files[i] = curproc->p_files[i];
                if (child_proc->p_files[i] != NULL)
                {
                        dbg(DBG_PRINT, "(GRADING3A)\n");
                        fref(child_proc->p_files[i]);
                }
        }

        if (NULL != child_proc->p_cwd)
        {
                dbg(DBG_PRINT, "(GRADING3A)\n");
                vput(child_proc->p_cwd);
        }
        child_proc->p_cwd = curproc->p_cwd;
        if (NULL != child_proc->p_cwd)
        {
                dbg(DBG_PRINT, "(GRADING3A)\n");
                vref(child_proc->p_cwd);
        }
        dbg(DBG_PRINT, "(GRADING3A)\n");
        regs->r_eax = child_proc->p_pid;

        child_thr->kt_proc = child_proc;
        list_insert_tail(&child_proc->p_threads, &child_thr->kt_plink);
        child_proc->p_brk = curproc->p_brk;
        child_proc->p_start_brk = curproc->p_start_brk;
        sched_make_runnable(child_thr);
        return child_proc->p_pid;
}