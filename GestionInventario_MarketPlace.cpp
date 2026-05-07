#include "pch.h"
#include "UserRepository.h"
#include "Auth.h"
#include "AuthUI.h"

using namespace System;

void main() {
    UsuarioRepository repo("clientes.bin", "vendedores.bin");
    Auth auth(&repo);
    AuthUI::Render(&auth);
}
