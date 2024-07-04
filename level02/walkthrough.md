# level02


## breakpoint
```
<main+453>     callq  0x4006f0 <fgets@plt>
```
Oh no... we can't really use `gdb`..

`ERROR: failed to open password file`

```C
 if ( strncmp(ptr, s2, 0x29uLL) )
  {
    printf(s);
    puts(" does not have access!");
    exit(1);
  }
```
they compare 0x29 = 41 `ptr` and `s2`(input of password)


## track ptr
```c
  char ptr[48];
  memset(ptr, 0, 41);
  v9 = fread(ptr, 1uLL, 0x29uLL, stream); // they read 41 times, each time 1 byte.
  ptr[strcspn(ptr, "\n")] = 0; // replace first '\n' with '0'
```
why they don't use whole 48 bytes and keep 7 bytes empty? 


```c
  char s2[96]; // [rsp+10h] [rbp-110h] BYREF
  // 0x520 = 1312
  char ptr[48]; // [rsp+80h] [rbp-A0h] BYREF
  // 0x4f0 = 1264
  char s[96]; // [rsp+B0h] [rbp-70h] BYREF
  // 0x480 = 1152
  int v5; // [rsp+70h] [rbp-B0h]
  int v8; // [rsp+110h] [rbp-10h]
  int v9; // [rsp+114h] [rbp-Ch]
  FILE *stream; // [rsp+118h] [rbp-8h]
```
offset of `s2` and `ptr` = 1312 - 1264 = 48 \
offset of `ptr` and `s` = 1264 - 1152 = 112

## printf format attacking

`0x38 25 20 70 38 25 20 70` = 8 & sp p 8\
`0x7fffffffe4a0` = address of password we put\
`%8$s`

In the end, I read token directly from stack by printing stack memory a lot.
## asm
```
0x400814 <main>         push   %rbp
0x400815 <main+1>       mov    %rsp,%rbp
0x400818 <main+4>       sub    $0x120,%rsp
0x40081f <main+11>      mov    %edi,-0x114(%rbp)
0x400825 <main+17>      mov    %rsi,-0x120(%rbp)
0x40082c <main+24>      lea    -0x70(%rbp),%rdx
0x400830 <main+28>      mov    $0x0,%eax
0x400835 <main+33>      mov    $0xc,%ecx
0x40083a <main+38>      mov    %rdx,%rdi
0x40083d <main+41>      rep stos %rax,%es:(%rdi)
0x400840 <main+44>      mov    %rdi,%rdx
0x400843 <main+47>      mov    %eax,(%rdx)
0x400845 <main+49>      add    $0x4,%rdx
0x400849 <main+53>      lea    -0xa0(%rbp),%rdx
0x400850 <main+60>      mov    $0x0,%eax
0x400855 <main+65>      mov    $0x5,%ecx
0x40085a <main+70>      mov    %rdx,%rdi
0x40085d <main+73>      rep stos %rax,%es:(%rdi)
0x400860 <main+76>      mov    %rdi,%rdx
0x400863 <main+79>      mov    %al,(%rdx)
0x400865 <main+81>      add    $0x1,%rdx
0x400869 <main+85>      lea    -0x110(%rbp),%rdx
0x400870 <main+92>      mov    $0x0,%eax
0x400875 <main+97>      mov    $0xc,%ecx
0x40087a <main+102>     mov    %rdx,%rdi
0x40087d <main+105>     rep stos %rax,%es:(%rdi)
0x400880 <main+108>     mov    %rdi,%rdx
0x400883 <main+111>     mov    %eax,(%rdx)
0x400885 <main+113>     add    $0x4,%rdx
0x400889 <main+117>     movq   $0x0,-0x8(%rbp)
0x400891 <main+125>     movl   $0x0,-0xc(%rbp)
0x400898 <main+132>     mov    $0x400bb0,%edx
0x40089d <main+137>     mov    $0x400bb2,%eax
0x4008a2 <main+142>     mov    %rdx,%rsi
0x4008a5 <main+145>     mov    %rax,%rdi
0x4008a8 <main+148>     callq  0x400700 <fopen@plt>
```