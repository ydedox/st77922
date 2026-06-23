# Manual de Referencia — Placa ESP32-S3 + LCD ST77922 (3.5", 320×480, Táctil Capacitiva)

## 1. Identificación del Hardware

| Componente | Descripción |
|---|---|
| **MCU** | ESP32-S3 (Xtensa LX7 dual-core) |
| **LCD** | ST77922, QSPI, 320×480 píxeles, profundidad 16-bit RGB565 |
| **Touch** | Táctil capacitivo integrado en el ST77922, comunicación I²C, dirección `0x55` |
| **Retroiluminación** | PWM por LEDC (canal 1, timer 1, 5 kHz, resolución 10 bits) |
| **TE (Tearing Effect)** | Pin GPIO42, usado para sincronizar refresco y evitar tearing |
| **Firmware del touch** | ST77922 v1.0.3 |

---

## 2. Pinout Completo (GPIOs)

| Función | Señal | GPIO | Notas |
|---|---|---|---|
| **LCD QSPI** | `QSPI_CS` | **GPIO10** | Chip Select |
| | `QSPI_PCLK` | **GPIO12** | Clock SPI |
| | `QSPI_DATA0` | **GPIO11** | D0 (MOSI en SPI) |
| | `QSPI_DATA1` | **GPIO13** | D1 (MISO en SPI) |
| | `QSPI_DATA2` | **GPIO14** | D2 |
| | `QSPI_DATA3` | **GPIO9** | D3 |
| | `QSPI_RST` | **GPIO_NC** (no conectado) | El reset se hace por software |
| | `QSPI_DC` | **GPIO_NC** (no usado en QSPI) | El comando/data se codifica en los opcodes |
| | `QSPI_TE` | **GPIO42** | Tearing Effect (sincronización VSync) |
| | `QSPI_BL` | **GPIO41** | Backlight (PWM) |
| **Touch I²C** | `TOUCH_SCL` | **GPIO39** | I²C Clock |
| | `TOUCH_SDA` | **GPIO38** | I²C Data |
| | `TOUCH_RST` | **GPIO48** | Reset del touch |
| | `TOUCH_INT` | **GPIO47** | Interrupción táctil (activa en nivel bajo) |

> **Importante**: El LCD usa **QSPI** (Quad SPI), no SPI convencional. Los pines `DATA0..DATA3` son bidireccionales. El pin DC no se usa porque en modo QSPI el comando/data se distingue por *opcode* (cmd=0x02, data=0x32, read=0x0B).

---

## 3. Configuración del Bus LCD (QSPI)

Se usa el periférico **SPI2_HOST** en modo Quad:

- Velocidad de reloj: **80 MHz** (en QSPI; en SPI normal sería 40 MHz)
- Modo SPI: **0** (CPOL=0, CPHA=0)
- Bits de comando: **32 bits** (opcode 8-bit + comando 8-bit + 16 bits reservados)
- Bits de parámetro: **8 bits**
- DMA: `SPI_DMA_CH_AUTO`
- Transferencia máxima: `hres × vres × 2` (320 × 480 × 2 = 307 200 bytes)

### Opcodes QSPI

| Operación | Opcode (32-bit) |
|---|---|
| Write Command | `0x02` << 24 \| cmd << 8 |
| Read Command | `0x0B` << 24 \| cmd << 8 |
| Write Color (RAM) | `0x32` << 24 \| cmd << 8 |

---

## 4. Secuencia de Inicialización del LCD (ST77922)

La inicialización se divide en **tres páginas** de comandos, seleccionadas mediante:

| Comando | Función |
|---|---|
| `0xF0` | Página CMD1 (por defecto) |
| `0xF1` | Página CMD2 |
| `0xF2` | Página CMD3 |

La secuencia completa (`vendor_specific_init_default`) es extensa (~60 comandos). Los comandos clave:

| Cmd | Valor | Descripción |
|---|---|---|
| `0x11` | — | Sleep OUT (espera 120 ms) |
| `0x28` | `0x00` | Display OFF |
| `0x10` | `0x00` | Deep Sleep OFF |
| `0x2A` | `0x00,0x00,0x02,0x13` | Column Address (531 columnas, ver §5) |
| `0x2B` | `0x00,0x00,0x01,0x2B` | Page Address (299 filas, ver §5) |
| `0x21` | — | Inversión ON (opcional, algunos paneles) |
| `0x35` | `0x00` | Tearing Effect Line ON |
| `0x3A` | `0x01` | COLMOD = 16-bit (RGB565) |
| `0x36` | `0x00` | MADCTL: RGB, sin mirror/swap |
| `0x29` | — | Display ON |

