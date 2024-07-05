# Level03 Decryption

## Overview

The binary is a basic program that prompts for a `password`, and if we enter the correct password, the program executes `system("/bin/sh")` as the level04 user. Decompiling the binary can help find the password that directs the flow of the program to that system call.

### Decrypt Function

As we see in the decrypt function, a **bitwise XOR** operation is being applied to each character in the string `enc` with the value of `X`.

The XOR operation is reversible, applying the same XOR operation again will return the original value.
```C
int decrypt(char X)
{
    strcpy(enc, "Q}|u`sfg~sf{}|a3");
    len = strlen(enc);

    for ( i = 0; i < len; ++i )
    enc[i] ^= X;          // XOR

    if ( !strcmp(enc, "Congratulations!") )
        return system("/bin/sh");
    else
        return puts("\nInvalid Password");
}
```

As we can see, the string:
```
"Q}|u\`sfg~sf{}|a3"
```
Must be able to convert to:
```
Congratulations!
```
When applying an **XOR** operation, we just need to find out which number will [decode the string](https://cyberchef.org/#recipe=XOR(%7B'option':'Hex','string':'12'%7D,'Standard',false)).

After decoding it, we find out that **XOR** each character with the value **18**, results in the `Congratulations!` string.

### Test Function

To be able to control which input we send to the `decrypt()` function, we need to make the operation `322424845 - a1` result be inside the range `(1, 9)` or `(16, 21)`, **and** be `18`, to make the `decrypt()` execute the shell.

$$
\begin{aligned}
\text{a1} &= \text{322424845} - \text{18} \\
\text{a1} &= \text{322424827} \\
\end{aligned}
$$

```C
int test(int a1, 322424845)
{
    switch ( 322424845 - a1 )
    {
        // [1, 9] + [16, 21]
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
            return decrypt(322424845 - a1);
            break;
        default:
            return decrypt(rand());
            break;
    }
}
```

## Payload
This one line command is able to run a shell as a level02 user.
```bash
cat <(echo 322424827) - | ./level03
```

## Flag
```bash
cat /home/users/level04/.pass
kgv3tkEb9h2mLkRsPkXRfc2mHbjMxQzvb2FrgKkf
```