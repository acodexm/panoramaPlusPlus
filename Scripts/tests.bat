START /W ImageStitching.exe pictures --features orb --compose_megapix 1.2 --try_cuda yes --save_graph adv_graph.txt --advanced --debug
ECHO finished ADVANCED IMAGE STITCHING

START /W ImageStitching.exe pictures --custom --cuda --debug
ECHO finished CUSTOM IMAGE STITCHING

START /W ImageStitching.exe pictures --compressed --debug
ECHO finished SIMPLE IMAGE STITCHING

START /W Comparator.exe result.jpg advResult.jpg --prefix adv --debug
ECHO finished Comare 1

START /W Comparator.exe result.jpg simpleResult.jpg --prefix simple --debug
ECHO finished Comare 2

START /W Comparator.exe advResult.jpg simpleResult.jpg --prefix adv_simple --debug
ECHO finished Comare 3