Los registros internos de calibración de gamma, VCOM, VGH/VGL, etc. se configuran en CMD2/CMD3 con comandos `0x70`–`0x9F`, `0xB0`–`0xBF` y `0xE3`.

> ⚠️ **Atención**: La secuencia `vendor_specific_init_default` usa **531×299** como tamaño de columna/página (valores de fábrica del IC). La secuencia **custom** en `esp_bsp.c` sobrescribe esto con `0x2A=0x00,0x00,0x01,0x3F` (320) y `0x2B=0x00,0x00,0x01,0xDF` (480), que son las dimensiones reales del panel.

---

## 5. Dimensiones Reales del Panel

| Parámetro | Valor |
|---|---|
| Resolución horizontal (`H_RES`) | **320 píxeles** |
| Resolución vertical (`V_RES`) | **480 píxeles** |
| Formato de color | **RGB565** (16 bits/píxel) |
| Orden de elementos | **RGB** (no BGR) |
| `LV_COLOR_DEPTH` | 16 |
| `LV_COLOR_16_SWAP` | 1 (necesario para QSPI) |

### Rotación por software (LVGL)

Se aplica rotación por software en LVGL. El `lv_port.c` implementa las 4 rotaciones reordenando píxeles en buffers de transporte. La rotación se define en el `.ino`:

```c
#define LVGL_PORT_ROTATION_DEGREE  (90)   // 0, 90, 180 o 270
```

Con rotación 90°, el ancho lógico pasa a ser 480 y el alto 320.

---

## 6. Touch Capacitivo I²C

| Parámetro | Valor |
|---|---|
| **Controlador** | ST77922 (touch integrado en el mismo IC) |
| **Dirección I²C** | `0x55` (7 bits) |
| **Velocidad I²C** | 400 kHz |
| **Bus I²C** | `I2C_NUM_0` |
| **Pull-ups internos** | Habilitados (`enable_internal_pullup = true`) |
| **Máx. puntos táctiles** | 1 (configurado; el HW soporta hasta 10) |
| **Interrupción** | GPIO47, activa en nivel bajo (`interrupt = 0`) |
| **Reset** | GPIO48, activo en nivel bajo (`reset = 0`) |

### Registros I²C del Touch

| Registro | Dirección | Descripción |
|---|---|---|
| `FW_VERSION` | `0x0000` | Versión de firmware |
| `FW_REVISION` | `0x000C` | Revisión (4 bytes) |
| `MAX_X_COORD_H/L` | `0x0005-0x0006` | Máxima coordenada X |
| `MAX_Y_COORD_H/L` | `0x0007-0x0008` | Máxima coordenada Y |
| `MAX_TOUCHES` | `0x0009` | Número máximo de toques |
| `TOUCH_INFO` | `0x0010` | Información avanzada (1 byte) |
| `REPORT_COORD_0` | `0x0014` | Primer reporte de coordenadas (8 bytes por toque) |

### Estructura del Reporte Táctil (8 bytes por toque)

```
Byte 0: [valid:1, reserved:1, x_h:6]
Byte 1: x_l (8 bits)
Byte 2: y_h (8 bits)
Byte 3: y_l (8 bits)
Byte 4: area
Byte 5: intensity
Byte 6-7: reserved
```

Coordenada X = `(x_h << 8) | x_l` (rango 0–531)
Coordenada Y = `(y_h << 8) | y_l` (rango 0–299)

> Los rangos nativos del touch (531×299) se **mapean internamente** a la resolución del panel (320×480) mediante el callback `bsp_touch_process_points_cb`, que aplica la rotación configurada.

---

## 7. Retroiluminación (PWM)

| Parámetro | Valor |
|---|---|
| **GPIO** | **41** |
| **Periférico** | LEDC (canal 1, timer 1) |
| **Modo** | `LEDC_LOW_SPEED_MODE` |
| **Frecuencia** | 5 kHz |
| **Resolución** | 10 bits (0–1023) |
| **Brillo 0%** | Duty = 0 |
| **Brillo 100%** | Duty = 1023 |

