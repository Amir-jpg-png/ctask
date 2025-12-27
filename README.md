# CTask

Ctask is a CLI Tool for quickly generating tasks.

The way it works is that it creates a .tasks.json file in the directory you are currently working on.
In this file all your tasks as well as their status gets saved.

To install it you need to have cmake installed afterwards you can run.

```bash
curl -S https://raw.githubusercontent.com/Amir-jpg-png/ctask/refs/heads/main/install.sh | bash
```

You are also invited to read the code of our install script either on our github or by issuing the same command without piping into bash, as piping into bash does have some controversy around it.

If your current directory is not a ctask directory (A directory where the .tasks.json file exists) you will get the following message:

```bash
fatal: not a ctask repository
 do you want to init it (y/n):
```

To find out more about the usage of ctask use 

```bash
ctask --help
```
