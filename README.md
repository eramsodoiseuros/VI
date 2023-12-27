# VI
Visualização e Iluminação

Na última fase do projeto de Visualização e Iluminação, foram implementados três temas principais: "Parallel Multithreading", "Sampling many lights", "Output JPG/PFM/OpenEXR images" e "Tone Mapping Reinhardt".

No tema "Parallel Multithreading", o objetivo era melhorar o desempenho do renderizador através da introdução de multithreading. Isso foi realizado dividindo a imagem em partes e alocando cada parte a uma thread individual. A introdução do multithreading acelerou o processo de renderização, mas exigiu uma gestão cuidadosa das threads para evitar condições de corrida e sobrecarga excessiva.

No tema "Sampling many lights", adotou-se a estratégia de amostragem estocástica das fontes de luz. Em vez de amostrar todas as luzes na cena, selecionaram-se apenas algumas com base em suas probabilidades de contribuição. Isso reduziu o número de cálculos necessários e melhorou o desempenho do renderizador. No entanto, a amostragem estocástica introduziu um nível de ruído na imagem, que pôde ser gerenciado ajustando o número de amostras de luz ou aplicando técnicas de redução de ruído no pós-processamento.

No tema "Output JPG/PFM/OpenEXR images", o objetivo era permitir a conversão das imagens renderizadas para diferentes formatos de arquivo. Utilizaram-se bibliotecas como Pillow e OpenCV para realizar a conversão para os formatos JPG, PFM e OpenEXR. Embora tenham surgido desafios na implementação dessas bibliotecas em C++, foi possível contorná-los utilizando código Python chamado a partir do código C++.

Por fim, com o "Tone Mapping Reinhardt", implementou-se o algoritmo Reinhardt, uma abordagem simples e direta para o Tone Mapping, visando comprimir a imagem para se adequar à faixa dinâmica dos dispositivos LDR. No entanto, essa abordagem não forneceu os resultados desejados pelo grupo.

Em resumo, este projeto explorou diferentes técnicas para melhorar o desempenho do renderizador, otimizar o processo de amostragem de luz e fornecer opções de saída flexíveis para as imagens renderizadas. Essas melhorias contribuem para a eficiência e qualidade geral do renderizador, possibilitando a geração de imagens visualmente impressionantes.

