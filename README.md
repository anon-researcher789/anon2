# anon2

-Post Quantum-
Clone the repo using Git Clone.

Navigate to the build directory and use Make for Building and Running.

By default the system functionality tests are run. For testing circuits uncomment the Circuit tests in Main.cpp and comment out the system tests, then rebuild.

```
cd build
make clean
make -j
./lattice_test
```
Note: Tests may take a few minutes to run due to heavy mathematical operations.

Note: MacOS and Windows may not run this code so please use a Linux OS or Linux Virtual Machine with Sufficient Resources.
