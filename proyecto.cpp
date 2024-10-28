#include <iostream>
#include <vector>
using namespace std;

//struct de accesorio

struct accesorio {
    string nombre;
    string tipo;
    string descripcion;
    int valor;
};

//struct de usos modificables

struct usos {
    accesorio accesorio;
    int usos;
};

//struct de mochila

struct mochila {
    string dueno;
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
    zombie zombie;
    int fortaleza;
};

//struct de soldado

struct soldado {
    string nombre;
    int salud;
    mochila mochila;
};

//struct de estacion

struct estacion {
    string nombre;
    int cantidadZombies;
    estacion* siguiente;
};

//soldado funciones 

soldado crearSoldado(string name, int vida) {
    soldado soldadoNuevo;
    mochila mochilaNueva;
    soldadoNuevo.nombre = name;
    soldadoNuevo.salud = vida;
    soldadoNuevo.mochila = mochilaNueva;
    soldadoNuevo.mochila.dueno = name;
    soldadoNuevo.mochila.accesoriosConUsos = {};

    return soldadoNuevo;
};

void anadirSoldado(vector<soldado>& soldados, string name, int vida) {
    while (vida < 0 || vida > 100) {
        cout << "La salud debe estar entre 0 y 100. Ingrese un valor v�lido: ";
        cin >> vida;
    }
    soldado soldadoNuevo = crearSoldado(name, vida);
    soldados.push_back(soldadoNuevo);
    cout << "Soldado agregado con �xito." << endl;
}

void mostrarSoldados(vector<soldado>soldados) {
    int numero = 1;
    if (soldados.size() <= 0)
    {
        cout << "No hay soldados creados." << endl;
    }
    else {
        for (auto& s : soldados) {
            cout << "\nSoldado #" << numero << endl;
            cout << "Nombre: " << s.nombre << endl;
            cout << "Salud: " << s.salud << endl;
            cout << "Mochila: " << s.mochila.dueno << endl;
            cout << "Capacidad de la mochila: " << s.mochila.capacidad << endl;
            cout << "Accesorios: " << endl;
            for (auto& a : s.mochila.accesoriosConUsos) {
                cout << "Nombre: " << a.accesorio.nombre << " - Usos: " << a.usos << endl;
            }
            cout << endl;
            numero++;
        }
    }

}

void eliminarSoldado(vector<soldado>& soldados) {
    mostrarSoldados(soldados);
    int soldadoAEliminar;
    cout << "Seleccione el n�mero del soldado que desea eliminar: ";
    cin >> soldadoAEliminar;
    while (soldadoAEliminar<0 || soldadoAEliminar > soldados.size()) {
        cout << "Seleccione un soldado que este creado: ";
        cin >> soldadoAEliminar;
    }

    soldados.erase(soldados.begin() + soldadoAEliminar - 1);
    cout << "Soldado eliminado con �xito." << endl;
}

void editarSoldado(vector<soldado>& soldados) {
    mostrarSoldados(soldados);
    int soldadoAEditar;
    cout << "Seleccione el n�mero del soldado que desea editar: ";
    cin >> soldadoAEditar;
    string nombre;
    int salud;
    cout << "Ingrese el nuevo nombre del soldado: ";
    cin >> nombre;
    cout << "Ingrese la nueva salud del soldado: ";
    cin >> salud;
    while (salud < 0 || salud > 100) {
        cout << "La salud debe estar entre 0 y 100. Ingrese un valor v�lido: ";
        cin >> salud;
    }
    soldados[soldadoAEditar - 1] = crearSoldado(nombre, salud);
    cout << "Soldado editado con �xito." << endl;
}

//zombie funciones

zombie crearZombie(string nombre, string descripcion, int ataque) {
    zombie zombienuevo;
    zombienuevo.nombre = nombre;
    zombienuevo.descripcion = descripcion;
    zombienuevo.ataque = ataque;

    return zombienuevo;
};

//zombies

