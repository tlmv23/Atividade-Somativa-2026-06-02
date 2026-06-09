# Atividade-Somativa-2026-06-02

# Sistema de Manutenção de Contas Bancárias em C
Este repositório contém a implementação prática de um sistema de gerenciamento de clientes e saldos utilizando persistência de dados em arquivos binários de tamanho fixo em Linguagem C.

Tecnologias e Conceitos Utilizados
- Linguagem C: Manipulação de streams de arquivos de baixo nível.
- Registros de Tamanho Fixo: Utilização de `struct` com tamanhos de campos predefinidos para garantir previsibilidade de bytes.
- Acesso Aleatório (`fseek`): Movimentação precisa do ponteiro de leitura/escrita para posições específicas sem a necessidade de ler o arquivo de forma sequencial.
- Leitura e Escrita Binária (`fread` / `fwrite`): Persistência eficiente direto da memória para o disco.
- Manipulação de Fluxo (`rewind`): Retorno explícito ao byte zero do arquivo.

Funcionalidades do Menu
1. Cadastrar cliente em posição específica: Grava um novo registro calculando o deslocamento dinamicamente via `fseek`.
2. Consultar cliente por conta: Varre o arquivo localizando o registro baseado no número identificador.
3. Atualizar saldo: Altera o dado financeiro reescrevendo exatamente os bytes do registro selecionado usando `SEEK_CUR`.
4. Encerrar conta: Realiza a remoção lógica do cliente através de uma flag de controle de status (`ativo`).
5. Listar clientes: Exibe de forma formatada em tabela todos os dados persistidos no arquivo.
6. Restaurar leitura com `rewind()`: Demonstra o uso do reset do ponteiro para reexecução de listagens.
