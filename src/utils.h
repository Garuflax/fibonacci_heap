#ifndef UTILS_H
#define UTILS_H

/**
 * @brief Logaritmo en base 2
 * @param n número a hacerle logaritmo
 *
 * @returns log2(n) redondeado para arriba
 *
 * \complexity{log(n)}
 *
 * \bug Si el resultado esperado es 64 se cuelga
 */
unsigned int log2ul(unsigned long long int n);

#endif //UTILS_H