vector<zombie> crearTiposZombies() {
    vector<zombie> zombies;

    zombies.push_back(crearZombie("Zombie R�pido y �gil",
        "Estos zombies son extremadamente r�pidos y �giles, capaces de perseguir a sus presas a altas velocidades. Podr�an ser el resultado de una mutaci�n que aceler� su metabolismo.",
        10));

    zombies.push_back(crearZombie("Zombie Tanque",
        "Grandes y lentos, estos zombies son casi indestructibles. Su fuerza bruta los convierte en una amenaza formidable, capaces de derribar puertas y paredes, su debilidad es la luz, la cual no pueden soportar.",
        25));

    zombies.push_back(crearZombie("Zombie Inteligente",
        "Estos zombies han conservado parte de su inteligencia, lo que les permite planificar emboscadas y utilizar herramientas. Podr�an ser una evoluci�n de los zombies, o el resultado de una infecci�n en individuos con un coeficiente intelectual alto. Sin embargo, son fr�giles y lentos.",
        15));

    zombies.push_back(crearZombie("Zombie Infectado por Hongos",
        "Estos zombies presentan caracter�sticas f�ngicas, como esporas que se propagan por el aire y la capacidad de controlar a otros organismos.",
        20));

    zombies.push_back(crearZombie("Zombie Bioluminiscente",
        "Estos zombies emiten una luz bioluminiscente, lo que los hace f�cilmente visibles en la oscuridad. Esta caracter�stica podr�a ser un efecto secundario de la mutaci�n o una adaptaci�n para comunicarse entre ellos.",
        18));

    return zombies;
}

vector<zombieFortaleza> crearZombiesConFortaleza() {

    vector<zombie> zombies = crearTiposZombies();
    vector<zombieFortaleza> zombiesConFortaleza;

    for (auto& z : zombies) {
        zombieFortaleza zFortaleza;
        zFortaleza.zombie = z;

        if (z.nombre == "Zombie R�pido y �gil") {
            zFortaleza.fortaleza = 50;
        }
        else if (z.nombre == "Zombie Tanque") {
            zFortaleza.fortaleza = 100;
        }
        else if (z.nombre == "Zombie Inteligente") {
            zFortaleza.fortaleza = 30;
        }
        else if (z.nombre == "Zombie Infectado por Hongos") {
            zFortaleza.fortaleza = 40;
        }
        else if (z.nombre == "Zombie Bioluminiscente") {
            zFortaleza.fortaleza = 35;
        }

        zombiesConFortaleza.push_back(zFortaleza);
    }

    return zombiesConFortaleza;
}

void zombiesUsuario(int opcion, vector<zombieFortaleza>& zombiesCreados) {
    vector<zombieFortaleza> zombies = crearZombiesConFortaleza();

    for (auto& z : zombies) {

        if ((opcion == 1 && z.zombie.nombre == "Zombie R�pido y �gil") ||
            (opcion == 2 && z.zombie.nombre == "Zombie Tanque") ||
            (opcion == 3 && z.zombie.nombre == "Zombie Inteligente") ||
            (opcion == 4 && z.zombie.nombre == "Zombie Infectado por Hongos") ||
            (opcion == 5 && z.zombie.nombre == "Zombie Bioluminiscente")) {


            cout << "\nNombre: " << z.zombie.nombre << endl;
            cout << "Descripci�n: " << z.zombie.descripcion << endl;
            cout << "Ataque: " << z.zombie.ataque << endl;
            cout << "Fortaleza: " << z.fortaleza << endl;
            cout << endl;


            zombiesCreados.push_back(z);
            cout << "Zombie creado con �xito." << endl;
            break;
        }
        else {
            cout << "\nOpci�n no v�lida." << endl;
            break;
        }
    }


}

void mostrarZombies(vector<zombieFortaleza> zombies) {

    for (auto& z : zombies) {
        cout << "\nNombre: " << z.zombie.nombre << endl;
        cout << "Descripci�n: " << z.zombie.descripcion << endl;
        cout << "Ataque: " << z.zombie.ataque << endl;
        cout << "Fortaleza: " << z.fortaleza << endl;
        cout << endl;
    }
}

//accesorios funciones

accesorio crearAccesorio(string name, string tipo, string descripcion, int valor) {
    accesorio accesorioNuevo;
    accesorioNuevo.nombre = name;
    accesorioNuevo.tipo = tipo;
    accesorioNuevo.valor = valor;
    return accesorioNuevo;
};

