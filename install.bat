@echo off
setlocal enabledelayedexpansion

REM Diretório atual
set "current_dir=%~dp0"
REM Remove a barra invertida final, se houver
if "%current_dir:~-1%"=="\" set "current_dir=%current_dir:~0,-1%"

echo 📁 Diretório atual: %current_dir%

REM Verifica se o PATH já contém o diretório
echo 🔍 Verificando se o diretório já está no PATH do usuário...
for /f "tokens=2*" %%a in ('reg query "HKCU\Environment" /v PATH 2^>nul') do (
    set "current_path=%%b"
)

echo PATH atual: !current_path!

echo !current_path! | find /I "%current_dir%" > nul
if not errorlevel 1 (
    echo ✅ O diretório já está no PATH do usuário.
) else (
    echo ➕ Adicionando %current_dir% ao PATH do usuário...

    if defined current_path (
        set "new_path=%current_path%;%current_dir%"
    ) else (
        set "new_path=%current_dir%"
    )

    REM Atualiza o PATH do usuário no registro
    setx PATH "!new_path!" > nul
    echo ✅ Diretório adicionado ao PATH com sucesso.
)

REM Cria o script rootGimn.bat
set "script_file=%current_dir%\rootGimn.bat"
echo ⚙️  Criando script rootGimn.bat...

(
    echo @echo off
    echo REM Script para executar Run.C com ROOT
    echo echo ☢️  Ativando Ambiente Root
    echo cd /d "%current_dir%"
    echo if exist "Run.C" (
    echo     echo 📄 Executando Run.C com ROOT no diretório %%cd%% ...
    echo     root -l -b Run.C
    echo ) else (
    echo     echo ❌ Arquivo Run.C não encontrado no diretório %%cd%%
    echo     exit /b 1
    echo )
) > "%script_file%"

echo ✅ Script rootGimn.bat criado com sucesso em %script_file%.

endlocal
pause
