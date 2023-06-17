#include "../rpc.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct {
    long unsigned int num;
    long unsigned int count;
    long unsigned int *primes;
} my_struct_t;

void *parse_parameters(void *data) {
    const char *buf = (const char *)(data);

    my_struct_t *d = (my_struct_t *)(malloc(sizeof(my_struct_t)));

    if (d) {
        sscanf(buf, "%ld", &d->num);
        d->count = 0;
        d->primes = NULL;
    }

    return (void *)d;
}


void *do_work(void *params) {
    // Convertir el puntero void a un puntero a my_struct_t
    my_struct_t *d = (my_struct_t *)(params);

    // Reservar memoria para el arreglo que contendrá los números primos encontrados
    long unsigned int *primes = (long unsigned int *)(malloc(d->num * sizeof(long unsigned int)));

    // Inicializar el contador de números primos
    long unsigned int count = 0;

    // Recorrer todos los números desde 2 hasta d->num
    for (long unsigned int i = 2; i < d->num; i++) {
        // Inicializar la variable que indica si el número i es primo
        int is_prime = 1;

        // Verificar si i es divisible por algún número en el rango [2, i-1]
        for (long unsigned int j = 2; j < i; j++) {
            if (i % j == 0) {
                // Si i es divisible por j, entonces no es un número primo
                is_prime = 0;
                break;
            }
        }

        // Si i es un número primo, almacenarlo en el arreglo y actualizar el contador
        if (is_prime) {
            primes[count] = i;
            count++;
        }
    }

    // Reservar memoria para una nueva estructura my_struct_t que contendrá los números primos encontrados
    my_struct_t *result = (my_struct_t *)(malloc(sizeof(my_struct_t)));

    // Copiar el valor de d->num en el campo num de la estructura result
    result->num = d->num;

    // Copiar la cantidad de números primos encontrados en el campo count de la estructura result
    result->count = count;

    // Asignar el puntero al arreglo de números primos al campo primes de la estructura result
    result->primes = (long unsigned int *)(realloc(primes, count * sizeof(long unsigned int)));

    // Devolver un puntero a la estructura result
    return (void *)result;
}

reportable_t *report(void *data) {
    my_struct_t *d = (my_struct_t *)(data);

    reportable_t *report = (reportable_t *)(malloc(sizeof(reportable_t)));
    report->data = (char *)(malloc(255 * sizeof(char)));

    snprintf(report->data, 255, "Los números primos menores que %ld son: ", d->num);
    for (long unsigned int i = 0; i < d->count; i++) {
        char str[20];
        snprintf(str, 20, "%lu ", d->primes[i]);
        strcat(report->data, str);
    }
    strcat(report->data, "\n");

    report->len = strlen(report->data);

    return report;
}


void clean_up(void *params, void *result, reportable_t *report) {
    if (report && report->data) {
        free(report->data);
    }

    my_struct_t *d = (my_struct_t *)(params);
    if (d && d->primes) {
        free(d->primes);
    }
    if (params) {
        free(params);
    }
}
