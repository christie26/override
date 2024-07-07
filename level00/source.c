int __cdecl main(int argc, const char **argv, const char **envp)
{
  int v4; // [esp+1Ch] [ebp-4h] BYREF

  puts("***********************************");
  puts("* \t     -Level00 -\t\t  *");
  puts("***********************************");
  printf("Password:");
  __isoc99_scanf("%d", &v4);
  if ( v4 == 5276 )
  {
    puts("\nAuthenticated!");
    system("/bin/sh");
    return 0;
  }
  else
  {
    puts("\nInvalid Password!");
    return 1;
  }
}