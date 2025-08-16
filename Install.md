# ☢️ rootGimn — Ambiente ROOT Automatizado para Execução de Scripts

Este repositório fornece um ambiente automatizado para execução do scripts `Run.C` com o ROOT, incluindo:

- Instalação do diretório no `PATH` do sistema (Linux e Windows)
- Geração de um script executável chamado `rootGimn` (Linux/macOS) ou `rootGimn.bat` (Windows)
- Execução automatizada de `Run.C` via terminal

 > 📦 Compatível com Linux e Windows.

`Run.C` - 

---

## 📁 Estrutura esperada

O diretório onde o usuário realizará a instalação deve conter:

```
/GimnROOT/
├── Run.C                # Script ROOT a ser executado
├── install.sh           # Instalador para Linux/macOS
├── install.bat          # Instalador para Windows
├── Makefile             # (opcional) Instalador via make
```

---

## 🔧 Instalação

### 🐧 Linux / macOS

> Requisitos: `bash`, `make` e `root` disponíveis no sistema.

1. Torne o script executável:
   ```bash
   chmod +x install.sh
   ```

2. Execute a instalação:
   ```bash
   ./install.sh
   ```

3. Ou com o Makefile:
   ```bash
   make install
   ```

4. Reinicie o terminal ou execute:
   ```bash
   source ~/.bashrc
   ```

---

### 🪟 Windows

> Requisitos: `ROOT` instalado e `root.exe` disponível no PATH.

1. Clique com o botão direito no arquivo `install.bat` e selecione **Executar como administrador** *(ou como usuário, se aplicável)*.

2. O diretório atual será adicionado ao `PATH` do usuário e um script `rootGimn.bat` será criado.

3. Feche e reabra o terminal `cmd.exe` ou `PowerShell`.

---

## ▶️ Utilização

Uma vez instalado, basta digitar o comando abaixo de qualquer lugar no terminal:

### Linux / macOS

```bash
rootGimn.sh
```

### Windows

```cmd
rootGimn.bat
```

O script irá:

- Detectar automaticamente o diretório onde está instalado
- Procurar por `Run.C`
- Executar `root -l -b Run.C`

---

## ⚠️ Observações

- O script espera que o arquivo `Run.C` esteja no mesmo diretório do instalador.
- O ROOT deve estar corretamente instalado e configurado no sistema.
- O script **não altera o PATH global do sistema**, apenas o do usuário.

---

## 🧼 Desinstalação

### Linux

```bash
make uninstall
```

Ou manualmente, remova a linha correspondente do `~/.bashrc`:

```bash
# >>> Adicionado pelo script install.sh
export PATH="/caminho/do/diretorio:$PATH"
# <<< Fim da adição do install.sh
```

E exclua o script:

```bash
rm rootGimn.sh
```

### Windows

Remova o caminho da variável de ambiente manualmente ou execute novamente o `install.bat` com a opção de remoção (caso você a implemente), e delete `rootGimn.bat`.

---

## 📜 Licença

Distribuído sob a Licença MIT. Consulte o arquivo `LICENSE` para mais informações.

---

## 👨‍🔬 Autor

**Michel David Raed**  
PhD em Física Médica  
Desenvolvedor PET | Simulação Monte Carlo | Reconstrução de Imagens