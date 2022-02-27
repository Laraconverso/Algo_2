def abb_crear(comparador):
    return {"raiz": None, "comparador": comparador}

def abb_crear_nodo(elemento):
    return {"elemento": elemento, "izquierdo": None ,"derecho": None}

def abb_insertar_recursivo(raiz, elemento, comparador):
    if raiz is None:
        return abb_crear_nodo(elemento)
    if comparador(elemento, raiz["elemento"]) >= 0:
        raiz["derecho"] = abb_insertar_recursivo(raiz["derecho"], elemento, comparador)
    else:
        raiz["izquierdo"] = abb_insertar_recursivo(raiz["izquierdo"], elemento, comparador)
    return raiz

def abb_insertar(abb, elemento):
    nueva_raiz = abb_insertar_recursivo(abb["raiz"], elemento, abb["comparador"])
    abb["raiz"]= nueva_raiz
    return abb;