#! /bin/bash

NAME=$1
DFC_PREFIX=${PREFIX}/${SUBPREFIX}
mkdir -p ${PREFIX}/bin/
#mkdir -p ${PREFIX}/lib/cmake/${PROJECT}
mkdir -p ${DFC_PREFIX}/bin
#mkdir -p ${DFC_PREFIX}/licenses/dfcio
#mkdir -p ${DFC_PREFIX}/include
#mkdir -p ${DFC_PREFIX}/lib/cmake/${PROJECT}
#mkdir -p ${DFC_PREFIX}/cmake
#mkdir -p ${DFC_PREFIX}/scripts

# install binaries 
cp -R ${BUILD_DIR}/bin/* ${DFC_PREFIX}/bin

# install licenses
#cp -R ${BUILD_DIR}/licenses/dfcio/* ${DFC_PREFIX}/licenses

# install libraries
#cp -R ${BUILD_DIR}/lib/* ${DFC_PREFIX}/lib

# install cmake modules
#sed "s/_PREFIX_/\/${SPREFIX}/g" ${BUILD_DIR}/modules/FscioTesterPackage.cmake &> ${DFC_PREFIX}/lib/cmake/${PROJECT}/FscioTester.cmake
#sed "s/_PREFIX_/\/${SPREFIX}\/${SSUBPREFIX}/g" ${BUILD_DIR}/modules/${PROJECT}-config.cmake.package &> ${DFC_PREFIX}/lib/cmake/${PROJECT}/${PROJECT}-config.cmake

# install includes
#cp -R ${BUILD_DIR}/include/* ${DFC_PREFIX}/include

# make symlinks
#pushd ${PREFIX}/lib/cmake/${PROJECT} &> /dev/null
#ln -sf ../../../${SUBPREFIX}/lib/cmake/${PROJECT}/${PROJECT}-config.cmake ${PROJECT}-config.cmake
#ln -sf ../../../${SUBPREFIX}/lib/cmake/${PROJECT}/FscioTester.cmake FscioTester.cmake
#popd &> /dev/null

pushd ${PREFIX}/bin &> /dev/null
for f in `ls ${BUILD_DIR}/bin/`; do
   bn=$(basename $f)
   ln -sf ../${SUBPREFIX}/bin/$bn $bn
done
popd &> /dev/null

tar -cvzf $NAME ./${PREFIX}/*
rm -r ${PREFIX}
