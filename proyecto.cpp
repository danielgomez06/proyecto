#include <iostream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <string>
#include <limits>
#include <fstream>
#include <algorithm> 
#include <unordered_map>
#include <tuple>
#include <ctime>
#include <queue>
#include <random>
#include <climits>
#include <numeric> 
#include <chrono>


using namespace std;

//struct de accesorio no modificable

struct accesorio {
    string nombre;
    string tipo;
    string descripcion;
    int valor;
};

//struct de usos modificables

struct usos {
    accesorio* accesorioPtr;
    int usos;
    bool operator==(const struct usos& otro) const {
        return accesorioPtr->nombre == otro.accesorioPtr->nombre && usos == otro.usos;
    }
};

//struct de mochila

struct mochila {
    int capacidad = 3;
    vector<usos> accesoriosConUsos;
};

//struct de zombie 

struct zombie {
    string nombre;
    string descripcion;
    int ataque;
};

//struct de zombie con fortaleza

struct zombieFortaleza {
    struct zombie* zombie;
    int fortaleza;
};

//struct de soldado

struct soldado {
    string nombre;
    int salud;
    struct mochila mochila;
};

//struct de estacion

struct estacion {
    string nombre;
    int cantidadZombies ;
    estacion* siguiente;
	vector<zombieFortaleza> zombies;
    vector<struct estacionesAdyacentes> ady;
	bool cura = false;

};

struct estacionesAdyacentes {
	string destino="";
	int distancia;
}; 

struct CompararEstaciones {
    bool operator()(const pair<estacion*, int>& a, const pair<estacion*, int>& b) {
        return a.second > b.second;     
    }
};

//struct de bitacora

struct bitacora {
    string estacion;
    vector<string> eventos;
};

//funciones generales

void agregarAccesorio(mochila& mochila, usos accesorio) {
    // Buscar si el accesorio ya existe en la mochila
    auto it = std::find(mochila.accesoriosConUsos.begin(), mochila.accesoriosConUsos.end(),
        accesorio);  // Ahora buscamos el accesorio completo

    // Si el accesorio ya está en la mochila
    if (it != mochila.accesoriosConUsos.end()) {
        // Sumar los usos al accesorio existente
        it->usos += accesorio.usos;
        cout << "\nLos usos del accesorio " << accesorio.accesorioPtr->nombre << " han sido actualizados. Usos totales: " << it->usos << endl;
    }
    else if (mochila.accesoriosConUsos.size() < mochila.capacidad) {
        // Si no está en la mochila y hay espacio, lo agregamos
        mochila.accesoriosConUsos.push_back({ accesorio.accesorioPtr, accesorio.usos });
        cout << "\nAccesorio agregado con éxito." << endl;
        mochila.capacidad--;  // Reducimos la capacidad de la mochila
        cout << "\nEspacio restante en la mochila: " << mochila.capacidad << endl;
    }
    else {
        // Si la mochila está llena
        cout << "La mochila está llena. Elimine un accesorio para agregar uno nuevo." << endl;
    }
}

int leerEntero(const string& mensaje) {
    int valor;
    while (true) {
        cout << mensaje;
        cin >> valor;
        if (cin.fail()) { 
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            return valor;
        }
    }
}

