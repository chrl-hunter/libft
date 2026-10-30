# Comparación y adaptación de libft

Análisis del 28 de octubre de 2026, sobre versiones fijadas:

- Original: [chrl-hunter/libft, 70fb826](https://github.com/chrl-hunter/libft/tree/70fb826d0b597ba9cad299fb90467c6ea337b54f).
- Referencia: [chrl-hunter/libft-tmp, 6d515f7](https://github.com/chrl-hunter/libft-tmp/tree/6d515f742e1a28ee78445cbccf7e5599a2d14a0d).

El original tiene 30 fuentes C. La adaptación modifica 22, conserva 8 sin cambios y
agrega 13: cuatro obligatorias y nueve de bonus. El resultado ofrece 34 funciones
obligatorias y nueve de listas. Se conserva la distribución plana, la cabecera
`libft.h`, el prefijo `ft_`, las cabeceras de autor de los archivos existentes y los
nombres/implementaciones originales que eran correctos. No se importan el PDF ni
el README del repositorio temporal.

## Errores del original

Las referencias siguientes apuntan a la versión original, no a la corregida.
Una compilación independiente de cada fuente con `cc -Wall -Wextra -Werror
-fsyntax-only` falla en **15 de los 30 archivos**. Que los otros compilen no
implica que se puedan enlazar ni que su comportamiento sea correcto.

| Archivo y líneas originales | Error y consecuencia | Corrección |
| --- | --- | --- |
| [libft.h:15–42](https://github.com/chrl-hunter/libft/blob/70fb826d0b597ba9cad299fb90467c6ea337b54f/libft.h#L15-L42) | `ft_strlen` está declarado como `int (int *)` en vez de `size_t (const char *)`. Faltan `<stdlib.h>` y las declaraciones de varias funciones implementadas; las de salida necesitan `<unistd.h>`. | Unificar firmas, cabeceras y declaraciones de las 43 funciones. |
| [ft_strlen.c:13–15](https://github.com/chrl-hunter/libft/blob/70fb826d0b597ba9cad299fb90467c6ea337b54f/ft_strlen.c#L13-L15) | Usa `size_t` sin incluir una cabecera que lo defina. | Incluir `libft.h`; conservar el algoritmo. |
| [ft_calloc.c:14–23](https://github.com/chrl-hunter/libft/blob/70fb826d0b597ba9cad299fb90467c6ea337b54f/ft_calloc.c#L14-L23) | El parámetro se llama `n`, pero el cuerpo usa `size`, inexistente. No compila. | Unificar el parámetro y conservar la protección contra desbordamiento del producto. |
| [ft_isalpha.c:15](https://github.com/chrl-hunter/libft/blob/70fb826d0b597ba9cad299fb90467c6ea337b54f/ft_isalpha.c#L15) | La precedencia produce la lógica esperada, pero GCC emite `-Wparentheses`, convertido en error por `-Werror`. | Agrupar los rangos entre paréntesis. |
| [ft_isdigit.c:15](https://github.com/chrl-hunter/libft/blob/70fb826d0b597ba9cad299fb90467c6ea337b54f/ft_isdigit.c#L15) | Excluye `'0'`. Usa `&`, que también dispara una advertencia de paréntesis. En esta expresión concreta ambos operandos son booleanos: `&` no es la causa del fallo con cero. | Usar el rango `'0'`–`'9'` y `&&`. `ft_isalnum('0')` se corrige por dependencia. |
| [ft_isprint.c:13](https://github.com/chrl-hunter/libft/blob/70fb826d0b597ba9cad299fb90467c6ea337b54f/ft_isprint.c#L13), [ft_memset.c:14](https://github.com/chrl-hunter/libft/blob/70fb826d0b597ba9cad299fb90467c6ea337b54f/ft_memset.c#L14), [ft_strdup.c:14](https://github.com/chrl-hunter/libft/blob/70fb826d0b597ba9cad299fb90467c6ea337b54f/ft_strdup.c#L14) | Exportan `isprint`, `memset` y `strdup`, no las funciones `ft_` esperadas. Puede fallar el enlace y se invade el espacio de nombres de libc. | Renombrar definiciones y declaraciones. |
| [ft_memcmp.c:27](https://github.com/chrl-hunter/libft/blob/70fb826d0b597ba9cad299fb90467c6ea337b54f/ft_memcmp.c#L27) | Devuelve `NULL`, de tipo puntero en este entorno, desde una función `int`. | Devolver `0` cuando los bloques son iguales. |
| [ft_memmove.c:25–38](https://github.com/chrl-hunter/libft/blob/70fb826d0b597ba9cad299fb90467c6ea337b54f/ft_memmove.c#L25-L38) | Cast inválido; la copia inversa empieza en `n`, fuera del intervalo, omite el byte cero y no hay retorno final. | Copiar los índices `n-1` a `0` cuando corresponde y devolver `dest`. |
| [ft_atoi.c:23–34](https://github.com/chrl-hunter/libft/blob/70fb826d0b597ba9cad299fb90467c6ea337b54f/ft_atoi.c#L23-L34) | No salta espacios iniciales, acepta varios signos y acumula positivamente en `int`: `INT_MIN` desborda antes de aplicar el signo. | Saltar los seis espacios ASCII, aceptar un signo y acumular en negativo. `" 42"` pasa de `0` a `42`; `"--1"` pasa de `-1` a `0`. |
| [ft_itoa.c:14–52](https://github.com/chrl-hunter/libft/blob/70fb826d0b597ba9cad299fb90467c6ea337b54f/ft_itoa.c#L14-L52) | Cuenta con `% 10` en vez de `/ 10`; reserva sin espacio para el terminador; escribe valores numéricos sin `'0'`; no termina la cadena; negar `INT_MIN` desborda. | Contar dígitos, usar magnitud sin signo, escribir de derecha a izquierda y terminar con NUL. |
| [ft_putchar_fd.c:14–16](https://github.com/chrl-hunter/libft/blob/70fb826d0b597ba9cad299fb90467c6ea337b54f/ft_putchar_fd.c#L14-L16), [ft_strmapi.c:14–16](https://github.com/chrl-hunter/libft/blob/70fb826d0b597ba9cad299fb90467c6ea337b54f/ft_strmapi.c#L14-L16) | Cuerpos incompletos y sin cierre. | Incorporar las implementaciones de tmp. |
| [ft_split.c:27–79](https://github.com/chrl-hunter/libft/blob/70fb826d0b597ba9cad299fb90467c6ea337b54f/ft_split.c#L27-L79) | `;` entre la firma de `count_words` y su cuerpo; devuelve `result`, inexistente. Si se reparan solo esos errores, el delimitador NUL hace que el salto de delimitadores avance fuera de la cadena. | Adaptar los auxiliares de tmp, comprobar el terminador en ambos bucles y devolver `ptr`. Limpiar reservas parciales ante fallo. |
| [ft_strchr.c:18](https://github.com/chrl-hunter/libft/blob/70fb826d0b597ba9cad299fb90467c6ea337b54f/ft_strchr.c#L18) | Compara `char` potencialmente negativo con `unsigned char`: no encuentra bytes como `0xff` en plataformas con `char` con signo. | Comparar ambos como `unsigned char`. |
| [ft_strrchr.c:23–30](https://github.com/chrl-hunter/libft/blob/70fb826d0b597ba9cad299fb90467c6ea337b54f/ft_strrchr.c#L23-L30) | Devuelve `NULL` antes de tratar la búsqueda de NUL; también falla con bytes altos. | Tratar NUL antes del retorno de ausencia y comparar bytes sin signo. |
| [ft_strncmp.c:19–21](https://github.com/chrl-hunter/libft/blob/70fb826d0b597ba9cad299fb90467c6ea337b54f/ft_strncmp.c#L19-L21) | Si los primeros `n` bytes coinciden, compara el byte `n`; con `n=0` también lee memoria. `ft_strncmp("ab", "ac", 1)` debería ser `0`. | Devolver `0` cuando `i == n`. |
| [ft_strnstr.c:22–30](https://github.com/chrl-hunter/libft/blob/70fb826d0b597ba9cad299fb90467c6ea337b54f/ft_strnstr.c#L22-L30) | No reinicia `j` por candidato y lee `big[i]` antes de comprobar `i<n`. Puede dar coincidencias falsas: buscar `"aby"` en `"abxby"` devuelve una dirección aunque no existe. | Reiniciar `j` y comprobar primero el límite. |
| [ft_strlcpy.c:19–25](https://github.com/chrl-hunter/libft/blob/70fb826d0b597ba9cad299fb90467c6ea337b54f/ft_strlcpy.c#L19-L25) | Usa el contenido de destino como condición, puede leer origen fuera de la cadena, hace `n-1` con `n=0`, escribe aunque no tenga capacidad y devuelve una longitud incorrecta. | Leer la condición desde `src`, tratar capacidad cero y devolver la longitud completa de origen. |
| [ft_strlcat.c:20–29](https://github.com/chrl-hunter/libft/blob/70fb826d0b597ba9cad299fb90467c6ea337b54f/ft_strlcat.c#L20-L29) | Busca NUL sin respetar `n`, usa `>` en el límite de copia, incrementa `i` en vez de `j`, escribe sin comprobar espacio y llama a `strlen` de libc sin declaración. | Medir destino hasta `n`, limitar copia y devolver `min(longitud_destino,n)+longitud_origen`. |
| [ft_strtrim.c:21–36](https://github.com/chrl-hunter/libft/blob/70fb826d0b597ba9cad299fb90467c6ea337b54f/ft_strtrim.c#L21-L36) | Resta longitudes para reservar, pudiendo desbordar `size_t`; compara solo el primer carácter de `set`; elimina caracteres interiores; puede escribir fuera de la reserva; no termina y devuelve el puntero ya avanzado, inválido para `free`. | Localizar los extremos por pertenencia al conjunto y devolver una cadena terminada desde la base de su reserva. |
| Raíz del repositorio | No hay `Makefile`; faltan cuatro funciones obligatorias. | Incorporar compilación estática y las funciones indicadas a continuación. |

## Comparación de las 30 funciones existentes

| Función | Estado observado y decisión |
| --- | --- |
| `ft_atoi` | Adaptar tmp y corregir además `INT_MIN`. |
| `ft_bzero` | Idéntica y correcta en los casos revisados; conservar original. |
| `ft_calloc` | Adaptar tmp: parámetro, producto y caso de tamaño cero. |
| `ft_isalnum` | Idéntica; conservar. El error con `'0'` venía de `ft_isdigit`. |
| `ft_isalpha` | Adaptar agrupación e inclusión de cabecera de tmp; sin cambio lógico. |
| `ft_isascii` | Lógica correcta; incorporar la inclusión de `libft.h` de tmp. |
| `ft_isdigit` | Adaptar corrección del rango y operador. |
| `ft_isprint` | Adaptar el nombre con prefijo y la inclusión de cabecera. |
| `ft_itoa` | Tomar la estrategia de tmp; mantener `count_num` y `result`, con magnitud `unsigned int` para evitar depender de que `long` sea más ancho que `int`. |
| `ft_memchr` | Idéntica y correcta en los casos revisados; conservar. |
| `ft_memcmp` | Adaptar tmp: retorno entero y auxiliares legibles. |
| `ft_memcpy` | Idéntica y correcta para bloques sin solapamiento; conservar. |
| `ft_memmove` | Adaptar copia direccional de tmp y acortar el cuerpo. Comparar direcciones mediante `uintptr_t` en la plataforma objetivo Linux. |
| `ft_memset` | Adaptar el nombre con prefijo. |
| `ft_putchar_fd` | Completar con tmp. |
| `ft_split` | Adaptar auxiliares de tmp y corregir delimitador NUL; todas las funciones quedan dentro del límite de líneas de Norminette. |
| `ft_strchr` | Idéntica en ambos, pero con fallo para bytes altos; corregir. |
| `ft_strdup` | Adaptar nombre y tipo del puntero reservado. |
| `ft_strjoin` | Ambas implementaciones son equivalentes en los casos revisados; conservar original. |
| `ft_strlcat` | Adaptar tmp y limitar también su recorrido inicial. |
| `ft_strlcpy` | Adaptar tmp. |
| `ft_strlen` | Algoritmo correcto; adaptar cabecera y firma declarada. |
| `ft_strmapi` | Completar con tmp. |
| `ft_strncmp` | Adaptar tmp. |
| `ft_strnstr` | Adaptar tmp y comprobar límite antes de leer. |
| `ft_strrchr` | Adaptar tmp: búsqueda de NUL y bytes altos. |
| `ft_strtrim` | Adaptar tmp. |
| `ft_substr` | Idéntica y correcta en los casos revisados; conservar. |
| `ft_tolower` | Idéntica y correcta para el dominio ASCII revisado; conservar. |
| `ft_toupper` | Idéntica y correcta para el dominio ASCII revisado; conservar. |

## Funciones añadidas desde tmp

Obligatorias ausentes: `ft_striteri`, `ft_putstr_fd`, `ft_putendl_fd`, `ft_putnbr_fd`.
La salida numérica se ajusta a magnitud sin signo para soportar `INT_MIN` sin depender
del ancho de `long`.

Bonus opcional ausente: `ft_lstnew`, `ft_lstadd_front`, `ft_lstsize`, `ft_lstlast`,
`ft_lstadd_back`, `ft_lstdelone`, `ft_lstclear`, `ft_lstiter`, `ft_lstmap`.
Su ausencia se distingue de los errores de la parte obligatoria. Se añaden `t_list`
y sus prototipos, y se ajusta `ft_lstsize` a retorno `int`.
Se revisan recorrido, inserción, liberación, callbacks y limpieza al fallar una
reserva de nodo en `ft_lstmap`. La adaptación inicial usaba `ft_lstadd_back`, con
coste O(n²). Desde la optimización del 30/10/2026 conserva un puntero local `last`
al último nodo: cada inserción cuesta O(1) y la construcción completa O(n), más
el coste de los callbacks y del asignador. Solo requiere O(1) de espacio auxiliar,
además de los nodos y contenidos de salida. `ft_lstadd_back` no se modifica.
El helper estático `clear_map` conserva el orden de limpieza si falla el nodo:
primero `del(content)` (si existe), después `ft_lstclear` de la lista parcial.
Un retorno NULL de `f` sigue siendo contenido válido; no se añade una política
de error para el callback. Se conservan las comprobaciones de callbacks nulos.

### Regresión de ft_lstmap (30/10/2026)

`tests/lstmap.c`, integrado en `make -C tests check`, verifica:

- Lista vacía, `f == NULL`, `del == NULL` y lista de un nodo.
- Listas de 64, 100.000 y 200.000 nodos: orden, transformación, nodos y contenidos
  independientes, terminación NULL y lista original intacta.
- Fallo de cada una de las 128 reservas de una lista de 64 nodos, las dos de
  una lista de un nodo y una reserva de nodo con contenido prestado y `del == NULL`.
  Los fallos de contenido conservan el nodo con contenido NULL; los fallos de nodo
  devuelven NULL. Se comprueba el número exacto de llamadas a `f` y `del`, y cero
  reservas vivas tras la limpieza.
- Una envoltura de `ft_lstlast` detecta la reintroducción de búsquedas del último
  nodo, sin depender de mediciones de tiempo. La complejidad O(n) se deduce del
  único bucle y del enlace directo, no de un benchmark.

Validación repetida con `-Wall -Wextra -Werror`, AddressSanitizer y
UndefinedBehaviorSanitizer (`ASAN_OPTIONS=detect_leaks=0 UBSAN_OPTIONS=halt_on_error=1`):
64.740 comprobaciones previas, 15 fallos previos y la nueva suite pasan.
Norminette 3.3.60 acepta los 43 fuentes y `libft.h`. Se verifican 34 símbolos
obligatorios después de `make -j4 re`, 43 tras `make bonus` y ausencia de
rearchivado al repetir `make`/`make bonus`. El Makefile raíz no cambia.

## Problemas de tmp que no deben copiarse sin revisión

- `ft_strchr` conserva el fallo de comparación de bytes altos del original.
- `ft_split` conserva el salto más allá de NUL cuando el delimitador es `0`.
- `ft_strlcat` calcula `ft_strlen(dest)` incluso si el buffer no contiene NUL
  dentro de su capacidad: una lectura fuera de límites.
- `ft_strnstr` comprueba el límite después de leer el candidato exterior.
- `ft_atoi` acumula positivamente en `int` y desborda para `INT_MIN`.
- `ft_lstsize` usa retorno `unsigned int`; se normaliza a `int`.
- El `Makefile` incluye las listas en `all` y carece de `bonus`. Se separan los
  grupos y se evita repetir el archivado sin cambios; `re` ejecuta la limpieza
  antes de reconstruir incluso con `make -j`.
- `ft_itoa` y `ft_putnbr_fd` dependen del ancho de `long` para negar `INT_MIN`.
  Funcionan en LP64, pero la adaptación elimina esa dependencia.

## Verificación y alcance

Comprobaciones ejecutadas en Linux x86_64, GCC 13.3.0:

| Comprobación | Resultado |
| --- | --- |
| `make -j4`, `make bonus`, `make -j4 re` | Correctos con `-Wall -Wextra -Werror`. |
| Repetir `make` y `make bonus` | Sin cambios en la fecha de modificación del archivo estático. |
| `nm -g --defined-only libft.a` después de bonus | 43 símbolos de función `ft_`. |
| `ASAN_OPTIONS=detect_leaks=0 make -C tests check` | 64.740 comprobaciones; las 43 funciones ejercitadas; sin errores detectados por AddressSanitizer/UndefinedBehaviorSanitizer. |
| Inyección de fallo de `malloc` | 15 casos: cuatro reservas de split, tres reservas de nodo en lstmap y ocho funciones de reserva; todos devuelven NULL y el contador de reservas vivas termina en cero. |
| Norminette 3.3.60 sobre `ft_*.c` y `libft.h` | Los 43 fuentes y la cabecera pasan. |

Casos cubiertos: todos los bytes y EOF para clasificación, comparación con libc,
solapamientos de memoria en ambas direcciones, truncamiento y capacidad cero,
buffer destino sin NUL dentro del límite, búsquedas con bytes altos y NUL,
delimitador NUL, límites de int, salidas por pipe, listas y callbacks.

LeakSanitizer no puede inspeccionar `/proc` en este entorno y se desactivó para
las ejecuciones verificadas. El contador explícito de reservas cubre los caminos
de error indicados, pero no sustituye un análisis completo de fugas.
No se ha ejecutado un evaluador oficial de 42 ni los testers externos. Los tests
no prueban equivalencia para todas las entradas posibles. `ft_atoi` no añade una
política para valores fuera del rango de `int`. No se promete portabilidad de
`ft_memmove` a arquitecturas cuya representación de punteros no preserve el orden
de direcciones al convertirlas a `uintptr_t`.
