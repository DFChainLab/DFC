#! /bin/bash

binaries=(cldfc
          dfcio-abigen
          dfcio-launcher
          dfcio-s2wasm
          dfcio-wast2wasm
          dfciocpp
          kdfcd
          noddfc
          dfcio-applesdemo)

if [ -d "/usr/local/dfcio" ]; then
   printf "\tDo you wish to remove this install? (requires sudo)\n"
   select yn in "Yes" "No"; do
      case $yn in
         [Yy]* )
            if [ "$(id -u)" -ne 0 ]; then
               printf "\n\tThis requires sudo, please run ./dfcio_uninstall.sh with sudo\n\n"
               exit -1
            fi

            pushd /usr/local &> /dev/null
            rm -rf dfcio
            pushd bin &> /dev/null
            for binary in ${binaries[@]}; do
               rm ${binary}
            done
            # Handle cleanup of directories created from installation
            if [ "$1" == "--full" ]; then
               if [ -d ~/Library/Application\ Support/dfcio ]; then rm -rf ~/Library/Application\ Support/dfcio; fi # Mac OS
               if [ -d ~/.local/share/dfcio ]; then rm -rf ~/.local/share/dfcio; fi # Linux
            fi
            popd &> /dev/null
            break;;
         [Nn]* )
            printf "\tAborting uninstall\n\n"
            exit -1;;
      esac
   done
fi
