# level 4 <vulnerability name/type>

## Overview
<explanation about: the type of binary in high level and the type of vulnerability>

## Steps (copiable for the correction)
<steps for the solution, with explanation on how you found out>

### gets
*there is vulnerability with `gets`*

let's check address of each variables.

argument of gets: `%esp+0x20` = `0xffffd610+0x20` = `0xffffd630`\
return value of `fork` = adress of `pid`: `%esp+0xac` = `0xffffd6bc`\
offset: 172 - 32 = 140
`&v4` = `0xffffd62c`

### challenge
1. weird if statement. \
`if ( (v4 & 0x7F) == 0 || (v7 = v4, (char)((v4 & 0x7F) + 1) >> 1 > 0) )`\
It seems impossible to pass it... 
2. how to make shellcode be executed?
3. not fully understand how `ptrace` works. 

### reference links
[Playing with ptrace](https://www.linuxjournal.com/article/6100)\
[ptrace.h](https://sites.uclouvain.be/SystInfo/usr/include/sys/ptrace.h.html)\
[Exploiting Linux systems with ptrace](https://raj3shp.medium.com/exploiting-linux-systems-with-ptrace-637f13a7d9ea)\
[Intercepting and Emulating Linux System Calls with Ptrace](https://nullprogram.com/blog/2018/06/23/)

## Code
### in child
```c
prctl(1, 1);
ptrace(0, 0, 0, 0);
puts("Give me some shellcode, k");
gets(v5);
```
- `prctl(1, 1)` sets the process to receive a SIGHUP signal if its parent process dies.
- `ptrace(PTRACE_TRACEME, 0, 0, 0)` declares that the current process should be traced by its parent, often used in debugging scenarios.

### in parent
```c
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

```
(gdb) set follow-fork-mode child
(gdb) set follow-fork-mode parent
```

```
v4 = 0x1c(%esp)=  0xffffd62c
v6 = 0xa0(%esp) = 0xffffd6bc
v7 = 0xa4(%esp) = 0xffffd6b4

0x8048775 <main+173>    mov    0x1c(%esp),%eax -> eax: 0x157f
0x8048779 <main+177>    mov    %eax,0xa0(%esp)
0x8048780 <main+184>    mov    0xa0(%esp),%eax
0x8048787 <main+191>    and    $0x7f,%eax
0x804878a <main+194>    test   %eax,%eax
0x804878c <main+196>    je     0x80487ac <main+228>

0x804878e <main+198>    mov    0x1c(%esp),%eax
0x8048792 <main+202>    mov    %eax,0xa4(%esp)
0x8048799 <main+209>    mov    0xa4(%esp),%eax
0x80487a0 <main+216>    and    $0x7f,%eax
0x80487a3 <main+219>    add    $0x1,%eax
0x80487a6 <main+222>    sar    %al
0x80487a8 <main+224>    test   %al,%al
```

## Payload (Most important command to obtain the flag)

## Flag