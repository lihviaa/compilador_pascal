INTEGRANTES DO GRUPO
NOME: Livia Alabarse dos Santos         RA: 10403046
NOME: Pedro Henrique Araujo Farias      RA: 10265432

Compile o programa com o seguinte comando:
gcc -g -Og -Wall pascal+-.c -o pascal+-

Execute-o com o seguinte comando:
.\nome_do_programa .\caminho_para_arquivo

> SOBRE O PROGRAMA DESENVOLVIDO
O pascal+-.c consiste em um analisador léxico e sintático implementados conforme
as definições léxicas e sintáticas estabelecidas no enunciado do trabalho. O 
analisador recebe como argumento o caminho para um arquivo cujo conteúdo será 
analisado. Caso o arquivo seja encontrado, a análise será iniciada.
Caso o arquivo não seja encontrado, o programa retornará erro. 

Isso significa que o analisador léxico:
- Apresenta a linha em que cada um dos átomos foi identificado 
- Ignora caracteres delimitadores
- Reporta comentários ao analisador sintático
- Reconhece átomos identificadores, definidos como identificador -> letra(letra|digito|_)*
- Não reconhece identificadores com mais de 15 caracteres
- Reconhece as 18 palavras reservadas definidas, gerando um átomo único para cada
- Reconhece números na notação binária, definidos como numero -> 0b(0|1)+
- Reporta os números em notação decimal
- Reconhece todos os símbolos terminais utilizados nas regras de produção da sintaxe
- Reporta erros léxicos

Além disso, o analisador sintático:
- Implementa corretamente as regras de produção definidas na sintaxe
- Identifica erros sintáticos 
- Informa a linha em que o erro foi identificado
- Em caso de erro sintático, indica o átomo esperado e o átomo encontrado
- Em caso de sucesso, informa o número de linhas lidas e a corretude sintáticas

> IMPORTANTE NOTAR QUE 
- Em caso de erro léxico: O átomo é reportado ao analisador sintático, mas não é 
exibido na saída da execução do programa.
- Em caso de erro sintático: Enquanto o exemplo de erro sintático no enunciado
do trabalho não exibe o átomo encontrado que difere do átomo esperado, nós optamos
por mostrá-lo na saída, uma vez que o átomo foi formado sem erros, apenas não
corresponde ao átomo esperado, além de ser um bom recurso visual para comprovar
que o átomo encontrado não é o átomo esperado pelas regras de formação da sintaxe.

> SOBRE OS TESTES
Além dos exemplos 01 e 02, disponibilizados no próprio enunciado do trabalho,
elaboramos alguns testes extras.

No diretório testes_lexicos, você encontrará alguns testes que apontam erros na
construção dos símbolos lidos pelo analisador léxico. Lá estão apontados erros como
tentativa de identificar letras maiúsculas, as quais não são contempladas pela
definição léxica, tentativa de identificar átomos identificadores com mais de 15
caracteres, construções errôneas de números (por exemplo, 0ba11 ou 0b1101_) e etc.

No diretório testes_sintaticos, são encontrados alguns testes corretos e outros 
incorretos no que diz respeito à construção da estrutura da linguagem, ou seja,
sua sintaxe. 

> COMENTANDO ALGUNS TESTES SINTÁTICOS
- erro2.pas: Na execução do pascal+- lendo o arquivo erro2.pas, é reportado erro
sintático, e a mensagem informa que o átomo esperado era BEGIN, mas foi encontrado
END. Isso acontece pois como o analisador léxico encontrou um ';' logo após o comando
write, gerado pela regra <comando_saida>, o sintático entendeu que este seria seguido
por um outro <comando>, conforme definido em <comando_composto>. Entretanto, como
após ';' foi encontrado END e não um átomo iniciador de algum dos tipos de comando
definidos em <comando>, foi apontado erro sintático. 
PERGUNTA: Mas por que o analisador sintático aponta o átomo BEGIN como esperado?
RESPOSTA: Pois na definição de <comando>, temos um switch que analisa lookahead.
No caso DEFAULT, a regra de produção chamada é <comando_composto>, o qual tenta
consumir BEGIN. Por isso, BEGIN é apontado como esperado, mas, na realidade, qualquer
átomo que inicie alguma das regras de produção definidas em <comando> é aceito.

- erro4.pas: Ao executar o pascal+- lendo o arquivo erro4.pas, é reportado erro
sintático, e a mensagem informa que o átomo esperado era TRUE, mas foi encontrado
MAIOR. Isso ocorre pois o sintático esperava uma <expressao>, a qual poderia acabar
gerando uma <expressao_relacional>, que teria obrigatoriamente uma <expressao_simples>
e, opcionalmente, um <op_relacional> e outra <expressao_simples>. Entretanto, como
não foi encontrada uma <expressao_simples>, apenas o <op_relacional> MAIOR, o analisador
sintático aponta erro sintático.
PERGUNTA: Por que o analisador sintático aponta o átomo TRUE como esperado?
RESPOSTA: Pois na definição de <expressao_simples>, derivamos para <termo> e, por fim,
para <fator>. Na definição de <fator>, temos um switch que analisa lookahead. No
caso DEFAULT, <fator> tenta consumir o átomo TRUE. Por isso, TRUE é apontado como
esperado, mas, na realidade, qualquer átomo que inicie as opções definidas em <fator>
é aceito.

CONCLUSÃO: É importante analisar os casos default em switch cases de regras de 
produção para entender o motivo de alguns átomos serem apontados como esperados 
e outros não.

> DESAFIOS ENCONTRADOS
Um dos desafios do trabalho foi o tratamento de comentários pelo analisador sintático.
Nosso raciocínio foi o de que enquanto o lookahead encontrasse comentários, ele
os descartaria e receberia o próximo átomo. Implementamos essa condição no início 
da função consome, mas ainda assim alguns comentários não eram ignorados, falhando
a execução do programa. Então, por propósitos de teste, adicionamos essa verificação
também no fim da função consome, e... bom, funcionou. 

Apontamos esse caso como um desafio pois não sabemos ao certo o motivo de uma única
verificação do átomo como comentário não ser suficiente para ignorar os comentários.
Outros desafios surgiram, obviamente, mas foram resolvidos. Esse foi o único que 
não conseguimos identificar uma solução - aliás, não conseguimos identificar o PORQUÊ
da solução.

Pedimos desculpas pelo README.txt extenso, mas consideramos imprescindível apontarmos
nossos questionamentos, desafios e conclusões neste trabalho extenso, porém divertido.