//accesorios
vector<accesorio> crearVectorArmas() {

    accesorio pistola = crearAccesorio("Pistola", "Arma de fuego",
        "Pr�ctica para espacios reducidos, pero con limitada capacidad de munici�n.", 20);

    accesorio escopeta = crearAccesorio("Escopeta", "Arma de fuego",
        "Ideal para disparos a corta distancia y causar da�o masivo.", 35);

    accesorio fusilAsalto = crearAccesorio("Fusil de Asalto", "Arma de fuego",
        "Mayor alcance y capacidad de fuego, pero requiere m�s munici�n.", 30);

    accesorio rifleFrancotirador = crearAccesorio("Rifle de Francotirador", "Arma de fuego",
        "Perfecto para eliminar amenazas a larga distancia, pero es pesado y requiere precisi�n.", 50);

    accesorio granada = crearAccesorio("Granada", "Arma arrojadiza",
        "�til para eliminar grupos de zombies o crear distracciones.", 60);

    accesorio molotov = crearAccesorio("C�ctel Molotov", "Arma arrojadiza",
        "Ideal para incendiar a los zombies y crear barreras de fuego.", 40);

    accesorio ballesta = crearAccesorio("Ballesta", "Arma de proyectiles",
        "Silenciosa y precisa, pero requiere munici�n especial.", 25);

    accesorio tirachinas = crearAccesorio("Tirachinas", "Arma de proyectiles",
        "Econ�mico y f�cil de usar, pero con alcance limitado.", 10);

    accesorio machete = crearAccesorio("Machete", "Arma blanca",
        "Ideal para cortar extremidades y causar da�o masivo.", 30);

    accesorio espada = crearAccesorio("Espada", "Arma blanca",
        "Ofrece mayor alcance y poder, pero es m�s dif�cil de manejar.", 35);

    accesorio bateBeisbol = crearAccesorio("Bate de B�isbol", "Arma contundente",
        "F�cil de encontrar y efectivo para aturdir a los zombies.", 20);

    accesorio martillo = crearAccesorio("Martillo", "Arma contundente",
        "Ideal para aplastar cr�neos.", 25);

    accesorio tuberia = crearAccesorio("Tuber�a", "Arma contundente",
        "Vers�til y f�cil de improvisar.", 15);

    accesorio objetoPunzante = crearAccesorio("Objeto Punzante", "Arma improvisada",
        "Clavos, tijeras, destornilladores, etc.", 10);
    vector<accesorio>armas = { pistola, escopeta, fusilAsalto, rifleFrancotirador, granada, molotov, ballesta, tirachinas, machete, espada, bateBeisbol, martillo, tuberia, objetoPunzante };
    return armas;
};

vector<accesorio> crearVectorDefensas() {

    accesorio botasAntideslizantes = crearAccesorio(
        "Botas Antideslizantes", "Defensa M�vil",
        "Botas con suelas especiales que aumentan la tracci�n y evitan que los soldados resbalen en persecuciones.", 35);

    accesorio linternaPotente = crearAccesorio(
        "Linterna T�ctica", "Defensa Luminosa",
        "Una linterna de alta potencia que emite un haz de luz cegador, debilitando a los zombies tanques.", 50);

    accesorio inhibidorUltrasonico = crearAccesorio(
        "Inhibidor Ultras�nico", "Defensa Electr�nica",
        "Emite ondas ultras�nicas que desorientan y dificultan la coordinaci�n de zombies inteligentes.", 40);

    accesorio filtroAireAvanzado = crearAccesorio(
        "Filtro de Aire Avanzado", "Defensa Biol�gica",
        "Un filtro especializado que bloquea esporas f�ngicas para evitar infecciones y control mental.", 45);

    accesorio camuflajeNocturno = crearAccesorio(
        "Camuflaje Nocturno", "Defensa T�ctica",
        "Un traje con material que absorbe luz y reduce la visibilidad en la oscuridad, evitando ser detectado por zombies bioluminiscentes.", 30);

    vector<accesorio> defensas = { botasAntideslizantes, linternaPotente, inhibidorUltrasonico,filtroAireAvanzado, camuflajeNocturno };
    return defensas;
};

vector<accesorio> crearVectorSalud() {

    accesorio botiquinPrimeraAuxilios = crearAccesorio(
        "Botiqu�n de Primeros Auxilios", "Salud",
        "Contiene vendajes, antis�pticos y analg�sicos para curar heridas menores.", 50);

    accesorio medicinaRegenerativa = crearAccesorio(
        "Medicina Regenerativa", "Salud",
        "Un suero que acelera la curaci�n de heridas y restaura la salud de forma r�pida.", 75);

    accesorio racionEnergetica = crearAccesorio(
        "Raci�n de Comida Energ�tica", "Salud",
        "Comida dise�ada para proporcionar energ�a r�pida y restaurar salud a corto plazo.", 25);

    accesorio aguaPurificada = crearAccesorio(
        "Agua Purificada", "Salud",
        "Ayuda a hidratar y revitalizar a los soldados, restaurando un poco de salud.", 15);

    accesorio paqueteVendas = crearAccesorio(
        "Paquete de Vendas", "Salud",
        "Vendas que ayudan a detener la hemorragia y curar heridas.", 30);

    vector<accesorio> salud = {
        botiquinPrimeraAuxilios, medicinaRegenerativa, racionEnergetica,aguaPurificada, paqueteVendas
    };
    return salud;
}

