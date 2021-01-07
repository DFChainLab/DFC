# Bios Boot Tutorial

The `bios-boot-tutorial.py` script simulates the DFCIO bios boot sequence.

``Prerequisites``:

1. Python 3.x
2. CMake
3. git

``Steps``:

1. Install dfcio binaries by following the steps outlined in below tutorial
[Install dfcio binaries](https://github.com/FutureSharesCloud/dfc#mac-os-x-brew-install)

2. Install dfcio.cdt binaries by following the steps outlined in below tutorial
[Install dfcio.cdt binaries](https://github.com/FutureSharesCloud/dfcio.cdt#binary-releases)

3. Compile dfcio.contracts

```bash
$ cd ~
$ git clone https://github.com/FutureSharesCloud/dfcio.contracts.git
$ cd ./dfcio.contracts/
$ ./build.sh
$ pwd

```

4. Make note of the directory where the contracts were compiled
The last command in the previous step printed on the bash console the contracts' directory, make note of it, we'll reference it from now on as `DFCIO_CONTRACTS_DIRECTORY`

5. Launch the `bios-boot-tutorial.py` script
Minimal command line to launch the script below, make sure you replace `DFCIO_CONTRACTS_DIRECTORY` with actual directory

```bash
$ cd ~
$ git clone https://github.com/FutureSharesCloud/dfc.git
$ cd ./dfc/tutorials/bios-boot-tutorial/
$ python3 bios-boot-tutorial.py --cldfc="cldfc --wallet-url http://127.0.0.1:6666 " --noddfc=noddfc --kdfcd=kdfcd --contracts-dir="/DFCIO_CONTRACTS_DIRECTORY/" -a

```

See [DFCIO Documentation Wiki: Tutorial - Bios Boot](https://github.com/FutureSharesCloud/dfc/wiki/Tutorial-Bios-Boot-Sequence) for additional information.
