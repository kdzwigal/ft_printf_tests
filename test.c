/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdzwigal <kdzwigal@student.42warsaw.p      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 14:31:10 by kdzwigal          #+#    #+#             */
/*   Updated: 2026/07/07 14:31:13 by kdzwigal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <limits.h>
#include <stdint.h>

// Deklaracja Twojej funkcji
int ft_printf(const char *str, ...);

/* ************************************************************************** */
/* MAKRO TESTUJĄCE: Przechwytuje stdout i porównuje wyniki oraz return value  */
/* ************************************************************************** */
#define ASSERT_PRINTF(...) do { \
    int pipe_fd[2]; \
    char buf_std[20000] = {0}; /* Zwiększamy do 20 tysięcy bajtów */ \
    char buf_ft[20000] = {0};  \
    int ret_std, ret_ft; \
    int saved_stdout = dup(STDOUT_FILENO); \
    \
    /* 1. Przechwycenie wyjścia oryginalnego printf */ \
    pipe(pipe_fd); \
    dup2(pipe_fd[1], STDOUT_FILENO); \
    close(pipe_fd[1]); \
    ret_std = printf(__VA_ARGS__); \
    fflush(stdout); \
    dup2(saved_stdout, STDOUT_FILENO); /* Przywrócenie stdout (zamyka potok) */ \
    read(pipe_fd[0], buf_std, sizeof(buf_std) - 1); \
    close(pipe_fd[0]); \
    \
    /* 2. Przechwycenie wyjścia ft_printf */ \
    pipe(pipe_fd); \
    dup2(pipe_fd[1], STDOUT_FILENO); \
    close(pipe_fd[1]); \
    ret_ft = ft_printf(__VA_ARGS__); \
    fflush(stdout); \
    dup2(saved_stdout, STDOUT_FILENO); \
    read(pipe_fd[0], buf_ft, sizeof(buf_ft) - 1); \
    close(pipe_fd[0]); \
    \
    /* BEZPIECZNE PORÓWNANIE (zabezpieczenie przed overflow) */ \
    int max_buf = (int)sizeof(buf_std); \
    int safe_len = (ret_std < max_buf) ? ret_std : max_buf; \
    int content_match = (ret_std >= 0 && ret_ft >= 0) ? (memcmp(buf_std, buf_ft, safe_len) == 0) : 1; \
    \
    if (ret_std != ret_ft || !content_match) { \
        printf("\n❌ BŁĄD DLA TESTU: \"%s\"\n", #__VA_ARGS__); \
        printf("   [printf]    ret: %d | wyjście: [%s]\n", ret_std, buf_std); \
        printf("   [ft_printf] ret: %d | wyjście: [%s]\n", ret_ft, buf_ft); \
    } else { \
        printf("✅ OK: \"%s\"\n", #__VA_ARGS__); \
    } \
    \
    /* 4. Asercje właściwe */ \
    assert(ret_std == ret_ft); \
    if (ret_std >= 0) { \
        assert(memcmp(buf_std, buf_ft, ret_std) == 0); \
    } \
} while(0)

int main(void)
{
    printf("=== ROZPOCZĘCIE TESTÓW Z ASERCJĄ ===\n\n");

    // 1. Wskaźniki (%p)
    ASSERT_PRINTF("%p\n", NULL);
    ASSERT_PRINTF("%p\n", (void *)0);
    ASSERT_PRINTF("%p\n", (void *)-1);
    ASSERT_PRINTF("%p\n", (void *)1);
    ASSERT_PRINTF("%p\n", (void *)INT_MIN);
    ASSERT_PRINTF("%p\n", (void *)INT_MAX);
    ASSERT_PRINTF("%p\n", (void *)ULONG_MAX);
    ASSERT_PRINTF("Wskaźnik: %p, po wskaźniku\n", (void *)0x7fff5fbff888);

    // 2. Ciągi znaków (%s) i pojedyncze znaki (%c)
	// 2. Ciągi znaków (%s) i pojedyncze znaki (%c)
    char * volatile null_str = NULL; // 'volatile' całkowicie oślepia analizator GCC!
    
    ASSERT_PRINTF("%s\n", null_str);
    ASSERT_PRINTF("%s\n", "");
    ASSERT_PRINTF("Początek %s koniec\n", null_str);
    ASSERT_PRINTF("%c\n", '0');
    ASSERT_PRINTF("%c\n", 0); // Kluczowy test na null byte!
    ASSERT_PRINTF("%c\n", -1);
    ASSERT_PRINTF("%c %c %c\n", 'a', 0, 'z');
    ASSERT_PRINTF("null byte: %c, po bajcie\n", 0);

    // 3. Liczby całkowite ze znakiem (%d, %i)
    ASSERT_PRINTF("%d\n", 0);
    ASSERT_PRINTF("%d\n", -1);
    ASSERT_PRINTF("%d\n", 1);
    ASSERT_PRINTF("%d\n", INT_MIN);
    ASSERT_PRINTF("%d\n", INT_MAX);
    ASSERT_PRINTF("%i\n", INT_MIN);
    ASSERT_PRINTF("%i\n", INT_MAX);
    ASSERT_PRINTF("%d %d %d\n", INT_MIN, 0, INT_MAX);

    // 4. Liczby bez znaku (%u)
    ASSERT_PRINTF("%u\n", 0);
    ASSERT_PRINTF("%u\n", -1); // Powinno rzutować na 4294967295
    ASSERT_PRINTF("%u\n", 1);
    ASSERT_PRINTF("%u\n", INT_MIN);
    ASSERT_PRINTF("%u\n", INT_MAX);
    ASSERT_PRINTF("%u\n", UINT_MAX);
    ASSERT_PRINTF("%u\n", UINT_MAX + 1);

    // 5. System szesnastkowy (%x, %X)
    ASSERT_PRINTF("%x\n", 0);
    ASSERT_PRINTF("%X\n", 0);
    ASSERT_PRINTF("%x\n", -1);
    ASSERT_PRINTF("%X\n", -1);
    ASSERT_PRINTF("%x\n", INT_MIN);
    ASSERT_PRINTF("%X\n", INT_MAX);
    ASSERT_PRINTF("%x\n", UINT_MAX);
    ASSERT_PRINTF("%x %X %x\n", 0xabcdef, 0xABCDEF, 0);

    // 6. Znak procenta (%%) i kombinacje (Stress Test)
    ASSERT_PRINTF("%%\n");
    ASSERT_PRINTF("%%%%%%\n");
    ASSERT_PRINTF("%%%c\n", 'x');
    ASSERT_PRINTF("%%%s%%\n", "test");
    ASSERT_PRINTF("%d%% co to było?\n", 100);
    ASSERT_PRINTF("Wiele na raz: %c, %s, %p, %d, %i, %u, %x, %X, %%\n", 'A', "42", (void *)0x42, INT_MIN, INT_MAX, UINT_MAX, 0x2a, 0x2A);
    
    // Dodaj to jako osobny test bez użycia makra ASSERT_PRINTF:
	int ret_null = ft_printf(NULL);
	printf("ft_printf(NULL) zwróciło: %d (powinno być -1)\n", ret_null);


	// Tworzymy bufor z 5000 znaków 'A'
	char big_str[5001];
	memset(big_str, 'A', 5000);
	big_str[5000] = '\0';

	ASSERT_PRINTF("%s\n", big_str);
	ASSERT_PRINTF("Początek: %s | Środek: %s | Koniec: %s\n", big_str, big_str, big_str);

	// Wyłączamy sprawdzanie poprawności formatu dla celowo błędnych testów:
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wformat"
	#pragma GCC diagnostic ignored "-Wformat-zero-length"

    // 4. Pusty string formatujący
    ASSERT_PRINTF("");

    // 5. Celowo uszkodzone ciągi formatujące (Edge cases standardu)
    ASSERT_PRINTF("Samotny procent na końcu: %");
    ASSERT_PRINTF("Nieznany specyfikator: %k co teraz?");

    // Przywracamy rygorystyczne sprawdzanie dla reszty pliku:
	#pragma GCC diagnostic pop

    printf("\n🎉 WSZYSTKIE TESTY PRZESZŁY POMYŚLNIE! 🎉\n");
    return (0);
}
