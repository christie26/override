int main(int argc, const char **argv, const char **envp)
{
  int v4; // [esp+1Ch] [ebp-9Ch] BYREF
  char v5[128]; // [esp+20h] [ebp-98h] BYREF
  int v6; // [esp+A0h] [ebp-18h]
  int v7; // [esp+A4h] [ebp-14h]
  int v8; // [esp+A8h] [ebp-10h]
  int v9; // [esp+ACh] [ebp-Ch]

  v9 = fork();
  memset(v5, 0, sizeof(v5));
  v8 = 0;
  v4 = 0;
  if ( v9 ) // parent
  {
    do
    {
      wait(&v4);
      v6 = v4;
      if ( (v6 & 0x7F) == 0 || (v7 = v4, (char)((v7 & 0x7F) + 1) >> 1 > 0) )
      {
        puts("child is exiting...");
        return 0;
      }
      v8 = ptrace(3, v9, 44, 0);
    }
    while ( v8 != 11 );
    puts("no exec() for you");
    kill(v9, 9);
  }
  else // child
  {
    prctl(1, 1);
    ptrace(0, 0, 0, 0);
    puts("Give me some shellcode, k");
    gets(v5);
  }
  return 0;
}