void mostrarAccesorios(vector<accesorio> accesorios) {
    int indice = 1;
    for (auto& a : accesorios) {
        cout << "\nAccesorio #" << indice << endl;
        cout << "Nombre: " << a.nombre << endl;
        cout << "Tipo: " << a.tipo << endl;
        cout << "Descripci�n: " << a.descripcion << endl;
        cout << "Valor: " << a.valor << endl;
        cout << endl;
        indice++;
    }
}

void mostrarAccesoriosConUsos(vector<usos> accesorios) {
    int indice = 1;
    for (auto& a : accesorios) {
        cout << "\nAccesorio #" << indice << endl;
        cout << "Nombre: " << a.accesorio.nombre << endl;
        cout << "Tipo: " << a.accesorio.tipo << endl;
        cout << "Descripci�n: " << a.accesorio.descripcion << endl;
        cout << "Valor: " << a.accesorio.valor << endl;
        cout << "Usos: " << a.usos << endl;
        cout << endl;
        indice++;
    }
}

void armasUsuarioCrear(vector<usos>& armasCreadas) {
    vector<accesorio> armas = crearVectorArmas();
    mostrarAccesorios(armas);
    int opcionArma;
    int contadorUsos;
    cout << "Seleccione el n�mero del arma que desea agregar: ";
    cin >> opcionArma;
    while (opcionArma < 1 || opcionArma > armas.size()) {
        cout << "Opci�n no v�lida. Int�ntelo de nuevo: ";
        cin >> opcionArma;
    }
    cout << "Ingrese la cantidad de usos del arma: ";
    cin >> contadorUsos;
    while (contadorUsos < 0) {
        cout << "La cantidad de usos debe ser mayor o igual a 0. Ingrese un valor v�lido: ";
        cin >> contadorUsos;
    }
    armasCreadas.push_back({ armas[opcionArma - 1], contadorUsos });
    cout << "Arma creada con exito" << endl;
    mostrarAccesoriosConUsos(armasCreadas);


}

void defensasUsuarioCrear(vector<usos>& defensasCreadas) {
    vector<accesorio> defensas = crearVectorDefensas();
    mostrarAccesorios(defensas);
    int opcionDefensa;
    int contadorUsos;
    cout << "Seleccione el n�mero de la defensa que desea agregar: ";
    cin >> opcionDefensa;
    while (!(cin >> opcionDefensa) || opcionDefensa < 1 || opcionDefensa > defensas.size()) {
        cout << "Opci�n no v�lida. Int�ntelo de nuevo: ";
        cin >> opcionDefensa;
    }
    cout << "Ingrese la cantidad de usos de la defensa: ";
    cin >> contadorUsos;
    while (contadorUsos < 0) {
        cout << "La cantidad de usos debe ser mayor o igual a 0. Ingrese un valor v�lido: ";
        cin >> contadorUsos;
    }
    defensasCreadas.push_back({ defensas[opcionDefensa - 1], contadorUsos });
    cout << "Defensa creada con exito" << endl;
    mostrarAccesoriosConUsos(defensasCreadas);


}

void saludUsuarioCrear(vector<usos>& saludCreada) {
    vector<accesorio> salud = crearVectorSalud();
    mostrarAccesorios(salud);
    int opcionSalud;
    int contadorUsos;
    cout << "Seleccione el n�mero del objeto de salud que desea agregar: ";
    cin >> opcionSalud;
    while (opcionSalud < 1 || opcionSalud > salud.size()) {
        cout << "Opci�n no v�lida. Int�ntelo de nuevo: ";
        cin >> opcionSalud;
    }
    cout << "Ingrese la cantidad de usos del objeto de salud: ";
    cin >> contadorUsos;
    while (contadorUsos < 0) {
        cout << "La cantidad de usos debe ser mayor o igual a 0. Ingrese un valor v�lido: ";
        cin >> contadorUsos;
    }

    saludCreada.push_back({ salud[opcionSalud - 1], contadorUsos });
    cout << "Objeto de salud creado con exito" << endl;
    mostrarAccesoriosConUsos(saludCreada);
}

