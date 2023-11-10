### Program implementujący problem Pięciu Filozofów.
### Do pobrania: 
- Filozofowie.cpp

Proszę wykonać test pracy programu i ustalić co powoduje blokadę.

Proszę uaktywnić fragment kodu:
```cpp
if ( widelec1 > widelec2 ) {
widelec1 = widelec2;
widelec2 = numerFilozofa;
}
```
za co on odpowiada i co zmienia w zachowaniu filozofów?

> **UWAGA** przed uruchomieniem nowszej wersji proszę posprzątać w systemie za pomocą pary poleceń ipcs i ipcrm.

### Program generujący liczby losowe i je uśredniający.
Komunikacja za pomocą pamięci współdzielonej, sygnałów i kolejki komunikatów.

### Do pobrania:
- Program usredniajacy
- Program generujacy
- common.h
- [Instrukcja](instrukcja.txt)

Program liczy srednie z danych losowych umieszczonych w pamieci wspoldzielonej.

### Zadanie zostalo rozdzielone na dwa osobne programy:
- generator -> zapisuje dane w pamieci wspoldzielonej
- usredniacz -> odczytuje dane z pamieci wspoldzielonej, liczy srednia, wypisuje ja na ekrenie.
