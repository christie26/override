# level 6 <>

## Overview

## Code
In the binary, they first get input with `fgets` and use `scanf`. And call function `auth` with two arguments.

#### fgets
It reads 32 bytes from `stdin` to `0xffffd63c`.
```c
fgets(0xffffd63c, 0x20, stdin);
```
#### scanf
It reads from `stdin` to `0xffffd638` as unsigned int format.
```c
scanf(&0xffffd638, "%u");
```
#### strcspn
```c
strcspn(0xffffd63c, "\n");
```
with this function they get len of string before "\n" occurs. 

#### movb
`movb   $0x0,(%eax)` will change first bytes of `0xffffd63c` + `resulf of strcspn`

```
0x8048748 <auth>        push   %ebp
0x8048749 <auth+1>      mov    %esp,%ebp
0x804874b <auth+3>      sub    $0x28,%esp
0x804874e <auth+6>      movl   $0x8048a63,0x4(%esp)
0x8048756 <auth+14>     mov    0x8(%ebp),%eax
0x8048759 <auth+17>     mov    %eax,(%esp)
0x804875c <auth+20>     call   0x8048520 <strcspn@plt>
0x8048761 <auth+25>     add    0x8(%ebp),%eax
0x8048764 <auth+28>     movb   $0x0,(%eax)
0x8048767 <auth+31>     movl   $0x20,0x4(%esp)
0x804876f <auth+39>     mov    0x8(%ebp),%eax
0x8048772 <auth+42>     mov    %eax,(%esp)
0x8048775 <auth+45>     call   0x80485d0 <strnlen@plt>
0x804877a <auth+50>     mov    %eax,-0xc(%ebp)
0x804877d <auth+53>     push   %eax
0x804877e <auth+54>     xor    %eax,%eax
0x8048780 <auth+56>     je     0x8048785 <auth+61>
0x8048782 <auth+58>     add    $0x4,%esp
0x8048785 <auth+61>     pop    %eax
0x8048786 <auth+62>     cmpl   $0x5,-0xc(%ebp)
0x804878a <auth+66>     jg     0x8048796 <auth+78>
0x804878c <auth+68>     mov    $0x1,%eax
0x8048791 <auth+73>     jmp    0x8048877 <auth+303>
0x8048796 <auth+78>     movl   $0x0,0xc(%esp)
0x804879e <auth+86>     movl   $0x1,0x8(%esp)
0x80487a6 <auth+94>     movl   $0x0,0x4(%esp)
0x80487ae <auth+102>    movl   $0x0,(%esp)
0x80487b5 <auth+109>    call   0x80485f0 <ptrace@plt>
0x80487ba <auth+114>    cmp    $0xffffffff,%eax
0x80487bd <auth+117>    jne    0x80487ed <auth+165>
0x80487bf <auth+119>    movl   $0x8048a68,(%esp)
0x80487c6 <auth+126>    call   0x8048590 <puts@plt>
0x80487cb <auth+131>    movl   $0x8048a8c,(%esp)
0x80487d2 <auth+138>    call   0x8048590 <puts@plt>
0x80487d7 <auth+143>    movl   $0x8048ab0,(%esp)
0x80487de <auth+150>    call   0x8048590 <puts@plt>
0x80487e3 <auth+155>    mov    $0x1,%eax
0x80487e8 <auth+160>    jmp    0x8048877 <auth+303>
0x80487ed <auth+165>    mov    0x8(%ebp),%eax
0x80487f0 <auth+168>    add    $0x3,%eax
0x80487f3 <auth+171>    movzbl (%eax),%eax
0x80487f6 <auth+174>    movsbl %al,%eax
0x80487f9 <auth+177>    xor    $0x1337,%eax
0x80487fe <auth+182>    add    $0x5eeded,%eax
0x8048803 <auth+187>    mov    %eax,-0x10(%ebp)
0x8048806 <auth+190>    movl   $0x0,-0x14(%ebp)
0x804880d <auth+197>    jmp    0x804885b <auth+275>
0x804880f <auth+199>    mov    -0x14(%ebp),%eax
0x8048812 <auth+202>    add    0x8(%ebp),%eax
0x8048815 <auth+205>    movzbl (%eax),%eax
0x8048818 <auth+208>    cmp    $0x1f,%al
0x804881a <auth+210>    jg     0x8048823 <auth+219>
0x804881c <auth+212>    mov    $0x1,%eax
0x8048821 <auth+217>    jmp    0x8048877 <auth+303>
0x8048823 <auth+219>    mov    -0x14(%ebp),%eax
0x8048826 <auth+222>    add    0x8(%ebp),%eax
0x8048829 <auth+225>    movzbl (%eax),%eax
0x804882c <auth+228>    movsbl %al,%eax
0x804882f <auth+231>    mov    %eax,%ecx
0x8048831 <auth+233>    xor    -0x10(%ebp),%ecx
0x8048834 <auth+236>    mov    $0x88233b2b,%edx
0x8048839 <auth+241>    mov    %ecx,%eax
0x804883b <auth+243>    mul    %edx
0x804883d <auth+245>    mov    %ecx,%eax
0x804883f <auth+247>    sub    %edx,%eax
0x8048841 <auth+249>    shr    %eax
0x8048843 <auth+251>    add    %edx,%eax
0x8048845 <auth+253>    shr    $0xa,%eax
0x8048848 <auth+256>    imul   $0x539,%eax,%eax
0x804884e <auth+262>    mov    %ecx,%edx
0x8048850 <auth+264>    sub    %eax,%edx
0x8048852 <auth+266>    mov    %edx,%eax
0x8048854 <auth+268>    add    %eax,-0x10(%ebp)
0x8048857 <auth+271>    addl   $0x1,-0x14(%ebp)
0x804885b <auth+275>    mov    -0x14(%ebp),%eax
0x804885e <auth+278>    cmp    -0xc(%ebp),%eax
0x8048861 <auth+281>    jl     0x804880f <auth+199>
0x8048863 <auth+283>    mov    0xc(%ebp),%eax
0x8048866 <auth+286>    cmp    -0x10(%ebp),%eax
0x8048869 <auth+289>    je     0x8048872 <auth+298>
0x804886b <auth+291>    mov    $0x1,%eax
0x8048870 <auth+296>    jmp    0x8048877 <auth+303>
0x8048872 <auth+298>    mov    $0x0,%eax
0x8048877 <auth+303>    leave
0x8048878 <auth+304>    ret
```