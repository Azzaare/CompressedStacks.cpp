Commands to use with cmake in the current folder

# For linking new files (or unlink removed files)
cmake ..

# To create debugger executable
cmake -DCMAKE_BUILD_TYPE=Debug ..

# For compilation (normal and debug)
make

# To clean
make clean

# To run gdb
gdb
file ./smartstack
run
