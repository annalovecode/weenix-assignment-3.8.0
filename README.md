# weenix OS
refer to: https://merlot.usc.edu/cs402-s23/projects.html The OS is based on the x86 architecture and should, in theory, compile to an image that can be run on a bare x86 machine. For simplicity, however, you will be testing and developing parts of the OS on an x86 emulator called Qemu.
A full operating system kernel, based on Unix, built as a semester-long project in CS 1690 (Operating Systems) at Brown University. Read more about the course and the individual projects [here](https://github.com/brown-cs1690/handout/wiki).

The projects, in order of completion:
* **Procs** - Threads, processes, and synchronization primitives.
* **Drivers** - Device drivers for terminals, disks, and the memory devices `/dev/zero` and `/dev/null`.
* **VFS (Virtual File System)** - A polymorphic interface between the operating system kernel and the various file systems (such as S5FS and device drivers).
* **S5FS (System V File System)** - A file system implementation based on the original Unix file system.
* **VM (Virtual Memory)** - Userspace address space management, running user-level code, servicing system calls, and basically everything else needed to combine all of the previous componenets into a fully functioning operating system. This includes virtual memory maps, handling page faults, memory management via anonymous objects and shadow objects, and system calls (in particular, the `fork` syscall).

NOTE: This is a placeholder repo with no code to respect Brown's Academic Code. If you are a potential employer and would like to look at the code, please send me an email.

Using Tech:
Abstraction of threads, processes, and mutex
Context switching
Interrupts
Understanding the boot process of the kernel
Reference counting in the file system
System call wrapper
Case study of S5FS (system V file system)
Demand paging
Virtual memory map and the address space representation of a process
Copy on write mechanism and shadow objects
Memory allocation strategies
First fit & Best fit (both of which suffer from external fragmentation)
Buddy system (which is relatively new and chooses internal fragmentation as its design strategy)
Slab allocation (for pre-defined storage requirements)
Memory layout of a program (text, data, stack and heap sections)
Write through and write back caches
Benfits of log structured file system
File system consistency/fault tolerance: Soft updates vs Shadow paging

这是关于在布朗大学（Brown University）的CS 1690（操作系统）课程中完成的一个学期项目，它构建了一个基于Unix的完整操作系统内核。该项目包含了一系列的子项目，按完成顺序如下：

Procs - 进程、线程和同步原语：

介绍了线程、进程以及用于实现同步的基本原语。
Drivers - 设备驱动程序：

实现了终端、磁盘以及内存设备（如/dev/zero和/dev/null）的设备驱动程序。
VFS（虚拟文件系统） - 操作系统内核与各种文件系统（如S5FS和设备驱动程序）之间的多态接口：

创建了一个多态接口，允许操作系统内核与不同的文件系统进行交互，包括S5FS和设备驱动程序。
S5FS（System V文件系统） - 基于原始Unix文件系统的文件系统实现：

基于原始Unix文件系统的概念，构建了S5FS文件系统的实现。
VM（虚拟内存） - 用户空间地址空间管理，运行用户级代码，处理系统调用以及整合前述所有组件以创建完全运行的操作系统所需的一切：

包括虚拟内存映射、处理页面错误、通过匿名对象和影子对象进行内存管理，以及系统调用（特别是fork系统调用）等内容。这个子项目的目标是将之前的所有组件整合在一起，构建一个完全功能的操作系统。
这个项目是在操作系统课程中完成的，它涵盖了操作系统内核的各个方面，包括进程管理、设备驱动程序、文件系统、虚拟内存管理等等。这为我提供了深入理解操作系统工作原理的机会，并且增强了我的操作系统开发技能。

使用的技术和概念:

线程、进程和互斥抽象: 抽象化了线程、进程和互斥操作，管理多任务操作。

上下文切换: 实现了上下文切换，允许操作系统在不同任务之间切换执行。

中断: 处理了硬件中断，使操作系统能够响应外部事件。

内核引导过程的理解: 理解了操作系统内核的启动过程，包括引导加载程序的执行。

文件系统中的引用计数: 在文件系统中使用引用计数来跟踪文件或资源的引用数量。

系统调用封装: 创建了系统调用的封装，提供了用户程序和操作系统之间的接口。

S5FS（System V文件系统）案例研究: 研究了S5FS文件系统，了解其设计和功能。

需求分页: 实现了按需页面调入，根据需要加载数据到内存中。

虚拟内存映射和进程的地址空间表示: 管理虚拟内存映射和进程的地址空间表示。

写时复制机制和影子对象: 实现了写时复制（Copy on Write）机制，以及使用影子对象管理内存。

内存分配策略: 实施不同的内存分配策略，包括首次适配（First Fit）、最佳适配（Best Fit）、伙伴系统（Buddy System）和Slab分配。

程序的内存布局: 了解程序在内存中的布局，包括文本段、数据段、堆栈和堆段。

写透和写回缓存: 理解写透缓存和写回缓存的概念，以优化内存操作。

日志结构文件系统的优势: 了解日志结构文件系统的优点，包括性能和恢复能力方面的优势。

文件系统的一致性/容错性: 探讨了文件系统一致性和容错性，包括软更新（Soft updates）和影子分页（Shadow Paging）的比较。

这些技术和概念涵盖了操作系统开发和文件系统管理的多个方面，包括多任务处理、内存管理、文件系统设计和性能优化等。
