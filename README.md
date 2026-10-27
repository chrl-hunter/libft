# Libft — estado de desarrollo

25 funciones implementadas y verificadas con las pruebas de `tests/`.
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
indefinido. No equivale a verificar fugas de memoria. Las pruebas de las funciones con
reserva dinámica liberan los buffers que reciben. `tests/run.sh` también usa
`--wrap=malloc` de GNU ld para comprobar la propagación de fallos de reserva
y el rechazo de overflow antes de llamar a `malloc` (comprobación de Linux).

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

## Integración de las dos ramas — 27 de octubre de 2026

Se integran `a94ec8a` (correcciones de la PR #1) y `c08f86c` (avances de
`master`) mediante un commit con ambos padres. Los conflictos de `ft_memchr`,
`ft_strlen`, `ft_strncmp` y `ft_strrchr` conservan las correcciones de la PR:
búsqueda del terminador, comparación de bytes sin signo y límites de lectura.
Se eliminan tres inclusiones duplicadas de `libft.h` que traía la PR.
Las siete funciones nuevas de `master` están en el Makefile y en la cabecera;
`ft_substr` y `ft_strjoin` conservan sus implementaciones originales.

- **`ft_calloc`**: se usa el parámetro `n` en lugar de `size`, que no existía.
  Antes de multiplicar se comprueba `n != 0 && nmemb > (size_t)-1 / n`.
  Así se evita que la multiplicación desborde `size_t` y reserve menos bytes
  de los necesarios. El caso de tamaño cero admite el resultado de `malloc(0)`;
  no se debe desreferenciar ese resultado. Si falla la reserva, devuelve `NULL`.
- **`ft_strdup`**: el nombre exportado pasa a ser `ft_strdup`, con su prototipo.
  Copia `ft_strlen(s) + 1` bytes para incluir el terminador en una reserva propia.
  El llamante es responsable de `free`.
- **`ft_strnstr`**: `j` se reinicia en cada candidato `i`; una coincidencia
  parcial anterior no se puede reutilizar como si perteneciera al candidato
  siguiente. Se comprueba `i < n` antes de leer `big[i]`, y `j < n - i` antes
  de leer `big[i + j]`. La resta evita desbordar la suma usada como límite.
  Una aguja vacía devuelve `big`, incluso con `n == 0`.
- **`ft_memcmp`**: igualdad devuelve el entero `0`, no el puntero `NULL`.
  Se mantiene la comparación como `unsigned char`, incluidos bytes > 127.
- **`ft_atoi`**: omite los seis espacios ASCII, consume como máximo un signo
  y termina en el primer carácter no numérico. Acumula en negativo para poder
  representar `INT_MIN` sin construir primero su magnitud positiva, que no
  cabe en `int`. Se comprueban entradas cuyo valor cabe en `int`; no se promete
  un comportamiento de extensión para valores fuera de ese rango.
- **`libft.h`**: incluye `<stdlib.h>` para `malloc`, declara las 25 funciones
  con nombres `ft_*` y elimina prototipos duplicados o incompatibles.

## Cobertura y resultados

Verificado el 27 de octubre de 2026 con GCC 13.3.0 en Linux:

- `cc -Wall -Wextra -Werror` y creación de `libft.a` con los 25 símbolos.
- `ASAN_OPTIONS=detect_leaks=0 ./tests/run.sh`: pasan las suites antiguas y
  nuevas, enlazadas con el archivo y repetidas con ASan/UBSan sobre las fuentes.
  UBSan se configura para detenerse ante el primer error.
- Suite original: matriz de solapamientos de `ft_memmove`, tamaños 0 y 1,
  truncamiento, bytes de guarda, bytes altos y búsqueda del terminador.
- Suite nueva: `ft_memcmp` frente a libc; `ft_strnstr` frente a un oráculo
  independiente, prefijos repetidos y buffers sin terminador; `ft_atoi` frente
  a libc en el rango representable, incluidos `INT_MIN` y `INT_MAX`.
- Reservas: contenido cero, cadenas independientes y terminadas, entradas
  vacías, subcadenas fuera de rango, longitudes máximas, overflow de `calloc`,
  y propagación de `malloc == NULL` en las cuatro funciones con reserva.
- Norminette **3.3.59**: pasan los 25 archivos `.c` y `libft.h` de la raíz.
  Los auxiliares de `tests/` no se presentan como conformes a la Norma.
- Ciclo de Makefile: segundo `make` sin recompilación ni relink, `clean`,
  `fclean`, `re`, construcción paralela y reconstrucción al cambiar la cabecera.

**Pendiente:** LeakSanitizer no puede inspeccionar `/proc` en este entorno;
la ejecución con su configuración predeterminada falla por ese motivo.
Se repitió con `detect_leaks=0`; no se afirma una verificación de fugas.
Valgrind no está instalado. Repetir la detección de fugas y la Norma del campus
antes de entregar. Tampoco se ha realizado la evaluación oficial de 42.
Estas pruebas no sustituyen la evaluación completa del proyecto.

## Pendiente: continuar por aquí

Faltan estas **9 funciones** respecto al enunciado público consultado:

1. Cadenas con reserva: `ft_strtrim`, `ft_split`, `ft_itoa`.
2. Callbacks: `ft_strmapi`, `ft_striteri`.
3. Salida: `ft_putchar_fd`, `ft_putstr_fd`, `ft_putendl_fd`, `ft_putnbr_fd`.

Para cada función nueva, añade su prototipo a `libft.h`, su archivo a `SRCS`
y una prueba. No se añadieron stubs de funciones pendientes.
Los nueve bonus de listas quedan para después de la parte obligatoria;
todavía no hay regla `bonus` ni `t_list`.

## Referencias y alcance de los requisitos

Se leyó el documento de 42 **Libft, versión 15**, conservado en esta
[copia pública del enunciado](https://raw.githubusercontent.com/angelamcosta/libft/main/en.subject.pdf).
Exige `libft.a`, los nombres `ft_*`, prototipos y comportamiento equivalentes,
`cc -Wall -Wextra -Werror`, `ar`, las reglas del Makefile y ausencia de relink.
No se pudo verificar el enunciado privado actual de 42 Barcelona: su versión
en Intra es la autoridad para la entrega y podría añadir requisitos.

- [Norminette oficial](https://github.com/42School/norminette)
- [Contrato POSIX de strlcat](https://pubs.opengroup.org/onlinepubs/9799919799/functions/strlcat.html)