La función `bsp_display_brightness_set(porcentaje)` acepta valores de 0 a 100.

---

## 8. Sincronización Tearing Effect (TE)

El pin **GPIO42** se configura como entrada con interrupción en **cualquier flanco** (`GPIO_INTR_ANYEDGE`).

- Se crea una tarea FreeRTOS (`Tear task`, prioridad 4, stack 2048) que espera el semáforo de TE.
- `time_Tvdl = 13 ms`: tiempo en que el panel se actualiza desde la memoria de trama.
- `time_Tvdh = 3 ms`: tiempo en que el panel NO se actualiza.

Esto permite sincronizar el envío de datos para evitar *tearing* (rotura de imagen).

---

## 9. Configuración de LVGL

| Parámetro | Valor |
|---|---|
| **Versión LVGL** | v8.4.x (≥ 8.3.9, < v9) |
| **Buffer principal** | 153 600 pixels (`H_RES × V_RES`) en PSRAM |
| **Buffers de transporte** | 2 buffers de `H_RES × V_RES / 10` en DMA |
| **Full refresh** | Habilitado (`full_refresh = 1`) |
| **Tarea LVGL** | Prioridad 4, stack 4096, sin afinidad de núcleo |
| **Timer tick** | Cada 5 ms (por `esp_timer`) |
| **Refresco pantalla** | Cada 30 ms (`LV_DISP_DEF_REFR_PERIOD`) |
| **Mutex** | Recursivo para acceso thread-safe |

### Configuración en `lv_conf.h`

```c
#define LV_COLOR_DEPTH     16
#define LV_COLOR_16_SWAP   1         // CRÍTICO para QSPI
#define LV_TICK_CUSTOM     1
#define LV_DPI_DEF         130
#define LV_USE_DEMO_WIDGETS   1      // Habilitado en ejemplo
#define LV_USE_DEMO_MUSIC     0      // Se usa en .ino pero desactivado en conf
#define LV_USE_DEMO_BENCHMARK 0
#define LV_USE_DEMO_STRESS    0
```

> `LV_COLOR_16_SWAP = 1` es **obligatorio** porque el ST77922 en QSPI espera los bytes RGB565 intercambiados (little-endian). Sin esto, los colores se ven corruptos.

---

## 10. Flujo de Inicialización (Arduino `setup()`)

```
Serial.begin(115200)
  → bsp_display_start_with_config(&cfg)
      → lvgl_port_init()              // Inicializa LVGL, timer tick, tarea
      → bsp_display_brightness_init() // Configura PWM del backlight
      → bsp_display_lcd_init()
          → bsp_display_new()
              → spi_bus_initialize()   // QSPI, SPI2_HOST, DMA
              → esp_lcd_new_panel_io_spi()
              → esp_lcd_new_panel_st77922()   // Driver del panel
              → esp_lcd_panel_reset()
              → esp_lcd_panel_init()          // Envía secuencia init
              → esp_lcd_panel_disp_on_off(true)
              → [Opcional] Configura TE (GPIO42)
          → lvgl_port_add_disp()      // Registra display en LVGL
      → bsp_display_indev_init()
          → bsp_i2c_init()            // I²C master
          → esp_lcd_touch_new_i2c_st77922()  // Touch por I²C
          → lvgl_port_add_touch()     // Registra touch en LVGL
  → bsp_display_backlight_on()        // Brillo al 100%
  → [Aquí se crea la UI con APIs de LVGL]
```

---

## 11. Modificaciones Clave sobre Librerías Estándar

### a) Driver `esp_lcd_st77922.c` (nuevo, no incluido en ESP-IDF estándar)

Es un driver *custom* para el modelo **ST77922**, que **no es el ST7796 común**. Las diferencias principales:

- Utiliza **páginas de comandos** (`0xF0`/`0xF1`/`0xF2`) para acceder a registros extendidos. Esto no existe en ST7796.
- Los opcodes QSPI son diferentes: `0x02` para comando, `0x32` para color (vs los estándar `0x02`/`0x12` de otros controladores).
- La secuencia de inicialización es específica del fabricante del panel (incluye calibración de VCOM, gamma, VGL/VGH pump).
- El touch capacitivo está **integrado en el mismo IC** ST77922 y se comunica por I²C, a diferencia de otros paneles que usan controladores externos como FT6336, CST820, etc.

