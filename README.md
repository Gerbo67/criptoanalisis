# Análisis y Evolución de Técnicas de Encriptación en Software 🛡️💻

Este proyecto se desarrolla en el marco de la asignatura **Vanguardia Guerrero** impartida en la **Universidad Cuauhtémoc Querétaro**. El presente repositorio documenta el estudio y la implementación de diversos tipos de algoritmos de encriptación y su evolución histórica y técnica.

## Resumen del Proyecto 📄

Este repositorio alberga implementaciones en **C++** y análisis detallados de múltiples algoritmos de encriptación. El propósito fundamental es examinar los principios operativos de técnicas criptográficas fundamentales, incluyendo:

* **Cifrados Clásicos:** (e.g., César, Vigenère)
* **Criptografía Simétrica:** (e.g., DES, AES)
* **Criptografía Asimétrica:** (e.g., RSA)
* **Funciones Hash y Firmas Digitales**

El enfoque combina la comprensión teórica con la aplicación práctica a través del desarrollo de código, permitiendo una evaluación de su robustez y posibles debilidades.

## Contenido del Repositorio 🤓

* **Código Fuente:** Implementaciones funcionales en C++ de los algoritmos de encriptación estudiados.
* **Ejemplos de Aplicación:** Demostraciones y casos de prueba para ilustrar el funcionamiento de cada algoritmo.
* **Documentación Técnica:** Descripciones concisas sobre la operatividad, fundamentos matemáticos e historia de cada método.
* **Análisis de Seguridad:** Evaluaciones sobre la fortaleza criptográfica y la eficiencia computacional de las técnicas implementadas.

---

## Pruebas de Fuerza Bruta con IA 🤖

Para explorar la resiliencia de algunos de los algoritmos implementados frente a ataques de **fuerza bruta asistidos por Inteligencia Artificial**, se incluye un módulo de prueba en la carpeta `modelpython`. Este módulo contiene un *script* principal (`main.py`) y un archivo de requisitos (`requirements.txt`) para configurar el entorno.

### Configuración y Ejecución del Módulo de IA

Para ejecutar el servidor de prueba de fuerza bruta, sigue estos pasos:

1.  **Crea un entorno virtual:**
    ```bash
    python -m venv venv
    ```
2.  **Activa el entorno virtual:**
    * **En Windows:**
        ```bash
        .\venv\Scripts\activate
        ```
    * **En macOS/Linux:**
        ```bash
        source venv/bin/activate
        ```
3.  **Instala las dependencias:**
    ```bash
    pip install -r modelpython/requirements.txt
    ```
4.  **Ejecuta el servidor:**
    ```bash
    python modelpython/main.py
    ```
    Una vez iniciado, podrás acceder al servidor en `http://localhost:8000`.

---

## Evolución de la Criptografía ⏳

La criptografía es un campo en constante desarrollo, adaptándose continuamente para contrarrestar nuevas amenazas a la seguridad de la información. Este proyecto busca ilustrar dicha progresión, desde los métodos criptográficos elementales hasta los algoritmos contemporáneos que garantizan la confidencialidad, integridad y autenticidad de los datos en el entorno digital.

---

*"La seguridad no es un producto, sino un proceso." - Bruce Schneier*