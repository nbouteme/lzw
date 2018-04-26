Compression LZW
===============

Instructions:

Compiler avec 'make'

Deux exécutables sont créés, lzw et lzwu. (C'est le même avec un nom différent)

./lzw compresse l'entrée standard et écrit dans la sortie standard

'cat fichier | ./lzw > compr'

./lzwu lit un fichier compressé de l'entrée standard et décompresse dans la sortie standard

'cat compr | ./lzwu > decompr'

Et normalement, 'diff fichier decompr' ne dit rien.