char leerChar(const string& mensaje) {
	char valor;
	while (true) {
		cout << mensaje;
		cin >> valor;
		if (cin.fail()) { 
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		else {
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
			return valor; 
		}
	}
}

string leerString(const string& mensaje) {
	string valor;
	cout << mensaje;
	cin.ignore();
	getline(cin, valor);
	return valor;
}

int generarNumeroAleatorio(int max) {
    // Crear un generador de números aleatorios basado en una semilla aleatoria
    std::random_device rd;
    std::mt19937 gen(rd());  // Generador Mersenne Twister inicializado con la semilla
    std::uniform_int_distribution<> distrib(0, max);  // Distribuidor para números entre 0 y max

    return distrib(gen);  // Genera y devuelve el número aleatorio
}  

//soldado funciones 

soldado crearSoldado(string name, int vida) {
    soldado soldadoNuevo;
    mochila mochilaNueva;
    soldadoNuevo.nombre = name;
    soldadoNuevo.salud = vida;
    soldadoNuevo.mochila = mochilaNueva;
    soldadoNuevo.mochila.accesoriosConUsos = {};

    return soldadoNuevo;
};

void anadirSoldado(vector<soldado>& soldados, string name, int vida) {
    while (vida <= 0 || vida > 100) {
		vida =leerEntero("La salud debe estar entre 0 y 100. Ingrese un valor válido: ");
    }
    soldado soldadoNuevo = crearSoldado(name, vida);
    soldados.push_back(soldadoNuevo);
    cout << "\nSoldado agregado con éxito." << endl;
}

void mostrarSoldados(const vector<soldado>& soldados) {
    int numero = 1;
    if (soldados.empty()) {  
        cout << "No hay soldados creados." << endl;
    }
    else {
        for (const auto& s : soldados) { 
            cout << "\nSoldado #" << numero << endl;
            cout << "Nombre: " << s.nombre << endl;
            cout << "Salud: " << s.salud << endl;
            cout << "Capacidad de la mochila: " << s.mochila.capacidad << endl;

            if (s.mochila.accesoriosConUsos.empty()) {
                cout << "Accesorios: Ninguno" << endl;
            }
            else {
                cout << "Accesorios: " << endl;
                for (const auto& a : s.mochila.accesoriosConUsos) {
                    if (a.accesorioPtr != nullptr) {
                        cout << "Nombre: " << a.accesorioPtr->nombre << " - Usos: " << a.usos << endl;
                    }
                }
            }

            cout << endl;
            numero++;
        }
    }
}

void eliminarSoldado(vector<soldado>& soldados) {
    mostrarSoldados(soldados);
    
    int soldadoAEliminar;
    soldadoAEliminar = leerEntero("Seleccione el numero del soldado que desea eliminar?:");
    while (soldadoAEliminar<0 || soldadoAEliminar > soldados.size()) {
		soldadoAEliminar = leerEntero("Seleccione un soldado que este creado: ");
    }

    soldados.erase(soldados.begin() + soldadoAEliminar - 1);
    cout << "Soldado eliminado con exito." << endl;

    char opcion = leerChar("Desea eliminar otro soldado? (s/n): ");
    if (opcion == 's') {
        eliminarSoldado(soldados);
    }
    else {
        return;
    }
}

void editarSoldado(vector<soldado>& soldados) {
    mostrarSoldados(soldados);

    int soldadoAEditar;
    soldadoAEditar = leerEntero("Seleccione el numero del soldado que desea editar: ");
    while (soldadoAEditar < 1 || soldadoAEditar > soldados.size()) {
        soldadoAEditar = leerEntero("Seleccione un soldado que este creado: ");
    }

    string nombre;
    int salud;
    cout << "El soldado a editar es: "
        << soldados[soldadoAEditar - 1].nombre
        << " con salud: "
        << soldados[soldadoAEditar - 1].salud << ".\n";


    cout << "Ingrese el nuevo nombre del soldado: ";
    getline(cin, nombre);

    salud = leerEntero("Ingrese la nueva salud del soldado: ");
    while (salud < 0 || salud > 100) {
        salud = leerEntero("La salud debe estar entre 0 y 100. Ingrese un valor válido: ");
    }

    soldados[soldadoAEditar - 1] = crearSoldado(nombre, salud);

    cout << "\nSoldado editado con exito." << endl;
    char opcion = leerChar("Desea editar otro soldado? (s/n): ");
    if (opcion == 's') {
        editarSoldado(soldados);
    }
}

void mostrarEquipos(vector<vector<soldado>>& soldados) {
    int length = soldados.size();
    if (length <= 0)
    {
        cout << "\nNo hay equipos creados." << endl;
    }
    else
    {
        for (int i = 0; i < length; i++)
        {
            cout << "\nEquipo #" << i + 1 << endl;
			mostrarSoldados(soldados[i]);
        }
    }


}

void crearEquipo(vector<vector<soldado>>& equiposCreados) {
    vector<soldado> equipo;
    int opcion = 0;
    int cantSoldados = leerEntero("Ingrese la cantidad de soldados que desea agregar al equipo: ");

	while (cantSoldados < 0 || cantSoldados>5) {
		cantSoldados = leerEntero("La cantidad de soldados debe ser entre 0 y 5. Ingrese un valor válido: ");
	}
    
        for (int i = 0; i < cantSoldados; i++)
        {
            string nombre;
            int salud;
     
            cout << "\nIngrese el nombre del soldado: ";
            getline(cin, nombre);
			cout << nombre << endl;
            salud = leerEntero("Ingrese la salud del soldado: ");
            anadirSoldado(equipo, nombre, salud);
        }
        equiposCreados.push_back(equipo);
        cout << "\nEquipo creado con éxito." << endl;

        char seguir;
        seguir = leerChar("Desea crear otro equipo? (s/n): ");
        if (seguir == 's') {
            crearEquipo(equiposCreados);
        }
        else {
            return;
        }


}

void editarEquipo(vector<vector<soldado>>& equiposCreados) {
	if (equiposCreados.size() <= 0) {
		cout << "\nNo hay equipos creados." << endl;
		return;
	}
	mostrarEquipos(equiposCreados);
	
    int equipoAEditar;
	equipoAEditar = leerEntero("\nSeleccione el número del equipo que desea editar: ");
	while (equipoAEditar < 1 || equipoAEditar > equiposCreados.size()) {
		equipoAEditar = leerEntero("Seleccione un equipo que este creado: ");
	}
	
    vector<soldado> equipo = equiposCreados[equipoAEditar - 1];
	editarSoldado(equipo);
	equiposCreados[equipoAEditar - 1] = equipo;
	
    cout << "\nEquipo editado con éxito." << endl;
	
    char opcion = leerChar("Desea editar otro equipo? (s/n): ");
	if (opcion == 's') {
		editarEquipo(equiposCreados);
	}
	else {
		return;
	}

}

void eliminarSoldadoDeEquipo(vector<vector<soldado>>& equiposCreados) {
    if (equiposCreados.size() <= 0) {
        cout << "\nNo hay equipos creados." << endl;
        return;
    }

    mostrarEquipos(equiposCreados);

    int equipoAEditar;
    equipoAEditar = leerEntero("Seleccione el numero del equipo del que desea eliminar un soldado: ");
    while (equipoAEditar < 1 || equipoAEditar > equiposCreados.size()) {
        equipoAEditar = leerEntero("Seleccione un equipo que este creado: ");
    }

    vector<soldado>& equipo = equiposCreados[equipoAEditar - 1];
    eliminarSoldado(equipo);

    cout << "\nSoldado eliminado del equipo con éxito." << endl;

    char opcion = leerChar("Desea eliminar otro soldado de un equipo? (s/n): ");
    if (opcion == 's') {
        eliminarSoldadoDeEquipo(equiposCreados);
    } else {
        return;
    }
}

int numeroSoldadoMenosVida(vector<soldado>soldados){
	int vida = INT_MAX;
	int numero = 0;
	for (int i = 0; i < soldados.size(); i++) {
		if (soldados[i].salud < vida) {
			vida = soldados[i].salud;
			numero = i;
		}
	}
	return numero;
}

void recogerArmaDeSoldadoMuerto(vector<soldado>&equipo,soldado& soldadoMuerto) {
	bool espacioEnMochila = false;
    //buscar el soldado muerto
	int numero = 0;
	for (int i = 0; i < equipo.size(); i++) {
		if (equipo[i].nombre == soldadoMuerto.nombre) {
			numero = i;
		}
	}
	//recoger las armas del soldado muerto
	int cantidadAcc = soldadoMuerto.mochila.accesoriosConUsos.size();
    
        for (int i = 0; i < equipo.size(); i++)
        {
			soldado soldadoActual = equipo[i];
            if (soldadoActual.mochila.capacidad > 0) {
                espacioEnMochila = true;
            }
        }
		if (espacioEnMochila) {
            for (int i = 0; i < equipo.size(); i++)
            {
                if (i != numero) {
                    soldado soldadoActual = equipo[i];
                    if (soldadoActual.mochila.capacidad > 0) {
						while (soldadoActual.mochila.capacidad >= cantidadAcc){
                            for (int j = 0; j < cantidadAcc; j++) {
                                usos accesorio = soldadoMuerto.mochila.accesoriosConUsos[j];
                                cout << "Accesorio recogido con éxito por el soldado: " << soldadoActual.nombre << ", recogio: " << accesorio.accesorioPtr->nombre << endl;
                                agregarAccesorio(soldadoActual.mochila, accesorio);
                            }
                        }
                        
                    }
                }
            }
		}
		else {
			cout << "No hay espacio en la mochila para recoger las armas." << endl;
		}

	
}



//zombie funciones


vector<zombie*> crearTiposZombies() {
    vector<zombie*> zombies;

    
    zombies.push_back(new zombie{ "Corredor",
        "Estos zombies son extremadamente rápidos y ágiles, capaces de perseguir a sus presas a altas velocidades. Podrían ser el resultado de una mutación que aceleró su metabolismo.",
        10 });

    zombies.push_back(new zombie{ "Zombie Tanque",
        "Grandes y lentos, estos zombies son casi indestructibles. Su fuerza bruta los convierte en una amenaza formidable, capaces de derribar puertas y paredes, su debilidad es la luz, la cual no pueden soportar.",
        25 });

    zombies.push_back(new zombie{ "Zombie Inteligente",
        "Estos zombies han conservado parte de su inteligencia, lo que les permite planificar emboscadas y utilizar herramientas. Podrían ser una evolución de los zombies, o el resultado de una infección en individuos con un coeficiente intelectual alto. Sin embargo, son frágiles y lentos.",
        15 });

    zombies.push_back(new zombie{ "Zombie Infectado por Hongos",
        "Estos zombies presentan características fúngicas, como esporas que se propagan por el aire y la capacidad de controlar a otros organismos.",
        20 });

    zombies.push_back(new zombie{ "Zombie Bioluminiscente",
        "Estos zombies emiten una luz bioluminiscente, lo que los hace fácilmente visibles en la oscuridad. Esta característica podría ser un efecto secundario de la mutación o una adaptación para comunicarse entre ellos.",
        18 });

    return zombies;
}

void liberarMemoria(vector<zombieFortaleza>& zombies) {
    for (auto& z : zombies) {
        delete z.zombie; 
        z.zombie = nullptr; 
    }
}

vector<zombieFortaleza> crearZombiesConFortaleza() {
    vector<zombie*> zombies = crearTiposZombies();
    vector<zombieFortaleza> zombiesConFortaleza;

    for (auto& z : zombies) {
        zombieFortaleza zFortaleza;
        zFortaleza.zombie = z;  

        if (z->nombre == "Corredor") {
            zFortaleza.fortaleza = 50;
        }
        else if (z->nombre == "Zombie Tanque") {
            zFortaleza.fortaleza = 100;
        }
        else if (z->nombre == "Zombie Inteligente") {
            zFortaleza.fortaleza = 30;
        }
        else if (z->nombre == "Zombie Infectado por Hongos") {
            zFortaleza.fortaleza = 40;
        }
        else if (z->nombre == "Zombie Bioluminiscente") {
            zFortaleza.fortaleza = 35;
        }

        zombiesConFortaleza.push_back(zFortaleza);
    }

    return zombiesConFortaleza;
}

void zombiesUsuario(vector<zombieFortaleza>& zombiesCreados) {
    vector<zombieFortaleza> zombies = crearZombiesConFortaleza();
    int opcion;
    cout << "\n--- Crear Tipos de Zombies ---" << endl;
    cout << "1. Corredores " << endl;
    cout << "2. Zombies Tanque" << endl;
    cout << "3. Zombies Inteligentes " << endl;
    cout << "4. Zombies Infectados por Hongos " << endl;
    cout << "5. Zombies Bioluminiscente " << endl;
    opcion = leerEntero("Ingrese opción: ");

    while (opcion<0 || opcion> zombies.size()) {
        opcion = leerEntero("Ingrese opción valida: ");
        
    }
    zombiesCreados.push_back(zombies[opcion - 1]);
    cout << "\nZombie creado con éxito." << endl;
    char opcion1 = leerChar("Desea crear otro zombie? (s/n): ");
    if (opcion1 == 's') {
		zombiesUsuario(zombiesCreados); 
    }
    else {
        return;
    }
}

void mostrarZombies(const vector<zombieFortaleza>& zombies) {
    int indice = 0;
    if (!zombies.empty()) {
        for (const auto& z : zombies) {
            if (z.zombie == nullptr) {
                continue; // Saltar zombies nulos
            }

            indice++;
            cout << "\nZombie #" << indice << endl;
            cout << "Nombre: " << z.zombie->nombre << endl;
            cout << "Descripción: " << z.zombie->descripcion << endl;
            cout << "Ataque: " << z.zombie->ataque << endl;
            cout << "Fortaleza: " << z.fortaleza << endl;
            cout << endl;
        }

        if (indice == 0) {
            cout << "No hay zombies válidos para mostrar." << endl;
        }
    }
    else {
        cout << "No hay zombies creados." << endl;
    }
}

int zombieMayorVida(const vector<zombieFortaleza>& zombies){
    int mayor = 0;
	for (int i = 0; i < zombies.size(); i++) {
		if (zombies[i].fortaleza > mayor) {
			mayor = zombies[i].fortaleza;
		}
	}
	return mayor;
}

vector<zombieFortaleza> zombiesAleatorios() {
	vector<zombieFortaleza> zombies = crearZombiesConFortaleza();
	vector<zombieFortaleza> zombiesAleatorios;
	int cantidadZombies = generarNumeroAleatorio(5);
    while (cantidadZombies < 1) {
		cantidadZombies = generarNumeroAleatorio(5);

    }
	for (int i = 0; i < cantidadZombies; i++) {
		int indice = generarNumeroAleatorio(zombies.size());

		zombiesAleatorios.push_back(zombies[indice]);
	}
	return zombiesAleatorios;
}

//accesorios funciones

accesorio crearAccesorio(string name, string tipo, string descripcion, int valor) {
    accesorio accesorioNuevo;
    accesorioNuevo.nombre = name;
    accesorioNuevo.tipo = tipo;
    accesorioNuevo.valor = valor;
    return accesorioNuevo;
};

vector<accesorio*> crearVectorArmas() {

    vector<accesorio*>armas;
    armas.push_back(new accesorio{ "Pistola", "Arma de fuego",
     "Práctica para espacios reducidos, pero con limitada capacidad de munición.", 30 });

    armas.push_back(new accesorio{ "Escopeta", "Arma de fuego",
        "Ideal para disparos a corta distancia y causar daño masivo.", 50 });

    armas.push_back(new accesorio{ "Fusil de Asalto", "Arma de fuego",
        "Mayor alcance y capacidad de fuego, pero requiere más munición.", 35 });

    armas.push_back(new accesorio{ "Rifle de Francotirador", "Arma de fuego",
        "Perfecto para eliminar amenazas a larga distancia, pero es pesado y requiere precisión.", 55 });

    armas.push_back(new accesorio{ "Granada", "Arma arrojadiza",
        "Útil para eliminar grupos de zombies o crear distracciones.", 100 });

    armas.push_back(new accesorio{ "Cóctel Molotov", "Arma arrojadiza",
        "Ideal para incendiar a los zombies y crear barreras de fuego.", 40 });

    armas.push_back(new accesorio{ "Ballesta", "Arma de proyectiles",
        "Silenciosa y precisa, pero requiere munición especial.", 30 });

    armas.push_back(new accesorio{ "Tirachinas", "Arma de proyectiles",
        "Económico y fácil de usar, pero con alcance limitado.", 25 });

    armas.push_back(new accesorio{ "Machete", "Arma blanca",
        "Ideal para cortar extremidades y causar daño masivo.", 50 });

    armas.push_back(new accesorio{ "Bate de Béisbol", "Arma contundente",
        "Fácil de encontrar y efectivo para aturdir a los zombies.", 40 });

    armas.push_back(new accesorio{ "Martillo", "Arma contundente",
        "Ideal para aplastar cráneos.", 45 });

    armas.push_back(new accesorio{ "Tubería", "Arma contundente",
        "Versátil y fácil de improvisar.", 25 });

    armas.push_back(new accesorio{ "Objeto Punzante", "Arma improvisada",
        "Clavos, tijeras, destornilladores, etc.", 10 });

    return armas;
};

vector<accesorio*> crearVectorDefensas() {
    vector<accesorio*> defensas;
    defensas.push_back(new accesorio{
        "Botas Antideslizantes", "Defensa",
        "Botas con suelas especiales que aumentan la tracción y evitan que los soldados resbalen en persecuciones.", 35
        });

    defensas.push_back(new accesorio{
        "Linterna Táctica", "Defensa",
        "Una linterna de alta potencia que emite un haz de luz cegador, debilitando a los zombies tanques.", 50
        });

    defensas.push_back(new accesorio{
        "Inhibidor Ultrasónico", "Defensa",
        "Emite ondas ultrasónicas que desorientan y dificultan la coordinación de zombies inteligentes.", 40
        });

    defensas.push_back(new accesorio{
        "Filtro de Aire Avanzado", "Defensa",
        "Un filtro especializado que bloquea esporas fúngicas para evitar infecciones y control mental.", 45
        });

    defensas.push_back(new accesorio{
        "Camuflaje Nocturno", "Defensa",
        "Un traje con material que absorbe luz y reduce la visibilidad en la oscuridad, evitando ser detectado por zombies bioluminiscentes.", 30
        });


    return defensas;
};

vector<accesorio*> crearVectorSalud() {
    
    vector<accesorio*> salud;

    salud.push_back(new accesorio{
      "Botiquín de Primeros Auxilios", "Salud",
      "Contiene vendajes, antisépticos y analgésicos para curar heridas menores.", 50
        });

    salud.push_back(new accesorio{
        "Medicina Regenerativa", "Salud",
        "Un suero que acelera la curación de heridas y restaura la salud de forma rápida.", 75
        });

    salud.push_back(new accesorio{
        "Ración de Comida Energética", "Salud",
        "Comida diseñada para proporcionar energía rápida y restaurar salud a corto plazo.", 25
        });

    salud.push_back(new accesorio{
        "Agua Purificada", "Salud",
        "Ayuda a hidratar y revitalizar a los soldados, restaurando un poco de salud.", 15
        });

    salud.push_back(new accesorio{
        "Paquete de Vendas", "Salud",
        "Vendas que ayudan a detener la hemorragia y curar heridas.", 30
        });

    return salud;
}

void mostrarAccesorios(vector<accesorio*> accesorios) {
    int indice = 1;
	if (accesorios.size() <= 0) {
		cout << "No hay accesorios creados." << endl;
		return;
    }
    else {
        for (auto& a : accesorios) {
            cout << "\nAccesorio #" << indice << endl;
            cout << "Nombre: " << a->nombre << endl;
            cout << "Tipo: " << a->tipo << endl;
            cout << "Descripción: " << a->descripcion << endl;
            cout << "Valor: " << a->valor << endl;
            cout << endl;
            indice++;
        }
    }
}

void mostrarAccesoriosConUsos(vector<usos> accesorios) {
    int indice = 1;
	if (accesorios.size() <= 0) {
		cout << "No hay accesorios creados." << endl;
		return;
    }
    else {
        for (auto& a : accesorios) {
            cout << "\nAccesorio #" << indice << endl;
            cout << "Nombre: " << a.accesorioPtr->nombre << endl;
            cout << "Tipo: " << a.accesorioPtr->tipo << endl;
            cout << "Descripción: " << a.accesorioPtr->descripcion << endl;
            cout << "Valor: " << a.accesorioPtr->valor << endl;
            cout << "Usos: " << a.usos << endl;
            cout << endl;
            indice++;
        }
    }
}

void armasUsuarioCrear(vector<usos>& armasCreadas) {
    vector<accesorio*> armas = crearVectorArmas();
    mostrarAccesorios(armas);
    int opcionArma;
    int contadorUsos;
    char seguir;
	opcionArma =leerEntero("Seleccione el número del arma que desea agregar: ");
    while (opcionArma < 1 || opcionArma > armas.size()) {
		opcionArma =leerEntero("Opción no válida. Inténtelo de nuevo: ");
    }
	contadorUsos = leerEntero("Ingrese la cantidad de usos del arma: ");
    while (contadorUsos < 0) {
		contadorUsos =leerEntero("La cantidad de usos debe ser mayor o igual a 0. Ingrese un valor válido: ");
    }
    accesorio* arma = armas[opcionArma - 1];
    armasCreadas.push_back({ arma, contadorUsos });
    cout << "\nArma creada con exito" << endl;
    mostrarAccesoriosConUsos(armasCreadas);
    

	seguir = leerChar("Seguir creando armas? (s/n): ");  

	if (seguir == 's') {
		armasUsuarioCrear(armasCreadas);
	}
	else {
		return;
	}

}

void defensasUsuarioCrear(vector<usos>& defensasCreadas) {
    vector<accesorio*> defensas = crearVectorDefensas();
    mostrarAccesorios(defensas);
    int opcionDefensa;
    int contadorUsos;
    char seguir;

    opcionDefensa = leerEntero("Seleccione el número de la defensa que desea agregar: ");
    while (opcionDefensa < 1 || opcionDefensa > defensas.size()) {
        opcionDefensa = leerEntero("Opción no válida. Inténtelo de nuevo: ");
    }

    contadorUsos = leerEntero("Ingrese la cantidad de usos de la defensa: ");
    while (contadorUsos < 0) {
        contadorUsos = leerEntero("La cantidad de usos debe ser mayor o igual a 0. Ingrese un valor válido: ");
    }

    accesorio* defensa = defensas[opcionDefensa - 1];
    defensasCreadas.push_back({ defensa, contadorUsos });
    cout << "\nDefensa creada con éxito" << endl;
    mostrarAccesoriosConUsos(defensasCreadas);

    seguir = leerChar("¿Seguir creando defensas? (s/n): ");
    if (seguir == 's') {
        defensasUsuarioCrear(defensasCreadas);
    }
    else {
        return;
    }
}

void saludUsuarioCrear(vector<usos>& saludCreados) {
    vector<accesorio*> salud = crearVectorSalud();
    mostrarAccesorios(salud);
    int opcionSalud;
    int contadorUsos;
    char seguir;

    opcionSalud = leerEntero("Seleccione el número del accesorio de salud que desea agregar: ");
    while (opcionSalud < 1 || opcionSalud > salud.size()) {
        opcionSalud = leerEntero("Opción no válida. Inténtelo de nuevo: ");
    }

    contadorUsos = leerEntero("Ingrese la cantidad de usos del accesorio de salud: ");
    while (contadorUsos < 0) {
        contadorUsos = leerEntero("La cantidad de usos debe ser mayor o igual a 0. Ingrese un valor válido: ");
    }

    accesorio* saludItem = salud[opcionSalud - 1];
    saludCreados.push_back({ saludItem, contadorUsos });
    cout << "\nAccesorio de salud creado con éxito" << endl;
    mostrarAccesoriosConUsos(saludCreados);

    seguir = leerChar("¿Seguir creando accesorios de salud? (s/n): ");
    if (seguir == 's') {
        saludUsuarioCrear(saludCreados);
    }
    else {
        return;
    }
}

void modificarAccesorio(vector<usos>& accesorios) {
    mostrarAccesoriosConUsos(accesorios);

    int opcionModificar;
    int contadorUsos;

    opcionModificar = leerEntero("Seleccione el número del accesorio que desea modificar: ");

    while (opcionModificar < 1 || opcionModificar > accesorios.size()) {
        opcionModificar = leerEntero("Opción no válida. Inténtelo de nuevo: ");
    }

    contadorUsos = leerEntero("Ingrese la cantidad de usos nueva del accesorio: ");

    while (contadorUsos < 0) {
        contadorUsos = leerEntero("La cantidad de usos debe ser mayor o igual a 0. Ingrese un valor válido: ");
    }

    accesorios[opcionModificar - 1].usos = contadorUsos;

    cout << "Accesorio modificado con éxito. Nueva información:" << endl;
    mostrarAccesoriosConUsos(accesorios);  
}

void eliminarAccesorio(vector<usos>& accesorios) {
    mostrarAccesoriosConUsos(accesorios);
    int opcionEliminar;
    opcionEliminar = leerEntero("Ingrese el accesorio que quiere eliminar");
    while (opcionEliminar < 1 || opcionEliminar > accesorios.size()) {
		opcionEliminar = leerEntero("Opción no válida. Inténtelo de nuevo: ");
    }
    accesorios.erase(accesorios.begin() + opcionEliminar - 1);
    cout << "Accesorio eliminado con éxito." << endl;
}

void limpiarMemoria(vector<usos>& accesorios) {
	for (auto& a : accesorios) {
		delete a.accesorioPtr;
		a.accesorioPtr = nullptr;
	}
}

//mochila funciones

void agregarAccesorioCualquierSoldado(vector<vector<soldado>>& equiposCreados, vector<usos>& accesoriosCreados) {
    if (equiposCreados.empty()) {
        cout << "\nNo hay equipos creados." << endl;
        return;
    }
    if (accesoriosCreados.empty()) {
        cout << "\nNo hay accesorios creados." << endl;
        return;
    }

    // Mostrar equipos y soldados
    cout << "\n--- Equipos Disponibles ---" << endl;
    mostrarEquipos(equiposCreados);

    int equipoSeleccionado = leerEntero("Seleccione el número del equipo: ");
    while (equipoSeleccionado < 1 || equipoSeleccionado > equiposCreados.size()) {
        equipoSeleccionado = leerEntero("Número de equipo no válido. Intente nuevamente: ");
    }

    vector<soldado>& equipo = equiposCreados[equipoSeleccionado - 1];
    mostrarSoldados(equipo);

    int soldadoSeleccionado = leerEntero("Seleccione el número del soldado: ");
    while (soldadoSeleccionado < 1 || soldadoSeleccionado > equipo.size()) {
        soldadoSeleccionado = leerEntero("Número de soldado no válido. Intente nuevamente: ");
    }
	if (equipo[soldadoSeleccionado - 1].mochila.capacidad == 0) {
		cout << "\nLa mochila del soldado seleccionado está llena. No se pueden agregar más accesorios." << endl;
		char opcion = leerChar("¿Desea agregar accesorios a otro soldado? (s/n): ");
        if (opcion == 's') {
            agregarAccesorioCualquierSoldado(equiposCreados, accesoriosCreados);

        }
		else {
			return;
		}
	}

    
    mostrarAccesoriosConUsos(accesoriosCreados);

    int accesorioSeleccionado = leerEntero("Seleccione el número del accesorio: ");
    while (accesorioSeleccionado < 1 || accesorioSeleccionado > accesoriosCreados.size()) {
        accesorioSeleccionado = leerEntero("Número de accesorio no válido. Intente nuevamente: ");
    }
	
	if (std::find(equipo[soldadoSeleccionado - 1].mochila.accesoriosConUsos.begin(), equipo[soldadoSeleccionado - 1].mochila.accesoriosConUsos.end(), accesoriosCreados[accesorioSeleccionado - 1]) != equipo[soldadoSeleccionado - 1].mochila.accesoriosConUsos.end()) {
        vector<usos> inventario = equipo[soldadoSeleccionado - 1].mochila.accesoriosConUsos;
		usos accesorio = accesoriosCreados[accesorioSeleccionado - 1];
        for (int i = 0; i < inventario.size(); i++)
        {
            if (inventario[i].accesorioPtr->nombre == accesorio.accesorioPtr->nombre &&
                (accesorio.accesorioPtr->tipo != "Defensa" &&
                    accesorio.accesorioPtr->tipo != "Arma blanca" &&
                    accesorio.accesorioPtr->tipo != "Arma contundente" &&
                    accesorio.accesorioPtr->tipo != "Arma improvisada" 
                    )) {

				cout << "\nEl accesorio ya está en la mochila del soldado seleccionado. Se le sumaran los usos al accesorio" << endl;
				inventario[i].usos += accesorio.usos;
                accesoriosCreados.erase(accesoriosCreados.begin() + accesorioSeleccionado - 1);
				equipo[soldadoSeleccionado - 1].mochila.accesoriosConUsos = inventario;
				cout << "\nAccesorio agregado con éxito." << endl;
				char continuar = leerChar("¿Desea agregar otro accesorio? (s/n): ");
				if (continuar == 's' || continuar == 'S') {
					agregarAccesorioCualquierSoldado(equiposCreados, accesoriosCreados);
				}
				else {
					return;
				}
				return;

			}

        }
	}
    // Agregar accesorio al soldado
    agregarAccesorio(equipo[soldadoSeleccionado - 1].mochila, accesoriosCreados[accesorioSeleccionado - 1]);
    accesoriosCreados.erase(accesoriosCreados.begin() + accesorioSeleccionado - 1);

    cout << "\nAccesorio agregado con éxito.\n";

    char continuar = leerChar("¿Desea agregar otro accesorio? (s/n): ");
    if (continuar == 's' || continuar == 'S') {
        agregarAccesorioCualquierSoldado(equiposCreados, accesoriosCreados);
    }
}

void modificarMochila(vector<vector<soldado>>& equiposCreados) {
    if (equiposCreados.empty()) {
        cout << "\nNo hay equipos creados." << endl;
        return;
    }

    cout << "\n--- Equipos Disponibles ---" << endl;
    mostrarEquipos(equiposCreados);

    int equipoSeleccionado = leerEntero("Seleccione el número del equipo: ");
    while (equipoSeleccionado < 1 || equipoSeleccionado > equiposCreados.size()) {
        equipoSeleccionado = leerEntero("Número de equipo no válido. Intente nuevamente: ");
    }

    vector<soldado>& equipo = equiposCreados[equipoSeleccionado - 1];
    mostrarSoldados(equipo);

    int opcionSoldado = leerEntero("Seleccione el número del soldado que desea modificar: ");
    while (opcionSoldado < 1 || opcionSoldado > equipo.size()) {
        opcionSoldado = leerEntero("Número de soldado no válido. Intente nuevamente: ");
    }

    // Verificar si el soldado tiene accesorios en su mochila
    vector<usos>& accesorios = equipo[opcionSoldado - 1].mochila.accesoriosConUsos;
    if (accesorios.empty()) {
        cout << "\nLa mochila del soldado seleccionado está vacía. No hay accesorios para modificar." << endl;
        return;
    }

    modificarAccesorio(accesorios);
}

void eliminarDeMochila(vector<vector<soldado>>& equiposCreados) {
    if (equiposCreados.empty()) {
        cout << "\nNo hay equipos creados." << endl;
        return;
    }

    cout << "\n--- Equipos Disponibles ---" << endl;
    mostrarEquipos(equiposCreados);

    int equipoSeleccionado = leerEntero("Seleccione el número del equipo: ");
    while (equipoSeleccionado < 1 || equipoSeleccionado > equiposCreados.size()) {
        equipoSeleccionado = leerEntero("Número de equipo no válido. Intente nuevamente: ");
    }

    vector<soldado>& equipo = equiposCreados[equipoSeleccionado - 1];
    mostrarSoldados(equipo);

    int opcionSoldado = leerEntero("Seleccione el número del soldado que desea modificar: ");
    while (opcionSoldado < 1 || opcionSoldado > equipo.size()) {
        opcionSoldado = leerEntero("Número de soldado no válido. Intente nuevamente: ");
    }

    // Verificar si el soldado tiene accesorios en su mochila
    vector<usos>& accesorios = equipo[opcionSoldado - 1].mochila.accesoriosConUsos;
    if (accesorios.empty()) {
        cout << "\nLa mochila del soldado seleccionado está vacía. No hay accesorios para eliminar." << endl;
        return;
    }

    eliminarAccesorio(accesorios);
}

bool buscarDefensaMochila(mochila mochila) {
	for (auto& a : mochila.accesoriosConUsos) {
		if (a.accesorioPtr->tipo == "Defensa") {
			return true;
		}
	}
	return false;
}
//mapa funciones

void mostrarEstaciones(estacion*& inicio) {
    if (inicio == nullptr) {
        cout << "\nNo hay estaciones creadas." << endl;
        return;
    }
    else {
        estacion* temp = inicio;
        int indice = 1;
        while (temp != nullptr) {
            cout << "\nEstación #" << indice << endl;
            cout << "Nombre: " << temp->nombre << endl;
            cout << "Zombies: " << temp->zombies.size() << endl;
            cout << "Conexiones: " << temp->ady.size() << endl;

            if (temp->ady.size() > 0) {
                cout << "Conexiones a: ";
                for (int i = 0; i < temp->ady.size(); i++) {
                    cout << temp->ady[i].destino << " (Distancia: " << temp->ady[i].distancia << ")";
                    if (i < temp->ady.size() - 1) {
                        cout << ", ";
                    }
                }
                cout << endl;
            }

            temp = temp->siguiente;
            indice++;
        }
    }
}

void agregarAdyacente(estacion & origen, string destino, int distancia) {
    estacionesAdyacentes conexion = { destino, distancia};
    origen.ady.push_back(conexion);
}

void agregarConexiones(estacion*& inicio) {
    if (inicio == nullptr) {
        cout << "\nNo hay estaciones creadas." << endl;
        return;
    }
    cout << "\n--- Estaciones Disponibles ---" << endl;
    mostrarEstaciones(inicio);
    int opcionEstacionOrigen = leerEntero("Seleccione el número de la estación de origen: ");
    while (opcionEstacionOrigen < 1) {
        opcionEstacionOrigen = leerEntero("Número de estación no válido. Intente nuevamente: ");
    }
    estacion* tempOrigen = inicio;
    for (int i = 1; i < opcionEstacionOrigen; i++) {
        tempOrigen = tempOrigen->siguiente;
    }
    cout << "\nEstación de origen seleccionada: " << tempOrigen->nombre << endl;

    cout << "\n--- Estaciones Disponibles ---" << endl;
    mostrarEstaciones(inicio);
    int opcionEstacionDestino = leerEntero("Seleccione el número de la estación de destino: ");
    while (opcionEstacionDestino < 1) {
        opcionEstacionDestino = leerEntero("Número de estación no válido. Intente nuevamente: ");
    }
    estacion* tempDestino = inicio;
    for (int i = 1; i < opcionEstacionDestino; i++) {
        tempDestino = tempDestino->siguiente;
    }
    cout << "\nEstación de destino seleccionada: " << tempDestino->nombre << endl;

    int distancia = leerEntero("Ingrese la distancia entre las estaciones: ");
    while (distancia < 0) {
        distancia = leerEntero("Distancia no válida. Intente nuevamente: ");
    }

    agregarAdyacente(*tempOrigen, tempDestino->nombre, distancia);
    cout << "\nConexión agregada con éxito." << endl;

    char continuar = leerChar("¿Desea agregar otra conexión? (s/n): ");
    if (continuar == 's' || continuar == 'S') {
        agregarConexiones(inicio);
    }
    else {
        return;
    }
}

void agregarEstacion(estacion*& inicio) {
    estacion* nuevaEstacion = new estacion;

    nuevaEstacion->nombre = leerString("Ingrese el nombre de la estación: ");

    nuevaEstacion->ady.clear();

    nuevaEstacion->siguiente = nullptr;

    if (inicio == nullptr) {
        inicio = nuevaEstacion;
    }
    else {
        estacion* temp = inicio;
        while (temp->siguiente != nullptr) {
            temp = temp->siguiente;
        }
        temp->siguiente = nuevaEstacion;
    }
}

void agregarZombieAEstacion(estacion*& inicio, vector<zombieFortaleza>& zombies) {
    if (inicio == nullptr) {
        cout << "\nNo hay estaciones creadas." << endl;
        return;
    }
    if (zombies.empty()) {
        cout << "\nNo hay zombies creados." << endl;
        return;
    }

    cout << "\n--- Estaciones Disponibles ---" << endl;
    mostrarEstaciones(inicio);

    int opcionEstacion = leerEntero("Seleccione el número de la estación a la que desea agregar un zombie: ");
    while (opcionEstacion < 1) {
        opcionEstacion = leerEntero("Número de estación no válido. Intente nuevamente: ");
    }

    
    estacion* temp = inicio;
    for (int i = 1; i < opcionEstacion; i++) {  
        if (temp == nullptr) {
            cout << "Error: No se encontró la estación." << endl;
            return;
        }
        temp = temp->siguiente;
    }

    if (temp == nullptr) {
        cout << "Error: Estación no encontrada." << endl;
        return;
    }

    cout << "\n--- Tipos de Zombies ---" << endl;
    mostrarZombies(zombies); 

    int opcionZombie = leerEntero("Seleccione el número del zombie que desea agregar: ");
    while (opcionZombie < 1 || opcionZombie > zombies.size()) {
        opcionZombie = leerEntero("Número de zombie no válido. Intente nuevamente: ");
    }
	int cantidadZombies = leerEntero("Ingrese la cantidad de zombies de este tipo: ");


    zombieFortaleza zombieSeleccionado = zombies[opcionZombie - 1];
	for (int i = 0; i < cantidadZombies; i++) {
		temp->zombies.push_back(zombieSeleccionado);
	}

    cout << "\nZombies agregado con éxito." << endl;

    char continuar = leerChar("¿Desea agregar otro zombie a una estación? (s/n): ");
    if (continuar == 's' || continuar == 'S') {
        agregarZombieAEstacion(inicio, zombies); 
    }
    else {
        return;
    }
}

void modificarEstacion(estacion*& inicio,vector<zombieFortaleza>zombiesCreados) {
	if (inicio == nullptr) {
		cout << "\nNo hay estaciones creadas." << endl;
		return;
	}
	cout << "\n--- Estaciones Disponibles ---" << endl;
	mostrarEstaciones(inicio);
	int opcionEstacion = leerEntero("Seleccione el número de la estación que desea modificar: ");
	while (opcionEstacion < 1) {
		opcionEstacion = leerEntero("Número de estación no válido. Intente nuevamente: ");
	}
	estacion* temp = inicio;
	for (int i = 0; i < opcionEstacion; i++) {
		temp = temp->siguiente;
	}
	cout << "\nEstación seleccionada: " << temp->nombre << endl;
	char opcionModificar = leerChar("¿Qué desea modificar? (n: nombre, z: zombies, c: conexiones): ");
	if (opcionModificar == 'n' || opcionModificar == 'N') {
		string nuevoNombre = leerString("Ingrese el nuevo nombre de la estación: ");
		temp->nombre = nuevoNombre;
		cout << "\nNombre de la estación modificado con éxito." << endl;
	}
	else if (opcionModificar == 'z' || opcionModificar == 'Z') {
		if (zombiesCreados.empty()) {
			cout << "\nNo hay zombies creados." << endl;
			return;
		}
		cout << "\n--- Tipos de Zombies ---" << endl;
		mostrarZombies(zombiesCreados);
		int opcionZombie = leerEntero("Seleccione el número del zombie que desea agregar: ");
		while (opcionZombie < 1 || opcionZombie > zombiesCreados.size()) {
			opcionZombie = leerEntero("Número de zombie no válido. Intente nuevamente: ");
		}
		zombieFortaleza zombie = zombiesCreados[opcionZombie - 1];
		temp->zombies.push_back(zombie);
		cout << "\nZombie agregado con éxito." << endl;
	}
	else if (opcionModificar == 'c' || opcionModificar == 'C') {
		cout << "\n--- Estaciones Disponibles ---" << endl;
		mostrarEstaciones(inicio);
		int opcionEstacion2 = leerEntero("Seleccione el número de la estación a la que desea agregar una conexión: ");
		while (opcionEstacion2 < 1) {
			opcionEstacion2 = leerEntero("Número de estación no válido. Intente nuevamente: ");
		}
		estacion* temp2 = inicio;
		for (int i = 0; i < opcionEstacion2; i++) {
			temp2 = temp2->siguiente;
		}
		int distancia = leerEntero("Ingrese la distancia entre las estaciones: ");
		agregarAdyacente(*temp, temp2->nombre, distancia);
		cout << "\nConexión agregada con éxito." << endl;
	}
	else {
		cout << "\nOpción no válida." << endl;

	}
	
}

void eliminarEstacion(estacion*& inicio) {
	if (inicio == nullptr) {
		cout << "\nNo hay estaciones creadas." << endl;
		return;
	}
	cout << "\n--- Estaciones Disponibles ---" << endl;
	mostrarEstaciones(inicio);
	int opcionEstacion = leerEntero("Seleccione el número de la estación que desea eliminar: ");
	while (opcionEstacion < 1) {
		opcionEstacion = leerEntero("Número de estación no válido. Intente nuevamente: ");
	}
	estacion* temp = inicio;
	estacion* anterior = nullptr;
	for (int i = 0; i < opcionEstacion - 1; i++) {
		anterior = temp;
		temp = temp->siguiente;
	}
	if (anterior == nullptr) {
		inicio = temp->siguiente;
	}
	else {
		anterior->siguiente = temp->siguiente;
	}
	delete temp;
	cout << "\nEstación eliminada con éxito." << endl;

}

vector<string> encontrarCaminoMasCorto(estacion* inicio, const string& destinoNombre) {
    if (inicio == nullptr) {
        cout << "No hay estaciones en el mapa." << endl;
        return {};
    }

    priority_queue<pair<estacion*, int>, vector<pair<estacion*, int>>, CompararEstaciones> pq;

    unordered_map<string, int> distancias;
    unordered_map<string, string> predecesores;

    for (estacion* temp = inicio; temp != nullptr; temp = temp->siguiente) {
        distancias[temp->nombre] = INT_MAX;
        predecesores[temp->nombre] = "";
    }

    distancias[inicio->nombre] = 0;
    pq.push({ inicio, 0 });

    while (!pq.empty()) {
        estacion* actual = pq.top().first;
        int distanciaActual = pq.top().second;
        pq.pop();

        if (actual->nombre == destinoNombre) {
            vector<string> camino;
            string actualNombre = destinoNombre;
            while (actualNombre != "") {
                camino.insert(camino.begin(), actualNombre);  
                actualNombre = predecesores[actualNombre];
            }
            return camino;
        }

        for (const estacionesAdyacentes& adyacente : actual->ady) {
            int nuevaDistancia = distanciaActual + adyacente.distancia;
            if (nuevaDistancia < distancias[adyacente.destino]) {
                distancias[adyacente.destino] = nuevaDistancia;
                predecesores[adyacente.destino] = actual->nombre;
                estacion* vecino = nullptr;
                for (estacion* temp = inicio; temp != nullptr; temp = temp->siguiente) {
                    if (temp->nombre == adyacente.destino) {
                        vecino = temp;
                        break;
                    }
                }
                pq.push({ vecino, nuevaDistancia });
            }
        }
    }

    cout << "No se encontró un camino a la estación destino." << endl;
    return {};
}

estacion* moverAEstacionSiguiente(estacion* inicio, const string& destinoNombre) {
	if (inicio == nullptr) {
		cout << "No hay estaciones en el mapa." << endl;
		return nullptr;
	}

	vector<string> camino = encontrarCaminoMasCorto(inicio, destinoNombre);
	if (camino.empty()) {
		return nullptr;
	}

	estacion* temp = inicio;
	for (const string& nombre : camino) {
		while (temp != nullptr) {
			if (temp->nombre == nombre) {
				break;
			}
			temp = temp->siguiente;
		}
	}

	return temp;
}

estacion* estacionSiguienteConMenosZombies(estacion* inicio) {
    // Verificamos que la estación no sea nula
    if (inicio == nullptr) {
        return nullptr;
    }

    // Inicializamos la variable para almacenar la estación con menos zombies
    estacion* estacionConMenosZombies = nullptr;
    int cantidadMinimaZombies = INT_MAX;  // Comenzamos con un valor muy alto

    // Recorremos las estaciones adyacentes de la estación inicial
    for (const auto& adyacente : inicio->ady) {
        // Buscamos la estación destino entre las estaciones adyacentes
        estacion* estacionDestino = nullptr;
        estacion* temp = inicio;

        // Recorremos la lista de estaciones para encontrar la estación destino
        while (temp != nullptr) {
            if (temp->nombre == adyacente.destino) {
                estacionDestino = temp;
                break;
            }
            temp = temp->siguiente;
        }

        if (estacionDestino != nullptr && estacionDestino->cantidadZombies < cantidadMinimaZombies) {
            // Actualizamos la estación con menos zombies
            cantidadMinimaZombies = estacionDestino->cantidadZombies;
            estacionConMenosZombies = estacionDestino;
        }
    }

    // Devolvemos la estación con menos zombies
    return estacionConMenosZombies;
}

int contarEstaciones(estacion* inicio) {
    int cont = 0;
	estacion* temp = inicio;
	while (temp != nullptr) {
		cont++;
		temp = temp->siguiente;
	}
	return cont;
}

void crearMapaDiezEstaciones(estacion*& inicio) {
    estacion* estacion1 = new estacion;
    estacion1->nombre = "Estación 1";
    estacion1->siguiente = nullptr;
    estacion1->ady.clear();
    estacion1->cantidadZombies = estacion1->zombies.size();

    estacion* estacion2 = new estacion;
    estacion2->nombre = "Estación 2";
    estacion2->siguiente = nullptr;
    estacion2->ady.clear();
    estacion2->cantidadZombies = estacion2->zombies.size();

    estacion* estacion3 = new estacion;
    estacion3->nombre = "Estación 3";
    estacion3->siguiente = nullptr;
    estacion3->ady.clear();
    estacion3->cantidadZombies = estacion3->zombies.size();

    estacion* estacion4 = new estacion;
    estacion4->nombre = "Estación 4";
    estacion4->siguiente = nullptr;
    estacion4->ady.clear();
    estacion4->cantidadZombies = estacion4->zombies.size();

    estacion* estacion5 = new estacion;
    estacion5->nombre = "Estación 5";
    estacion5->siguiente = nullptr;
    estacion5->ady.clear();
    estacion5->cantidadZombies = estacion5->zombies.size();

    estacion* estacion6 = new estacion;
    estacion6->nombre = "Estación 6";
    estacion6->siguiente = nullptr;
    estacion6->ady.clear();
    estacion6->cantidadZombies = estacion6->zombies.size();

    estacion* estacion7 = new estacion;
    estacion7->nombre = "Estación 7";
    estacion7->siguiente = nullptr;
    estacion7->ady.clear();
    estacion7->cantidadZombies = estacion7->zombies.size();

    estacion* estacion8 = new estacion;
    estacion8->nombre = "Estación 8";
    estacion8->siguiente = nullptr;
    estacion8->ady.clear();
    estacion8->cantidadZombies = estacion8->zombies.size();

    estacion* estacion9 = new estacion;
    estacion9->nombre = "Estación 9";
    estacion9->siguiente = nullptr;
    estacion9->ady.clear();
    estacion9->cantidadZombies = estacion9->zombies.size();

    estacion* estacion10 = new estacion;
    estacion10->nombre = "Estación 10";
    estacion10->siguiente = nullptr;
    estacion10->ady.clear();
    estacion10->cantidadZombies = estacion10->zombies.size();

    // Enlazamos las estaciones en la lista
    inicio = estacion1;
    estacion1->siguiente = estacion2;
    estacion2->siguiente = estacion3;
    estacion3->siguiente = estacion4;
    estacion4->siguiente = estacion5;
    estacion5->siguiente = estacion6;
    estacion6->siguiente = estacion7;
    estacion7->siguiente = estacion8;
    estacion8->siguiente = estacion9;
    estacion9->siguiente = estacion10;

    agregarAdyacente(*estacion1, "Estación 2", 10);
    agregarAdyacente(*estacion1, "Estación 3", 20);
    agregarAdyacente(*estacion1, "Estación 4", 15);

    agregarAdyacente(*estacion2, "Estación 1", 10);
    agregarAdyacente(*estacion2, "Estación 3", 5);
    agregarAdyacente(*estacion2, "Estación 5", 25);

    agregarAdyacente(*estacion3, "Estación 1", 20);
    agregarAdyacente(*estacion3, "Estación 2", 5);
    agregarAdyacente(*estacion3, "Estación 4", 8);
    agregarAdyacente(*estacion3, "Estación 6", 30);

    agregarAdyacente(*estacion4, "Estación 1", 15);
    agregarAdyacente(*estacion4, "Estación 3", 8);
    agregarAdyacente(*estacion4, "Estación 5", 12);
    agregarAdyacente(*estacion4, "Estación 7", 18);

    agregarAdyacente(*estacion5, "Estación 2", 25);
    agregarAdyacente(*estacion5, "Estación 4", 12);
    agregarAdyacente(*estacion5, "Estación 6", 10);
    agregarAdyacente(*estacion5, "Estación 9", 5);

    agregarAdyacente(*estacion6, "Estación 3", 30);
    agregarAdyacente(*estacion6, "Estación 5", 10);
    agregarAdyacente(*estacion6, "Estación 7", 5);
    agregarAdyacente(*estacion6, "Estación 10", 50);

    agregarAdyacente(*estacion7, "Estación 4", 18);
    agregarAdyacente(*estacion7, "Estación 6", 5);
    agregarAdyacente(*estacion7, "Estación 8", 20);
    agregarAdyacente(*estacion7, "Estación 9", 8);

    agregarAdyacente(*estacion8, "Estación 7", 20);
    agregarAdyacente(*estacion8, "Estación 9", 10);
    agregarAdyacente(*estacion8, "Estación 10", 25);

    agregarAdyacente(*estacion9, "Estación 5", 5);
    agregarAdyacente(*estacion9, "Estación 7", 8);
    agregarAdyacente(*estacion9, "Estación 8", 10);
    agregarAdyacente(*estacion9, "Estación 10", 3);

    agregarAdyacente(*estacion10, "Estación 6", 50);
    agregarAdyacente(*estacion10, "Estación 8", 25);
    agregarAdyacente(*estacion10, "Estación 9", 3);
    agregarAdyacente(*estacion10, "Estación 3", 15);

	//Meter zombies
	while (estacion1 != nullptr) {
		estacion1->zombies = zombiesAleatorios();
		estacion1 = estacion1->siguiente;
	}


}

void limpiarMemoriaest(estacion*& inicio) {
	while (inicio != nullptr) {
		estacion* temp = inicio;
		inicio = inicio->siguiente;
		delete temp;
	}
}

//batalla funciones
vector<vector<soldado>>crearTresEquipos() {
    vector<vector<soldado>>equipos;
    vector<soldado>equipo1;
    vector<soldado>equipo2;
    vector<soldado>equipo3;
    equipo1.push_back(crearSoldado("Soldado 1", 100));
    equipo1.push_back(crearSoldado("Soldado 2", 100));
    equipo1.push_back(crearSoldado("Soldado 3", 100));
    equipo2.push_back(crearSoldado("Soldado 4", 100));
    equipo2.push_back(crearSoldado("Soldado 5", 100));
    equipo2.push_back(crearSoldado("Soldado 6", 100));
    equipo3.push_back(crearSoldado("Soldado 7", 100));
    equipo3.push_back(crearSoldado("Soldado 8", 100));
    equipo3.push_back(crearSoldado("Soldado 9", 100));
    equipos.push_back(equipo1);
    equipos.push_back(equipo2);
    equipos.push_back(equipo3);
    //Meter accesorios, armas, defensas y salud aleatorios entre los soldados, usos aleatorios
    vector<accesorio*>  armas = crearVectorArmas();
    vector<accesorio*>  defensas = crearVectorDefensas();
    vector<accesorio*> salud = crearVectorSalud();

    for (int i = 0; i < equipos.size(); i++) {

        
        for (int j = 0; j < equipos[i].size(); j++) {
            int indiceArma = generarNumeroAleatorio( armas.size() - 1);
			int indiceDefensa = generarNumeroAleatorio( defensas.size() - 1);
			int indiceSalud = generarNumeroAleatorio( salud.size() - 1);
			int usosArma = generarNumeroAleatorio(5);
			int usosDefensa = generarNumeroAleatorio(5);
			int usosSalud = generarNumeroAleatorio(5);
			while (usosArma == 0) {
				usosArma = generarNumeroAleatorio(5);
			}
			while (usosDefensa == 0) {
				usosDefensa = generarNumeroAleatorio(5);
			}
			while (usosSalud == 0) {
				usosSalud = generarNumeroAleatorio(5);
			}

			equipos[i][j].mochila.accesoriosConUsos.push_back({ armas[indiceArma], usosArma });
			equipos[i][j].mochila.accesoriosConUsos.push_back({ defensas[indiceDefensa], usosDefensa });
			equipos[i][j].mochila.accesoriosConUsos.push_back({ salud[indiceSalud], usosSalud });

            equipos[i][j].mochila.capacidad = 0;
        }
    }
    return equipos;

}

void meterZombiesRapido(estacion*&inicio, vector<zombieFortaleza>& zombies) {
	    cout << "Estaciones Disponibles" << endl;
	    mostrarEstaciones(inicio);
	    int opcionEstacion = leerEntero("Seleccione el número de la estación en la que desea agregar zombies: ");
	    while (opcionEstacion < 1) {
		    opcionEstacion = leerEntero("Número de estación no válido. Intente nuevamente: ");
	    }
	    estacion* temp = inicio;
	    for (int i = 0; i < opcionEstacion - 1; i++) {
		    temp = temp->siguiente;
	    }
	    cout << "Zombies Disponibles" << endl;
	    mostrarZombies(zombies);
	    int opcionZombie = leerEntero("Seleccione el número del zombie que desea agregar: ");
	    while (opcionZombie < 1 || opcionZombie > zombies.size()) {
		    opcionZombie = leerEntero("Número de zombie no válido. Intente nuevamente: ");
	    }
	    zombieFortaleza zombie = zombies[opcionZombie - 1];
	    int cantidadZombies = leerEntero("Ingrese la cantidad de zombies que desea agregar: ");
	    while (cantidadZombies < 1) {
		    cantidadZombies = leerEntero("Cantidad de zombies no válida. Intente nuevamente: ");
	    }
	    for (int i = 0; i < cantidadZombies; i++) {
		    temp->zombies.push_back(zombie);
	    }
		char continuar = leerChar("¿Desea agregar más zombies a otra estación? (s/n): ");
		if (continuar == 's' || continuar == 'S') {
			meterZombiesRapido(inicio, zombies);
		}
		else {
			return;
		}


};

void aplicarDanio(soldado& soldadoAtacado, int danio) {
    soldadoAtacado.salud -= danio;
    cout << "El soldado " << soldadoAtacado.nombre << " recibió " << danio << " de daño. Salud restante: " << max(0, soldadoAtacado.salud) << endl;
}

void usarDefensa(soldado& soldadoAtacado, zombieFortaleza& zombie) {
    cout << "\nEl ataque del zombie es de: " << zombie.zombie->ataque << endl;
    cout << "\nOpciones de defensa disponibles:" << endl;
    for (int i = 0; i < soldadoAtacado.mochila.accesoriosConUsos.size(); i++) {
        if (soldadoAtacado.mochila.accesoriosConUsos[i].accesorioPtr->tipo == "Defensa") {
            cout << i + 1 << ". " << soldadoAtacado.mochila.accesoriosConUsos[i].accesorioPtr->nombre << " (Usos: " << soldadoAtacado.mochila.accesoriosConUsos[i].usos << ")" <<soldadoAtacado.mochila.accesoriosConUsos[i].accesorioPtr->valor << endl;

        }
    }

    char opcion = leerChar("¿Desea usar una defensa? (s/n): ");
    if (opcion == 's' || opcion == 'S') {

        int seleccion = leerEntero("\nSeleccione el número de defensa: ");
        while (seleccion < 1 || seleccion > soldadoAtacado.mochila.accesoriosConUsos.size() ||
            soldadoAtacado.mochila.accesoriosConUsos[seleccion - 1].accesorioPtr->tipo != "Defensa") {
            seleccion = leerEntero("Opción no válida. Intente de nuevo: ");
        }

        usos& defensa = soldadoAtacado.mochila.accesoriosConUsos[seleccion - 1];
        int danioReducido = max(0, zombie.zombie->ataque - defensa.accesorioPtr->valor);
        aplicarDanio(soldadoAtacado, danioReducido);

        defensa.usos--;
        if (defensa.usos == 0) {
            cout << "El accesorio " << defensa.accesorioPtr->nombre << " se ha agotado y será removido." << endl;
            soldadoAtacado.mochila.accesoriosConUsos.erase(soldadoAtacado.mochila.accesoriosConUsos.begin() + seleccion - 1);
        }
    }
    else {
        aplicarDanio(soldadoAtacado, zombie.zombie->ataque);
    }
}

void turnoZombies(vector<soldado>& equipo, vector<zombieFortaleza>& zombies, int& contadorDeAtaques) {
    
    
    int ataquesZombies = generarNumeroAleatorio(5);
    cout << "\nNúmero de ataques de los zombies en este turno: " << ataquesZombies << endl;

    for (int i = 0; i < ataquesZombies; i++) {
        if (equipo.empty() || zombies.empty()) {
            cout << "No hay más soldados o zombies para continuar los ataques." << endl;
            break;
        }

        // Seleccionar el soldado a atacar
        int soldadoAtacar;
        if (contadorDeAtaques == 0) { // Primer turno: ataque aleatorio
            soldadoAtacar = generarNumeroAleatorio(equipo.size()-1);
            cout << "\nAtaque aleatorio: El zombie atacará a un soldado al azar." << endl;
        }
        else { // Turnos posteriores: atacar al soldado con menos vida
            soldadoAtacar = numeroSoldadoMenosVida(equipo);
            cout << "\nAtaque dirigido: El zombie atacará al soldado con menos vida." << endl;
        }

        int zombieAtaca = generarNumeroAleatorio(zombies.size()-1);
        zombieFortaleza& zombie = zombies[zombieAtaca];
        soldado& soldadoAtacado = equipo[soldadoAtacar];

        if (zombie.zombie != nullptr) {
            cout << "\nZombie " << zombie.zombie->nombre << " ataca al soldado " << soldadoAtacado.nombre << "." << endl;
        };

        if (buscarDefensaMochila(soldadoAtacado.mochila)) {
            cout << "El soldado tiene defensas disponibles. Mostrando opciones..." << endl;
            usarDefensa(soldadoAtacado, zombie);
        }
        else {
            cout << "El soldado no tiene defensas disponibles. Recibe el ataque directo." << endl;
            int ataqueZombie = zombie.zombie->ataque;
            aplicarDanio(soldadoAtacado, ataqueZombie);
        }

        // Verificar si el soldado fue eliminado
        if (soldadoAtacado.salud <= 0) {
            cout << "El soldado " << soldadoAtacado.nombre << " ha muerto." << endl;
            recogerArmaDeSoldadoMuerto(equipo, soldadoAtacado);
            equipo.erase(equipo.begin() + soldadoAtacar);
            if (equipo.empty())
            {
                break;
            }
        }
        else {
            cout << "El soldado " << soldadoAtacado.nombre << " tiene de vida: " << soldadoAtacado.salud<<endl;
        }
    }

    
}

void turnoSoldados(vector<soldado>& equipo, vector<zombieFortaleza>& zombies) {
	

    for (size_t i = 0; i < 2; ++i) {
        for (size_t i = 0; i < equipo.size(); ++i) {
            int mayorVidaZombie = zombieMayorVida(zombies);

            soldado& soldadoActual = equipo[i];
            int valorAtaque = 0;

            // Caso: Sin accesorios en la mochila
            if (soldadoActual.mochila.accesoriosConUsos.empty()) {
                cout << "\nEl soldado " << soldadoActual.nombre << " no tiene accesorios, ataca con la mano (10 de daño)." << endl;
                valorAtaque = 10;
                mostrarZombies(zombies);
                int opcionZombie = leerEntero("\nSelecciona un zombie que quieras atacar: ");
                while (opcionZombie < 1 || opcionZombie > zombies.size()) {
                    opcionZombie = leerEntero("Número de zombie no válido. Intente nuevamente: ");
                }

                // Aplicar daño al zombie
                zombieFortaleza& zombieAtacado = zombies[opcionZombie - 1];
                zombieAtacado.fortaleza -= valorAtaque;

                if (zombieAtacado.fortaleza <= 0) {
                    cout << "¡El zombie ha sido eliminado!" << endl;
                    zombies.erase(zombies.begin() + opcionZombie - 1);
                }
                else {
                    cout << "El zombie ha sido atacado, su fortaleza actual es de " << zombieAtacado.fortaleza << "." << endl;
                }
                continue;
            }

            // Caso: Con accesorios en la mochila
            cout << "\nEl soldado " << soldadoActual.nombre << " tiene los siguientes accesorios: " << endl;
            int indice = 0;
            for (int j = 0; j < soldadoActual.mochila.accesoriosConUsos.size(); ++j) {
                indice += 1;
                const auto& acc = soldadoActual.mochila.accesoriosConUsos[j];
                cout << indice << ". " << acc.accesorioPtr->nombre << " con " << acc.usos << " usos (Tipo: " << acc.accesorioPtr->tipo << ")." << " valor:" << acc.accesorioPtr->valor << endl;
            }
            cout << indice + 1 << ". Atacar con la mano, valor: 10 " << endl;
            int opcionAccesorio = 0;
            bool opcionValida = false;

            while (!opcionValida) {
                opcionAccesorio = leerEntero("Selecciona una opción para interactuar: ");

                if (opcionAccesorio < 1 || opcionAccesorio > indice + 1) {
                    cout << "Número de accesorio no válido. Intente nuevamente." << endl;
                    continue;
                }

                if (opcionAccesorio <= indice) {
                    if (soldadoActual.mochila.accesoriosConUsos[opcionAccesorio - 1].accesorioPtr->tipo == "Defensa") {
                        cout << "No puedes atacar con una defensa, selecciona otro accesorio." << endl;
                        continue;
                    }
                    
                }
                //si dano es mayor que vida de soldado soldado no usa nada
				if (opcionAccesorio <= indice) {
                    if (soldadoActual.mochila.accesoriosConUsos[opcionAccesorio - 1].accesorioPtr->tipo.find("Arma") != string::npos) {
                        if (soldadoActual.mochila.accesoriosConUsos[opcionAccesorio - 1].accesorioPtr->valor > mayorVidaZombie) {
                            cout << "El arma seleccionada tiene un valor de ataque mayor a la vida del zombie con mayor vida, por lo que no se puede usar." << endl;
                            continue;
                        }
                    }
                }

                opcionValida = true;
            }
            // Si el soldado decide atacar con la mano
            if (opcionAccesorio == indice + 1) {
                valorAtaque = 10;
                mostrarZombies(zombies);
                int opcionZombie = leerEntero("\nSelecciona un zombie que quieras atacar: ");
                while (opcionZombie < 1 || opcionZombie > zombies.size()) {
                    opcionZombie = leerEntero("Número de zombie no válido. Intente nuevamente: ");
                }

                zombieFortaleza& zombieAtacado = zombies[opcionZombie - 1];
                zombieAtacado.fortaleza -= valorAtaque;

                if (zombieAtacado.fortaleza <= 0) {
                    cout << "¡El zombie ha sido eliminado!" << endl;
                    zombies.erase(zombies.begin() + opcionZombie - 1);
                    // Después de eliminar un zombie, asegúrate de que el índice de zombie esté ajustado
                    if (opcionZombie - 1 < zombies.size()) {
                        // Si hay zombies restantes, muestra la lista de zombies
                        mostrarZombies(zombies);
                    }
                    if (zombies.empty()) {
                        cout << "Humanos ganaron batalla. " << endl;
                        return;
                    }
                }
                else {
                    cout << "El zombie ha sido atacado, su fortaleza actual es de " << zombieAtacado.fortaleza << "." << endl;
                }
            }
            else
            {

                usos& accesorioSeleccionado = soldadoActual.mochila.accesoriosConUsos[opcionAccesorio - 1];

                if (accesorioSeleccionado.accesorioPtr->tipo.find("Arma") != string::npos) {
                    mostrarZombies(zombies);
                    int opcionZombie = leerEntero("\nSelecciona un zombie que quieras atacar: ");
                    while (opcionZombie < 1 || opcionZombie > zombies.size()) {
                        opcionZombie = leerEntero("Número de zombie no válido. Intente nuevamente: ");
                    }
                    valorAtaque = accesorioSeleccionado.accesorioPtr->valor;
                    zombieFortaleza& zombieAtacado = zombies[opcionZombie - 1];
                    zombieAtacado.fortaleza = zombieAtacado.fortaleza - valorAtaque;

                    if (zombieAtacado.fortaleza <= 0) {
                        cout << "¡El zombie ha sido eliminado!" << endl;
                        zombies.erase(zombies.begin() + opcionZombie - 1);
                        // Después de eliminar un zombie, asegúrate de que el índice de zombie esté ajustado
                        if (opcionZombie - 1 < zombies.size()) {
                            // Si hay zombies restantes, muestra la lista de zombies
                            mostrarZombies(zombies);
                        }
                        if (zombies.empty()) {
                            cout << "Humanos ganaron batalla. " << endl;
                            return;
                        }
                    }
                    else {
                        cout << "El zombie ha sido atacado, su fortaleza actual es de " << zombieAtacado.fortaleza << "." << endl;
                    }
                }
                // Si el accesorio es de tipo "Salud"
                else if (accesorioSeleccionado.accesorioPtr->tipo == "Salud") {
                    valorAtaque = accesorioSeleccionado.accesorioPtr->valor;
                    soldadoActual.salud += valorAtaque;
                    if (soldadoActual.salud > 100) soldadoActual.salud = 100;
                    cout << "El soldado " << soldadoActual.nombre << " ha sido curado. Su salud ahora es de " << soldadoActual.salud << "." << endl;
                }

                // Reducir usos del accesorio y eliminar si ya no tiene más
                accesorioSeleccionado.usos -= 1;
                if (accesorioSeleccionado.usos == 0) {
                    soldadoActual.mochila.accesoriosConUsos.erase(soldadoActual.mochila.accesoriosConUsos.begin() + opcionAccesorio - 1);
                }

            }

            // Si el accesorio es de ataque

        }
    }
}


bool batalla(estacion* estacionActual, vector<bitacora>& registros, vector<soldado>& equipo) {
    string evento;
    if (!estacionActual) {
        cout << "No hay una estación válida para la batalla." << endl;
        return false;
    }

    // Si no hay zombies en la estación, la batalla no ocurre
    if (estacionActual->zombies.empty()) {
        cout << "No hay zombies en la estación, pueden avanzar " << estacionActual->nombre << "." << endl;
        return true;
    }
    //Batalla hasta que gane uno
    int contadorDeAtaques = 0;
    vector<zombieFortaleza> zombies = estacionActual->zombies;
	int ronda = 1;
    while (!equipo.empty() && !estacionActual->zombies.empty()) {
        //Turno de los soldados
		cout << "\n---Ronda " << ronda << "---" << endl;
        cout << "\n---Tu equipo en esta estacion---" << endl;
		cout << "Estación: " << estacionActual->nombre << endl;
        mostrarSoldados(equipo);

        cout << "\n---Zombies restantes en la estacion---" << endl;
        //Humanos atacan 2 veces
        mostrarZombies(estacionActual->zombies);
		turnoSoldados(equipo, estacionActual->zombies);
        //Zombies Atacan
		turnoZombies(equipo, estacionActual->zombies, contadorDeAtaques);
        contadorDeAtaques += 1;
		ronda++;
    }
	if (equipo.empty()) {
		cout << "¡Los zombies han ganado la batalla!" << endl;
        bitacora registro;
        registro.estacion = estacionActual->nombre;
        registro.eventos.push_back("Los humanos han sido derrotados en la estación " + estacionActual->nombre + ".");
        registros.push_back(registro);
		return false;
	}
	else {

		cout << "¡Los humanos han ganado la batalla!" << endl;
		bitacora registro;
		registro.estacion = estacionActual->nombre;
		registro.eventos.push_back("Los humanos han ganado la batalla en la estación " + estacionActual->nombre + ".");
		registros.push_back(registro);
		return true;
	}


    
}
    
void jugar(estacion* inicio, vector<vector<soldado>>& equipos) {
    if (equipos.empty()) {
        cout << "No hay equipos disponibles para jugar." << endl;
        return;
    }

    cout << "\n--- Equipos Disponibles ---" << endl;
    mostrarEquipos(equipos);

    int opcionEquipo = leerEntero("Seleccione el número del equipo con el que desea jugar: ");
    while (opcionEquipo < 1 || opcionEquipo > equipos.size()) {
        opcionEquipo = leerEntero("Número de equipo no válido. Intente nuevamente: ");
    }

    vector<soldado> equipo = equipos[opcionEquipo - 1];
    if (equipo.empty()) {
        cout << "El equipo seleccionado no tiene soldados." << endl;
        return;
    }


    if (!inicio) {
        cout << "No hay estaciones disponibles para jugar." << endl;
        return;
    }

    // Mostrar estaciones disponibles
    cout << "Estaciones Disponibles" << endl;
    mostrarEstaciones(inicio);

    // Seleccionar estación para la batalla
	int cantidadEstaciones = contarEstaciones(inicio);
	int estacionInicial = generarNumeroAleatorio(cantidadEstaciones);
	while (estacionInicial < 1) {
		estacionInicial = generarNumeroAleatorio(cantidadEstaciones);
	}
	estacion* estacionSeleccionada = inicio;
	for (int i = 0; i < estacionInicial; i++) {
		estacionSeleccionada = estacionSeleccionada->siguiente;
	}
	cout << "Estación seleccionada: " << estacionSeleccionada->nombre << endl;

	// Definir estación con cura aleatoria
	int estacionConCura = generarNumeroAleatorio(cantidadEstaciones);
	while (estacionConCura < 1) {
		estacionConCura = generarNumeroAleatorio(cantidadEstaciones);
	}
	estacion* tempCura = inicio;
	for (int i = 0; i < estacionConCura; i++) {
		tempCura = tempCura->siguiente;
	}
	tempCura->cura = true;
	cout << "La estación con cura es: " << tempCura->nombre << endl;

	// Iniciar la batalla en la estación seleccionada
	vector<bitacora> registros;
	bool resultadoBatalla = batalla(estacionSeleccionada, registros, equipo);

	// Batalla hasta que consiga la cura o pierda
    while (resultadoBatalla) {
        // Verificar si la estación actual tiene la cura
        if (estacionSeleccionada->cura) {
            cout << "¡El equipo ha llegado a la estación con la cura! El juego ha terminado." << endl;
            break;
        }

        // Mover al equipo a la siguiente camino mas corto a la cura
		estacion* estacionSiguiente = moverAEstacionSiguiente(inicio,tempCura->nombre);
        if (!estacionSiguiente) {
            cout << "No hay más estaciones para explorar. Fin del juego." << endl;
            break;
        }

        // Continuar con la batalla en la siguiente estación}
        resultadoBatalla = batalla(estacionSiguiente, registros, equipo);
        estacionSeleccionada = estacionSiguiente;
    }

	// Mostrar resultados de la batalla
	cout << "\n--- Resultados de la Batalla ---" << endl;
	for (const bitacora& registro : registros) {
		cout << "Estación: " << registro.estacion << endl;
		for (const string& evento : registro.eventos) {
			cout << "  " << evento << endl;
		}
	}

}

void jugarConArchivos(vector<usos>accesorios,estacion*& inicio, vector<soldado>soldados) {
	// Meter accesorios a la mochila de los soldados
	for (auto& soldado : soldados) {
		for (int i = 0; i < 3; i++) {
			usos arma = accesorios[generarNumeroAleatorio(accesorios.size())];
			soldado.mochila.accesoriosConUsos.push_back(arma);
		}
	}
    // Mostrar estaciones disponibles
    cout << "Estaciones Disponibles" << endl;
    mostrarEstaciones(inicio);

    // Seleccionar estación para la batalla
    int cantidadEstaciones = contarEstaciones(inicio);
    int estacionInicial = generarNumeroAleatorio(cantidadEstaciones);
    while (estacionInicial < 1) {
        estacionInicial = generarNumeroAleatorio(cantidadEstaciones);
    }
    estacion* estacionSeleccionada = inicio;
    for (int i = 0; i < estacionInicial; i++) {
        estacionSeleccionada = estacionSeleccionada->siguiente;
    }
    cout << "Estación seleccionada: " << estacionSeleccionada->nombre << endl;

    // Definir estación con cura aleatoria
    int estacionConCura = generarNumeroAleatorio(cantidadEstaciones);
    while (estacionConCura < 1) {
        estacionConCura = generarNumeroAleatorio(cantidadEstaciones);
    }
    estacion* tempCura = inicio;
    for (int i = 0; i < estacionConCura; i++) {
        tempCura = tempCura->siguiente;
    }
    tempCura->cura = true;
    cout << "La estación con cura es: " << tempCura->nombre << endl;

    // Iniciar la batalla en la estación seleccionada
    vector<bitacora> registros;
    bool resultadoBatalla = batalla(estacionSeleccionada, registros, soldados);

    // Batalla hasta que consiga la cura o pierda
    while (resultadoBatalla) {
        // Verificar si la estación actual tiene la cura
        if (estacionSeleccionada->cura) {
            cout << "¡El equipo ha llegado a la estación con la cura! El juego ha terminado." << endl;
            break;
        }

        // Mover al equipo a la siguiente camino mas corto a la cura
        estacion* estacionSiguiente = moverAEstacionSiguiente(inicio, tempCura->nombre);
        if (!estacionSiguiente) {
            cout << "No hay más estaciones para explorar. Fin del juego." << endl;
            break;
        }

        // Continuar con la batalla en la siguiente estación}
        resultadoBatalla = batalla(estacionSiguiente, registros, soldados);
        estacionSeleccionada = estacionSiguiente;
    }

    // Mostrar resultados de la batalla
    cout << "\n--- Resultados de la Batalla ---" << endl;
    for (const bitacora& registro : registros) {
        cout << "Estación: " << registro.estacion << endl;
        for (const string& evento : registro.eventos) {
            cout << "  " << evento << endl;
        }
    }




}

//manejo de archivos 

vector<soldado> leerSoldados(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error al abrir el archivo de soldados: " << filename << endl;
        exit(1);
    }

    vector<soldado> soldados;
    int cantidad;
    file >> cantidad;
    file.ignore(); // Ignorar el salto de línea después de la cantidad

    // Verificar si la cantidad es válida
    if (cantidad <= 0) {
        cout << "La cantidad de soldados es inválida." << endl;
        exit(1);
    }

    // Leer los datos de cada soldado
    for (int i = 0; i < cantidad; ++i) {
        string nombre;
        int salud;

        // Leer el nombre del soldado
        getline(file, nombre);  // Obtener nombre del soldado
        cout << "Nombre leído: [" << nombre << "]" << endl; // Depuración

        // Verifica que el nombre no esté vacío
        if (nombre.empty()) {
            cout << "Error: Nombre vacío para el soldado #" << i + 1 << endl;
            continue; // Si el nombre es vacío, se salta al siguiente soldado
        }

        // Leer la salud del soldado
        file >> salud;
        file.ignore(); // Ignorar el salto de línea después de la salud

        // Verifica si la salud es válida
        if (salud <= 0) {
            cout << "Error: Salud inválida para el soldado #" << i + 1 << endl;
            salud = 100; // Asignar valor predeterminado si la salud es inválida
        }

        // Crear el soldado y añadirlo al vector
        mochila mochila;
        soldados.push_back({ nombre, salud,mochila });

        // Leer la línea separadora (si es necesario)
        string separator;
        getline(file, separator);  // Leer la línea de separación
        cout << "Separador leído: [" << separator << "]" << endl; // Depuración
    }

    return soldados;
}

