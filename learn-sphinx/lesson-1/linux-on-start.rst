Linux从开机到桌面启动，经历过什么事情？
==================================================

BIOS
--------

1. 硬件自检（Power-On Self-Test），缩写为POST

2. BIOS初始化必要的硬件

3. 启动顺序, 根据BIOS设置的顺序进行启动，将控制权转交给下一阶段启动程序。

主引导记录(MBR)
-----------------
主引导记录主要作用, 是告诉计算机到硬盘的哪一个位置去找操作系统。它由三个部分组成：

  (1) 第1-446字节：调用操作系统的机器码。
  (2) 第447-510字节：分区表（Partition table）。
  (3) 第511-512字节：主引导记录签名（0x55和0xAA）。

与主引导记录类似的， 文件系统分区中，区域最前面有一个boot sector, 可以安装其他不同于MBR的引导程序。因为如此，通过链式引导，使得我们可以安装多种操作系统在同个主机上。

.. loader_menu:
.. figure:: http://linux.vbird.org/linux_basic/0510osloader//loader_menu.gif

常见的引导程序LILO, GRUB等就是安装在MBR中的。安装了GRUB之后，BIOS完成自身的任务后就会将控制权交给GRUB，此时GRUB会加载/boot/grub, 提供操作系统等选项。

载入内核
--------------------
Linux的设备驱动程序的加载，有一部分驱动程序直接被编译进内核镜像中，另一部分驱动程序则是以模块的形式放在initrd(ramdisk)中, 可以动态加载。

Linux的内核镜像仅是包含了基本的硬件驱动，在系统安装过程中会检测系统硬件信息，根据安装信息和系统硬件信息将一部分设备驱动写入 initrd,
initrd 系统中的文件在开机阶段可以被核心访问，里面的内容会被挂载成一个 loop 型态的文件, 它可直接加载驱动到内存而无需依赖其它设备。

一般来说, 内核文件会存放在 /boot 下: 

.. code:: sh

    % ll
    .
    ├── [drwxr-xr-x root     1.0K Dec 16 19:45]  grub/
    ├── [-rw-r--r-- root      18M Dec 16 19:35]  initramfs-linux-fallback.img
    ├── [-rw-r--r-- root     3.4M Dec 16 19:35]  initramfs-linux.img
    └── [-rw-r--r-- root     4.1M Oct 27 15:14]  vmlinuz-linux

在/boot 目录下,还存在文件/boot/initramfs-linux.img, 它是一个cpio包，这个cpio包中包含了一个小型的文件系统。initramfs 是 initrd 的一个替代品，以另外一种方式实现了曾经 initrd 的功能。

内核通过调用``start_kernel``, 进行建立内存管理、中断、配置cpu等， 通过加载“initrd”进行挂载假的根目录， 
进而直接加载驱动，加载成功后再umount，挂载真实的根目录等。(因为未加载驱动前，无法识别分区文件系统)

.. osloader-flow-initramfs:
.. figure:: http://linux.vbird.org/linux_basic/0510osloader//osloader-flow-initramfs.jpg

初始化系统
----------
载入内核后， 执行 /sbin/init 作为第一个进程（以守护进程的方式存在，其进程号为1）。

init是 Unix 和 类Unix 系统中用来产生其它所有进程的程序， 使用不同的发行版本有不同的风格。
除了系统初始化，init还负责重启、关机、单用户恢复模式。为了支持上述操作，inittab把条目分到不同的运行级别(runlevel)中去。

  Arch使用以下运行级别：0——关机，1（又叫S）——单用户模式，3——普通的多用户模式，5——X使用，6——重启。其他发行版可能有所不同，但0、1、6级别是通用的。

init 风格
  * Ubuntu 使用的是upstart, 具体介绍可看  Upstart与系统启动过程_ 
  * arch linux 使用的是Systemd

    Systemd使用“target”来处理引导和服务管理过程。这些systemd里的“target”文件被用于分组不同的引导单元以及启动同步进程。
    对应过程如下图所示：

   .. syntemd:
   .. figure:: https://dn-linuxcn.qbox.me/data/attachment/album/201505/17/203724enh95nh9idhkyglc.jpg

Getty
------
init 为每一个 虚拟终端 调用 getty，前者一般有六个，每个虚拟终端都会初始化 tty 并请求输入用户名和密码。当在某虚拟终端输入用户名和密码后，其 getty 会通过 /etc/passwd 检查是否正确，如果正确，就接着调用 login, 即为用户启动一个「会话」，接着根据 /etc/passwd 文件启动用户专用 shell. 此外，getty 也可能会改启动一个显示管理器。
显示管理器

如果事先装了某个 显示管理器, 它会代替原来的 getty 登录命令行提示符而启动。如果没有显示管理器，getty 只会显示向用户请求用户名和密码以登录的若干命令行，以准备调用 login.

Login
------
所谓的 login 程序会为用户启动一个设置了环境变量的「会话」，接着根据 /etc/passwd 配置以启动用户专用 shell.

Shell
-----
一旦用户专用的 shell 启动了，它会在显示命令行提示符前，执行一个「有可执行性的配置文件」，比如 .bashrc. 如果用户有设定了 Start X at login, 原来那个「有可执行性的配置文件」会调用 startx or xinit.

xinit
------
xinit 也会调用用户的 .xinitrc这个「有可执行性的配置文件」，后者一般用来启动一个 窗口管理器。如果用户退出了窗口管理器，xinit, startx, shell login 就会先后中断，返回到 getty.

.. _Upstart与系统启动过程: https://conf.umlife.net/pages/viewpage.action?pageId=27627447
