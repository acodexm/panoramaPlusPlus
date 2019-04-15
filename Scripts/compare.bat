START /W Comparator.exe result.jpg advResult.jpg --prefix adv --debug
ECHO finished Comare 1

START /W Comparator.exe result.jpg simpleResult.jpg --prefix simple --debug
ECHO finished Comare 2

START /W Comparator.exe advResult.jpg simpleResult.jpg --prefix adv_simple --debug
ECHO finished Comare 3
