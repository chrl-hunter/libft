# Libft — estado de desarrollo

18 funciones implementadas y verificadas con las pruebas de `tests/`.
La biblioteca compila, pero **la parte obligatoria todavía está incompleta**.

## Compilar y comprobar

```sh
make
make                  # No recompila ni vuelve a ejecutar ar
./tests/run.sh        # Prueba el archivo estático y repite con ASan + UBSan
python3 -m norminette ft_*.c libft.h
make clean            # Borra objetos; conserva libft.a
make fclean           # Borra también libft.a
make re               # Reconstrucción completa
```

Las pruebas requieren un compilador con soporte para AddressSanitizer y UBSan.
En entornos donde LeakSanitizer no puede inspeccionar `/proc`, usar:

```sh
ASAN_OPTIONS=detect_leaks=0 ./tests/run.sh
```

Esto mantiene activas las comprobaciones de accesos a memoria y comportamiento
indefinido. No equivale a verificar fugas de memoria. Las funciones actuales no
reservan memoria dinámica; las pruebas liberan sus buffers.

`tests/` contiene herramientas de desarrollo y no se enlaza en `libft.a`.
La comprobación de Norminette se aplica a los archivos de entrega de la raíz;
el programa de pruebas usa deliberadamente estilo C convencional.
No copiar los tests a la entrega de evaluación sin revisar las instrucciones
vigentes sobre archivos auxiliares.

## Qué se corrigió y por qué

- **`ft_memmove`**: el cast correcto es `((const unsigned char *)src)[i]`:
  primero convertir el puntero y después indexar bytes. `void *` no proporciona
  un tamaño de elemento para indexación estándar. Con `n` bytes, los índices
  válidos son `0` a `n - 1`; por eso la copia inversa decrementa antes de acceder.
  Se devuelve siempre `dest`. Si el destino comienza dentro del bloque fuente,
  se copia hacia atrás para no destruir bytes aún pendientes de leer. En los
  demás casos se usa `ft_memcpy`, cuya copia hacia delante también es segura
  para el solapamiento hacia la izquierda. La detección usa igualdad entre
  punteros, evitando ordenar punteros de objetos independientes con `<`.
  Este recorrido adicional conserva complejidad O(n) y espacio O(1).
- **`ft_strlcat`**: buscar el final del destino está limitado a `n`. Si no hay
  terminador dentro de ese espacio, no se escribe y se devuelve
  `n + ft_strlen(src)`. En otro caso, `i` conserva la longitud inicial y `j`
  cuenta lo añadido. Se copia mientras `j < n - i - 1`, reservando un byte para
  `\0`. Esta resta se evalúa solo después de comprobar `i < n`, evitando el
  desbordamiento por debajo de cero de `size_t`. La devolución expresa la
  longitud que se intentó crear, aunque hubiera truncamiento; no cuenta solo
  los bytes copiados. Se usa `ft_strlen`, no `strlen`.
- **`ft_strlen` y `libft.h`**: una cadena se recibe como `const char *`, no
  `int *`. Indexar `int *` avanzaba unidades de `sizeof(int)`, no caracteres.
  La longitud se devuelve como `size_t`. Se añadieron las declaraciones que
  faltaban de `ft_strchr`, `ft_strrchr`, `ft_strncmp` y `ft_memchr`.
- **`ft_memchr`**: se implementó la búsqueda de la primera coincidencia entre
  exactamente los primeros `n` bytes, incluidos ceros. Tanto el dato como el
  carácter buscado se interpretan como `unsigned char`.
- **Nombres y caracteres**: `memset` e `isprint` pasan a `ft_memset` y
  `ft_isprint`; `ft_isdigit` incluye `'0'` y usa `&&`. `&` es un operador de bits,
  no la conjunción lógica con cortocircuito. Se agruparon las condiciones de
  `ft_isalpha` para compilar con `-Werror` sin cambiar su resultado.
