# Pipe Demo
This project is a demonstration of creating a pipe to allow inter-process communication on a *nix system.  The example is built up sequentially in the history, so checkout the initial commit first and run

    make

to compile.  The executable will be called `main` and will stop itself (with `SIGSTOP`) after interesting changes. Watch that is going on with its file descriptors by running

    watch -n .5 ls -l /proc/$PID/fd

where `$PID` is the PID of the process (printed to /standard error/ immediately upon start) in another window (or just run the `ls` command in the same terminal after each time the program has stopped).
