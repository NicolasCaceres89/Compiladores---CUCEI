Sin escribir código completo, aquí está la justificación conceptual de qué estructuras de C++ encajan en cada módulo:

A. Módulo Léxico (include/lexico/ y src/lexico/)
	Token (token.hpp): 
		Se representa como una struct ligera. Puedes usar un enum class TokenType para catalogar cada elemento (MNEMONICO, REGISTRO, COMA, NUMERO, DESCONOCIDO).
	lexer_maps.hpp / cpp:
		Se utiliza std::unordered_map o std::unordered_map.Usar std::unordered_map da búsquedas en tiempo constante promediado $O(1)$, ideal para comparar lexemas de mnemónicos ("MOV", "ADD") y registros ("AL", "R1").
	afd.hpp / cpp:
		La clase del AFD recibe un std::string o un flujo (std::istream), itera carácter por carácter con un bucle y una matriz/switch de estados, agrupa el sub-string y consulta el std::unordered_map para determinar si es un mnemónico/registro o un identificador/error.

B. Módulo Sintetizador (include/sintetizador/)
	Instruction (instruction.hpp):
		Una struct que representa el Objeto Instrucción.Los campos como la dirección o el valor inmediato pueden representarse usando std::variant o std::optional (por ejemplo, si una instrucción no usa dirección de memoria).
	Parser (parser.cpp):
		Recibe un std::vector constante y devuelve la Instruction.

C. Módulo Unidad de Control (include/control_unit/)
	MooreMachine (moore_fsm.cpp):
		La máquina de estados de Moore en C++ se implementa habitualmente mediante un enum class State y un bloque switch(current_state) o una tabla de transiciones usando arreglos/mapas.
		En cada estado, se emite una instancia de MicroOp (o un std::vector).

Para conectar las carpetas en C++, la estructura de un CMakeLists.txt modular se organiza creando bibliotecas estáticas (STATIC) para cada capa:
    lexico_lib: Compila src/lexico/*.cpp.
    sintetizador_lib: Compila src/sintetizador/*.cpp (depende de lexico_lib).
    control_unit_lib: Compila src/control_unit/*.cpp (depende de sintetizador_lib).
    ejecutable_principal: Vincula las 3 bibliotecas con main.cpp.

D. Formatos aceptados por el parser
	MOV registro, numero: carga en el registro el dato de la direccion indicada.
	MOV registro, registro: copia el registro fuente al registro destino.
	ADD registro, registro: suma el registro fuente al registro destino.
	STO numero: guarda el contenido de AL en la direccion indicada.
	END: termina sin operandos.

La coma es obligatoria entre los operandos de MOV y ADD. El AFD rechaza
operandos faltantes, separadores faltantes y componentes adicionales.

La unidad de control recibe el objeto de instruccion ya analizado. Cada llamada
a MooreFSM::siguientePaso emite la microoperacion del estado actual y avanza:
MOV directo lee memoria y carga el registro; MOV por registro copia el valor;
ADD suma los registros; STO escribe AL en memoria; END termina sin emitir
microoperaciones.