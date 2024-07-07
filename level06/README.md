# level 6 \<bitwise and arithmetic manipulations>

## Overview
The binary gets Login as input and performs several bitwise and arithmetic manipulations to generate a serial number. They block the use of gdb, so we have to create our own program to calculate the serial number.

## Code
In the binary, they get input with `fgets` and use `scanf`. And call function `auth` with two arguments which are login and serial number.

#### functions
`fgets` reads 32 bytes from `stdin` to `0xffffd63c`.
```c
fgets(0xffffd63c, 0x20, stdin);
```
`scanf` reads from `stdin` to `0xffffd638` as unsigned int format.
```c
scanf(&0xffffd638, "%u");
```
`strcspn` gets len of string before "\n" occurs.  = length of string
```c
strcspn(0xffffd63c, "\n");
```

#### variables
`0x8(%ebp)` = `Login` = `0xffffd63c`\
`-0xc(%ebp)` = `v5` = result of `strnlen(a1, 32)`\
`-0x10(%ebp)` = `v4`
`-0x14(%ebp)` = `i`

In the `auth` function, it performs many bitwise and arithmetic manipulations. We need to replicate these operations to get the serial number based on our input.

## Payload
```python
# Get login input from the user
login = input("Enter login: ")

if len(login) < 6:
    raise ValueError("Login must be at least 6 characters long")

char_value = ord(login[3])  # Get ASCII value of the fourth character
v4 = (char_value ^ 0x1337) + 0x5eeded

# Loop through each character in the string and perform the operations
for char in login:
    char_value = ord(char)  # Get ASCII value of the character
    v4 += (v4 ^ char_value) % 0x539

print(f'Serial: {v4}')
```

```
python3 calculateSerial.py
Enter login: hihihi
Serial: 6232825

./level06
***********************************
*		level06		  *
***********************************
-> Enter Login: hihihi
***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
-> Enter Serial: 6232825
Authenticated!
$ cat /home/users/level07/.pass
GbcPDRgsFK77LNnnuh7QyFYA2942Gp8yKj9KrWD8
```

## Flag
```
$ cat /home/users/level07/.pass
GbcPDRgsFK77LNnnuh7QyFYA2942Gp8yKj9KrWD8
```