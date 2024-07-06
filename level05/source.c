int __cdecl __noreturn main(int argc, const char **argv, const char **envp)
{
  char s[100]; // [esp+28h] [ebp-70h] BYREF
  unsigned int i; // [esp+8Ch] [ebp-Ch]

  i = 0;
  fgets(s, 100, stdin);
  for ( i = 0; i < strlen(s); ++i )
  {
    if ( s[i] > 64 && s[i] <= 90 )  // If uppercase letter
      s[i] ^= 0x20u;                // XOR with 0x20 (32) to convert to lowercase
  }
  printf(s);
  exit(0);
}
