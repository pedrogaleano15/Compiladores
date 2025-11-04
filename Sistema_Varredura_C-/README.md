# 🧩 Analisador Léxico (Scanner) para C-

 Este projeto implementa um **analisador léxico (scanner)** para a linguagem **C-**, uma versão simplificada da linguagem C, desenvolvido em **linguagem C**.

 O programa lê um arquivo de código-fonte `.c-` e exibe na tela a sequência de **tokens** encontrados, informando o **tipo** e o **lexema** (cadeia de caracteres) de cada um.

---

## 👨‍💻 Membros do Grupo

 - Pedro Henrique Morais Galeano

---

## ⚙️ Como Compilar (Via Terminal)

 Certifique-se de ter o compilador `gcc` (como o MinGW no Windows) instalado e acessível no seu terminal.

# Navegue ate a pasta onde esta o Scanner.c
 gcc Scanner.c -o scanner
 
 Isso criará um arquivo executável chamado scanner (ou scanner.exe no Windows).

▶️ Como Executar
 O programa precisa de um argumento de linha de comando: o caminho para o arquivo .c- que você deseja analisar.

 1. Via Terminal (Recomendado)
 
 Após compilar, execute o programa passando o arquivo de exemplo como argumento:

# Exemplo no Windows (PowerShell/CMD)
 
 .\scanner.exe .\Exemplos\Exemplo1.c-

# Exemplo no Linux/macOS
 
 ./scanner ./Exemplos/Exemplo1.c-

2. No Visual Studio Code (com Terminal)
 O VS Code tem um terminal integrado que funciona exatamente como o terminal padrão.

 Certifique-se de ter a extensão C/C++ da Microsoft instalada.

 Abra a pasta do seu projeto no VS Code (Ex: File -> Open Folder...).

 Abra o terminal integrado (Use Ctrl + ' ou Terminal -> New Terminal).

 Compile digitando o comando no terminal:

 gcc Scanner.c -o scanner
 
 Execute o programa no mesmo terminal, passando o arquivo de exemplo:

 .\scanner.exe .\Exemplos\Exemplo4.c-

3. No Code::Blocks (IDE)
 O Code::Blocks é uma IDE que gerencia a compilação, mas você precisa informar a ele qual argumento (seu arquivo .c-) passar para o programa.

 Crie um novo projeto: File -> New -> Project....

 Selecione Console application e clique Go.

 Escolha a linguagem C e dê um nome ao seu projeto.

 No painel esquerdo, encontre o arquivo main.c que o Code::Blocks criou.

 Apague todo o conteúdo do main.c e cole o código completo do Scanner.c no lugar.

 O programa não vai funcionar se você apenas clicar em Build and run (F9), pois ele precisa do nome do arquivo .c- como argumento.

 Para configurar o argumento, vá ao menu: Project -> Set programs' arguments....

 No campo Program arguments, digite o caminho para o seu arquivo de teste.

 Exemplo: .\Exemplos\Exemplo1.c- (se a pasta Exemplos estiver dentro da pasta do seu projeto).

 Ou (caminho absoluto): C:\Users\User\Desktop\Compiladores\Sistema_Varredura_C-\Exemplos\Exemplo1.c-

 Clique OK.

 Agora sim, compile e execute o projeto (F9 ou o botão Build and run). A saída do scanner aparecerá na janela do console.