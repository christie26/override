# Level03 Decryption

## Overview

The binary is a basic program that prompts for a `password`, and if the correct password is entered, the program executes `system("/bin/sh")` as the `level04` user. Decompiling the binary can help find the password that directs the flow of the program to that system call.

## Decrypt Function

In the `decrypt` function, a **bitwise XOR** operation is applied to each character in the string `enc` with the value of `X`. The XOR operation is reversible, meaning applying the same XOR operation again will return the original value.

```c
int decrypt(char X)
{
    strcpy(enc, "Q}|u`sfg~sf{}|a3");
    len = strlen(enc);

    for (i = 0; i < len; ++i)
        enc[i] ^= X;          // XOR

    if (!strcmp(enc, "Congratulations!"))
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

Using an **XOR** value of **18** will decode the string to `"Congratulations!"`.

## Test Function

To control the input sent to the `decrypt()` function, the result of `322424845 - a1` must be within the range `(1, 9)` or `(16, 21)`, and specifically `18` to trigger the execution of the shell in `decrypt()`.

\[
\begin{aligned}
a1 &= 322424845 - 18 \\
a1 &= 322424827 \\
\end{aligned}
\]

```c
int test(int a1, 322424845)
{
    switch (322424845 - a1)
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

This one-line command can be used to run a shell as the `level02` user:

```bash
cat <(echo 322424827) - | ./level03
```

## Flag

To obtain the flag, use the following command:

```bash
cat /home/users/level04/.pass
kgv3tkEb9h2mLkRsPkXRfc2mHbjMxQzvb2FrgKkf
```
