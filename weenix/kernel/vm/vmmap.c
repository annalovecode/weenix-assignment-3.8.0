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

#include "kernel.h"
#include "errno.h"
#include "globals.h"

#include "vm/vmmap.h"
#include "vm/shadow.h"
#include "vm/anon.h"

#include "proc/proc.h"

#include "util/debug.h"
#include "util/list.h"
#include "util/string.h"
#include "util/printf.h"

#include "fs/vnode.h"
#include "fs/file.h"
#include "fs/fcntl.h"
#include "fs/vfs_syscall.h"

#include "mm/slab.h"
#include "mm/page.h"
#include "mm/mm.h"
#include "mm/mman.h"
#include "mm/mmobj.h"
#include "mm/tlb.h"

static slab_allocator_t *vmmap_allocator;
static slab_allocator_t *vmarea_allocator;

void vmmap_init(void)
{
        vmmap_allocator = slab_allocator_create("vmmap", sizeof(vmmap_t));
        KASSERT(NULL != vmmap_allocator && "failed to create vmmap allocator!");
        vmarea_allocator = slab_allocator_create("vmarea", sizeof(vmarea_t));
        KASSERT(NULL != vmarea_allocator && "failed to create vmarea allocator!");
}

vmarea_t *
vmarea_alloc(void)
{
        vmarea_t *newvma = (vmarea_t *)slab_obj_alloc(vmarea_allocator);
        if (newvma)
        {
                newvma->vma_vmmap = NULL;
        }
        return newvma;
}

void vmarea_free(vmarea_t *vma)
{
        KASSERT(NULL != vma);
        slab_obj_free(vmarea_allocator, vma);
}

/* a debugging routine: dumps the mappings of the given address space. */
size_t
vmmap_mapping_info(const void *vmmap, char *buf, size_t osize)
{
        KASSERT(0 < osize);
        KASSERT(NULL != buf);
        KASSERT(NULL != vmmap);

        vmmap_t *map = (vmmap_t *)vmmap;
        vmarea_t *vma;
        ssize_t size = (ssize_t)osize;

        int len = snprintf(buf, size, "%21s %5s %7s %8s %10s %12s\n",
                           "VADDR RANGE", "PROT", "FLAGS", "MMOBJ", "OFFSET",
                           "VFN RANGE");

        list_iterate_begin(&map->vmm_list, vma, vmarea_t, vma_plink)
        {
                size -= len;
                buf += len;
                if (0 >= size)
                {
                        goto end;
                }

                len = snprintf(buf, size,
                               "%#.8x-%#.8x  %c%c%c  %7s 0x%p %#.5x %#.5x-%#.5x\n",
                               vma->vma_start << PAGE_SHIFT,
                               vma->vma_end << PAGE_SHIFT,
                               (vma->vma_prot & PROT_READ ? 'r' : '-'),
                               (vma->vma_prot & PROT_WRITE ? 'w' : '-'),
                               (vma->vma_prot & PROT_EXEC ? 'x' : '-'),
                               (vma->vma_flags & MAP_SHARED ? " SHARED" : "PRIVATE"),
                               vma->vma_obj, vma->vma_off, vma->vma_start, vma->vma_end);
        }
        list_iterate_end();

end:
        if (size <= 0)
        {
                size = osize;
                buf[osize - 1] = '\0';
        }
        /*
        KASSERT(0 <= size);
        if (0 == size) {
                size++;
                buf--;
                buf[0] = '\0';
        }
        */
        return osize - size;
}

/* Create a new vmmap, which has no vmareas and does
 * not refer to a process. */
vmmap_t *
vmmap_create(void)
{
        // NOT_YET_IMPLEMENTED("VM: vmmap_create");
        vmmap_t *new_vmmap = (vmmap_t *)slab_obj_alloc(vmmap_allocator);
        new_vmmap->vmm_proc = NULL;
        list_init(&new_vmmap->vmm_list);
        return new_vmmap;
}

/* Removes all vmareas from the address space and frees the
 * vmmap struct. */
