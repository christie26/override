# level00 Find a password

## Overview
level00 is simple. We can easily find the input they want from `asm`.

## Code
```
0x80484de <main+74>     call   0x80483d0 <__isoc99_scanf@plt>
0x80484e3 <main+79>     mov    0x1c(%esp),%eax
0x80484e7 <main+83>     cmp    $0x149c,%eax
0x80484ec <main+88>     jne    0x804850d <main+121>
0x80484ee <main+90>     movl   $0x8048639,(%esp)
0x80484f5 <main+97>     call   0x8048390 <puts@plt>
0x80484fa <main+102>    movl   $0x8048649,(%esp)
0x8048501 <main+109>    call   0x80483a0 <system@plt>
0x8048506 <main+114>    mov    $0x0,%eax
0x804850b <main+119>    jmp    0x804851e <main+138>
0x804850d <main+121>    movl   $0x8048651,(%esp)
0x8048514 <main+128>    call   0x8048390 <puts@plt>
```
It compares result of `scanf` with `0x149c` and if they are same, they run `system("/bin/sh")`.

We just have to put `5276` as input. 

## Payload
```
level00@OverRide:~$ ./level00
***********************************
* 	     -Level00 -		  *
***********************************
Password:5276

Authenticated!
$ whoami
level01
$ cat /home/users/level01/.pass
uSq2ehEGT6c9S24zbshexZQBXUGrncxn5sD5QfGL
```

## Flag
```
uSq2ehEGT6c9S24zbshexZQBXUGrncxn5sD5QfGL
```