vector<usos> leerUsos(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error al abrir el archivo: " << filename << endl;
        exit(1);
    }

    vector<usos> listaUsos; // Vector de usos
    int cantidad;

    // Leer cantidad de accesorios
    if (!(file >> cantidad)) {
        cerr << "Error: El archivo no contiene un número válido al inicio." << endl;
        exit(1);
    }
    file.ignore(); // Ignorar el salto de línea después de la cantidad
    string separator;

    for (int i = 0; i < cantidad; ++i) {
        int indice;
        accesorio* acc = new accesorio; // Crear un nuevo accesorio dinámico
        usos uso;

        // Leer datos del accesorio
        file >> indice;        // Leer y descartar el índice
        file.ignore();         // Ignorar el salto de línea después del índice
        getline(file, acc->nombre);
        getline(file, acc->tipo);
        file >> acc->valor;
        file >> uso.usos;
        file.ignore();         // Ignorar el salto de línea después del número de usos

        // Generar descripción automática (puedes personalizarla)
        acc->descripcion = "Tipo: " + acc->tipo + ", Valor: " + to_string(acc->valor);

        // Asociar accesorio al uso
        uso.accesorioPtr = acc;

        // Leer y descartar el separador "---"
        getline(file, separator);
        if (separator != "---") {
            cerr << "Error: Formato incorrecto, se esperaba '---' después del accesorio #" << i + 1 << endl;
            exit(1);
        }

        listaUsos.push_back(uso);
    }

    return listaUsos;
}

