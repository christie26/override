BOOL verify_user_name()
{
  puts("verifying username....\n");
  return memcmp(&a_user_name, "dat_wil", 7u) != 0;
}

BOOL __cdecl verify_user_pass(const void *a1)
{
  return memcmp(a1, "admin", 5u) != 0;
}

int __cdecl main(int argc, const char **argv, const char **envp)
{
  _BYTE v4[64]; // [esp+1Ch] [ebp-4Ch] BYREF
  int v5; // [esp+5Ch] [ebp-Ch]

  memset(v4, 0, sizeof(v4));
  v5 = 0;
  puts("********* ADMIN LOGIN PROMPT *********");
  printf("Enter Username: ");
  fgets(&a_user_name, 256, stdin);
  v5 = verify_user_name();
  if ( v5 )
  {
    puts("nope, incorrect username...\n");
  }
  else
  {
    puts("Enter Password: ");
    fgets(v4, 100, stdin);
    v5 = verify_user_pass(v4);
    puts("nope, incorrect password...\n");
  }
  return 1;
}