void modificarAccesorio(vector<usos>& accesorios) {
    mostrarAccesoriosConUsos(accesorios);
    int opcionModificar;
    int contadorUsos;
    cout << "Seleccione el n�mero del accesorio que desea modificar: ";
    cin >> opcionModificar;
    while (opcionModificar < 1 || opcionModificar > accesorios.size()) {
        cout << "Opci�n no v�lida. Int�ntelo de nuevo: ";
        cin >> opcionModificar;
    }
    cout << "Ingrese la cantidad de usos nueva del accesorio: ";
    cin >> contadorUsos;
    while (contadorUsos < 0) {
        cout << "La cantidad de usos debe ser mayor o igual a 0. Ingrese un valor v�lido: ";
        cin >> contadorUsos;
    }
    accesorios[opcionModificar - 1].usos = contadorUsos;
    cout << "Accesorio modificado con �xito." << endl;

}

void eliminarAccesorio(vector<usos>& accesorios) {
    mostrarAccesoriosConUsos(accesorios);
    int opcionEliminar;
    cout << "Seleccione el n�mero del accesorio que desea eliminar: ";
    cin >> opcionEliminar;
    while (opcionEliminar < 1 || opcionEliminar > accesorios.size()) {
        cout << "Opci�n no v�lida. Int�ntelo de nuevo: ";
        cin >> opcionEliminar;
    }
    accesorios.erase(accesorios.begin() + opcionEliminar - 1);
    cout << "Accesorio eliminado con �xito." << endl;
}


//mochila funciones

void agregarAccesorio(mochila& mochila, usos accesorio) {
    if (mochila.accesoriosConUsos.size() < mochila.capacidad) {
        mochila.accesoriosConUsos.push_back({ accesorio.accesorio, accesorio.usos });
        cout << "\nAccesorio agregado con �xito." << endl;
        mochila.capacidad--;
        cout << "\nEspacio restante en la mochila: " << mochila.capacidad << endl;
    }
    else {
        cout << "La mochila est� llena. Elimine un accesorio para agregar uno nuevo." << endl;
    }

};

void agregarAccesorioASoldado(vector<soldado>& soldadosCreados, vector<usos> accesoriosCreados) {
    if (soldadosCreados.size() <= 0) {
        cout << "No hay soldados creados." << endl;
        return;
    }
    if (accesoriosCreados.size() <= 0) {
        cout << "No hay accesorios creados." << endl;
        return;
    }
    mostrarSoldados(soldadosCreados);
    int opcionAccesorio;
    int opcionSoldadoacc;
    cout << "Seleccione el n�mero del soldado al que desea agregar el accesorio: ";
    cin >> opcionSoldadoacc;
    while (opcionSoldadoacc < 1 || opcionSoldadoacc > soldadosCreados.size()) {
        cout << "Opci�n no v�lida. Int�ntelo de nuevo: ";
        cin >> opcionSoldadoacc;
    }
    mostrarAccesoriosConUsos(accesoriosCreados);
    cout << "Seleccione el n�mero del accesorio que desea agregar: ";
    cin >> opcionAccesorio;
    while (opcionAccesorio < 1 || opcionAccesorio > accesoriosCreados.size()) {
        cout << "Opci�n no v�lida. Int�ntelo de nuevo: ";
        cin >> opcionAccesorio;
    }

    agregarAccesorio(soldadosCreados[opcionSoldadoacc - 1].mochila, accesoriosCreados[opcionAccesorio - 1]);

};

void modificarMochila(vector<soldado>& soldadosCreados) {
    int opcionAccesorio;
    int contadorUsos;
    mostrarSoldados(soldadosCreados);
    int opcionSoldado;
    cout << "Seleccione el n�mero del soldado que desea modificar: ";
    cin >> opcionSoldado;
    while (opcionSoldado < 1 || opcionSoldado > soldadosCreados.size()) {
        cout << "Opci�n no v�lida. Int�ntelo de nuevo: ";
        cin >> opcionSoldado;
    }

    modificarAccesorio(soldadosCreados[opcionSoldado - 1].mochila.accesoriosConUsos);
};

void eliminarDeMochila(vector<soldado>& soldadosCreados) {
    mostrarSoldados(soldadosCreados);
    int opcionSoldado;
    cout << "Seleccione el n�mero del soldado que desea modificar: ";
    cin >> opcionSoldado;
    while (opcionSoldado < 1 || opcionSoldado > soldadosCreados.size()) {
        cout << "Opci�n no v�lida. Int�ntelo de nuevo: ";
        cin >> opcionSoldado;
    }
    eliminarAccesorio(soldadosCreados[opcionSoldado - 1].mochila.accesoriosConUsos);
};

//mapa funciones

