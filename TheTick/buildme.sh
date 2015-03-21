#!/bin/sh

DIRNAME=$(dirname $0)
MACHINE=$(uname -m)
if [ $MACHINE == "BePC" ]
then
	CPU=x86
else
	CPU=ppc
fi

cd $DIRNAME

#alert $DIRNAME

cd src/addon-sources/BOSC/
make
mv obj.$CPU/BeOSCentral ../../../add-ons

cd ../BeNEWS/
make
mv obj.$CPU/BeNEWS ../../../add-ons

cd ../Slashdot/
make
mv obj.$CPU/Slashdot ../../../add-ons

cd ../Segfault/
make
mv obj.$CPU/Segfault ../../../add-ons

cd ../../
make
mv obj.$CPU/TheTick ../


# uncomment this if you want to do some debugging ...
#if [ -f obj.$CPU/TheTick.xMAP ]
#then
#	mv obj.$CPU/TheTick.xMAP ../
#fi

echo "All done!"