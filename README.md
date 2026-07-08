# ft_printf - Zaawansowany Zestaw Testowy (Test Suite) 🚀

Rygorystyczny i zautomatyzowany zestaw testowy dla projektu `ft_printf` (42 School). Program porównuje w czasie rzeczywistym wyjście (`stdout`) oraz wartość zwracaną (`return value`) Twojej funkcji `ft_printf` z oryginalną biblioteczną funkcją `printf`.

---

## 📌 Główne Funkcjonalności

* **Przechwytywanie strumienia (Pipe Redirection):** Wykorzystuje funkcje systemowe `pipe()`, `dup()` i `dup2()` do bezkolizyjnego przechwytywania wyjścia standardowego do buforów o pojemności do 20 000 bajtów.
* **Pełna weryfikacja specyfikatorów:** Kompletny zestaw testów dla konwersji: `%c`, `%s`, `%p`, `%d`, `%i`, `%u`, `%x`, `%X` oraz `%%`.
* **Testy skrajne (Edge Cases & Stress Tests):**
    * Wskaźniki i stringi równe `NULL`.
    * Obsługa znaku *null byte* (`\0`) w środku i na końcu formatu.
    * Skrajne wartości liczbowe (`INT_MIN`, `INT_MAX`, `UINT_MAX`, `ULONG_MAX`).
    * Ogromne bufory danych (ciągi znaków powyżej 5000 bajtów).
    * Błędne i niekompletne ciągi formatujące (np. samotny `%` na końcu, nieznane specyfikatory jak `%k`).
* **Wykrywanie wycieków pamięci:** Kompilacja z flagą `-fsanitize=address` pozwala na natychmiastowe wykrycie błędów naruszenia ochrony pamięci (segfault, heap-buffer-overflow).

---

## 🛠️ Wymagania i Struktura Projektu

Aby testy działały poprawnie, zachowaj następującą strukturę katalogów:

```text
.
├── Makefile
├── test.c
├── main.c
└── ft_printf/
    ├── ft_printf.h
    └── libftprintf.a  # Skompilowana biblioteka ft_printf
```

---

## 🚀 Instrukcja Użycia

Projekt zawiera dedykowany plik `Makefile` automatyzujący proces budowania i uruchamiania.

### Podstawowe komendy:

* **Skompilowanie programu testowego:**
    ```bash
    make
    ```
* **Skompilowanie i natychmiastowe uruchomienie testów:**
    ```bash
    make run
    ```
* **Wyczyszczenie plików obiektowych (.o):**
    ```bash
    make clean
    ```
* **Pełne czyszczenie (pliki .o oraz plik wykonywalny):**
    ```bash
    make fclean
    ```
* **Re-kompilacja (czyszczenie i ponowne budowanie):**
    ```bash
    make re
    ```
* **Pełny cykl (Fclean -> All -> Clean):**
    ```bash
    make rec
    ```

---

## ⚙️ Jak działa makro `ASSERT_PRINTF`?

Każdy test jest uruchamiany wewnątrz dedykowanego makra, które wykonuje sekwencję kroków:
1. Klonuje aktualny uchwyt wyjścia standardowego (`STDOUT_FILENO`).
2. Tworzy potok (`pipe`) i przekierowuje do niego wyjście standardowego `printf`, zapisując wynik w buforze kontrolnym.
3. Powtarza operację dla `ft_printf`, zapisując wynik w buforze testowym.
4. Porównuje długości zwróconych wartości oraz zawartość pamięci buforów za pomocą `memcmp()`.
5. W przypadku rozbieżności wypisuje szczegółowy raport o błędzie i zatrzymuje działanie programu funkcją `assert()`.