void vmmap_destroy(vmmap_t *map)
{
        KASSERT(NULL != map);
        dbg(DBG_PRINT, "(GRADING3A 3.a)\n");

        vmarea_t *cur_maparea;
        list_iterate_begin(&map->vmm_list, cur_maparea, vmarea_t, vma_plink)
        {
                dbg(DBG_PRINT, "(GRADING3A)\n");
                uint32_t lopage = cur_maparea->vma_start;
                uint32_t npages = cur_maparea->vma_end - lopage;
                vmmap_remove(map, lopage, npages);
        }
        list_iterate_end();
        dbg(DBG_PRINT, "(GRADING3A)\n");
        slab_obj_free(vmmap_allocator, map);
}

/* Add a vmarea to an address space. Assumes (i.e. asserts to some extent)
 * the vmarea is valid.  This involves finding where to put it in the list
 * of VM areas, and adding it. Don't forget to set the vma_vmmap for the
 * area. */
void vmmap_insert(vmmap_t *map, vmarea_t *newvma)
{
        // NOT_YET_IMPLEMENTED("VM: vmmap_insert");
        KASSERT(NULL != map && NULL != newvma);
        dbg(DBG_PRINT, "(GRADING3A 3.b)\n");

        //???????
        //KASSERT(NULL == newvma->vma_vmmap);
        dbg(DBG_PRINT, "(GRADING3A 3.b)\n");

        KASSERT(newvma->vma_start < newvma->vma_end);
        dbg(DBG_PRINT, "(GRADING3A 3.b)\n");
        KASSERT(ADDR_TO_PN(USER_MEM_LOW) <= newvma->vma_start && ADDR_TO_PN(USER_MEM_HIGH) >= newvma->vma_end);
        dbg(DBG_PRINT, "(GRADING3A 3.b)\n");
        vmarea_t *cur_maparea;
        list_iterate_begin(&map->vmm_list, cur_maparea, vmarea_t, vma_plink)
        {
                dbg(DBG_PRINT, "(GRADING3A)\n");
                uint32_t cur_lopage = cur_maparea->vma_start;
                uint32_t new_lopages = newvma->vma_end;
                if (new_lopages <= cur_lopage)
                {
                        dbg(DBG_PRINT, "(GRADING3A)\n");
                        newvma->vma_vmmap = map;
                        list_insert_tail(&cur_maparea->vma_plink, &newvma->vma_plink);
                        return;
                }
        }
        list_iterate_end();
        dbg(DBG_PRINT, "(GRADING3A)\n");
        list_insert_tail(&map->vmm_list, &newvma->vma_plink);
        newvma->vma_vmmap = map;
}

/* Find a contiguous range of free virtual pages of length npages in
 * the given address space. Returns starting vfn for the range,
 * without altering the map. Returns -1 if no such range exists.
 *
 * Your algorithm should be first fit. If dir is VMMAP_DIR_HILO, you
 * should find a gap as high in the address space as possible; if dir
 * is VMMAP_DIR_LOHI, the gap should be as low as possible. */
int vmmap_find_range(vmmap_t *map, uint32_t npages, int dir)
{
        // NOT_YET_IMPLEMENTED("VM: vmmap_find_range");
        vmarea_t *map_area;
        uint32_t high = ADDR_TO_PN(USER_MEM_HIGH);
        int result = -1;
        if (dir == VMMAP_DIR_HILO)
        {
                dbg(DBG_PRINT, "(GRADING3A)\n");
                list_iterate_reverse(&map->vmm_list, map_area, vmarea_t, vma_plink)
                {
                        dbg(DBG_PRINT, "(GRADING3A)\n");
                        if (high < npages + map_area->vma_end)
                        {
                                dbg(DBG_PRINT, "(GRADING3D 1)\n");
                                high = map_area->vma_start;
                        }
                        else
                        {
                                dbg(DBG_PRINT, "(GRADING3A)\n");
                                result = high - npages;
                                break;
                        }
                }
                list_iterate_end();
                // return (high >= npages + low) ? (high - npages) : (-1);
                dbg(DBG_PRINT, "(GRADING3D 2)\n");
                if (high >= npages + ADDR_TO_PN(USER_MEM_LOW))
                {
                        dbg(DBG_PRINT, "(GRADING3D 2)\n");
                        result = high - npages;
                }
        }

        dbg(DBG_PRINT, "(GRADING3D 2)\n");
        return result;
}

/* Find the vm_area that vfn lies in. Simply scan the address space
 * looking for a vma whose range covers vfn. If the page is unmapped,
 * return NULL. */