void agregarEstacion(estacion*& inicio) {
    string nombre;
    int cantidadZombies;
    cout << "Ingrese el nombre de la estaci�n: ";
    cin >> nombre;
    cout << "Ingrese la cantidad de zombies en la estaci�n: ";
    cin >> cantidadZombies;
    estacion* nuevaEstacion = new estacion;
    nuevaEstacion->nombre = nombre;
    nuevaEstacion->cantidadZombies = cantidadZombies;
    nuevaEstacion->siguiente = NULL;
    if (inicio == NULL) {
        inicio = nuevaEstacion;
    }
    else {
        estacion* actual = inicio;
        while (actual->siguiente != NULL) {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevaEstacion;
    }
    cout << "Estaci�n agregada con �xito." << endl;


}

void mostrarEstaciones(estacion* inicio) {
    estacion* actual = inicio;
    int numero = 1;
    if (actual == NULL) {
        cout << "No hay estaciones creadas." << endl;
        return;
    }
    else
    {
        while (actual != NULL) {
            cout << "\nEstaci�n #" << numero << endl;
            cout << "Nombre: " << actual->nombre << endl;
            cout << "Cantidad de zombies: " << actual->cantidadZombies << endl;
            cout << endl;
            actual = actual->siguiente;
            numero++;
        }
    }

}

void modificarEstacion(estacion*& inicio) {
    mostrarEstaciones(inicio);
    int numeroEstacion;
    cout << "Seleccione el n�mero de la estaci�n que desea modificar: ";
    cin >> numeroEstacion;
    while (numeroEstacion < 1) {
        cout << "Seleccione un n�mero v�lido: ";
        cin >> numeroEstacion;
    }
    estacion* actual = inicio;
    int contador = 1;
    while (contador < numeroEstacion) {
        actual = actual->siguiente;
        contador++;
    }
    string nombre;
    int cantidadZombies;
    cout << "Ingrese el nuevo nombre de la estaci�n: ";
    cin >> nombre;
    cout << "Ingrese la nueva cantidad de zombies en la estaci�n: ";
    cin >> cantidadZombies;
    actual->nombre = nombre;
    actual->cantidadZombies = cantidadZombies;
    cout << "Estaci�n modificada con �xito." << endl;

}

void eliminarEstacion(estacion*& inicio) {
    mostrarEstaciones(inicio);
    int numeroEstacion;
    cout << "Seleccione el n�mero de la estaci�n que desea eliminar: ";
    cin >> numeroEstacion;
    while (numeroEstacion < 1) {
        cout << "Seleccione un n�mero v�lido: ";
        cin >> numeroEstacion;
    }
    if (numeroEstacion == 1) {
        estacion* temp = inicio;
        inicio = inicio->siguiente;
        delete temp;
    }
    else {
        estacion* actual = inicio;
        int contador = 1;
        while (contador < numeroEstacion - 1) {
            actual = actual->siguiente;
            contador++;
        }
        estacion* temp = actual->siguiente;
        actual->siguiente = temp->siguiente;
        delete temp;
    }
    cout << "Estaci�n eliminada con �xito." << endl;
}

void liberarestaciones(estacion*& inicio) {
    estacion* actual = inicio;
    while (actual != NULL) {
        estacion* aux = actual;
        actual = actual->siguiente;
        delete aux;
    }
    inicio = NULL;
    cout << "Estaciones liberadas con �xito." << endl;
}

//funcion principal 

int main() {
    int opcionPrincipal = -1;
    vector<zombieFortaleza> zombiesCreados;
    vector<soldado> soldadosCreados;
    vector<usos> armasCreadas;
    vector<usos> defensasCreadas;
    vector<usos> saludCreada;
    estacion* inicio = NULL;

    while (opcionPrincipal != 7) {
        cout << "\n=== Juego: Salvar al planeta de los zombies ===" << endl;
        cout << "1. Zombies" << endl;
        cout << "2. Accesorios" << endl;
        cout << "3. Equipos" << endl;
        cout << "4. Mochilas" << endl;
        cout << "5. Mapa" << endl;
        cout << "6. Consultar Informaci�n del Equipo" << endl;
        cout << "7. Salir del Juego" << endl;
        cout << "Ingrese el n�mero de lo que desea hacer: ";
        cin >> opcionPrincipal;

        switch (opcionPrincipal) {
        case 1: {
            int opcionZombies;
            cout << "\n--- Zombies ---" << endl;
            cout << "1. Crear Tipos de Zombies" << endl;
            cout << "2. Ver Tipos de Zombies" << endl;
            cout << "3. Ver zombies Creados" << endl;
            cout << "4. Volver " << endl;
            cout << "Ingrese opci�n: ";
            cin >> opcionZombies;

            switch (opcionZombies) {
            case 1:
                int opcionZombies1;
                cout << "\n--- Zombies ---" << endl;
                cout << "1. Zombies R�pidos y �giles " << endl;
                cout << "2. Zombies Tanque" << endl;
                cout << "3. Zombies Inteligentes " << endl;
                cout << "4. Zombies Infectados por Hongos " << endl;
                cout << "5. Zombies Bioluminiscente " << endl;
                cout << "Ingrese opci�n: ";
                cin >> opcionZombies1;
                zombiesUsuario(opcionZombies1, zombiesCreados);

                break;
            case 2:
                mostrarZombies(crearZombiesConFortaleza());
                break;
            case 3:
                mostrarZombies(zombiesCreados);
                break;
            case 4:
                cout << "Volviendo al men� principal...\n";
                break;
            default:
                cout << "Opci�n no v�lida.\n";
            }
            break;
        }
        case 2: {
            int opcionAccesorios;
            cout << "\n--- Accesorios ---" << endl;
            cout << "1. Agregar Accesorios" << endl;
            cout << "2. Modificar Accesorios" << endl;
            cout << "3. Eliminar Accesorios" << endl;
            cout << "4. Mostrar Accesorios" << endl;
            cout << "5. Volver" << endl;
            cout << "Ingrese opci�n: ";
            cin >> opcionAccesorios;

            switch (opcionAccesorios) {
            case 1:
                int opcionAccesorios1;
                cout << "\n--- Accesorios ---" << endl;
                cout << "1. Armas" << endl;
                cout << "2. Defensas" << endl;
                cout << "3. Salud" << endl;
                cout << "4. Volver" << endl;
                cout << "Ingrese opci�n: ";
                cin >> opcionAccesorios1;
                switch (opcionAccesorios1) {
                case 1:
                    armasUsuarioCrear(armasCreadas);
                    break;
                case 2:
                    defensasUsuarioCrear(defensasCreadas);
                    break;
                case 3:
                    saludUsuarioCrear(saludCreada);
                    break;
                case 4:
                    cout << "Volviendo al men� principal...\n";
                    break;
                default:
                    cout << "Opci�n no v�lida.\n";
                }
                break;
            case 2:
                int opcionAccesorios2;
                cout << "\n--- Accesorios ---" << endl;
                cout << "1. Armas" << endl;
                cout << "2. Defensas" << endl;
                cout << "3. Salud" << endl;
                cout << "4. Volver" << endl;
                cout << "Ingrese opci�n: ";
                cin >> opcionAccesorios2;
                switch (opcionAccesorios2) {
                case 1:
                    modificarAccesorio(armasCreadas);
                    break;
                case 2:
                    modificarAccesorio(defensasCreadas);
                    break;
                case 3:
                    modificarAccesorio(saludCreada);
                    break;
                case 4:
                    cout << "Volviendo al men� principal...\n";
                    break;
                default:
                    cout << "Opci�n no v�lida.\n";
                }
                break;

            case 3:
                int opcionAccesorios3;
                cout << "\n--- Accesorios ---" << endl;
                cout << "1. Armas" << endl;
                cout << "2. Defensas" << endl;
                cout << "3. Salud" << endl;
                cout << "4. Volver" << endl;
                cout << "Ingrese opci�n: ";
                cin >> opcionAccesorios3;
                switch (opcionAccesorios3) {
                case 1:
                    eliminarAccesorio(armasCreadas);
                    break;
                case 2:
                    eliminarAccesorio(defensasCreadas);
                    break;
                case 3:
                    eliminarAccesorio(saludCreada);
                    break;
                case 4:
                    cout << "Volviendo al men� principal...\n";
                    break;
                default:
                    cout << "Opci�n no v�lida.\n";
                }
                break;
            case 4:
                int opcionAccesorios4;
                cout << "\n--- Accesorios ---" << endl;
                cout << "1. Armas" << endl;
                cout << "2. Defensas" << endl;
                cout << "3. Salud" << endl;
                cout << "4. Volver" << endl;
                cout << "Ingrese opci�n: ";
                cin >> opcionAccesorios4;
                switch (opcionAccesorios4) {
                case 1:
                    mostrarAccesoriosConUsos(armasCreadas);
                    break;
                case 2:
                    mostrarAccesoriosConUsos(defensasCreadas);
                    break;
                case 3:
                    mostrarAccesoriosConUsos(saludCreada);
                    break;
                case 4:
                    cout << "Volviendo al men� principal...\n";
                    break;
                default:
                    cout << "Opci�n no v�lida.\n";
                }
                break;
            case 5:
                cout << "Volviendo al men� principal...\n";
                break;
            default:
                cout << "Opci�n no v�lida.\n";
            }
            break;
        }
        case 3: {
            int opcionEquipos;
            string nombre;
            int salud;
            cout << "\n--- Equipos ---" << endl;
            cout << "1. Agregar Soldado" << endl;
            cout << "2. Modificar Soldado" << endl;
            cout << "3. Eliminar Soldado" << endl;
            cout << "4. Mostrar Soldados" << endl;
            cout << "5. Volver" << endl;
            cout << "Ingrese opci�n: ";
            cin >> opcionEquipos;

            switch (opcionEquipos) {
            case 1:
                cout << "\n--- Equipos ---" << endl;
                cout << "1. Ingrese nombre de su soldado" << endl;
                cin >> nombre;
                cout << "2. Ingrese salud de su soldado" << endl;
                cin >> salud;
                anadirSoldado(soldadosCreados, nombre, salud);

                break;
            case 2:
                editarSoldado(soldadosCreados);
                break;
            case 3:
                eliminarSoldado(soldadosCreados);
                break;
            case 4:
                mostrarSoldados(soldadosCreados);
                break;
            case 5:
                cout << "Volviendo al men� principal...\n";
                break;
            default:
                cout << "Opci�n no v�lida.\n";
            }
            break;
        }
        case 4: {
            int opcionMochilas;
            cout << "\n--- Mochilas ---" << endl;
            cout << "1. Agregar Mochila" << endl;
            cout << "2. Modificar Mochila" << endl;
            cout << "3. Eliminar Objeto de Mochila" << endl;
            cout << "4. Volver" << endl;
            cout << "Ingrese opci�n: ";
            cin >> opcionMochilas;

            switch (opcionMochilas) {
            case 1:
                int opcionMochilas1;
                cout << "\n--- Mochilas ---" << endl;
                cout << "1. Agregar Arma a Soldado" << endl;
                cout << "2. Agregar Defensa a Soldado" << endl;
                cout << "3. Agregar Salud a Soldado" << endl;
                cout << "4. Volver" << endl;
                cout << "Ingrese opci�n: ";
                cin >> opcionMochilas1;
                switch (opcionMochilas1) {
                case 1:
                    agregarAccesorioASoldado(soldadosCreados, armasCreadas);
                    break;
                case 2:
                    agregarAccesorioASoldado(soldadosCreados, defensasCreadas);
                    break;
                case 3:
                    agregarAccesorioASoldado(soldadosCreados, saludCreada);
                    break;
                case 4:
                    cout << "Volviendo al men� principal...\n";
                    break;
                default:
                    cout << "Opci�n no v�lida.\n";
                }
                break;
            case 2:
                modificarMochila(soldadosCreados);
                break;
            case 3:
                eliminarDeMochila(soldadosCreados);
                break;
            case 4:
                cout << "Volviendo al men� principal...\n";
                break;
            default:
                cout << "Opci�n no v�lida.\n";
            }
            break;
        }
        case 5: {
            int opcionMapa;
            cout << "\n--- Mapa ---" << endl;
            cout << "1. Agregar Estaci�n" << endl;
            cout << "2. Modificar Estaci�n" << endl;
            cout << "3. Eliminar Estaci�n" << endl;
            cout << "4. Mostrar Estaciones" << endl;
            cout << "5. Volver" << endl;
            cout << "Ingrese opci�n: ";
            cin >> opcionMapa;

            switch (opcionMapa) {
            case 1:
                agregarEstacion(inicio);
                break;
            case 2:
                modificarEstacion(inicio);
                break;
            case 3:
                eliminarEstacion(inicio);
                break;
            case 4:
                mostrarEstaciones(inicio);
                break;
            case 5:
                cout << "Volviendo al men� principal...\n";
                break;
            default:
                cout << "Opci�n no v�lida.\n";
            }
            break;
        }
        case 6: {
            int opcionInfo;
            cout << "\n--- Consultar Informaci�n del Equipo ---" << endl;
            cout << "1. Informaci�n de Soldados y mochilas" << endl;
            cout << "2. Volver" << endl;
            cout << "Ingrese opci�n: ";
            cin >> opcionInfo;

            switch (opcionInfo) {
            case 1:
                mostrarSoldados(soldadosCreados);
                break;
            case 2:
                cout << "Volviendo al men� principal...\n";
                break;
            default:
                cout << "Opci�n no v�lida.\n";
            }
            break;
        }
        case 7:
            cout << "Gracias por jugar. �Hasta luego!\n";
            break;
        default:
            cout << "Opci�n no v�lida. Int�ntelo de nuevo.\n";
        }
    }
    return 0;
}





