──────────────────────────────────────────────────────────────────────  
# PROJETO - Joystick PWM - Controle de objetos no display via joystick
(SUB GRUPO 6)  
**Aluno: Lucas Ricardo de Lima Figueiredo** 

──────────────────────────────────────────────────────────────────────  
## OBJETIVO DA ATIVIDADE  
──────────────────────────────────────────────────────────────────────  
O objetivo dessa atividade é desenvolver um sistema que permita identificar a posição do Joystick por meio do Conversor Analógico-Digital(ADC) e, a partir dessa informação, alterar a posição de um quadrado no display em tempo real. Além disso, também foram explorados conceitos de interrupção, temporização e Pulse Width Modulation(PWM) dentro de seu funcionamento.  

──────────────────────────────────────────────────────────────────────  
## FUNÇÕES IMPLEMENTADAS
──────────────────────────────────────────────────────────────────────
Aqui vão algumas funções implementadas que valem a pena mencionar:
- `ssd1306_init_config_clean()`:Inicializa e configura o i2c além de limpar a tela do display;
- `ssd1306_select_edge()`:Recebe um inteiro de 1 a 4 e envia a borda que será apresentada no display;
- `joystick_adc_init()`:Inicializa o ADC do Joystick;
- `gpio_pwm_config()`:Inicializa e configura uma porta GPIO como PWM e retorna o seu Slice;
- `set_pwm_level_on_leds()`: Executa o trecho de código que atualiza a intensidade dos LEDs vermelho e azul caso a função de PWM esteja habilitada; 
  

──────────────────────────────────────────────────────────────────────  
## FUNCIONALIDADES DO PROJETO  
──────────────────────────────────────────────────────────────────────  

**Controles na plaquinha Bit Dog Lab:**  
- **Mover Joystick(Qualquer direção):** Move o Quadrado no Display.  
- **Mover Joystick(Eixo X):** Altera a intensidade do LED Vermelho.
- **Mover Joystick(Eixo Y):** Altera a intensidade do LED Azul.
- **Botão A:** Habilita/Desabilita os LEDs azul e vermelho.  
- **Botão Joystick:** Liga/Desliga o LED verde; Altera a borda apresentada no display(São 4 tipos diferentes).  
- **Botão B:** Põe a placa em modo BOOTSEL.
- **Monitor Serial:** Apresenta a posição do Joystick e a posição do quadrado no display; apresenta se o PWM dos LEDs azul e vermelho foi habilitado/desabilitado; apresenta se o LED verde foi ativado/desativado; informa a borda que está sendo exibida no display.
  

──────────────────────────────────────────────────────────────────────  
## ESTRUTURA DO PROGRAMA  
──────────────────────────────────────────────────────────────────────  
O programa segue a seguinte estrutura:  

1. *Inicialização:* Configuração inicial do Display Oled, ADC do Joystick, LEDs, botões e interrupções.  
2. *Leitura de Entrada:* O microcontrolador detecta a posição do Joystick.  
3. *Execução de Animações:* De acordo com a posição lida, a posição do quadrado no display é atualizada.  
4. *Interrupções:* Pressionar o Botão A, B ou do Joystick gera um interrupção que altera características do programa.  
5. *Interatividade:* O usuário pode controlar livremente o quadradinho, estados dos LEDs e a borda apresentada no display.  

──────────────────────────────────────────────────────────────────────  
## TECNOLOGIAS UTILIZADAS  
──────────────────────────────────────────────────────────────────────  

- **Linguagem:** C  
- **Microcontrolador:** Raspberry Pi Pico W
- **Plataforma de desenvolvimento:** Plaquinha BitDogLab 

──────────────────────────────────────────────────────────────────────  
## COMO EXECUTAR O PROJETO
──────────────────────────────────────────────────────────────────────  
- **Clone o Repositório:**
  git clone [https://github.com/LucasrFig/Joystick_PWM.git](https://github.com/LucasrFig/Joystick_PWM.git)
  
- **Compile o Código:**
  Utilize o ambiente de desenvolvimento VS Code. 
  Instalar extensão: Raspberry Pi Pico.

- **Carregue na Raspberry Pi Pico:**
  Conecte a Raspberry Pi Pico em modo de boot.
  Copie o arquivo .uf2 gerado para a unidade de armazenamento da Pico.

──────────────────────────────────────────────────────────────────────  
## VÍDEO DO PROJETO  
────────────────────────────────────────────────────────────────────── 
[https://drive.google.com/file/d/1Nf9n1o1f_lvU9id5cwCZaTa_hdpXrth7/view?usp=drivesdk
](https://drive.google.com/file/d/1Nf9n1o1f_lvU9id5cwCZaTa_hdpXrth7/view?usp=drivesdk)


 