vmarea_t *
vmmap_lookup(vmmap_t *map, uint32_t vfn)
{
        // NOT_YET_IMPLEMENTED("VM: vmmap_lookup");
        KASSERT(NULL != map);
        dbg(DBG_PRINT, "(GRADING3A 3.c)\n");
        vmarea_t *vmarea;
        // If vfn is between vma_start and vma_end area
        list_iterate_begin(&map->vmm_list, vmarea, vmarea_t, vma_plink)
        {
                dbg(DBG_PRINT, "(GRADING3A)\n");
                uint32_t start = vmarea->vma_start;
                uint32_t end = vmarea->vma_end;
                if (start <= vfn && end > vfn)
                {
                        dbg(DBG_PRINT, "(GRADING3A)\n");
                        return vmarea;
                }
        }
        list_iterate_end();
        dbg(DBG_PRINT, "(GRADING3C 5)\n");
        return NULL;
}

/* Allocates a new vmmap containing a new vmarea for each area in the
 * given map. The areas should have no mmobjs set yet. Returns pointer
 * to the new vmmap on success, NULL on failure. This function is
 * called when implementing fork(2). */
vmmap_t *
vmmap_clone(vmmap_t *map)
{
        // NOT_YET_IMPLEMENTED("VM: vmmap_clone");
        vmmap_t *copy_vmmap = vmmap_create();
        vmarea_t *map_area;
        list_iterate_begin(&map->vmm_list, map_area, vmarea_t, vma_plink)
        {
                dbg(DBG_PRINT, "(GRADING3A)\n");
                vmarea_t *copy_vmarea = vmarea_alloc();
                copy_vmarea->vma_start = map_area->vma_start;
                copy_vmarea->vma_end = map_area->vma_end;
                copy_vmarea->vma_off = map_area->vma_off;
                copy_vmarea->vma_obj = NULL;
                copy_vmarea->vma_prot = map_area->vma_prot;
                copy_vmarea->vma_flags = map_area->vma_flags;
                copy_vmarea->vma_vmmap = copy_vmmap;

                list_link_init(&copy_vmarea->vma_plink);
                list_link_init(&copy_vmarea->vma_olink);

                list_insert_before(&copy_vmmap->vmm_list, &copy_vmarea->vma_plink);
        }
        list_iterate_end();
        dbg(DBG_PRINT, "(GRADING3A)\n");
        return copy_vmmap;
}

/* Insert a mapping into the map starting at lopage for npages pages.
 * If lopage is zero, we will find a range of virtual addresses in the
 * process that is big enough, by using vmmap_find_range with the same
 * dir argument.  If lopage is non-zero and the specified region
 * contains another mapping that mapping should be unmapped.
 *
 * If file is NULL an anon mmobj will be used to create a mapping
 * of 0's.  If file is non-null that vnode's file will be mapped in
 * for the given range.  Use the vnode's mmap operation to get the
 * mmobj for the file; do not assume it is file->vn_obj. Make sure all
 * of the area's fields except for vma_obj have been set before
 * calling mmap.
 *
 * If MAP_PRIVATE is specified set up a shadow object for the mmobj.
 *
 * All of the input to this function should be valid (KASSERT!).
 * See mmap(2) for for description of legal input.
 * Note that off should be page aligned.
 *
 * Be very careful about the order operations are performed in here. Some
 * operation are impossible to undo and should be saved until there
 * is no chance of failure.
 *
 * If 'new' is non-NULL a pointer to the new vmarea_t should be stored in it.
 */
