int __fastcall main(int argc, const char **argv, const char **envp)
{
  char s2[96]; // [rsp+10h] [rbp-110h] BYREF
  int v5; // [rsp+70h] [rbp-B0h]
  char ptr[48]; // [rsp+80h] [rbp-A0h] BYREF
  char s[96]; // [rsp+B0h] [rbp-70h] BYREF
  int v8; // [rsp+110h] [rbp-10h]
  int v9; // [rsp+114h] [rbp-Ch]
  FILE *stream; // [rsp+118h] [rbp-8h]

  memset(s, 0, sizeof(s));
  v8 = 0;
  memset(ptr, 0, 41);
  memset(s2, 0, sizeof(s2));
  v5 = 0;
  stream = 0LL;
  v9 = 0;
  stream = fopen("/home/users/level03/.pass", "r");
  if ( !stream )
  {
    fwrite("ERROR: failed to open password file\n", 1uLL, 0x24uLL, stderr);
    exit(1);
  }
  v9 = fread(ptr, 1uLL, 0x29uLL, stream);
  ptr[strcspn(ptr, "\n")] = 0;
  if ( v9 != 41 )
  {
    fwrite("ERROR: failed to read password file\n", 1uLL, 0x24uLL, stderr);
    fwrite("ERROR: failed to read password file\n", 1uLL, 0x24uLL, stderr);
    exit(1);
  }
  fclose(stream);
  puts("===== [ Secure Access System v1.0 ] =====");
  puts("/***************************************\\");
  puts("| You must login to access this system. |");
  puts("\\**************************************/");
  printf("--[ Username: ");
  fgets(s, 100, stdin);
  s[strcspn(s, "\n")] = 0;
  printf("--[ Password: ");
  fgets(s2, 100, stdin);
  s2[strcspn(s2, "\n")] = 0;
  puts("*****************************************");
  if ( strncmp(ptr, s2, 0x29uLL) )
  {
    printf(s);
    puts(" does not have access!");
    exit(1);
  }
  printf("Greetings, %s!\n", s);
  system("/bin/sh");
  return 0;
}