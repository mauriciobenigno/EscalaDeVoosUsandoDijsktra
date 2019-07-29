# EscalaDeVoosUsandoDijsktra
 Trabalho escrito em C++ com Interface Gráfica feita no QtCreator. Fazendo uso do algoritmo de Dijsktra para encontrar a escala de voos ideal para chegar no destino em menor tempo.

Funciona da seguinte forma:
O arquivo grafo-01 possui diversas linhas com dados no seguinte formato -> "VARIG4338 RioDeJaneiro 04:10 Roma 15:00" e 
"KLM5009 RioDeJaneiro 06:38 Bruxelas 09:21 09:48 Hamburg 10:15"

- Cada nomenclatura indica um nome de voo.
- O primeiro nome de cidade a aparecer � a origem.
- O ultimo nome de cidade a aparecer � o destino.
- Caso tenha um nome de cidade entre duas, � uma escala.
- Os horarios representam embarque e chegada.

Quanto as regras:
- � poss�vel n?o completar a escala de voo e seguir viagem em outro voo caso necess�rio.
- Cada embarque adiciona pelo menos 30 minutos no tempo final (importante considerar no caso de troca de voo em escala)
- O programa possibilita a escala de voo envolver horarios inferiores ao de embarque (por exemplo, embarcar 23h e chegar no destino as 15h)


Diante dessas regras e condi�?es, o programa faz a leitura do arquivo contendo as escalas e realiza seu tratamento, resultando em um grafo orientado e com pesos em suas arestas.
O n� de inicio do gr�fo s� � selecionado quando o usu�rio escolhe a cidade de origem, e o final definido quando ele escolhe a cidade destino.

A aplica�?o do algoritmo de Dijkstra � feita quando pressionado o bot?o para calcular e ent?o o programa apresenta um Itnerario de voo.