int vmmap_map(vmmap_t *map, vnode_t *file, uint32_t lopage, uint32_t npages,
              int prot, int flags, off_t off, int dir, vmarea_t **new)
{
        // NOT_YET_IMPLEMENTED("VM: vmmap_map");
        KASSERT(NULL != map);
        dbg(DBG_PRINT, "(GRADING3A 3.d)\n");
        KASSERT(0 < npages);
        dbg(DBG_PRINT, "(GRADING3A 3.d)\n");
        KASSERT((MAP_SHARED & flags) || (MAP_PRIVATE & flags));
        dbg(DBG_PRINT, "(GRADING3A 3.d)\n");
        KASSERT((0 == lopage) || (ADDR_TO_PN(USER_MEM_LOW) <= lopage));
        dbg(DBG_PRINT, "(GRADING3A 3.d)\n");
        KASSERT((0 == lopage) || (ADDR_TO_PN(USER_MEM_HIGH) >= (lopage + npages)));
        dbg(DBG_PRINT, "(GRADING3A 3.d)\n");
        KASSERT(PAGE_ALIGNED(off));
        dbg(DBG_PRINT, "(GRADING3A 3.d)\n");
        if (lopage == 0)
        {
                dbg(DBG_PRINT, "(GRADING3A)\n");
                // find a range of virtual addresses in the process that is big enough
                int start = vmmap_find_range(map, npages, dir);
                if (start == -1)
                {
                        dbg(DBG_PRINT, "(GRADING3D 2)\n");
                        // no such range exists
                        return -1;
                }
                dbg(DBG_PRINT, "(GRADING3A)\n");
                lopage = start;
        }
        else
        {
                dbg(DBG_PRINT, "(GRADING3A)\n");
                // 1: no mappings, 0: has mapping
                int isRangeEmpty = vmmap_is_range_empty(map, lopage, npages);

                if (isRangeEmpty == 0)
                {
                        dbg(DBG_PRINT, "(GRADING3A)\n");
                        // unmap this mapping, always return 0
                        int err = vmmap_remove(map, lopage, npages);
                }
        }
        vmarea_t *vmarea = vmarea_alloc();

        vmarea->vma_start = lopage;
        vmarea->vma_end = lopage + npages;
        vmarea->vma_off = ADDR_TO_PN(off);
        vmarea->vma_prot = prot;
        vmarea->vma_flags = flags;
        vmarea->vma_obj = NULL;
        list_init(&vmarea->vma_plink);
        list_init(&vmarea->vma_olink);
        vmmap_insert(map, vmarea);

        mmobj_t *mmobj;
        if (file == NULL)
        {
                dbg(DBG_PRINT, "(GRADING3A)\n");
                mmobj = anon_create();
                vmarea->vma_obj = mmobj;
        }
        else
        {
                dbg(DBG_PRINT, "(GRADING3A)\n");
                int mapErrCode = file->vn_ops->mmap(file, vmarea, &mmobj);
                // if (mapErrCode == -1)
                // {
                //         dbg(DBG_PRINT, "(NEWPATH)\n");
                //         vmarea_free(vmarea);
                //         return -1;
                // }
                dbg(DBG_PRINT, "(GRADING3A)\n");
                vmarea->vma_obj = mmobj;
        }

        if (MAP_PRIVATE & flags)
        {
                dbg(DBG_PRINT, "(GRADING3A)\n");
                // set up a shadow object
                mmobj_t *shadow = shadow_create();
                mmobj_t *bottom = mmobj_bottom_obj(mmobj);
                shadow->mmo_shadowed = mmobj;
                shadow->mmo_un.mmo_bottom_obj = bottom;

                // change vmarea obj to shadow obj
                vmarea->vma_obj = shadow;
                bottom->mmo_ops->ref(bottom);
        }
        dbg(DBG_PRINT, "(GRADING3A)\n");
        list_insert_head(&mmobj->mmo_un.mmo_vmas, &vmarea->vma_olink);

        if (new != NULL)
        {
                dbg(DBG_PRINT, "(GRADING3A)\n");
                *new = vmarea;
        }
        dbg(DBG_PRINT, "(GRADING3A)\n");
        return 0;
}
/*
 * We have no guarantee that the region of the address space being
 * unmapped will play nicely with our list of vmareas.
 *
 * You must iterate over each vmarea that is partially or wholly covered
 * by the address range [addr ... addr+len). The vm-area will fall into one
 * of four cases, as illustrated below:
 *
 * key:
 *          [             ]   Existing VM Area
 *        *******             Region to be unmapped
 *
 * Case 1:  [   ******    ]
 * The region to be unmapped lies completely inside the vmarea. We need to
 * split the old vmarea into two vmareas. be sure to increment the
 * reference count to the file associated with the vmarea.
 *
 * Case 2:  [      *******]**
 * The region overlaps the end of the vmarea. Just shorten the length of
 * the mapping.
 *
 * Case 3: *[*****        ]
 * The region overlaps the beginning of the vmarea. Move the beginning of
 * the mapping (remember to update vma_off), and shorten its length.
 *
 * Case 4: *[*************]**
 * The region completely contains the vmarea. Remove the vmarea from the
 * list.
 */