vector<zombieFortaleza> leerZombiesConFortaleza(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error al abrir el archivo: " << filename << endl;
        exit(1);
    }

    vector<zombieFortaleza> listaZombies;
    int cantidad;

    // Leer la cantidad de zombies
    if (!(file >> cantidad)) {
        cerr << "Error: El archivo no contiene un número válido al inicio." << endl;
        exit(1);
    }
    file.ignore(); // Ignorar el salto de línea después de la cantidad
    string separator;

    for (int i = 0; i < cantidad; ++i) {
        string nombre;
        int ataque, fortaleza;

        // Leer datos del zombie
        getline(file, nombre);       // Leer el nombre
        file >> ataque;              // Leer el ataque
        file >> fortaleza;           // Leer la fortaleza
        file.ignore();               // Ignorar el salto de línea después de fortaleza

        // Crear la descripción del zombie
        string descripcion = "Zombie peligroso: " + nombre;

        // Crear un zombie dinámicamente
        zombie* z = new zombie{ nombre, descripcion, ataque };

        // Asociar el zombie con su fortaleza y agregarlo al vector
        listaZombies.push_back({ z, fortaleza });

        // Leer y descartar el separador "---"
        getline(file, separator);
        if (separator != "---" && i < cantidad - 1) {
            cerr << "Error: Formato incorrecto, se esperaba '---' después del zombie #" << i + 1 << endl;
            exit(1);
        }
    }

    return listaZombies;
}