- **Otros límites confirmados**: `ft_strlcpy` lee `src`, no `dest`, no escribe
  con tamaño cero y devuelve la longitud completa de `src`. `ft_strncmp` no lee
  el índice `n` ni accede a las cadenas con `n == 0`. `ft_strrchr` busca también
  el terminador y devuelve su dirección, no un carácter convertido a puntero.
  `ft_strchr` y `ft_strrchr` comparan bytes sin signo para funcionar con valores
  mayores que 127 en este entorno.

## Cobertura y resultados

Verificado el 21 de octubre de 2026, a partir de `c52b888`:

- Compilación con `cc -Wall -Wextra -Werror` y creación con `ar rcs`.
- Pruebas del archivo `libft.a` y de las fuentes instrumentadas con ASan/UBSan:
  ambas pasan (`detect_leaks=0` por la restricción del entorno).
- `ft_memmove`: todas las combinaciones válidas de origen, destino y longitud
  dentro de un buffer de 32 bytes, más buffers independientes; se compara
  contenido completo y puntero devuelto con `memmove` de libc.
- `ft_strlcat` y `ft_strlcpy`: cadenas vacías, tamaños 0 y 1, encaje exacto,
  truncamiento, bytes de guarda y destino sin terminador dentro del límite.
  Resultados esperados calculados según el contrato, sin depender de libbsd.
- Comparaciones con libc para las otras funciones, valores de byte de rango
  alto, búsqueda de `\0`, y buffers sin terminador de tamaño exacto.
- Norminette **3.3.59**: 18 archivos `.c` y `libft.h` pasan. Repetir con la
  versión instalada en el campus antes de entregar.
- `make`, segundo `make`, `clean`, `fclean`, `re` y compilación paralela pasan.
  `nm -u libft.a` solo muestra dependencias internas `ft_*`.

Estas pruebas no sustituyen la evaluación completa del proyecto.

## Pendiente: continuar por aquí

No quedan funciones vacías entre los archivos existentes: `ft_memchr.c` era
la única. Faltan estas **16 funciones** respecto a la parte obligatoria del
enunciado público consultado:

1. Sin reserva dinámica: `ft_memcmp`, `ft_strnstr`, `ft_atoi`.
2. Reserva básica: `ft_calloc`, `ft_strdup`.
3. Cadenas con reserva: `ft_substr`, `ft_strjoin`, `ft_strtrim`, `ft_split`,
   `ft_itoa`.
4. Callbacks: `ft_strmapi`, `ft_striteri`.
5. Salida: `ft_putchar_fd`, `ft_putstr_fd`, `ft_putendl_fd`, `ft_putnbr_fd`.

Empieza por `ft_memcmp`: recorre como máximo `n` bytes y compara como
`unsigned char`. Prueba `n == 0`, igualdad, diferencia en el último byte y
valores mayores que 127. Después sigue con `ft_strnstr` y `ft_atoi`.
Para cada nueva función, añade su prototipo a `libft.h`, su archivo a `SRCS`
en el Makefile y una prueba. No se añadieron prototipos ni stubs de funciones
inexistentes, para no presentar una API que todavía no se puede enlazar.

Los nueve bonus de listas se dejan para después de completar la parte
obligatoria. Por eso todavía no hay regla `bonus` ni `t_list`.

## Referencias y alcance de los requisitos

Se leyó el documento de 42 **Libft, versión 15**, conservado en esta
[copia pública del enunciado](https://raw.githubusercontent.com/angelamcosta/libft/main/en.subject.pdf).
Exige `libft.a`, los nombres `ft_*`, prototipos y comportamiento equivalentes,
`cc -Wall -Wextra -Werror`, `ar`, las reglas del Makefile y ausencia de relink.
No se pudo verificar el enunciado privado actual de 42 Barcelona: su versión
en Intra es la autoridad para la entrega y podría añadir requisitos.

- [Norminette oficial](https://github.com/42School/norminette)
- [Contrato POSIX de strlcat](https://pubs.opengroup.org/onlinepubs/9799919799/functions/strlcat.html)
