Překlad programu pro použití z příkazového řádku se 
provádí pomocí cmake (lze přeložit na serveru Merlin):

cd 2DFFT
./build2DFFT.sh cmd


Překlad programu s uživatelským rozhraním:

Pro překlad programu s uživatelským rozhraním je potřebná 
aktuální instalace prostředí QTCreator 5.10.1 včetně 
patřičné aktuální verze qmake a s balíčkem QCharts. 
V případě, že se instalace nástroje qmake nachází 
v jiném umístění, než je využito v kompilačním skriptu 
build2DFFT.sh, je potřeba skript před kompilací upravit.

cd 2DFFT
./build2DFFT.sh gui


Překlad obou verzí programu:

cd 2DFFT
./build2DFFT.sh 


Spuštění jednoduchého měření definovaného v souboru ../tests/run.txt
na filtrech definovaných v souboru ../tests/filters.txt:

cd build
./2dfft -r ../tests/run.txt -f ../tests/filters.txt -o ../tests/statistics.txt
