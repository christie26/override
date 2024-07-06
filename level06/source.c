BOOL __cdecl auth(int a1, int a2)
{
  int i; // [esp+14h] [ebp-14h]
  int v4; // [esp+18h] [ebp-10h]
  int v5; // [esp+1Ch] [ebp-Ch]

  *(_BYTE *)(a1 + strcspn(a1, &unk_8048A63)) = 0;
  v5 = strnlen(a1, 32);
  if ( v5 <= 5 )
    return 1;
  if ( ptrace(0, 0, 1, 0) == -1 )
  {
    puts("\x1B[32m.---------------------------.");
    puts("\x1B[31m| !! TAMPERING DETECTED !!  |");
    puts("\x1B[32m'---------------------------'");
    return 1;
  }
  else
  {
    v4 = (*(char *)(a1 + 3) ^ 0x1337) + 6221293;
    for ( i = 0; i < v5; ++i )
    {
      if ( *(char *)(a1 + i) <= 31 )
        return 1;
      v4 += (v4 ^ (unsigned int)*(char *)(a1 + i)) % 0x539;
    }
    return a2 != v4;
  }
}

int __cdecl main(int argc, const char **argv, const char **envp)
{
  int v4; // [esp+2Ch] [ebp-24h] BYREF
  _DWORD v5[8]; // [esp+30h] [ebp-20h] BYREF

  v5[7] = __readgsdword(0x14u);
  puts("***********************************");
  puts("*\t\tlevel06\t\t  *");
  puts("***********************************");
  printf("-> Enter Login: ");
  fgets(v5, 32, stdin);
  puts("***********************************");
  puts("***** NEW ACCOUNT DETECTED ********");
  puts("***********************************");
  printf("-> Enter Serial: ");
  __isoc99_scanf(&unk_8048A60, &v4);
  if ( auth(v5, v4) )
    return 1;
  puts("Authenticated!");
  system("/bin/sh");
  return 0;
}