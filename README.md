# libft

Biblioteca en C con 34 funciones obligatorias y 9 funciones de listas (bonus).
Conserva la estructura original: fuentes `ft_*.c` y `libft.h` en la raíz.

```sh
make
make bonus
make clean
make fclean
make re
```

`make` crea `libft.a`; `make bonus` incorpora las listas al mismo archivo.
Repetir cualquiera de los dos sin cambios no vuelve a crear el archivo.
Los programas consumidores deben incluir `libft.h` y enlazar `libft.a`.

La adaptación procede de `chrl-hunter/libft-tmp`, revisada para corregir también
los casos límite compartidos. El análisis por función y los errores del original
están en [AUDIT.md](AUDIT.md).

## Validación

```sh
make -C tests check
norminette ft_*.c libft.h
```

Las pruebas requieren GCC o Clang en Linux, AddressSanitizer, UndefinedBehaviorSanitizer
y un enlazador compatible con `--wrap`. Ejercitan las 43 funciones, comparan con libc
cuando procede e inyectan 15 fallos de reserva. El seguimiento de `malloc`/`free`
comprueba que no queden reservas pendientes en esos caminos de error.
Los archivos de pruebas son herramientas de desarrollo y no forman parte del archivo
de biblioteca ni del conjunto de fuentes sometido a Norminette.

La suite adicional `tests/lstmap.c` cubre listas vacías y de hasta 200.000 nodos,
callbacks nulos, conservación del orden y de la entrada, y 131 fallos de reserva
adicionales. Comprueba la limpieza exacta y detecta búsquedas repetidas del último
nodo. `ft_lstmap` conserva un puntero al último nodo para construir en O(n), con
O(1) de espacio auxiliar (sin contar salida ni callbacks).

En entornos donde LeakSanitizer no puede inspeccionar `/proc`, puede ejecutarse:

```sh
ASAN_OPTIONS=detect_leaks=0 make -C tests check
```

Esto mantiene AddressSanitizer y UndefinedBehaviorSanitizer, y el seguimiento explícito
de reservas de las pruebas de fallo; no equivale a un análisis completo de fugas.

`ft_atoi` admite las representaciones de todos los valores de `int`, incluido `INT_MIN`.
No establece una política adicional para entradas numéricas fuera de ese rango.
