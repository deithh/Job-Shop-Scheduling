kompilacja:
g++ solution.cpp -o main

użycie:
./main [format] [ścieżka do instancji] [ilość zadań do uwzględnienia] [opcjonalny K] [opcjonalny t]

parametry:
format - O dla beasley i T dla Tailarda
ścieżka do instancji - relatywna lub bezwzględna ścieżka do pliku .txt zawierającego instancję problemu
ilość zadań do uwzględnienia - -1 jeśli program ma odczytać wszystkie zadania albo ilość zadań do odczytania z instancji

parametry opcjonalne (jeżeli korzystamy z parametrów opcjonalnych wymagane jest podanie obydwu z nich):
K - parametr kontrolujący losowość algorytmu domyślnie ustawiony na 5
t - parametr kontrolujący czas pracy algorytmu nie uwzględniając operacji I/O podany w sekundach (tylko liczby naturalne) domyślnie ustawiony na 5

wyjście: 
wypisanie najlepszego znalezionego rozwiązania na konsolę (stdout)

przykłady użycia:
./main O fs1.txt -1
program odczyta instancje fs1.txt w formacie beasley w całośći i po 5 sekundach pracy wypisze rozwiązanie na konsolę

./main O fs1.txt 3 4 6
program odczyta 3 pierwsze zadania instancji fs1.txt w formacie beasley i po 6 sekundach pracy wypisze rozwiązanie na konsolę, program 
będzię pracował z parametrem losowości = 4;