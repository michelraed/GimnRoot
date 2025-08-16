#!/bin/bash
# install.sh - Adiciona o diretório atual ao PATH no ~/.bashrc de forma segura
# e gera o script rootGimn.sh na mesma pasta

# Diretório atual
current_dir="$(pwd)"
bashrc_file="$HOME/.bashrc"
marker="# >>> Adicionado pelo script install.sh"

# Função para verificar se o PATH já foi adicionado
path_already_in_bashrc() {
    grep -F "$current_dir" "$bashrc_file" > /dev/null 2>&1
}

# Adiciona ao PATH se necessário
if path_already_in_bashrc; then
    echo "✅ O diretório '$current_dir' já está no PATH dentro de '$bashrc_file'."
else
    echo "➕ Adicionando '$current_dir' ao PATH no '$bashrc_file'..."

    {
        echo ""
        echo "$marker"
        echo "# Data: $(date)"
        echo "export PATH=\"$current_dir:\$PATH\""
        echo "# <<< Fim da adição do install.sh"
    } >> "$bashrc_file"

    echo "✅ PATH atualizado no '$bashrc_file'."
fi

# Criação do script rootGimn.sh
echo "⚙️  Gerando rootGimn em $current_dir ..."

cat > "$current_dir/rootGimn" << 'EOF'
#!/bin/bash

echo "☢️  Activating Environment Root"

# Diretório onde o script está instalado
rootPath="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

cd "$rootPath" || exit 1

if [ -f "Run.C" ]; then
    echo "📄 Executando Run.C com ROOT no diretório $rootPath ..."
    root -l -b Run.C
else
    echo "❌ Arquivo Run.C não encontrado em $rootPath"
    exit 1
fi
EOF

chmod +x "$current_dir/rootGimn"

echo "✅ Script rootGimn.sh criado com permissão de execução."



#criacao do tomoGimn
cat > "$current_dir/tomoGimn" << 'EOF'
#!/bin/bash

# Diretório onde o script está instalado
rootPath="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

cd "$rootPath" || exit 1

if [ -f "Run.C" ]; then
    echo "📄 Executando Run.C com ROOT no diretório $rootPath ..."
    root -l -b Run.C
else
    echo "❌ Arquivo Run.C não encontrado em $rootPath"
    exit 1
fi
EOF

chmod +x "$current_dir/tomoGimn"