void leerMapa(const string& filename, vector<zombieFortaleza>& zombiesDisponibles, estacion*& inicio) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error al abrir el archivo: " << filename << endl;
        exit(1);
    }

    int cantidadEstaciones;
    file >> cantidadEstaciones;
    file.ignore();

    estacion* ultimaEstacion = nullptr;

    for (int i = 0; i < cantidadEstaciones; ++i) {
        string linea;
        estacion* nuevaEstacion = new estacion();

        // Leer número de estación
        getline(file, linea); // Ignorar número de estación
        getline(file, nuevaEstacion->nombre); // Leer nombre de la estación

        // Leer zombies
        getline(file, linea); // Ignorar separador "-"
        while (getline(file, linea) && linea != "--") {
            istringstream iss(linea);
            int cantidad;
            string nombreZombie;
            iss >> cantidad;
            getline(iss, nombreZombie, '|');
            nombreZombie = nombreZombie.substr(0, nombreZombie.find_last_not_of(' ') + 1);

            // Buscar el zombie en la lista de zombies disponibles
            bool encontrado = false;
            for (const auto& zf : zombiesDisponibles) {
                if (zf.zombie->nombre == nombreZombie) {
                    zombieFortaleza zombieEncontrado = { zf.zombie, zf.fortaleza };
					for (int j = 0; j < cantidad; ++j) {
                    nuevaEstacion->zombies.push_back(zombieEncontrado);
					}
                    encontrado = true;
                    break;
                }
            }
            if (!encontrado) {
                cerr << "Error: Zombie no encontrado: " << nombreZombie << endl;
            }
        }

        // Leer conexiones adyacentes
        while (getline(file, linea) && !linea.empty()) {
            istringstream iss(linea);
            string conexion;
            while (getline(iss, conexion, '|')) {
                size_t pos = conexion.find(':');
                if (pos != string::npos) {
                    int destino = stoi(conexion.substr(0, pos));
                    int distancia = stoi(conexion.substr(pos + 1));
                    nuevaEstacion->ady.push_back({ nullptr, distancia });
                }
            }
        }

        // Enlazar la lista
        if (inicio == nullptr) {
            inicio = nuevaEstacion;
        }
        else {
            ultimaEstacion->siguiente = nuevaEstacion;
        }
        ultimaEstacion = nuevaEstacion;

        // Validar separador "---"
        getline(file, linea);
        if (linea != "---") {
            cerr << "Error: Se esperaba '---' después de la estación #" << i + 1 << endl;
            exit(1);
        }
    }
}


