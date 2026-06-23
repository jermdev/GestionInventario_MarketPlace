#pragma once
template<typename T>
class ComponenteUI {

    T elemento;
public:
    ComponenteUI(T elemento ) {
        this->elemento = elemento;
    }
    ComponenteUI() {
        this->elemento = NULL;
    }
    ~ComponenteUI() {

    }
    template <typename C, typename P>
    void render(C card, P posicion) {  card(elemento, posicion); }
};