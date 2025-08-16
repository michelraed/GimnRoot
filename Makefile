# Makefile para instalar o ambiente e script rootGimn.sh

INSTALL_DIR := $(CURDIR)
SCRIPT_NAME := rootGimn.sh

.PHONY: all install clean uninstall

all:
	@echo "Use 'make install' para instalar o ambiente rootGimn."

install:
	@echo "🔧 Instalando rootGimn no diretório $(INSTALL_DIR) ..."
	@bash install.sh

uninstall:
	@echo "❌ Removendo PATH do ~/.bashrc..."
	@sed -i '/# >>> Adicionado pelo script install.sh/,/# <<< Fim da adição do install.sh/d' $(HOME)/.bashrc
	@echo "❌ Removendo script $(SCRIPT_NAME)..."
	@rm -f $(SCRIPT_NAME)
	@echo "✅ Desinstalação concluída."

clean:
	@rm -f $(SCRIPT_NAME)
	@echo "🧹 Limpeza concluída."
