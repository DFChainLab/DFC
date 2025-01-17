
# DFCIO - The Most Powerful Infrastructure for Decentralized Applications

Welcome to the DFCIO source code repository! This software enables businesses to rapidly build and deploy high-performance and high-security blockchain-based applications.

Some of the groundbreaking features of DFCIO include:

1. Free Rate Limited Transactions 
1. Low Latency Block confirmation (0.5 seconds)
1. Low-overhead Byzantine Fault Tolerant Finality
1. Designed for optional high-overhead, low-latency BFT finality 
1. Smart contract platform powered by Web Assembly
1. Designed for Sparse Header Light Client Validation
1. Scheduled Recurring Transactions 
1. Time Delay Security
1. Hierarchical Role Based Permissions
1. Support for Biometric Hardware Secured Keys (e.g. Apple Secure Enclave)
1. Designed for Parallel Execution of Context Free Validation Logic
1. Designed for Inter Blockchain Communication 

DFCIO is released under the open source MIT license and is offered “AS IS” without warranty of any kind, express or implied. Any security provided by the DFCIO software depends in part on how it is used, configured, and deployed. DFCIO is built upon many third-party libraries such as Binaryen (Apache License) and WAVM  (BSD 3-clause) which are also provided “AS IS” without warranty of any kind. Without limiting the generality of the foregoing, Block.one makes no representation or guarantee that DFCIO or any third-party libraries will perform as intended or will be free of errors, bugs or faulty code. Both may fail in large or small ways that could completely or partially limit functionality or compromise computer systems. If you use or implement DFCIO, you do so at your own risk. In no event will Block.one be liable to any party for any damages whatsoever, even if it had been advised of the possibility of damage.  

Block.one is neither launching nor operating any initial public blockchains based upon the DFCIO software. This release refers only to version 1.0 of our open source software. We caution those who wish to use blockchains built on DFCIO to carefully vet the companies and organizations launching blockchains based on DFCIO before disclosing any private keys to their derivative software.
 
#### Use DFCIO by Source

##### Build DFCIO
```sh
$ git clone https://github.com/FutureSharesCloud/dfc.git
$ cd dfc
$ ./dfcio_build.sh
```

##### Install DFCIO 
```sh
$ ./dfcio_install.sh
```

##### Uninstall DFCIO
```sh
$ cd dfc
$ ./dfcio_uninstall.sh
```

#### Use DFCIO by Debian Package Manager

##### Build DFCIO deb Package
```sh
$ git clone https://github.com/FutureSharesCloud/dfc.git
$ cd dfc
$ ./dfcio_build.sh
$ cd build/packages/
$ bash ./generate_package.sh [deb/rpm] [dev/beta/dawn/release] <build id[must be a num,Identifies the number of compilations]>
```

##### Install DFCIO deb Package
```sh
$ sudo dpkg -i dfcio_release_1.0.0-1_amd64.deb
```

##### Uninstall DFCIO deb Package
```sh
$ sudo dpkg --list |grep dfcio
$ sudo dpkg -r dfcio-release
```

## Supported Operating Systems
DFCIO currently supports the following operating systems:  
1. Amazon 2017.09 and higher
2. Centos 7
3. Fedora 25 and higher (Fedora 27 recommended)
4. Mint 18
5. Ubuntu 16.04 (Ubuntu 16.10 recommended)
6. Ubuntu 18.04
7. MacOS Darwin 10.12 and higher (MacOS 10.13.x recommended)

## Resources
1. [Website](https://www.fshares.io)

<a name="gettingstarted"></a>
## Getting Started
Instructions detailing the process of getting the software, building it, running a simple test network that produces blocks, account creation and uploading a sample contract to the blockchain can be found on the [Fshares Official Website](https://www.fshares.io).
