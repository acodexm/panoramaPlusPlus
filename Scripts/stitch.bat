START /W ImageStitching.exe pictures --features orb --compose_megapix 1.2 --advanced --debug
ECHO finished ADVANCED IMAGE STITCHING

START /W ImageStitching.exe pictures --custom --debug
ECHO finished CUSTOM IMAGE STITCHING

START /W ImageStitching.exe pictures --compressed --debug
ECHO finished SIMPLE IMAGE STITCHING