int main() {
    vector<vector<soldado>> equiposPredeterminados = crearTresEquipos();
    int opcionPrincipal = -1;
    vector<zombieFortaleza> zombiesCreados;
    vector<vector<soldado>> equiposCreados;
    vector<soldado> soldadoscreados;
    vector<usos> armasCreadas;
    vector<usos> defensasCreadas;
    vector<usos> saludCreada;
    estacion* inicio = nullptr;
    while (opcionPrincipal != 7) {

        cout << "\n=== Juego: Salvar al planeta de los zombies ===" << endl;
        cout << "1. Zombies" << endl;
        cout << "2. Accesorios" << endl;
        cout << "3. Equipos" << endl;
        cout << "4. Mochilas" << endl;
        cout << "5. Mapa" << endl;
        cout << "6. Jugar con cosas creadas" << endl;
        cout << "7  Jugar con archivos" << endl;
        cout << "8. Salir del Juego" << endl;
        opcionPrincipal = leerEntero("Eliga una opcion:");


        switch (opcionPrincipal) {
        case 1: {
            system("cls");
            int opcionZombies;
            cout << "\n--- Zombies ---" << endl;
            cout << "1. Crear Tipos de Zombies" << endl;
            cout << "2. Ver Tipos de Zombies" << endl;
            cout << "3. Ver Zombies Creados" << endl;
            cout << "4. Volver " << endl;
            cout << "Ingrese opción: ";
            cin >> opcionZombies;

            switch (opcionZombies) {
            case 1:
                system("cls");
                zombiesUsuario(zombiesCreados);
                break;
            case 2:
                system("cls");
                mostrarZombies(crearZombiesConFortaleza());
                break;
            case 3:
                system("cls");
                mostrarZombies(zombiesCreados);
                break;
            case 4:
                cout << "Volviendo al menú principal...\n";
                break;
            default:
                cout << "Opción no válida.\n";
            }

            break;
        }
        case 2: {
            system("cls");
            int opcionAccesorios;
            cout << "\n--- Accesorios ---" << endl;
            cout << "1. Agregar Accesorios" << endl;
            cout << "2. Modificar Accesorios" << endl;
            cout << "3. Eliminar Accesorios" << endl;
            cout << "4. Mostrar Accesorios" << endl;
            cout << "5. Volver" << endl;
            opcionAccesorios = leerEntero("Ingrese opción: ");

            switch (opcionAccesorios) {
            case 1: {
                system("cls");
                int opcionAccesorios1;
                cout << "\n--- Agregar Accesorios ---" << endl;
                cout << "1. Armas" << endl;
                cout << "2. Defensas" << endl;
                cout << "3. Salud" << endl;
                cout << "4. Volver" << endl;
                opcionAccesorios1 = leerEntero("Ingrese opción: ");
                switch (opcionAccesorios1) {
                case 1:
                    system("cls");
                    armasUsuarioCrear(armasCreadas);
                    break;
                case 2:
                    system("cls");
                    defensasUsuarioCrear(defensasCreadas);
                    break;
                case 3:
                    system("cls");
                    saludUsuarioCrear(saludCreada);
                    break;
                case 4:
                    cout << "Volviendo al menú principal...\n";
                    break;
                default:
                    cout << "Opción no válida.\n";
                }
                break;
            }
            case 2: {
                system("cls");
                int opcionAccesorios2;
                cout << "\n--- Modificar Accesorios ---" << endl;
                cout << "1. Armas" << endl;
                cout << "2. Defensas" << endl;
                cout << "3. Salud" << endl;
                cout << "4. Volver" << endl;
                opcionAccesorios2 = leerEntero("Ingrese opción: ");
                switch (opcionAccesorios2) {
                case 1:
                    system("cls");
                    modificarAccesorio(armasCreadas);
                    break;
                case 2:
                    system("cls");
                    modificarAccesorio(defensasCreadas);
                    break;
                case 3:
                    system("cls");
                    modificarAccesorio(saludCreada);
                    break;
                case 4:
                    cout << "Volviendo al menú principal...\n";
                    break;
                default:
                    cout << "Opción no válida.\n";
                }
                break;
            }
            case 3: {
                system("cls");
                int opcionAccesorios3;
                cout << "\n--- Eliminar Accesorios ---" << endl;
                cout << "1. Armas" << endl;
                cout << "2. Defensas" << endl;
                cout << "3. Salud" << endl;
                cout << "4. Volver" << endl;
                opcionAccesorios3 = leerEntero("Ingrese opción: ");
                switch (opcionAccesorios3) {
                case 1:
                    system("cls");
                    eliminarAccesorio(armasCreadas);
                    break;
                case 2:
                    system("cls");
                    eliminarAccesorio(defensasCreadas);
                    break;
                case 3:
                    system("cls");
                    eliminarAccesorio(saludCreada);
                    break;
                case 4:
                    cout << "Volviendo al menú principal...\n";
                    break;
                default:
                    cout << "Opción no válida.\n";
                }
                break;
            }
            case 4: {
                system("cls");
                int opcionAccesorios4;
                cout << "\n--- Mostrar Accesorios ---" << endl;
                cout << "1. Armas" << endl;
                cout << "2. Defensas" << endl;
                cout << "3. Salud" << endl;
                cout << "4. Volver" << endl;
                opcionAccesorios4 = leerEntero("Ingrese opción: ");
                switch (opcionAccesorios4) {
                case 1:
                    system("cls");
                    mostrarAccesoriosConUsos(armasCreadas);
                    break;
                case 2:
                    system("cls");
                    mostrarAccesoriosConUsos(defensasCreadas);
                    break;
                case 3:
                    system("cls");
                    mostrarAccesoriosConUsos(saludCreada);
                    break;
                case 4:
                    cout << "Volviendo al menú principal...\n";
                    break;
                default:
                    cout << "Opción no válida.\n";
                }
                break;
            }
            case 5:
                cout << "Volviendo al menú principal...\n";
                break;
            default:
                cout << "Opción no válida.\n";
            }
            break;
        }
        case 3: {
            system("cls");
            int opcionEquipos;
            cout << "\n--- Equipos ---" << endl;
            cout << "1. Crear equipo" << endl;
            cout << "2. Modificar Equipo" << endl;
            cout << "3. Eliminar Equipo" << endl;
            cout << "4. Mostrar Equipos" << endl;
            cout << "5. Volver" << endl;
            cout << "Ingrese opción: ";
            cin >> opcionEquipos;

            switch (opcionEquipos) {
            case 1: {
                system("cls");
                crearEquipo(equiposCreados);
                break;
            }
            case 2:
                system("cls");
                editarEquipo(equiposCreados);
                break;
            case 3:
                system("cls");
                eliminarSoldadoDeEquipo(equiposCreados);
                break;
            case 4:
                system("cls");
                mostrarEquipos(equiposCreados);
                cout << "Presione cualquier tecla para continuar..." << endl;
                cin.ignore();
                cin.get();

                break;
            case 5:
                cout << "Volviendo al menú principal...\n";
                break;
            default:
                cout << "Opción no válida.\n";
            }
            break;
        }
        case 4: {
            system("cls");
            int opcionMochilas;
            cout << "\n--- Mochilas ---" << endl;
            cout << "1. Agregar Accesorios a Soldado" << endl;
            cout << "2. Modificar Mochila" << endl;
            cout << "3. Eliminar Accesorios de Mochila" << endl;
            cout << "4. Volver" << endl;
            cout << "Ingrese opción: ";
            cin >> opcionMochilas;

            switch (opcionMochilas) {
            case 1:
                system("cls");
                agregarAccesorioCualquierSoldado(equiposCreados, armasCreadas);
                break;
            case 2:
                system("cls");
                modificarMochila(equiposCreados);

                break;
            case 3:
                system("cls");
                eliminarDeMochila(equiposCreados);
                break;
            case 4:
                cout << "Volviendo al menú principal...\n";
                break;
            default:
                cout << "Opción no válida.\n";
            }
            break;
        }
        case 5: {
            system("cls");
            int opcionMapa;
            cout << "\n--- Mapa ---" << endl;
            cout << "1. Agregar Estación" << endl;
            cout << "2. Modificar Estación" << endl;
            cout << "3. Eliminar Estación" << endl;
            cout << "4. Mostrar Estaciones" << endl;
            cout << "5. Agregar Zombie a Estación" << endl;
            cout << "6. Agregar Conexión entre Estaciones" << endl;
            cout << "7. Volver" << endl;
            cout << "Ingrese opción: ";
            opcionMapa = leerEntero("Ingrese opción: ");

            switch (opcionMapa) {
            case 1:
                system("cls");
                agregarEstacion(inicio);
                break;
            case 2:
                system("cls");
                modificarEstacion(inicio, zombiesCreados);
                break;
            case 3:
                system("cls");
                eliminarEstacion(inicio);
                break;
            case 4:
                system("cls");
                mostrarEstaciones(inicio);
                break;
            case 5:
                system("cls");
                agregarZombieAEstacion(inicio, zombiesCreados);
                break;
            case 6:
                system("cls");
                agregarConexiones(inicio);
                break;
            case 7:
                cout << "Volviendo al menú principal...\n";
                break;
            default:
                cout << "Opción no válida.\n";
            }
            break;
        }
        case 6:
            system("cls");
            cout << "Como quieres jugar" << endl;
            cout << "1. Jugar con cosas creadas" << endl;
            cout << "2. Jugar de inmediato" << endl;
            cout << "3. Volver" << endl;
            int opcionJugar = leerEntero("Ingrese Opcion: ");

            switch (opcionJugar) {
            case 1:
                system("cls");
                jugar(inicio, equiposCreados);
                break;

            case 2:
                system("cls");

                inicio = nullptr;

                // Crear el mapa con las 10 estaciones
                crearMapaDiezEstaciones(inicio);
				

                // Luego se pasa a la función jugar con los equipos predeterminados
                jugar(inicio, equiposPredeterminados);

                break;

            case 3:
                cout << "Volviendo al menú principal...\n";
                break;

            default:
                cout << "Opción no válida.\n";
                break;
            }
            break; // Se asegura de que el case 6 termine correctamente

        case 7:
            system("cls");
            inicio = nullptr;
            zombiesCreados = leerZombiesConFortaleza("Zombie.zmb");
            leerMapa("Mapa.zmb", zombiesCreados, inicio);
            jugarConArchivos(leerUsos("Accesorios.zmb"), inicio, leerSoldados("Soldados.zmb"));

        case 8:
            cout << "Gracias por jugar. ¡Hasta luego!\n";
            liberarMemoria(zombiesCreados);
            limpiarMemoria(armasCreadas);
            limpiarMemoria(defensasCreadas);
            limpiarMemoria(saludCreada);
            limpiarMemoriaest(inicio);

            break;
        default:
            cout << "Opción no válida. Inténtelo de nuevo.\n";
        }
    }

    return 0;
}
