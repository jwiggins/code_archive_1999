#!/bin/sh

DIRNAME=$(dirname $0)
RESFILE="$DIRNAME/JellikitStrings.rsrc"
COPY_RESFILE="$DIRNAME/Resource-copy.rsrc"
XRES="xres -o $RESFILE"
EXECUTABLE="$DIRNAME/Jellikit"
ELLIPSIS='…'

# make a copy of old non-string resources
xres -o $COPY_RESFILE -d CSTR $EXECUTABLE

#erase all the executable's resources (and DWARF2 stuff as a consequence)
strip $EXECUTABLE

# dump a wad of strings into new resource file
$XRES -a CSTR:0 -s "Shit Yeah"
$XRES -a CSTR:1 -s "Fuck No" $RESFILE
$XRES -a CSTR:2 -s "File" $RESFILE
$XRES -a CSTR:3 -s "Newborn fuckin' File" $RESFILE
$XRES -a CSTR:4 -s 'Open another fuckin File'$ELLIPSIS $RESFILE
$XRES -a CSTR:5 -s "Get Rid of this fuckin window" $RESFILE
$XRES -a CSTR:6 -s "Save before this fuckin app crashes" $RESFILE
$XRES -a CSTR:7 -s 'Save, but give it a new fuckin name'$ELLIPSIS $RESFILE
$XRES -a CSTR:8 -s "Explain this shitty app" $RESFILE
$XRES -a CSTR:9 -s "Quit, damn it!" $RESFILE
$XRES -a CSTR:10 -s "Fuck wit'" $RESFILE
$XRES -a CSTR:11 -s "Add another fuckin" $RESFILE
$XRES -a CSTR:12 -s "Remove this fuckin" $RESFILE
$XRES -a CSTR:13 -s "Name this fucker" $RESFILE
$XRES -a CSTR:14 -s "Type" $RESFILE
$XRES -a CSTR:15 -s "string" $RESFILE
$XRES -a CSTR:16 -s "int64" $RESFILE
$XRES -a CSTR:17 -s "int32" $RESFILE
$XRES -a CSTR:18 -s "double" $RESFILE
$XRES -a CSTR:19 -s "float" $RESFILE
$XRES -a CSTR:20 -s "rect" $RESFILE
$XRES -a CSTR:21 -s "bool" $RESFILE
$XRES -a CSTR:22 -s "raw" $RESFILE
$XRES -a CSTR:23 -s "You dipshit! There's already an attribute with that name for this file. You want me to fucking replace it with the one yer moving dumbass?" $RESFILE
$XRES -a CSTR:24 -s "Released way too early"  $RESFILE
$XRES -a CSTR:25 -s "The fucking %s manager failed to initialize correctly.\nThat pretty much makes this program non-functional.\nI will quit now."  $RESFILE
$XRES -a CSTR:26 -s "Attribute" $RESFILE
$XRES -a CSTR:27 -s "add-on"  $RESFILE
$XRES -a CSTR:28 -s "Tag"  $RESFILE
$XRES -a CSTR:29 -s "mime string"  $RESFILE
$XRES -a CSTR:30 -s "Defined-muthafucker"  $RESFILE
$XRES -a CSTR:31 -s "other"  $RESFILE
$XRES -a CSTR:32 -s "Save the fucking changes to '%s'?"  $RESFILE
$XRES -a CSTR:33 -s "Hell no"  $RESFILE


# dump string resource file and old resources into executable file
xres -o $EXECUTABLE $RESFILE $COPY_RESFILE

# clean up
rm $COPY_RESFILE $RESFILE