### b) `esp_bsp.c` — Configuración BSP personalizada

- La configuración de pines y buses está adaptada para este PCB específico.
- Se usa `i2c_master_bus` (nuevo driver I²C de ESP-IDF v5+) en lugar del antiguo `i2c_driver_install`.
- El callback `bsp_touch_process_points_cb` ajusta las coordenadas del touch según la rotación de LVGL.

### c) Buffer en PSRAM

```c
.flags = {
    .buff_dma = false,
    .buff_spiram = true,
},
```

El buffer principal de LVGL se asigna en **PSRAM** (SPIRAM). Los buffers de transporte se asignan en **DMA** (SRAM interna). Esto requiere que el proyecto tenga habilitada la PSRAM en la configuración del menú de Arduino/PlatformIO.

### d) `lv_port.c` — Rotación por software

Implementa rotación de framebuffer completa en software (no usa `swap_xy`/`mirror` del panel porque `swap_xy` devuelve `ESP_ERR_NOT_SUPPORTED`). Los algoritmos de rotación reordenan píxeles en bloques de tamaño `trans_size`.

---

## 12. Posibles Problemas y Soluciones

| Problema | Causa | Solución |
|---|---|---|
| Pantalla blanca/negra | Secuencia de inicialización incorrecta o falta de `LV_COLOR_16_SWAP=1` | Verificar `lv_conf.h` y la secuencia de comandos |
| Colores incorrectos (púrpura/verde) | `LV_COLOR_16_SWAP` = 0 | Cambiar a `#define LV_COLOR_16_SWAP 1` |
| Touch no responde | Dirección I²C incorrecta o pines equivocados | Verificar `0x55` y pines GPIO38/39 |
| Tearing evidente | TE no configurado | Asegurar que `EXAMPLE_PIN_NUM_QSPI_TE = GPIO_NUM_42` |
| Error de memoria al compilar | PSRAM no configurada | Habilitar PSRAM en el menú de configuración |
| LCD no enciende tras reset | Reset solo por software (RST=NC) | El `panel_st77922_reset` hace reset por software con comando `0x01` |
| La demo `lv_demo_music()` no se ve | `LV_USE_DEMO_MUSIC` = 0 en `lv_conf.h` | Cambiar a `#define LV_USE_DEMO_MUSIC 1` |

---

## 13. Resumen de Archivos del Proyecto

| Archivo | Propósito |
|---|---|
| `LVGL_Demos.ino` | Punto de entrada Arduino. Configura rotación e inicia el demo |
| `display.h` | Definiciones de resolución (`320×480`), formato de color, y configuración TE |
| `esp_bsp.h` | Definiciones de pines (`EXAMPLE_PIN_NUM_QSPI_*`, `BSP_I2C_*`) y prototipos BSP |
| `esp_bsp.c` | Implementación BSP: init I²C, LCD (QSPI), touch, backlight PWM |
| `esp_lcd_st77922.h` | Macros de configuración SPI/QSPI, definiciones I²C touch, init commands struct |
| `esp_lcd_st77922.c` | Driver LCD + touch para ST77922 (páginas CMD1/2/3, opcodes QSPI, lectura táctil) |
| `esp_lcd_touch.h` | Framework genérico para touch (estructura, callbacks, funciones públicas) |
| `esp_lcd_touch.c` | Implementación genérica del framework touch |
| `lv_port.h` | Prototipos para integración LVGL (init, add_disp, add_touch, mutex) |
| `lv_port.c` | Implementación de la integración LVGL (flush callback, rotación, lectura táctil) |
| `bsp_err_check.h` | Macros de verificación de errores (assert condicional) |
| `lv_conf.h` | Configuración global de LVGL (color depth, demos, features) |

---

> **Conclusión**: Este proyecto implementa un BSP (Board Support Package) completo y funcional para una placa ESP32-S3 con pantalla ST77922 de 3.5" QSPI y táctil capacitivo I²C. La complejidad principal reside en que el ST77922 es un controlador relativamente nuevo que requiere una secuencia de inicialización extensa en 3 páginas de comandos, un driver touch I²C propio, y opcodes QSPI no estándar. La integración con LVGL v8 incluye rotación por software, sincronización TE anti-tearing, y doble buffer DMA+PSRAM para rendimiento.