int vmmap_remove(vmmap_t *map, uint32_t lopage, uint32_t npages)
{
        // NOT_YET_IMPLEMENTED("VM: vmmap_remove");

        vmarea_t *vm;

        list_iterate_begin(&map->vmm_list, vm, vmarea_t, vma_plink)
        {
                dbg(DBG_PRINT, "(GRADING3A)\n");
                if (vm->vma_end > (lopage + npages) && lopage > vm->vma_start)
                {
                        dbg(DBG_PRINT, "(GRADING3D 2)\n");
                        vmarea_t *nvma = vmarea_alloc();
                        nvma->vma_start = lopage + npages;
                        nvma->vma_end = vm->vma_end;
                        nvma->vma_off = vm->vma_off + lopage + npages - vm->vma_start;
                        nvma->vma_flags = vm->vma_flags;
                        nvma->vma_vmmap = map;
                        nvma->vma_prot = vm->vma_prot;
                        nvma->vma_obj = vm->vma_obj;
                        list_init(&nvma->vma_plink);
                        list_init(&nvma->vma_olink);
                        vmmap_insert(map, nvma);
                        nvma->vma_obj->mmo_ops->ref(nvma->vma_obj);
                        vm->vma_end = lopage;
                }
                else if (vm->vma_end <= (lopage + npages) && vm->vma_start < lopage && vm->vma_end > lopage)
                {
                        dbg(DBG_PRINT, "(GRADING3D 1)\n");
                        vm->vma_end = lopage;
                }
                else if (vm->vma_end > (lopage + npages) && vm->vma_start >= lopage && vm->vma_start < (lopage + npages))
                {
                        dbg(DBG_PRINT, "(GRADING3D 2)\n");
                        vm->vma_off += lopage + npages - vm->vma_start;
                        vm->vma_start = lopage + npages;
                }
                else if (vm->vma_end <= (lopage + npages) && vm->vma_start >= lopage)
                {
                        dbg(DBG_PRINT, "(GRADING3A)\n");
                        list_remove(&vm->vma_plink);
                        list_remove(&vm->vma_olink);
                        vm->vma_obj->mmo_ops->put(vm->vma_obj);
                        vmarea_free(vm);
                }
        }
        list_iterate_end();
        dbg(DBG_PRINT, "(GRADING3A)\n");
        tlb_flush_all();
        pt_unmap_range(curproc->p_pagedir, (uintptr_t)PN_TO_ADDR(lopage), (uintptr_t)PN_TO_ADDR(lopage + npages));

        return 0;
}

/*
 * Returns 1 if the given address space has no mappings for the
 * given range, 0 otherwise.
 */
int vmmap_is_range_empty(vmmap_t *map, uint32_t startvfn, uint32_t npages)
{
        // NOT_YET_IMPLEMENTED("VM: vmmap_is_range_empty");
        KASSERT((startvfn < startvfn + npages) && (ADDR_TO_PN(USER_MEM_LOW) <= startvfn) && (ADDR_TO_PN(USER_MEM_HIGH) >= startvfn + npages));
        dbg(DBG_PRINT, "(GRADING3A 3.d)\n");
        if (!list_empty(&map->vmm_list))
        {
                dbg(DBG_PRINT, "(GRADING3A)\n");
                vmarea_t *vma;
                list_iterate_begin(&map->vmm_list, vma, vmarea_t, vma_plink)
                {
                        dbg(DBG_PRINT, "(GRADING3A)\n");
                        /* [   *****]**** */
                        if (!(startvfn >= vma->vma_end) && !(npages + startvfn <= vma->vma_start))
                        {
                                dbg(DBG_PRINT, "(GRADING3A)\n");
                                return 0;
                        }
                }
                list_iterate_end();
                dbg(DBG_PRINT, "(GRADING3A)\n");
        }
        dbg(DBG_PRINT, "(GRADING3A)\n");
        return 1;
}
/* Read into 'buf' from the virtual address space of 'map' starting at
 * 'vaddr' for size 'count'. To do so, you will want to find the vmareas
 * to read from, then find the pframes within those vmareas corresponding
 * to the virtual addresses you want to read, and then read from the
 * physical memory that pframe points to. You should not check permissions
 * of the areas. Assume (KASSERT) that all the areas you are accessing exist.
 * Returns 0 on success, -errno on error.
 */
