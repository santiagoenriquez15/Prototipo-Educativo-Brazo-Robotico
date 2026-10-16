# Brazo Robótico Educativo 🤖

Prototipo de brazo robótico educativo desarrollado mediante Arduino Nano, servomotores y piezas fabricadas mediante impresión 3D, como recurso didáctico para la enseñanza de la robótica en Nariño, Colombia.

## 📌 Descripción del proyecto

Este prototipo forma parte del proyecto académico **“Desarrollo de Prototipos Robóticos Educativos para la enseñanza en Nariño”**.

El brazo robótico permite explorar conceptos básicos de electrónica, programación, control de movimiento y diseño mecánico mediante la construcción y manipulación de un sistema robótico de pequeña escala.

Su estructura está conformada por piezas diseñadas para impresión 3D y cuatro servomotores que permiten controlar los movimientos de la base, el hombro, el codo y la pinza.

## 🎯 Objetivos

- Facilitar el aprendizaje práctico de la robótica educativa.
- Comprender la integración entre componentes electrónicos, programación y estructuras mecánicas.
- Desarrollar habilidades de diseño, construcción y control de prototipos robóticos.
- Promover el aprendizaje de áreas STEAM: ciencia, tecnología, ingeniería, arte y matemáticas.

## ⚙️ Características del prototipo

- **Microcontrolador:** Arduino Nano.
- **Actuadores:** cuatro servomotores.
- **Control:** cuatro potenciómetros de 10 kΩ.
- **Estructura:** piezas fabricadas mediante impresión 3D.
- **Monitoreo de batería:** medición de voltaje mediante un divisor resistivo e indicadores LED.
- **Programación:** Arduino IDE.

## 🔩 Componentes principales

- Arduino Nano.
- Dos servomotores SG-5010.
- Un servomotor MG90S.
- Un servomotor SG90.
- Cuatro potenciómetros de 10 kΩ.
- Baterías recargables y módulo regulador de voltaje.
- Diodos LED para indicar el estado de la batería.
- Resistencias, cables y elementos de conexión.
- Piezas mecánicas impresas en 3D.

## 🔌 Conexiones electrónicas

La distribución principal de pines del prototipo es la siguiente:

| Elemento | Pin del Arduino Nano |
|---|---|
| Servo de la base | D2 |
| Servo del hombro | D3 |
| Servo del codo | D4 |
| Servo de la pinza | D5 |
| LED verde | D6 |
| LED amarillo | D7 |
| LED rojo | D8 |
| Potenciómetro de la base | A0 |
| Potenciómetro del hombro | A1 |
| Potenciómetro del codo | A2 |
| Potenciómetro de la pinza | A3 |
| Medición del voltaje de batería | A7 |

**Importante:** los servomotores requieren una alimentación adecuada a sus necesidades de corriente. El circuito debe compartir una referencia GND entre el Arduino y la alimentación de los servos.

## 💻 Programación

El programa del prototipo está desarrollado para Arduino Nano y permite:

- Controlar individualmente los cuatro servomotores mediante potenciómetros.
- Ajustar los movimientos dentro de los rangos definidos para cada articulación.
- Suavizar los movimientos para mejorar la respuesta del brazo.
- Supervisar el voltaje de la batería mediante un divisor resistivo.
- Indicar el nivel estimado de batería utilizando tres LED.

## 📂 Recursos educativos

En este repositorio se encontraran los siguientes materiales:

- **Código fuente:** programa del Arduino Nano.
- **Modelos 3D:** archivos STL para la fabricación de las piezas.
- **Circuito eléctrico:** esquema de conexiones de los componentes.
- **Cartilla educativa:** guía de armado, conexiones y uso.
- **Tutoriales:** recursos audiovisuales para facilitar la construcción y programación.

## 🧑‍🏫 Aplicación educativa

El prototipo está pensado como una herramienta de aprendizaje práctico para estudiantes e instituciones educativas. Su construcción permite relacionar el diseño mecánico, la electrónica y la programación en una actividad de robótica aplicada.

## 📄 Créditos y licencias

Las piezas, el código y los materiales educativos de este proyecto tendrán sus respectivas atribuciones y condiciones de uso.

La pinza se basa en un diseño publicado en Thingiverse por **SuperbTech**, bajo licencia Creative Commons Attribution (CC BY), con modificaciones realizadas para su adaptación al prototipo. Se incluirá la atribución completa y el enlace al diseño original en la documentación correspondiente.

Los términos de uso de los demás recursos se especificarán de acuerdo con su autoría y las licencias aplicables.

---

**Proyecto:** Desarrollo de Prototipos Robóticos Educativos para la enseñanza en Nariño.

**Área:** Ingeniería Electrónica y Robótica Educativa.

**Ubicación:** Nariño, Colombia.
