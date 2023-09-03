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