int vmmap_read(vmmap_t *map, const void *vaddr, void *buf, size_t count)
{
        // NOT_YET_IMPLEMENTED("VM: vmmap_read");
        uint32_t bytes = 0;
        uint32_t cur = (uint32_t)vaddr;
        uint32_t end = (uint32_t)cur + count;
        uint32_t cur_addr = ADDR_TO_PN(cur);
        uint32_t cur_offset = PAGE_OFFSET(cur);
        vmarea_t *vma;
        mmobj_t *o;

        while (cur < end)
        {
                dbg(DBG_PRINT, "(GRADING3A)\n");
                vma = vmmap_lookup(map, cur_addr);
                o = vma->vma_obj;
                pframe_t *pf;
                uint32_t pagenum = vma->vma_off + cur_addr - vma->vma_start;

                o->mmo_ops->lookuppage(o, pagenum, 0, &pf);

                if (count - bytes < PAGE_SIZE - cur_offset)
                {
                        dbg(DBG_PRINT, "(GRADING3A)\n");
                        int temp = count - bytes;
                        memcpy((char *)buf + bytes, (char *)pf->pf_addr + cur_offset, temp);
                        cur = cur + temp;
                        bytes = bytes + temp;
                }
                else
                {
                        dbg(DBG_PRINT, "(GRADING3A)\n");
                        int temp = PAGE_SIZE - cur_offset;
                        memcpy((char *)buf + bytes, (char *)pf->pf_addr + cur_offset, temp);
                        cur = cur + temp;
                        bytes = bytes + temp;
                }
                cur_addr = ADDR_TO_PN(cur);
                cur_offset = PAGE_OFFSET(cur);
                dbg(DBG_PRINT, "(GRADING3A)\n");
        }
        dbg(DBG_PRINT, "(GRADING3A)\n");
        return 0;
}

/* Write from 'buf' into the virtual address space of 'map' starting at
 * 'vaddr' for size 'count'. To do this, you will need to find the correct
 * vmareas to write into, then find the correct pframes within those vmareas,
 * and finally write into the physical addresses that those pframes correspond
 * to. You should not check permissions of the areas you use. Assume (KASSERT)
 * that all the areas you are accessing exist. Remember to dirty pages!
 * Returns 0 on success, -errno on error.
 */
int vmmap_write(vmmap_t *map, void *vaddr, const void *buf, size_t count)
{
        // NOT_YET_IMPLEMENTED("VM: vmmap_write");
        uint32_t bytes = 0;
        uint32_t cur = (uint32_t)vaddr;
        uint32_t end = (uint32_t)cur + count;
        uint32_t cur_addr = ADDR_TO_PN(cur);
        uint32_t cur_offset = PAGE_OFFSET(cur);
        vmarea_t *vma;
        mmobj_t *o;

        while (cur < end)
        {
                dbg(DBG_PRINT, "(GRADING3A)\n");
                vma = vmmap_lookup(map, cur_addr);
                o = vma->vma_obj;
                pframe_t *pf;
                uint32_t pagenum = vma->vma_off + cur_addr - vma->vma_start;
                o->mmo_ops->lookuppage(o, pagenum, 1, &pf);

                if (count - bytes < PAGE_SIZE - cur_offset)
                {
                        dbg(DBG_PRINT, "(GRADING3A)\n");
                        int temp = count - bytes;
                        memcpy((char *)pf->pf_addr + cur_offset, (char *)buf + bytes, temp);
                        pframe_dirty(pf);
                        cur += temp;
                        bytes += temp;
                }
                else
                {
                        dbg(DBG_PRINT, "(GRADING3A)\n");
                        int temp = PAGE_SIZE - cur_offset;
                        memcpy((char *)pf->pf_addr + cur_offset, (char *)buf + bytes, temp);
                        pframe_dirty(pf);
                        cur += temp;
                        bytes += temp;
                }
                cur_addr = ADDR_TO_PN(cur);
                cur_offset = PAGE_OFFSET(cur);
                dbg(DBG_PRINT, "(GRADING3A)\n");
        }

        return 0;
}